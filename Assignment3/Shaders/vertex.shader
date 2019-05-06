#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec3 lightPosition;

void main()
{
    fragmentPosition = vec3(view * vec4(position, 1.0));
    fragmentNormal = mat3(transpose(inverse(view))) * normal;
    lightPosition = lightPos;

    gl_Position = model * view * projection * vec4(position, 1.0f);
}
