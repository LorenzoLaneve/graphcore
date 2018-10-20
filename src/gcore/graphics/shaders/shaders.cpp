//
// => gcore/shaders/shaders.cpp
//
//                                 GraphCore
//
// Copyright (c) 2018 Lorenzo Laneve
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include <gcore/graphics/shaders/shaders.h>

#include <GL/glew.h>

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace gcore;

static bool readShaderCode(const char *shaderFile, std::string &out) {
    std::ifstream is(shaderFile, std::ios::in);
    if (!is.is_open()) return false;
    
    std::ostringstream os;
    
    os << is.rdbuf();
    out = os.str();
    return true;
}


bool gcore::ShaderProgram::addUniform(const char *uniformName) {
    GLint uniform = glGetUniformLocation(program, uniformName);
    
    if (uniform < 0) {
        return false;
    }
    uniforms.push_back(uniform);
    return true;
}

ShaderProgram *gcore::ShaderProgram::fromSources(const char *vShaderPath, const char *fShaderPath) {
    
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    std::string vShaderCode;
    if (!readShaderCode(vShaderPath, vShaderCode)) {
        fprintf(stderr, "Could not load vertex shader file: %s\n", vShaderPath);
        return nullptr;
    }
    
    std::string fShaderCode;
    if (!readShaderCode(fShaderPath, fShaderCode)) {
        fprintf(stderr, "Could not load fragment shader file: %s\n", fShaderPath);
        return nullptr;
    }
    
    GLint result = GL_FALSE;
    int infoLogLength;
    const char *shaderSource;
    
    printf("Compiling vertex shader: %s\n", vShaderPath);
    shaderSource = vShaderCode.c_str();
    glShaderSource(vShader, 1, &shaderSource, nullptr);
    glCompileShader(vShader);
    
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> compileLog(infoLogLength+1);
        glGetShaderInfoLog(vShader, infoLogLength, nullptr, &compileLog[0]);
        fprintf(stderr, "Diagnostics for vertex shader:\n%s\n", &compileLog[0]);
        return nullptr;
    }
    
    printf("Compiling fragment shader: %s\n", fShaderPath);
    shaderSource = fShaderCode.c_str();
    glShaderSource(fShader, 1, &shaderSource, nullptr);
    glCompileShader(fShader);
    
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> compileLog(infoLogLength+1);
        glGetShaderInfoLog(fShader, infoLogLength, nullptr, &compileLog[0]);
        fprintf(stderr, "Failed to compile fragment shader: %s\n", &compileLog[0]);
        return nullptr;
    }
    
    printf("Linking program... ");
    GLuint program = glCreateProgram();
    
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> linkLog(infoLogLength+1);
        glGetProgramInfoLog(program, infoLogLength, nullptr, &linkLog[0]);
        fprintf(stderr, "\nDiagnostics for fragment shader:\n%s\n", &linkLog[0]);
        return nullptr;
    }
    printf("ok!\n");
    
    glDetachShader(program, vShader);
    glDetachShader(program, fShader);
    
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    
    
    return new ShaderProgram(program);
}













