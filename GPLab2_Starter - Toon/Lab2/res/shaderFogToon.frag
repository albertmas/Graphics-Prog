#version 400

out vec4 FragColor;
in vec3 v_norm;
in vec4 v_pos;

uniform vec3 lightDir;
uniform vec3 fogColor;

uniform float maxDist;
uniform float minDist;

void main()
{
	//Calculate fog color
	float dist = abs(v_pos.z);
	float fogFactor = (maxDist - dist) / (maxDist - minDist);
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	vec3 lightColor = vec3(0.5,0.0,0.0);
	

	//Calculate toon color
	float intensity;
	vec4 color;
	intensity = dot(lightDir, v_norm);

	if (intensity > 0.95)
		color = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > 0.25)
		color = vec4(0.4,0.2,0.2,1.0);
	else
		color = vec4(0.2,0.1,0.1,1.0);

	FragColor = mix(vec4(fogColor, 1.0), color, fogFactor);
}