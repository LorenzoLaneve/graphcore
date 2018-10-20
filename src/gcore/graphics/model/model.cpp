//
// => gcore/model/model.cpp
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

#include <gcore/graphics/model/model.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

using namespace gcore;

void Model::update(double dt) {
    
    _animations[0]->update(dt);
    
    _skeleton->resetAllJoints();
    
}

void Model::draw(GLint jointsUniform) {

    glUniformMatrix4fv(jointsUniform, _skeleton->bonesCount, 0, glm::value_ptr(_skeleton->joints[0]));
    
    for (int i = 0; i < meshCount; i++) {
        vaos[i]->bind();
        glDrawArrays(GL_TRIANGLES, 0, vaos[i]->getVertexCount());
    }
    
}
