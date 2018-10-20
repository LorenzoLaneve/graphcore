//
// => gcore/shaders/shaders.h
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

#ifndef __ogl_graphcore_shaders
#define __ogl_graphcore_shaders

#include <GL/glew.h>

#include <vector>

#include <assert.h>

namespace gcore {
    
    inline namespace __opengl {
        /*!
         \brief Class that holds shader programs and provides methods to compile them and link them. It also stores and retrieves uniform locations.
         */
        class ShaderProgram {
            /*!
             \brief The id of the OpenGL shader program, already compiled.
             */
            GLuint program;
            /*!
             \brief An array containing all the uniform locations for the program, in the order they're added with <code>ShaderProgram::addUniform(const char *)</code>.
             */
            std::vector<GLint> uniforms;
            
            /*!
             \brief Initializes the program with an id of an OpenGL shader program.
             \note The program pointed by the id must be already compiled and linked. The constructor will assert if the program id is not valid or the program is not ready to be used.
             */
            explicit ShaderProgram(GLuint program) : program(program) {
                assert(glIsProgram(program));
            }
            
        public:
            ~ShaderProgram() {
                glDeleteProgram(program);
            }
            
            /*!
             \brief Selects the enclosing shader program to be used for next draw calls.
             */
            inline void use() const {
                glUseProgram(program);
            }
            /*!
             \brief Adds the uniform location of the uniform in the program with the given name. If there are more than one uniform, then uniforms locations can be accessed through \c getUniform(i) where \c i depends on the order in which the calls to \c addUniform() are done.
             \return \c true if the uniform has been found and added successfully, \c false otherwise.
             */
            bool addUniform(const char *uniformName);
            /*!
             \brief Returns the uniform location at index \c uniformIndex . The index of each uniform is determined by the order of the calls to \c addUniform() , giving \c 0 to the first added uniform, \c 1 to the second, and so on...
             */
            inline GLint getUniform(int uniformIndex) const {
                assert(uniformIndex < uniforms.size());
                return uniforms[uniformIndex];
            }
            
            
            /*!
             \brief Creates and compiles a new shader program, picking the source code in the files at the given paths.
             \return A newly created shader program object containing the compiled and linked OpenGL program, or \c nullptr if an error occurred.
             */
            static ShaderProgram *fromSources(const char *vShaderPath, const char *fShaderPath);
            
            
        };
    }

}

#endif
