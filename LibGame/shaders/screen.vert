#version 460 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(aPosition.xy, 0.0, 1.0);
	TexCoords = aTexCoord;
}