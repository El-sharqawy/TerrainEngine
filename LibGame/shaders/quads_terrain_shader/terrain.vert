#version 460 core

layout (location = 0) in vec3 aPosition;	    // position has attribute position 0
layout (location = 1) in vec3 aNormals;         // Normals has attribute position 1
layout (location = 2) in vec2 aTexture;		    // textures has attribute position 2
layout (location = 3) in vec4 aTextureBlend;	// position has attribute position 3

out vec3 Normals;  // This matches the TCS input Normals[]
out vec2 TexCoord;  // This matches the TCS input TexCoord[]
out vec4 v4BlendWeights;

uniform mat4 ViewMatrix;
uniform vec4 v4ClipPlane;

void main()
{
    gl_Position = vec4(aPosition, 1.0f);
    Normals = aNormals;
    TexCoord = aTexture;
    v4BlendWeights = aTextureBlend;

    gl_ClipDistance[0] = dot(vec4(aPosition, 1.0), v4ClipPlane);
}