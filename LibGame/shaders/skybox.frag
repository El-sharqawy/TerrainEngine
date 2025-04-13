#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform vec2 v2Resolution;
uniform mat4 m4Inv_proj;
uniform mat4 m4Inv_view;

uniform vec3 v3LightDir;
uniform vec3 v3SkyColorTop;
uniform vec3 v3SkyColorBottom;

uniform sampler2D screenTexture;

#define SUN_DIR v3LightDir

void RaySphereIntersectionSkyMap(vec3 v3RayDir, float fRadius, out vec3 v3StartPos)
{
	float fT;

	vec3 v3SphereCenter = vec3(0.0f);

	float fRadius2 = fRadius * fRadius;

	vec3 L = -v3SphereCenter;

	float a = dot(v3RayDir, v3RayDir);
	float b = 2.0f * dot(v3RayDir, L);
	float c = dot(L, L) - fRadius2;

	//float fDiscr = b * b - 4.0f * a * c;
	float fDiscr = max(0.0, b*b - 4.0*a*c);

	fT = max(0.0f, (-b + sqrt(fDiscr)) / 2.0f);

	v3StartPos = v3RayDir * fT;
}

vec3 ComputeClipSpaceCoord(ivec2 v2FragCoord)
{
    // Flip the Y-axis (since OpenGL's NDC has [0, 1] at the bottom, not the top)
	vec2 ray_nds = 2.0f * vec2(v2FragCoord.xy) / v2Resolution.xy - 1.0f;
	//return vec3(ray_nds, 1.0f);
	return vec3(-ray_nds.x, -ray_nds.y, 1.0f);
}

vec3 GetSunColor(const vec3 v3Dir, float fPowExp)
{
	float fSun = clamp(dot(SUN_DIR, v3Dir), 0.0f, 1.0f);
	vec3 v3SunColor = 0.8f * vec3(1.0f, 0.6f, 0.1f) * pow(fSun, fPowExp);
	return v3SunColor;
}

vec4 ColorCubeMap(vec3 v3EndPos, const vec3 v3Dir)
{
	// Background Sky
	vec3 v3Color = mix(v3SkyColorBottom, v3SkyColorTop, clamp(1 - exp(8.5 - 17 * clamp(normalize(v3Dir).y * 0.5f + 0.5f, 0.0f, 1.0f)), 0.0f, 1.0f));

	// Sun
	v3Color += GetSunColor(v3Dir, 350.0f);

	return vec4(v3Color, 1.0f);
}

void main()
{
	ivec2 v2FragCoord = ivec2(gl_FragCoord.xy);
	vec4 v4RayClip = vec4(ComputeClipSpaceCoord(v2FragCoord), 1.0f);
	vec4 v4RayView = m4Inv_proj * v4RayClip;
	v4RayView = vec4(v4RayView.xy, -1.0f, 0.0f);
	vec3 v3WorldDir = (m4Inv_view * v4RayView).xyz;
	v3WorldDir = normalize(v3WorldDir);

	vec3 v3StartPos, v3EndPos;

	// If you really need a sphere intersection for a cubemap, do it here.
    // Otherwise just shade by direction:

    // Undo the flip for shading:
    vec3 shadeDir = vec3(-v3WorldDir.x, -v3WorldDir.y, v3WorldDir.z);

	//compute background color
	vec3 v3CubeMapEndPos;
	RaySphereIntersectionSkyMap(shadeDir, 0.5f, v3CubeMapEndPos);
	vec4 v4Background = ColorCubeMap(v3CubeMapEndPos, shadeDir);
	FragColor = v4Background;
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}