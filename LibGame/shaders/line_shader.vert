#version 460 core

layout (location = 0) in vec3 aPos;     // Vertex position
layout (location = 1) in vec4 aColors;     // Vertex position

uniform mat4 ViewMatrix;                // Line color
out vec4 v4Color;

void main()
{
    gl_Position = ViewMatrix * vec4(aPos, 1.0);
    v4Color = aColors;
}
