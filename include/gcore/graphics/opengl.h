//
// => gcore/graphics/opengl.h
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

#ifndef __graphcore_graphics_opengl
#define __graphcore_graphics_opengl

#include <GL/glew.h>

#include <vector>

#define BUFFER_OFFSET(x) (void*)(x)

namespace gcore {
    
    inline namespace __opengl {
        
        /*!
         \brief Values for vertex attributes locations in OpenGL shaders.
         \note These values are set by the CPU for the Vertex Array Objects that are created with the imported models. OpenGL shaders should conform to these values with \c layout(location) specified in GLSL declarations.
         */
        typedef enum : GLuint {
            OGLVertexAttribPosition = 0,
            OGLVertexAttribNormal = 1,
            OGLVertexAttribTexCoord2 = 2,
            OGLVertexAttribBoneID = 3,
            OGLVertexAttribBoneWeight = 4
        } OGLVertexAttrib;
        
        
        class VertexArrayObject {
            
            size_t vertexCount;
            
            GLuint vaoID;
            
            std::vector<GLuint> buffersID;
            
            
            GLuint createBuffer();
            
        public:
            explicit VertexArrayObject(size_t vertexCount) : vertexCount(vertexCount) {
                glGenVertexArrays(1, &vaoID);
            }
            
            ~VertexArrayObject() {
                glDeleteVertexArrays(1, &vaoID);
                glDeleteBuffers((GLsizei)buffersID.size(), buffersID.data());
            }
            
            inline void bind() const {
                glBindVertexArray(vaoID);
            }
            
            inline size_t getVertexCount() const {
                return vertexCount;
            }
            
            void bindPositions(GLfloat *data);
            
            void bindNormals(GLfloat *data);
            
            void bindTexCoords2D(GLfloat *data);
            
            void bindBoneIDs(GLuint *data, GLint weightsPerVertex);
            
            
            
        };
        
    }
    
}

#endif
