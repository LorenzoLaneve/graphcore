//
// => gcore/model/fdmd_loader.h
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

#ifndef __graphcore_graphics_fdmd_format
#define __graphcore_graphics_fdmd_format

#include <cstdint>

/*!
 \brief The max number of bone weights per vertex that can be supported.
 */
#define MAX_WEIGHTS_PER_VERTEX 4

namespace gcore {
    
    /*!
     \brief Values indicating different objects that can be stored in the FDMD file format.
     \warning NEVER CHANGE THE VALUES SINCE THEY CONFORM TO THE FDMD FILE FORMAT SPECIFICATION.
     */
    typedef enum : uint8_t {
        FDMDModelAttribEndFile = 0,
        FDMDModelAttribMesh = 1,
        FDMDModelAttribSkeleton = 2,
        FDMDModelAttribAnimation = 3
    } FDMDModelAttrib;
    
    /*!
     \brief Values indicating different vertex attributes that can be stored in the FDMD file format.
     \warning NEVER CHANGE THE VALUES SINCE THEY CONFORM TO THE FDMD FILE FORMAT SPECIFICATION.
     */
    typedef enum : uint8_t {
        FDMDModelVertexAttribEndMesh = 0,
        FDMDModelVertexAttribPosition = 1,
        FDMDModelVertexAttribNormal = 2,
        FDMDModelVertexAttribTexCoord2 = 3,
        FDMDModelVertexAttribTexCoord3 = 4,
        FDMDModelVertexAttribColor = 5,
        FDMDModelVertexAttribBoneID = 6,
        FDMDModelVertexAttribBoneWeight = 7
    } FDMDVertexAttrib;
    
    /*!
     \brief Values indicating different node types that can be stored in the FDMD file format.
     \warning NEVER CHANGE THE VALUES SINCE THEY CONFORM TO THE FDMD FILE FORMAT SPECIFICATION.
     */
    typedef enum : uint8_t {
        FDMDSkeletonNodeEmpty = 1,
        FDMDSkeletonNodeBone = 2
    } FDMDSkeletonNode;
    
    
    class FDMDLoader {
        
        
    public:
        FDMDLoader();
        
    };
    
}


#endif
