//
// => gcore/model/fdmd_loader.cpp
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

#include <gcore/graphics/model/fdmd_loader.h>
#include <gcore/graphics/model/model.h>
#include <gcore/graphics/model/skeleton.h>
#include <gcore/graphics/model/animation.h>
#include <gcore/io/bin_istream.h>

#include <cstdlib>
#include <cstdint>

#include <iostream>

using namespace gcore;

typedef enum : GLuint {
    OGLVertexAttribPosition = 0,
    OGLVertexAttribNormal = 1,
    OGLVertexAttribTexCoord2 = 2,
    OGLVertexAttribBoneID = 3,
    OGLVertexAttribBoneWeight = 4
} OGLVertexAttrib;

SkeletonBone *Skeleton::readNode(BinaryInputStream &is) {
    
    bool isBone = is.readByte() == FDMDSkeletonNodeBone; // bone or node?
    
    uint32_t boneID = is.readInt32();
    
    SkeletonBone *newBone = new SkeletonBone(*this, boneID, is.readMat4());
    
    if (isBone) {
        newBone->offsetMatrix = is.readMat4();
    }
    

    bones[boneID] = newBone;

    uint32_t childrenCount = is.readInt32();
    for (uint32_t i = 0; i < childrenCount; i++) {
        newBone->addChild(readNode(is));
    }
    
    return newBone;
}


Model *Model::fromFile(const char *fileName) {
    BinaryInputStream is(fileName);

	if (!is.good()) {
		return nullptr;
	}
    
    Model *model = new Model();
    
    Skeleton *skel = nullptr;
    
    uint32_t meshIndex = 0;
    
    uint32_t meshCount = model->meshCount = is.readByte();
    uint32_t animCount = model->_animCount = is.readByte();
    
    model->vaos = new VertexArrayObject *[meshCount];
    model->_animations = new Animation *[animCount];
    
    uint32_t modelAttrib;
    while ((modelAttrib = is.readByte()) != FDMDModelAttribEndFile) {
        if (modelAttrib == FDMDModelAttribMesh) {
            
            is.readByte(); // MeshID

            
            uint32_t vertexCount = is.readInt32();
            VertexArrayObject &theVAO = *(model->vaos[meshIndex] = new VertexArrayObject(vertexCount));
            theVAO.bind();
            
            
            uint32_t vertexAttrib;
            while ((vertexAttrib = is.readByte()) != FDMDModelVertexAttribEndMesh) {
                switch (vertexAttrib) {
                    case FDMDModelVertexAttribPosition: {
                        GLfloat *vertexData = (GLfloat *)malloc(vertexCount * sizeof(GLfloat) * 3);
                        
                        uint64_t j = 0;
                        for (uint32_t i = 0; i < vertexCount; i++) {
                            vertexData[j++] = is.readFloat();
                            vertexData[j++] = is.readFloat();
                            vertexData[j++] = is.readFloat();
                        }
                        
                        theVAO.bindPositions(vertexData);
                        free(vertexData);
                        break;
                    }
                        
                    case FDMDModelVertexAttribNormal: {
                        GLfloat *normalData = (GLfloat *)malloc(vertexCount * sizeof(GLfloat) * 3);
                        
                        uint64_t j = 0;
                        for (uint32_t i = 0; i < vertexCount; i++) {
                            normalData[j++] = is.readFloat();
                            normalData[j++] = is.readFloat();
                            normalData[j++] = is.readFloat();
                        }
                        
                        theVAO.bindNormals(normalData);
                        free(normalData);
                        break;
                    }
                        
                    case FDMDModelVertexAttribTexCoord2: {
                        is.readByte(); // texIndex
                        
                        GLfloat *uvData = (GLfloat *)malloc(vertexCount * sizeof(GLfloat) * 2);
                        
                        uint64_t j = 0;
                        for (uint32_t i = 0; i < vertexCount; i++) {
                            uvData[j++] = is.readFloat();
                            uvData[j++] = is.readFloat();
                        }
                        
                        theVAO.bindTexCoords2D(uvData);
                        free(uvData);
                        break;
                    }
                        
                    case FDMDModelVertexAttribBoneID: {
                        is.readByte(); // assuming 4 weights per vertex
                        
                        GLuint *boneData = (GLuint *)malloc(vertexCount * sizeof(GLuint) * MAX_WEIGHTS_PER_VERTEX);
                
                        uint64_t j = 0;
                        for (uint32_t i = 0; i < vertexCount; i++) {
                            boneData[j++] = is.readInt32();
                            boneData[j++] = is.readInt32();
                            boneData[j++] = is.readInt32();
                            boneData[j++] = is.readInt32();
                        }
                        
                        theVAO.bindBoneIDs(boneData, MAX_WEIGHTS_PER_VERTEX);
                        free(boneData);
                        break;
                    }
                        
                    case FDMDModelVertexAttribBoneWeight: {
                        GLuint _boneWeightBuffer;
                        glGenBuffers(1, &_boneWeightBuffer); // Bone Weights
                        glBindBuffer(GL_ARRAY_BUFFER, _boneWeightBuffer);
                        
                        is.readByte(); // assuming 4 weights per vertex
                        
                        GLfloat *boneData = (GLfloat *)malloc(vertexCount * sizeof(GLfloat) * MAX_WEIGHTS_PER_VERTEX);
                        
                        uint64_t j = 0;
                        for (uint32_t i = 0; i < vertexCount; i++) {
                            boneData[j++] = is.readFloat();
                            boneData[j++] = is.readFloat();
                            boneData[j++] = is.readFloat();
                            boneData[j++] = is.readFloat();
                        }

                        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat) * MAX_WEIGHTS_PER_VERTEX, boneData, GL_STATIC_DRAW);
                        free(boneData);
                        
                        glEnableVertexAttribArray(OGLVertexAttribBoneWeight);
                        glVertexAttribPointer(OGLVertexAttribBoneWeight, MAX_WEIGHTS_PER_VERTEX, GL_FLOAT, GL_FALSE, 0, (void*)(0));
                        
                        break;
                    }

                    default: break;
                }

            }
            glBindVertexArray(0);
            meshIndex++;

        } else if (modelAttrib == FDMDModelAttribSkeleton) {
            skel = model->_skeleton = new Skeleton(is.readInt32(), is.readInt32()); // boneCount, nodeCount
            
            skel->finalTransform = is.readMat4();
            skel->setRootBone(skel->readNode(is));
            skel->resetAllJoints();
        } else if (modelAttrib == FDMDModelAttribAnimation) {
            uint32_t animID = is.readByte();
            
            Animation *anim = model->_animations[animID] = new Animation(animID, is.readFloat()); // ID, duration
            
            uint32_t chanCount = anim->keyChannelsCount = is.readInt32();
            anim->keyChannels = new KeyFrameChannel *[chanCount];
            
            is.readByte(); // Skeletal animation
            for (uint32_t chanIndex = 0; chanIndex < chanCount; chanIndex++) {

                SkeletonBone *affectedBone = skel->bones[is.readByte()];
                
                KeyFrameChannel *keyChannel = anim->keyChannels[chanIndex] = new KeyFrameChannel(*anim, *affectedBone);
                keyChannel->preState = (AnimationBehaviour) is.readByte();
                keyChannel->postState = (AnimationBehaviour) is.readByte();
                
                if (uint32_t keysCount = is.readByte()) {
                    keyChannel->posKeyCount = keysCount;
                    keyChannel->posKeys = new VectorKey[keysCount];
                    
                    VectorKey *&keyVector = keyChannel->posKeys;
                    for (uint32_t i = 0; i < keysCount; i++) {
                        keyVector[i].t = is.readFloat();
                        keyVector[i].v = glm::vec3(is.readFloat(), is.readFloat(), is.readFloat());
                    }
                    
                }
                
                if (uint32_t keysCount = is.readByte()) {
                    keyChannel->rotKeyCount = keysCount;
                    keyChannel->rotKeys = new QuaternionKey[keysCount];
                    
                    QuaternionKey *&keyVector = keyChannel->rotKeys;
                    for (uint32_t i = 0; i < keysCount; i++) {
                        keyVector[i].t = is.readFloat();
                        keyVector[i].q = glm::quat(is.readFloat(), is.readFloat(), is.readFloat(), is.readFloat());
                    }
                    
                }
                
                if (uint32_t keysCount = is.readByte()) {
                    keyChannel->scalKeyCount = keysCount;
                    keyChannel->scalKeys = new VectorKey[keysCount];
                    
                    VectorKey *&keyVector = keyChannel->scalKeys;
                    for (uint32_t i = 0; i < keysCount; i++) {
                        keyVector[i].t = is.readFloat();
                        keyVector[i].v = glm::vec3(is.readFloat(), is.readFloat(), is.readFloat());
                    }
                    
                }
            }
            
        } else {
            std::cout << "no!" << std::endl;
        }
        
    }
    
    return model;
}


