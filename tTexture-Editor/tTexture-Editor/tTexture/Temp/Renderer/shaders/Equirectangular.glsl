#type vertex
#version 430

layout (location = 0) in vec3 a_Position;

out vec3 v_WorldPosition;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

void main()
{
	v_WorldPosition = a_Position;
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 430

out vec4 outputColor;
in vec3 v_WorldPosition;

uniform sampler2D u_Texture;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;

	return uv;
}

vec2 FlipTexCoord(vec2 uv)
{
	return vec2(uv.x, -uv.y);
}

uniform float u_OutputAlpha;
uniform float u_Exposure;

void main()
{
	const float gamma     = 2.2;
	const float pureWhite = 1.0;

	vec2 uv = SampleSphericalMap(normalize(v_WorldPosition));
	vec3 color = texture(u_Texture, FlipTexCoord(uv)).rgb  * u_Exposure;
	// Reinhard tonemapping operator.
	// see: "Photographic Tone Reproduction for Digital Images", eq. 4
	float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));
	float mappedLuminance = (luminance * (1.0 + luminance / (pureWhite * pureWhite))) / (1.0 + luminance);

	// Scale color by ratio of average luminances.
	vec3 mappedColor = (mappedLuminance / luminance) * color;

	// Gamma correction.
	outputColor = vec4(pow(mappedColor, vec3(1.0/gamma)), u_OutputAlpha);
}
