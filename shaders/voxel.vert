#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 instancePos;
layout(location = 2) in vec3 color;

out vec3 fragColor;
flat out int faceID;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec3 worldPos = instancePos + position;
    gl_Position = projection * view * model * vec4(worldPos, 1.0);
    fragColor = color;
    faceID = gl_VertexID / 6;
}
