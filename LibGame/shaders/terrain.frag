#version 460 core

layout(location = 0) out vec4 FragColor;

in vec2 TexTes;
in vec3 WorldPos;
in float fHeight;

uniform sampler2D TextureHeight0;
uniform sampler2D TextureHeight1;
uniform sampler2D TextureHeight2;
uniform sampler2D TextureHeight3;

uniform sampler2D HeightMapTex;

uniform float fHeight0 = 80.0f;
uniform float fHeight1 = 250.0f;
uniform float fHeight2 = 350.0f;
uniform float fHeight3 = 450.0f;

uniform vec3 v3LightDir; // this should be reversed
uniform vec3 v3LightColor;

uniform float fColorTexcoordScaling = 16.0;

vec4 CalculateTexturesColors()
{
    vec4 TexColor;

    vec2 ScaledTexCoord = TexTes * fColorTexcoordScaling;

    if (fHeight < fHeight0)
    {
        TexColor = texture(TextureHeight0, ScaledTexCoord);
    }
    else if (fHeight < fHeight1)
    {
        vec4 Color0 = texture(TextureHeight0, ScaledTexCoord);
        vec4 Color1 = texture(TextureHeight1, ScaledTexCoord);
        float fDelta = fHeight1 - fHeight0;
        float fFactor = (fHeight - fHeight0) / fDelta;
        TexColor = mix(Color0, Color1, fFactor);
    }
    else if (fHeight < fHeight2)
    {
        vec4 Color0 = texture(TextureHeight1, ScaledTexCoord);
        vec4 Color1 = texture(TextureHeight2, ScaledTexCoord);
        float fDelta = fHeight2 - fHeight1;
        float fFactor = (fHeight - fHeight1) / fDelta;
        TexColor = mix(Color0, Color1, fFactor);
    }
    else if (fHeight < fHeight3)
    {
        vec4 Color0 = texture(TextureHeight2, ScaledTexCoord);
        vec4 Color1 = texture(TextureHeight3, ScaledTexCoord);
        float fDelta = fHeight3 - fHeight2;
        float fFactor = (fHeight - fHeight2) / fDelta;
        TexColor = mix(Color0, Color1, fFactor);
    }
    else
    {
        TexColor = texture(TextureHeight3, ScaledTexCoord);
    }
    return (TexColor);
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

    float fDiffuse = dot(Normals, v3LightDir);
    if (fDiffuse < 0.0f)
    {
        fDiffuse = 0.0f;
    }
    fDiffuse = max(0.2f, fDiffuse);

    FragColor = TexColor * vec4(v3LightColor, 1.0f) * fDiffuse;
}
