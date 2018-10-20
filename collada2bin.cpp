//
// => collada2bin.cpp
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

#include <iostream>
#include <fstream>
#include <string>

#include <netinet/in.h>

#include "scene.h"
#include "Importer.hpp"
#include "postprocess.h"

#define min(a,b) (a>b ? b : a)

using namespace std;
using namespace Assimp;

enum : unsigned int {
    TYPE_ENDFILE = 0,
    TYPE_MESH = 1,
};

enum : unsigned int {
    VERTEX_ATTRIB_ENDMESH = 0,
    VERTEX_ATTRIB_POS = 1,
    VERTEX_ATTRIB_NORMAL = 2,
    VERTEX_ATTRIB_UV2 = 3,
    VERTEX_ATTRIB_UV3 = 4,
    VERTEX_ATTRIB_COLOR = 5
};

void writeByte(ostream &os, uint8_t x) {
    os.write((const char *)&x, 1);
}

void writeInt16(ostream &os, uint16_t x) {
    const uint8_t data[2] {
        (uint8_t)((x >> 8) & 255),
        (uint8_t)(x & 255)
    };
    
    os.write((const char *)data, 2);
}

void writeInt32(ostream &os, uint32_t x) {
    const uint8_t data[4] = {
        (uint8_t)((x >> 24) & 255),
        (uint8_t)((x >> 16) & 255),
        (uint8_t)((x >> 8) & 255),
        (uint8_t)(x & 255)
    };
    os.write((const char *)data, 4);
}

void writeFloat(ostream &os, float x) {
    uint32_t ix = *((uint32_t *)&x);
    const uint8_t data[4] = {
        (uint8_t)((ix >> 24) & 255),
        (uint8_t)((ix >> 16) & 255),
        (uint8_t)((ix >> 8) & 255),
        (uint8_t)(ix & 255)
    };
    os.write((const char *)data, 4);
}

inline void writeVec3(ostream &os, float x, float y, float z) {
    writeFloat(os, z);
    writeFloat(os, y);
    writeFloat(os, -x);
}

inline void writeVec2(ostream &os, float x, float y) {
    writeFloat(os, x);
    writeFloat(os, y);
}


int main(int argc, const char * argv[]) {
    
    if (argc < 2) {
        cout << "File name not specified." << endl;
        return 0;
    }
    
    Importer imp;
    const aiScene *scene = imp.ReadFile(argv[1], aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
    
    ofstream os(std::string(argv[1]) + ".mdl", ios::binary);
    
    writeByte(os, scene->mNumMeshes);
    writeByte(os, 0); // animations count
    
    for (int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[i];
        
        
        writeByte(os, TYPE_MESH); // Type = MESH
        writeByte(os, i); // MeshID
        writeInt32(os, mesh->mNumVertices); // Number of vertices
        
        if (mesh->HasPositions()) {
            writeByte(os, VERTEX_ATTRIB_POS); // VertexAttrib = POSITION

            for (int j = 0; j < mesh->mNumVertices; j++) {
                aiVector3D &vertex = mesh->mVertices[j];
                
                writeVec3(os, vertex.x, vertex.y, vertex.z);
            }
        }
        
        if (mesh->HasNormals()) {
            writeByte(os, VERTEX_ATTRIB_NORMAL); // VertexAttrib = NORMAL
            
            for (int j = 0; j < mesh->mNumVertices; j++) {
                aiVector3D &normal = mesh->mNormals[j];
                
                writeVec3(os, normal.x, normal.y, normal.z);
            }
        }
        
        
        int texIndex = 0;
        while (mesh->HasTextureCoords(texIndex)) {
            writeByte(os, VERTEX_ATTRIB_UV2); // VertexAttrib = UV2
            writeByte(os, texIndex); // TextureIndex
            
            for (int j = 0; j < mesh->mNumVertices; j++) {
                aiVector3D &texCoords = mesh->mTextureCoords[texIndex][j];
                
                writeVec2(os, texCoords.x, texCoords.y);
            }
            
            texIndex++;
        }
        
        writeByte(os, VERTEX_ATTRIB_ENDMESH);
    }
    writeByte(os, TYPE_ENDFILE);

    return 0;
}
