//
// => gcore/graphics/opengl.cpp
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

#include <gcore/graphics/opengl.h>

using namespace gcore;

GLuint VertexArrayObject::createBuffer() {
    GLuint ret;
    glGenBuffers(1, &ret);
    buffersID.push_back(ret);
    return ret;
}

void VertexArrayObject::bindPositions(GLfloat *data) {
    GLuint posBuffer = createBuffer();
    glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat) * 3, data, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(OGLVertexAttribPosition);
    glVertexAttribPointer(OGLVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
}

void VertexArrayObject::bindNormals(GLfloat *data) {
    GLuint normBuffer = createBuffer();
    glBindBuffer(GL_ARRAY_BUFFER, normBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat) * 3, data, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(OGLVertexAttribNormal);
    glVertexAttribPointer(OGLVertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
}

void VertexArrayObject::bindTexCoords2D(GLfloat *data) {
    GLuint texCoordsBuffer = createBuffer();
    glBindBuffer(GL_ARRAY_BUFFER, texCoordsBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat) * 2, data, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(OGLVertexAttribTexCoord2);
    glVertexAttribPointer(OGLVertexAttribTexCoord2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
}

void VertexArrayObject::bindBoneIDs(GLuint *data, GLint weightsPerVertex) {
    GLuint boneIDBuffer = createBuffer();
    glBindBuffer(GL_ARRAY_BUFFER, boneIDBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLuint) * weightsPerVertex, data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(OGLVertexAttribBoneID);
    glVertexAttribIPointer(OGLVertexAttribBoneID, weightsPerVertex, GL_UNSIGNED_INT, 0, BUFFER_OFFSET(0));
}
