#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in ivec4 boneID;
layout(location = 4) in vec4 weights;

out vec3 compNormal;
out vec2 fragTexCoords;

uniform mat4 mvp;
uniform mat4 normalMatrix;
uniform mat4 boneJoints[64];

void main() {
    mat4 joint = boneJoints[boneID.x] * weights.x + boneJoints[boneID.y] * weights.y + boneJoints[boneID.z] * weights.z + boneJoints[boneID.w] * weights.w;

    gl_Position = mvp * joint * vec4(position, 1.0);
    
    compNormal = normalize(vec3(normalMatrix * vec4(normal, 0.0)));
    
    fragTexCoords = texCoords;
}
