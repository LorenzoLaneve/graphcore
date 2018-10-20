#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 compNormal;
out vec2 fragTexCoords;

uniform mat4 mvp;
uniform mat4 normalMatrix;

void main() {
    gl_Position = mvp * vec4(vertex, 1.0);
    
    compNormal = normalize(vec3(normalMatrix * vec4(normal, 0.0)));
    
    fragTexCoords = texCoords;
}
