#version 460 core

#extension GL_ARB_bindless_texture : require

layout(location = 0) out vec4 FragColor;

layout(std430, binding = 0) buffer TextureHandles
{
    sampler2D textures[];
};

layout(std430, binding = 1) buffer PatchSplatBindings
{
    uvec4 patchTextureIDs[];
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

uniform float fColorTexcoordScaling = 16.0;

uniform sampler2D splatmap;

void main()
{
    vec4 splatWeights = texture(splatmap, v2TexCoord * fColorTexcoordScaling);
    uvec4 texIndices = patchTextureIDs[iPatchIndex];

    vec3 color = vec3(0.0);
    float totalWeight = 0.0;

    for (int i = 0; i < 4; ++i)
    {
        if (texIndices[i] != uint(-1))
        {
            vec3 texColor = texture(textures[texIndices[i]], v2TexCoord * fColorTexcoordScaling).rgb;
            color += texColor * splatWeights[i];
            totalWeight += splatWeights[i];
        }
    }

    if (totalWeight > 0.0)
    {
        color /= totalWeight;
    }

    if (u_HasHit)
    {
        float dist = length(v3WorldPos.xz - u_HitPosition.xz);
        float edge = 2.0;
        float circle = 1.0 - smoothstep(u_HitRadius - edge, u_HitRadius, dist);
        float thickness = 1.0;
        float ring = smoothstep(u_HitRadius - thickness, u_HitRadius, dist) * 
                     (1.0 - smoothstep(u_HitRadius, u_HitRadius + thickness, dist));

        vec3 circleColor = mix(color.xyz, vec3(1.0, 0.2, 0.2), ring);
        FragColor = vec4(circleColor, 1.0);
        return;
    }

    FragColor = vec4(color, 1.0);
}