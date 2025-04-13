#version 460 core

layout (location = 0) in vec3 aPosition;	// position has attribute position 0
layout (location = 1) in vec2 aTexture;		// textures has attribute position 1

out vec2 Tex;  // This matches the TCS input Tex[]

uniform mat4 ViewMatrix;
uniform vec4 v4ClipPlane;

void main()
{
    gl_Position = vec4(aPosition, 1.0f);
    Tex = aTexture;

    gl_ClipDistance[0] = dot(vec4(aPosition, 1.0), v4ClipPlane);
}