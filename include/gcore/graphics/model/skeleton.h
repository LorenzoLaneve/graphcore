//
// => gcore/model/skeleton.h
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

#ifndef __graphcore_graphics_model_skeleton
#define __graphcore_graphics_model_skeleton

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gcore {
    
    class BinaryInputStream;
    class Model;
    class Skeleton;
    class Animation;
    
    class SkeletonBone {
        friend class Skeleton;
        
        Skeleton &skeleton;
        
        uint32_t boneID;
        
        glm::mat4 bindPose;
        glm::mat4 transform;
        glm::mat4 offsetMatrix;
        
        SkeletonBone *parent = nullptr;
        std::vector<SkeletonBone *> children;
        
    public:
        inline SkeletonBone(Skeleton &skeleton, uint32_t boneID, glm::mat4 transform) : skeleton(skeleton), boneID(boneID), transform(transform), bindPose(transform) {  }

        inline Skeleton &getSkeleton() const {
            return skeleton;
        }
        
        inline glm::mat4 &joint() {
            return transform;
        }
        
        inline uint32_t getBoneID() const {
            return boneID;
        }
        
        inline const glm::mat4 &getNodeTransform() const {
            return transform;
        }
        
        inline const glm::mat4 &getOffsetMatrix() const {
            return offsetMatrix;
        }
        
        inline const glm::mat4 &getBindPose() const {
            return bindPose;
        }
        
        inline void setTransform(glm::mat4 newTransform) {
            transform = bindPose * newTransform;
        }
        
        inline SkeletonBone *getParent() const {
            return parent;
        }
        
        inline const std::vector<SkeletonBone *> &getChildren() const {
            return children;
        }
        
        inline void addChild(SkeletonBone *newChild) {
            children.push_back(newChild);
            newChild->parent = this;
        }
        
    };
    
    class Skeleton {
        friend class Model;
        
        SkeletonBone *rootBone;
        
        uint32_t bonesCount;
        SkeletonBone **bones;
        glm::mat4 *joints;
        
        glm::mat4 finalTransform;
        
        
        SkeletonBone *readNode(BinaryInputStream &is);
        
    public:
        Skeleton(uint32_t bonesCount, uint32_t nodesCount) : bonesCount(bonesCount) {
            bones = new SkeletonBone *[nodesCount];
            joints = new glm::mat4[bonesCount];
        }
        
        ~Skeleton() {
            delete[] bones;
            delete[] joints;
        }
        
        inline void setRootBone(SkeletonBone *bone) {
            rootBone = bone;
        }
        
        inline const glm::mat4 &getFinalTransform() const {
            return finalTransform;
        }
        
        inline SkeletonBone *operator[](uint32_t boneID) const {
            return bones[boneID];
        }
        
        void resetJoint(uint32_t boneID);
        
        void resetAllJoints();
        
    };
    
}

#endif
