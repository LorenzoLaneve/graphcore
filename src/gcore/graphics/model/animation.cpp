//
// => gcore/model/animation.cpp
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

#include <gcore/graphics/model/animation.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <math.h>

#include <iostream>

#define DOT_THRESHOLD 0.9995

using namespace gcore;


void KeyFrameChannel::refreshNextKeyFrames(double t) {
    
    double T = animation.getTotalDuration();
    t = fmod(t, T);
    
    double revert = t;
    
    
    uint32_t i;
    
    i = currentKeyFrame.nextPosKey;
    if (posKeys) {
        while (posKeys[i].t < t) {
            i = (i+1) % posKeyCount;
            //if (!i) t -= T;
        }
        currentKeyFrame.nextPosKey = i;
    }
    
    t = revert;
    i = currentKeyFrame.nextRotKey;
    if (rotKeys) {
        while (rotKeys[i].t < t) {
            i = (i+1) % rotKeyCount;
            //if (!i) t -= T;
        }
        currentKeyFrame.nextRotKey = i;
    }
    
    t = revert;
    i = currentKeyFrame.nextScalKey;
    if (scalKeys) {
        while (scalKeys[i].t < t) {
            i = (i+1) % scalKeyCount;
            //if (!i) t -= T;
        }
        currentKeyFrame.nextScalKey = i;
    }
}


glm::vec3 interpolate(glm::vec3 last, glm::vec3 next, float t) {
    return last + t * next;
}




inline uint32_t getLastKeyFrame(const uint32_t next, const uint32_t keyCount) {
    return (next > 0 ? next : keyCount) - 1;
}


glm::mat3 quat_to_mat(glm::quat q) {
    glm::mat3 ret;
    ret[0][0] = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    ret[1][0] = 2.0f * (q.x * q.y - q.z * q.w);
    ret[2][0] = 2.0f * (q.x * q.z + q.y * q.w);
    ret[0][1] = 2.0f * (q.x * q.y + q.z * q.w);
    ret[1][1] = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
    ret[2][1] = 2.0f * (q.y * q.z - q.x * q.w);
    ret[0][2] = 2.0f * (q.x * q.z - q.y * q.w);
    ret[1][2] = 2.0f * (q.y * q.z + q.x * q.w);
    ret[2][2] = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    
    return ret;
}


glm::mat4 KeyFrameChannel::interpolateJoint(double t) {
    
    refreshNextKeyFrames(t);
    
    const KeyFrame &current = currentKeyFrame;
    
    uint32_t nextKey;
    uint32_t lastKey;
    
    glm::vec3 pos = posKeys[current.nextPosKey].v;
    glm::quat rot = rotKeys[current.nextRotKey].q;
    glm::vec3 scal = scalKeys[current.nextScalKey].v;
    

    glm::mat4 ret = glm::mat4_cast(rot);// * glm::scale(glm::mat4(), scal);
    
    
    ret[0][0] *= scal.x; ret[1][0] *= scal.y; ret[2][0] *= scal.z; ret[3][0] = pos.x;
    ret[0][1] *= scal.x; ret[1][1] *= scal.y; ret[2][1] *= scal.z; ret[3][1] = pos.y;
    ret[0][2] *= scal.x; ret[1][2] *= scal.y; ret[2][2] *= scal.z; ret[3][2] = pos.z;
    
    return ret;
}



void Animation::update(double dt) {
    
    elapsed += dt;
    
    for (uint32_t i = 0; i < keyChannelsCount; i++)
    {
        KeyFrameChannel &keyChannel = *keyChannels[i];
        
        keyChannel.getAffectedBone().joint() = keyChannel.interpolateJoint(elapsed);
    }
    
}
