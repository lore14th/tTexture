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

uniform float u_OutputAlpha;
uniform float u_Exposure;
uniform samplerCube u_Texture;

const float PI = 3.14159265359;

void main()
{
	// The world vector acts as the normal of a tangent surface
	// from the origin, aligned to WorldPos. Given this normal, calculate all
	// incoming radiance of the environment. The result of this radiance
	// is the radiance of light coming from -Normal direction, which is what
	// we use in the PBR shader to sample irradiance.
	vec3 N = normalize(v_WorldPosition);

	vec3 irradiance = vec3(0.0);

	// tangent space calculation from origin point
	vec3 up    = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up, N);
	up         = cross(N, right);

	float sampleDelta = 0.025;
	float nrSamples = 0.0;
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			// spherical to cartesian (in tangent space)
			vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
			// tangent space to world
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

			irradiance += texture(u_Texture, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}

	irradiance = PI * irradiance * (1.0 / float(nrSamples));
	outputColor = vec4(irradiance, u_OutputAlpha);
}
