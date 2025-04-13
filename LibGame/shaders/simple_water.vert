#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;

uniform mat4 ViewMatrix;
uniform float fWaterHeight = 20.0f;
uniform float fTime;
uniform vec3 v3CameraPos;

out vec2 v2Tex;
out vec4 v4ClipSpaceCoords;
out vec3 v3VertexToCamera;

const float fTiling = 6.0;

// New Float Values For Water
const float fWaterDepth = 1.0f;
const float fWaveAmplitude = 1.3f;
const float fWaveLength = 20.0f;
const vec2 v2WaveDir = vec2(1.0f, 0.0f);
const float g = 9.81f;
const float PI = 3.1415926535897932384626f;

void main()
{
	vec3 v3NewPos = Position + vec3(0.0f, fWaterHeight, 0.0f);

	// compute wave number (k = 2* PI / lambda)
	float k = 2.0f * PI / fWaveLength;

	// Shallow water dispersion relation w = k * sqrt(g * k)
	float omega = k * sqrt(g * fWaterDepth);

	vec2 waveDir = normalize(v2WaveDir);

	// compute phase dot(fWaveDirection, v3NewPos.xz) * k  - w * t;
	float fPhase = dot(waveDir, v3NewPos.xz) * k - omega * fTime;

	v3NewPos.y += fWaveAmplitude * sin(fPhase);

	v4ClipSpaceCoords = ViewMatrix * vec4(v3NewPos, 1.0f);
	gl_Position = v4ClipSpaceCoords;

	v2Tex = TexCoord * fTiling;
	v3VertexToCamera = v3CameraPos - v3NewPos;
}
