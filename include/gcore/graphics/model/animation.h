//
// => gcore/model/animation.h
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

#ifndef __graphcore_graphics_model_animations
#define __graphcore_graphics_model_animations

#include <gcore/graphics/model/skeleton.h>

#include <glm/glm.hpp>

#include <cstdint>

namespace gcore {
    
    typedef enum : uint8_t {
        AnimationBehaviourToBindPose = 0,
        AnimationBehaviourConstant = 1,
        AnimationBehaviourLinear = 2,
        AnimationBehaviourRepeat = 3
    } AnimationBehaviour;
    
    class Model;
    
    struct VectorKey {
        glm::vec3 v;
        float t;
    };
    
    struct QuaternionKey {
        glm::quat q;
        float t;
    };
    
    
    struct KeyFrame {
        uint32_t nextPosKey = 0;
        uint32_t nextRotKey = 0;
        uint32_t nextScalKey = 0;
    };
    
    
    class KeyFrameChannel {
        friend class Model;
        
        Animation &animation;
        
        SkeletonBone &affectedBone;
        
        AnimationBehaviour preState;
        AnimationBehaviour postState;
        
        
        uint32_t posKeyCount;
        uint32_t rotKeyCount;
        uint32_t scalKeyCount;
        
        VectorKey *posKeys;
        QuaternionKey *rotKeys;
        VectorKey *scalKeys;
    
        KeyFrame currentKeyFrame;
        
        void refreshNextKeyFrames(double t);
        
        
    public:
        KeyFrameChannel(Animation &animation, SkeletonBone &affectedBone) : animation(animation), affectedBone(affectedBone) {  }
        
        ~KeyFrameChannel() {
            if (posKeys) delete posKeys;
            if (rotKeys) delete rotKeys;
            if (scalKeys) delete scalKeys;
        }
        
        inline SkeletonBone &getAffectedBone() const {
            return affectedBone;
        }
        
        glm::mat4 interpolateJoint(double t);
        
    };
    
    class Animation {
        friend class Model;
        
        uint32_t animID;
        
        double elapsed = 0;
        float totalDuration;
        
        uint32_t keyChannelsCount;
        KeyFrameChannel **keyChannels = nullptr;
        
    public:
        Animation(uint32_t animID, float totalDuration) : animID(animID), totalDuration(totalDuration) {  }
        
        ~Animation() {
            delete keyChannels;
        }
        
        inline uint32_t getAnimationID() const {
            return animID;
        }
        
        inline float getTotalDuration() const {
            return totalDuration;
        }
        
        void update(double dt);
        
    };
    
}


#endif
