#version 330 core

in vec3 compNormal;
in vec2 fragTexCoords;

out vec3 color;

uniform sampler2D texSampler;

const vec3 directionalLight = -normalize(vec3(1.0, -1.0, -1.0));

void main() {
    
    float lambertian = max(0, dot(compNormal, -directionalLight));
    
    color = texture(texSampler, vec2(fragTexCoords.x, fragTexCoords.y)).rgb * (lambertian * 0.8 + 0.2);
}
