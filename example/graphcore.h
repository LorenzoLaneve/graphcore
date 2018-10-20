//
// => example/graphcore.h
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

#ifndef __game_graphcore
#define __game_graphcore

#include <gcore/window/window.h>

#include <GL/glew.h>
    
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
    
#include <gcore/graphics/shaders/shaders.h>
#include <gcore/graphics/model/textures.h>
#include <gcore/graphics/model/model.h>
    
class GraphCore : public gcore::WindowDrawer {
    
    //gcore::ShaderProgram *triangleProgram;
    gcore::ShaderProgram *skeletonProgram;
    
    gcore::Model *myModel;
    
    glm::mat4 mvp;
    glm::mat4 normalMatrix;
    GLuint charizardTexture;
    
    float t, r;
    
public:
    
    GraphCore(gcore::Window &window) : gcore::WindowDrawer(window) {  }
    
    
    void doInit() {
 
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        
        //glEnable(GL_CULL_FACE);
        
        glClearColor(1.0, 1.0, 0.0, 0.0);

        skeletonProgram = gcore::ShaderProgram::fromSources("skeleton.vsh", "shader.fsh");
        skeletonProgram->addUniform("mvp");
        skeletonProgram->addUniform("normalMatrix");
        skeletonProgram->addUniform("boneJoints");
        skeletonProgram->addUniform("texSampler");
        
        
        myModel = gcore::Model::fromFile("wolf.mdl");
        charizardTexture = gcore::loadTexture("charizard.tga");
        
        t = 0;
        r = 5;
    }
    
    void doResize() {
        
    }
    
    void doUpdate(double dt) {
        
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 viewMatrix = glm::lookAt(glm::vec3(r*cos(t), r*sin(t), 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
        glm::mat4 projectionMatrix = glm::perspective((float)3.14/4, getTargetWindow().getAspectRatio(), 0.1f, 100.0f);
        mvp = projectionMatrix * viewMatrix * modelMatrix;
        normalMatrix = glm::transpose(glm::inverse(modelMatrix));
        
        
        if (getTargetWindow().isKeyPressed(GLFW_KEY_LEFT)) {
            t -= 5 * dt;
        } else if (getTargetWindow().isKeyPressed(GLFW_KEY_RIGHT)) {
            t += 5 * dt;
        } else {
            //t += dt;
        }
        
        if (getTargetWindow().isKeyPressed(GLFW_KEY_UP)) {
            r -= 10 * dt;
            if (r < 1) r = 1;
        } else if (getTargetWindow().isKeyPressed(GLFW_KEY_DOWN)) {
            r += 10 * dt;
        }
        
        myModel->update(dt);
        
    }
    
    void doRender() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        skeletonProgram->use();
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, charizardTexture);
        
        glUniformMatrix4fv(skeletonProgram->getUniform(0), 1, 0, &mvp[0][0]);
        glUniformMatrix4fv(skeletonProgram->getUniform(1), 1, 0, &normalMatrix[0][0]);
        glUniform1i(skeletonProgram->getUniform(3), 0); // texSampler
        
        myModel->draw(skeletonProgram->getUniform(2));
        
        glBindVertexArray(0);

    }
    
    void doDestroy() {
        
        delete skeletonProgram;
        
    }
    
};

#endif
