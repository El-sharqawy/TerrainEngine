#version 460 core

layout (location = 0) in vec3 v3Position;					// position has attribute position 0
layout (location = 1) in vec2 v2TexCoords;					// textures has attribute position 1
layout (location = 2) in vec3 v3Normals;					// normals has attribute position 2

out vec3 WorldPos_CS_in;                                                                        
out vec2 TexCoord_CS_in;                                                                        
out vec3 Normal_CS_in;
flat out int InstanceID_CS_in;			// flat qualifier for integer varying
out float TextureBlendFactor_CS_in;

void main()
{
	WorldPos_CS_in =  v3Position;
	TexCoord_CS_in = v2TexCoords;
	Normal_CS_in = v3Normals;
}