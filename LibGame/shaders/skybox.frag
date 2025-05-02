#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform vec2 v2Resolution;
uniform mat4 m4Inv_proj;
uniform mat4 m4Inv_view;

uniform vec3 v3LightDir;
uniform vec3 v3SkyColorTop;
uniform vec3 v3SkyColorBottom;

uniform bool bIsNight = true;
uniform float fTime;

uniform float fStarDensity = 1.5;    // [0.5 - 3.0]
uniform float fStarBrightness = 1.0; // [0.1 - 2.0]

uniform float fSunSizeDay = 0.9995;    // Increased from 0.995 (smaller sun)
uniform float fSunSizeNight = 0.9999;  // Increased from 0.999 (smaller sun)
uniform float fSunCoreIntensity = 2.0;

const float fSunEdgeSoftness = 0.01; // Reduced softness for sharper edges

#define SUN_DIR v3LightDir

#define STAR_SIZE 0.25

const float pi = 3.14159265358979323846;

/**
 * hsv2rgb - Converts HSV color to RGB color.
 * @c: A vec3 where x = hue, y = saturation, z = value.
 *
 * Return: RGB color as vec3.
 */
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

/**
 * rand - Generates a pseudo-random float based on a 2D coordinate.
 * @co: A vec2 coordinate input used for seeding randomness.
 *
 * Return: A pseudo-random float in the range [0, 1).
 */
float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

/**
 * getStarGlowColor - Calculates the glow color of a star.
 * @distance: The distance from the star center.
 * @size: The radius or size of the star's glow.
 * @offset: A vec2 offset from the center used to determine angle.
 * @seed: A seed value for generating hue variation.
 *
 * Return: A vec4 RGBA color representing the star glow with falloff.
 */
vec4 getStarGlowColor(float distance, float size, vec2 offset, float seed)
{
    float progress = 1.0 - clamp(distance/size, 0.0, 1.0);
    float angle = atan(offset.y, offset.x);
    float hue = fract(seed * 0.618);
    
    return vec4(
        hsv2rgb(vec3(hue, 0.3, 1.0)),
        0.4 * pow(progress, 2.0) * mix(pow(abs(sin(angle * 2.5)), 8.0), 1.0, progress)
    );
}

//-----------------------------------------------------------------------------
// A smooth Hermite interpolation between 0 and 1 over [edge0, edge1].
// Returns 0 when x <= edge0, 1 when x >= edge1, and a smooth curve in between.
//-----------------------------------------------------------------------------
float customSmooth(float edge0, float edge1, float x)
{
    // Normalize x into [0,1] over the interval
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);

    // Hermite (smoothstep) polynomial:  t2 (3 – 2t)
    return t * t * (3.0 - 2.0 * t);

}

//-----------------------------------------------------------------------------
// A simple 3D-to-1D hash that maps a vec3 -> [0,1) pseudorandomly.
// Good for generating repeatable “random” values per cell.
//-----------------------------------------------------------------------------
float hash(vec3 p)
{
    // fract(p * inv_pi + offset)
    p  = fract(p * 0.3183099 + 0.1);

    // amplify
    p *= 17.0;

    // combine components and fract again
    return fract(p.x * p.y * p.z * (p.x + p.y + p.z));
}

//-----------------------------------------------------------------------------
// Draws a soft circle at `center` in UV space.
//   uv       : current sample coordinate (e.g. star grid space)
//   center   : circle center in same space
//   radius   : circle radius
//   softness : edge falloff amount
//-----------------------------------------------------------------------------
float circleStar(vec2 uv, vec2 center, float radius, float softness)
{
    // distance from sample to circle center
    float dist = length(uv - center);

    // smoothstep from (radius+softness) down to (radius-softness)
    return smoothstep(radius + softness, radius - softness, dist);
}

//-----------------------------------------------------------------------------
// Procedurally generate a starfield value in [0,1] for a given ray direction.
//   rayDir  : normalized view direction in world space
//   density : controls how many stars appear (higher = more)
//   time    : allows optional twinkling (if used in offset calc)
//-----------------------------------------------------------------------------
float generateStars(vec3 rayDir, float density, float time)
{
    // 1) Project the 3D ray onto a 2D equirectangular UV
    vec2 uv = vec2(
        atan(rayDir.x, rayDir.z) / (2.0 * pi) + 0.5,
        asin(rayDir.y) / pi + 0.5
    );
    
    vec4 starGlow = vec4(0.0);
    float starValue = 0.0;
    float scale = 800.0;           // grid resolution (higher = more, smaller stars)

    // 2) Determine which grid cell we’re in
    vec2 grid = floor(uv * scale);
    vec2 f = fract(uv * scale);     // local position within cell
    
    // 3) Examine this cell and its 8 neighbors for potential stars
    for(int y = -1; y <= 1; y++)
    {
        for(int x = -1; x <= 1; x++)
        {
            vec2 neighbor = vec2(float(x), float(y));
            vec2 cell = grid + neighbor;
            
            float seed = rand(cell);
            
            // 4) Compute a pseudorandom offset in [0,1) for star position
            vec2 offset = 0.5 + 0.5 * sin(time + 6.2831 * vec2(
                hash(vec3(cell, 0.0)),
                hash(vec3(cell, 1.0))
            ));
            
            // 5) Compute the star position in the cell
            vec2 starPos = neighbor + offset - f;
            float size = STAR_SIZE * (0.5 + 0.5 * hash(vec3(cell, 1.0)));
            float brightness = pow(hash(vec3(cell, 2.0)), 50.0 / density);
            
            // 6) Compute the star value based on distance from center
            float dist = length(starPos);
            starValue += brightness * circleStar(starPos, vec2(0.0), size, size * 0.5);
            
            // 7) If the star is close enough, compute its glow
            if (dist < size * 2.0)
            {
                vec4 glow = getStarGlowColor(dist, size * 1.5, starPos, time);
                starGlow = mix(starGlow, glow, glow.a);
            }
        }
    }
    
    // 8) Normalize the star value to [0,1]
    return clamp(starValue + starGlow.a * 2.0, 0.0, 1.0);
}

//-----------------------------------------------------------------------------
// Computes the intersection point of a ray with a sphere centered at the origin.
//   v3RayDir   : normalized ray direction (origin assumed at (0,0,0))
//   fRadius    : radius of the sphere
//   v3StartPos : output position on the sphere where the ray first intersects
//-----------------------------------------------------------------------------
void RaySphereIntersectionSkyMap(vec3 v3RayDir, float fRadius, out vec3 v3StartPos)
{
    // The sphere is centered at the origin:
	vec3 v3SphereCenter = vec3(0.0f);

    // Precompute radius squared:
	float fRadius2 = fRadius * fRadius;

    // Vector from sphere center to ray origin (origin at (0,0,0)):
    //   L = O - C  =>  (0,0,0) - sphereCenter  =>  -sphereCenter
	vec3 L = -v3SphereCenter;

    // Quadratic coefficients for |P(t) - C|^2 = R^2 with P(t)=t*v3RayDir:
    //   a = v3RayDir · v3RayDir
	float a = dot(v3RayDir, v3RayDir);

    //   b = 2 * v3RayDir · L
	float b = 2.0f * dot(v3RayDir, L);

    //   c = L·L - R^2
	float c = dot(L, L) - fRadius2;

    // Discriminant: D = b^2 - 4ac
    // Clamp to >=0 to avoid NaNs if ray misses the sphere
	float fDiscr = max(0.0, b*b - 4.0*a*c);

    // Solve for the smallest positive t:  t = (-b + sqrt(D)) / (2a)
    // Clamp to >=0 so we don’t go “behind” the origin
	float fT = max(0.0f, (-b + sqrt(fDiscr)) / 2.0f);

    // Compute intersection point:  P = t * v3RayDir
	v3StartPos = v3RayDir * fT;
}

//-----------------------------------------------------------------------------
// Convert a fragment coordinate into a view-space ray's clip-space coordinate.
//   v2FragCoord : integer pixel position (gl_FragCoord.xy cast to ivec2)
// Returns a direction in clip-space (z=1) ready for unprojection.
//-----------------------------------------------------------------------------
vec3 ComputeClipSpaceCoord(ivec2 v2FragCoord)
{
    // Flip the Y-axis (since OpenGL's NDC has [0, 1] at the bottom, not the top)
    // Normalize to [-1,1] range in X/Y
	vec2 ray_nds = 2.0f * vec2(v2FragCoord.xy) / v2Resolution.xy - 1.0f;

    // 2) Flip Y because OpenGL’s window coords origin is bottom-left
    //    but gl_FragCoord.y is bottom-left = 0, top = height.
	return vec3(-ray_nds.x, -ray_nds.y, 1.0f);
}

//-----------------------------------------------------------------------------
// Computes the sun (or moon) color & visibility based on view direction.
//   v3Dir    : normalized view direction in world space
//   fPowExp  : exponent controlling halo falloff
//-----------------------------------------------------------------------------
vec3 GetSunColor(const vec3 v3Dir, float fPowExp)
{
    // 1) How directly we’re looking at the sun: 1.0 = straight on, 0.0 = perpendicular
    float fSun = clamp(dot(SUN_DIR, v3Dir), 0.0f, 1.0f);
    
    // 2) Choose base disc size (day vs night)
    float baseSunSize = bIsNight ? fSunSizeNight : fSunSizeDay;

    // 3) Adapt size slightly based on sky brightness (e.g. a brighter sky makes the disc a bit smaller)
    //    dot(v3SkyColorTop, vec3(0.2126,0.7152,0.0722)) computes luminance of the top sky color

    float adaptiveSunSize = mix(baseSunSize - 0.005, baseSunSize, 
                              dot(v3SkyColorTop, vec3(0.2126, 0.7152, 0.0722)));

    // 4) Compute a smooth mask (0 -> 1) over a small angular range for the sun’s disc edge
    float sunVisibility = smoothstep(adaptiveSunSize - fSunEdgeSoftness, adaptiveSunSize, fSun);
    
    // 5) Control overall intensity: stronger “core” at night vs day
    float compensation = mix(fSunCoreIntensity, 1.0, 
                           bIsNight ? 0.3 : 0.7); // Stronger core at night

    // 6) Build final sun color:
    //    - For night: pale white -> mix toward pure white based on pow(fSun,8)
    //    - For day : warm yellow -> mix toward white based on pow(fSun,4)
    //    - Then apply halo falloff pow(fSun, fPowExp) for glow around the disc
    if (bIsNight)
    {
        return compensation * sunVisibility * 
               mix(vec3(1.0, 0.95, 0.9), vec3(1.0), pow(fSun, 8.0)) * 
               pow(fSun, fPowExp);
    }
    else
    {
        return compensation * sunVisibility * 
               mix(vec3(1.0, 0.6, 0.1), vec3(1.0), pow(fSun, 4.0)) * 
               pow(fSun, fPowExp);
    }
}

//-----------------------------------------------------------------------------
// Combine sky gradient, sun/moon, and stars into a final color.
//   v3EndPos : (optional) intersection on sky sphere — unused here
//   v3Dir    : normalized view direction in world space
//-----------------------------------------------------------------------------
vec4 ColorCubeMap(vec3 v3EndPos, const vec3 v3Dir)
{
    // --- 1) Sky gradient from bottom -> top ---
    // Remap v3Dir.y from [-1,1] to [0,1], then apply an exponential fade
    float fLowFade = 10.0f; // 8.5
    float fFadeStepness = 17.0f; // 17

    float fFactor = clamp(1 - exp(fLowFade - fFadeStepness * clamp(normalize(v3Dir).y * 0.5f + 0.5f, 0.0f, 1.0f)), 0.0f, 1.0f);
	vec3 v3Color = mix(v3SkyColorBottom, v3SkyColorTop, fFactor);

    // --- 2) Add sun or moon ---
    // GetSunColor already applies disc mask, tint, and halo
	v3Color += GetSunColor(v3Dir, 550.0f);

    // --- 3) Add stars if it’s night ---
    if (bIsNight)
    {
        float stars = generateStars(normalize(v3Dir), fStarDensity, fTime);
        float horizonFade = smoothstep(0.1, 0.4, abs(v3Dir.y));
        float sunFade = 1.0 - smoothstep(0.995, 1.0, dot(normalize(v3Dir), SUN_DIR));
        
        // Color variation with glow consideration
        vec3 starColor = mix(
            mix(vec3(0.9, 0.9, 1.0), vec3(1.0, 0.8, 0.6), hash(normalize(v3Dir) * 2.0)),
            vec3(1.0), 
            smoothstep(0.7, 1.0, stars) // Brighter core
        );
        
        v3Color += pow(stars * fStarBrightness, 2.2) * horizonFade * sunFade * starColor;
    }


	return vec4(v3Color, 1.0f);
}

//-----------------------------------------------------------------------------
// Main entry: build a view-ray from the fragment, then shade it.
//-----------------------------------------------------------------------------
void main()
{
    // 1) Convert pixel to clip-space ray
	ivec2 v2FragCoord = ivec2(gl_FragCoord.xy);
	vec4 v4RayClip = vec4(ComputeClipSpaceCoord(v2FragCoord), 1.0f);

    // 2) Unproject to view space
	vec4 v4RayView = m4Inv_proj * v4RayClip;
	v4RayView = vec4(v4RayView.xy, -1.0f, 0.0f);

    // 3) Unproject to world space and normalize
	vec3 v3WorldDir = normalize((m4Inv_view * v4RayView).xyz);

    // 4) Flip X/Y if needed (undo the NDC flip)
    vec3 shadeDir = vec3(-v3WorldDir.x, -v3WorldDir.y, v3WorldDir.z);

	// Compute background color
	vec3 v3CubeMapEndPos;

    // Compute the 3D point on our “sky sphere” where this view-ray exits:
    // We can then use v3CubeMapEndPos to look up or procedurally shade the sky
    // at exactly the point this pixel’s ray would hit the sky dome.
    // Currently UNUSED.
	RaySphereIntersectionSkyMap(shadeDir, 0.5f, v3CubeMapEndPos);

    // 5) Shade the sky along this direction
	vec4 v4FinalColor = ColorCubeMap(v3CubeMapEndPos, shadeDir);

	FragColor = v4FinalColor;
}