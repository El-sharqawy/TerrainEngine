#version 460 core

layout (location = 0) out vec4 FragColor;

in vec2 v2Tex;
in vec4 v4ClipSpaceCoords;
in vec3 v3VertexToCamera;

uniform sampler2D ReflectionTexture;
uniform sampler2D RefractionTexture;
uniform sampler2D DUDVMapTexture;
uniform sampler2D NormalMapTexture;
uniform sampler2D DepthMapTexture;
uniform float fDUDVMapTextureOffset = 0.0;
uniform vec3 v3LightColor;
uniform vec3 v3ReservedLightDir;

const float fWaveLength = 0.02;
const float fShininess = 50.0;
const float fReflectivity = 1.6;

void main()
{
	vec4 NDDCoords = (v4ClipSpaceCoords / v4ClipSpaceCoords.w) / 2.0f + vec4(0.5f);
	vec2 RefractionTexCoord = vec2(NDDCoords.x, NDDCoords.y);
	vec2 ReflectionTexCoord = vec2(NDDCoords.x, 1.0 - NDDCoords.y);

	float fDepth = texture(DepthMapTexture, RefractionTexCoord).r;

	float fZNear = 0.1f;
	float fZFar = 1000.0f;

	float fFloorDistance = 2.0f * fZNear * fZFar / (fZFar + fZNear - (2.0f * fDepth - 1.0f) * (fZFar - fZNear));
	float fWaterDistance = 2.0f * fZNear * fZFar / (fZFar + fZNear - (2.0f * gl_FragCoord.z - 1.0f) * (fZFar - fZNear));
	float fFloorToWaterSurface = fFloorDistance - fWaterDistance;

	vec2 DUDVMapTexCoordv1 = texture(DUDVMapTexture, vec2(v2Tex.x + fDUDVMapTextureOffset, v2Tex.y)).rg * 0.1f;
	DUDVMapTexCoordv1 = v2Tex + vec2(DUDVMapTexCoordv1.x, DUDVMapTexCoordv1.y + fDUDVMapTextureOffset);
	vec2 DUDVMapTexCoord = (texture(DUDVMapTexture, DUDVMapTexCoordv1).rg * 2.0f - 1.0f) * fWaveLength * clamp(fFloorToWaterSurface / 20.0f, 0.0f, 1.0f);

	ReflectionTexCoord = clamp(ReflectionTexCoord + DUDVMapTexCoord, 0.001f, 0.999f);
	RefractionTexCoord = clamp(RefractionTexCoord + DUDVMapTexCoord, 0.001f, 0.999f);
	vec4 ReflectionColor = texture(ReflectionTexture, ReflectionTexCoord);
	vec4 RefractionColor = texture(RefractionTexture, RefractionTexCoord);

	vec4 NormalColor = texture(NormalMapTexture, DUDVMapTexCoordv1);
	vec3 Normal = vec3(NormalColor.r * 2.0f - 1.0f, NormalColor.b * 4.0f, NormalColor.g * 2.0f - 1.0f);
	Normal = normalize(Normal);

	vec3 ViewVector = normalize(v3VertexToCamera);
	float fRefractiveFactor = dot(ViewVector, Normal);

	vec3 ReflectedLight = reflect(v3ReservedLightDir, Normal);
	float fSpecularFactor = max(dot(ReflectedLight, ViewVector), 0.0f);
	fSpecularFactor = pow(fSpecularFactor, fShininess);
	vec4 SpecularColor = vec4(v3LightColor * fSpecularFactor * fReflectivity * clamp(fFloorToWaterSurface / 10.0f, 0.0f, 1.0f), 0.0f);

	FragColor = mix(ReflectionColor, RefractionColor, 0.55f);
	FragColor = mix(FragColor, vec4(0.0f, 0.3f, 0.5f, 1.0f), 0.2f) + SpecularColor;
	FragColor.a = clamp(fFloorToWaterSurface / 10.0f, 0.0f, 1.0f);
}