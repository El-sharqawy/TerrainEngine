#version 460 core

layout(location = 0) out vec4 FragColor;

in vec2 TexTes;
in vec4 v4BlendWeights;
in vec3 WorldPos;
in vec3 NormalsPos;
in float fHeight;

uniform sampler2D TextureHeight0;
uniform sampler2D TextureHeight1;
uniform sampler2D TextureHeight2;
uniform sampler2D TextureHeight3;

uniform sampler2D HeightMapTex;
uniform sampler2D BlendMapTex;

uniform float fHeight0 = 80.0f;
uniform float fHeight1 = 250.0f;
uniform float fHeight2 = 350.0f;
uniform float fHeight3 = 450.0f;

uniform vec3 v3LightDir; // this should be reversed
uniform vec3 v3LightColor;

uniform float fColorTexcoordScaling = 16.0;
uniform float fTextureRadius = 0.5; // Radius for the circular texture area
uniform float fBrushEdgeSoftness = 0.1; // Softness for the brush edge (tweak this value)

uniform vec3 u_HitPosition;
uniform float u_HitRadius = 50.0f;
uniform bool u_HasHit = false;

uniform float fBlendMapScale = 1.0 / 512.0; // world scale per texel (adjust to terrain size)

vec4 CalculateTexturesColors()
{
    // Always calculate base terrain color
    vec2 ScaledTexCoord = TexTes * fColorTexcoordScaling;

    // 1. Define base color/texture for unpainted areas
    //vec4 baseColor = vec4(1.0, 1.0, 1.0, 1.0); // Green default
    // OR use a texture:
    vec4 baseColor = texture(TextureHeight0, ScaledTexCoord);

    // Sample textures
    vec4 tex0 = texture(TextureHeight0, ScaledTexCoord);
    vec4 tex1 = texture(TextureHeight1, ScaledTexCoord);
    vec4 tex2 = texture(TextureHeight2, ScaledTexCoord);
    vec4 tex3 = texture(TextureHeight3, ScaledTexCoord);

    // Base color from blended textures
    vec4 blendedColor = tex0 * v4BlendWeights.r + tex1 * v4BlendWeights.g + tex2 * v4BlendWeights.b + tex3 * v4BlendWeights.a;

    // Add brush visualization if active
    /*if (u_HasHit)
    {
        float dist = length(WorldPos.xz - u_HitPosition.xz);
        float circle = 1.0 - smoothstep(u_HitRadius - 2.0, u_HitRadius, dist);
        blendedColor = mix(blendedColor, vec4(1.0, 0.0, 0.0, 1.0), circle * 0.5); // Red overlay
    }*/

    float totalWeights = v4BlendWeights.r + v4BlendWeights.g 
                       + v4BlendWeights.b + v4BlendWeights.a;

    float blendAlpha = clamp(totalWeights, 0.0, 1.0);

    return mix(baseColor, blendedColor, blendAlpha);
}

vec3 CalcNormals()
{
    float fLeft = textureOffset(HeightMapTex, TexTes, ivec2(-1, 0)).r;
    float fRight = textureOffset(HeightMapTex, TexTes, ivec2(1, 0)).r;
    float fBottom = textureOffset(HeightMapTex, TexTes, ivec2(0, -1)).r;
    float fTop = textureOffset(HeightMapTex, TexTes, ivec2(0, 1)).r;

    vec3 v3Normal = normalize(vec3(fLeft - fRight, 2.0f, fTop - fBottom));

    return (v3Normal);
}

void main()
{
    vec4 TexColor = CalculateTexturesColors();
    vec3 Normals = CalcNormals();
    //vec3 Normals = normalize(NormalsPos);

    float fDiffuse = dot(Normals, v3LightDir);
    if (fDiffuse < 0.0f)
    {
        fDiffuse = 0.0f;
    }
    fDiffuse = max(0.2f, fDiffuse);
    vec4 finalColor = TexColor * vec4(v3LightColor, 1.0f) * fDiffuse;

    if (u_HasHit)
    {
        // Soft circle edge
        // Distance from this fragment to the brush center
        float dist = length(WorldPos.xz - u_HitPosition.xz);

        float edge = 2.0; // fade distance in units
        float circle = 1.0 - smoothstep(u_HitRadius - edge, u_HitRadius, dist);

        float thickness = 1.0;
        float ring = smoothstep(u_HitRadius - thickness, u_HitRadius, dist) * 
             (1.0 - smoothstep(u_HitRadius, u_HitRadius + thickness, dist));

        // Apply the circle effect (e.g., a red ring)
        vec3 baseColor = vec3(finalColor); // normal terrain color
        vec3 circleColor = mix(baseColor, vec3(1.0, 0.2, 0.2), ring);

        FragColor = vec4(circleColor, 1.0f);
    }
    else
    {
        FragColor = finalColor;
    }
}
