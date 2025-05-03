#version 460 core

#extension GL_ARB_bindless_texture : require
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) out vec4 FragColor;

// Main textures
layout(std430, binding = 0) buffer TextureHandles
{ 
    sampler2D textures[];
};

 // Splat maps Textures
layout(std430, binding = 1) buffer IndexHandles
{
    usampler2D indexMaps[]; // [indexMap0, indexMap1, ...]
};

layout(std430, binding = 2) buffer WeightHandles
{
    sampler2D weightMaps[]; // [indexMap0, weightMap0, indexMap1, weightMap1...]
};

in vec3 v3WorldPos;
in vec2 v2TexCoord;
in vec3 v3Normal;

uniform int iPatchIndex;

uniform vec3 v3LightDirection;   // Direction of the light
uniform vec3 v3LightColor;       // Color of the light
uniform vec3 v3AmbientColor;     // Ambient light color
uniform vec3 v3CameraPosition;   // Camera position (for specular lighting)
uniform float fShininess;        // Shininess factor for specular highlights

uniform vec3 u_HitPosition;
uniform float u_HitRadius = 50.0f;
uniform bool u_HasHit = false;

uniform float fColorTexcoordScaling = 1.0; // Number of times to be repeated per patch!

uniform vec2 numPatches;      // pass (m_iNumPatchesX, m_iNumPatchesZ)


void main()
{
    // 1) compute patch-local UV in [0,1]
    vec2 uv_patch = fract(v2TexCoord * numPatches);

    // 2) fetch indices & weights
    uvec4 texIndices = texture(indexMaps[iPatchIndex], uv_patch);
    vec4  weights    = texture(weightMaps[iPatchIndex], uv_patch);

    // 3) normalize weights so sum == 1
    float tot = dot(weights, vec4(1.0));

    if (tot > 0.0)
    {
        weights /= tot;
    }

    // 4) blend up to 4 layers
    vec3 albedo = vec3(0.0);
    for (int i = 0; i < 4; ++i)
    {
        uint ti = texIndices[i];
        if (weights[i] > 0.001 && ti < textures.length())
        {
            albedo += texture(textures[ti], uv_patch * fColorTexcoordScaling).rgb * weights[i];
        }
    }

    // 5) simple phong lighting
    vec3 N = normalize(v3Normal);
    vec3 L = normalize(v3LightDirection);
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * v3LightColor * albedo;
    vec3 ambient = v3AmbientColor * albedo;

    // specular
    vec3 V = normalize(v3CameraPosition - v3WorldPos);
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(R, V), 0.0), fShininess);
    vec3 specular = spec * v3LightColor;

    float totalWeight = weights[0] + weights[1] + weights[2] + weights[3];
    if (totalWeight > 0.0)
	{
        FragColor = vec4(albedo, totalWeight);
	}
    else
    {
        FragColor = texture(textures[0], uv_patch * fColorTexcoordScaling);
    }
}