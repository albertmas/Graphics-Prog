#version 400

out vec4 FragColor;
in vec3 v_norm;
in vec4 v_pos;

uniform vec3 viewDir;

uniform vec3 lightDir;
uniform vec4 fogColor;

uniform float maxDist;
uniform float minDist;

void main()
{
	float rim = 1.0 - dot(viewDir, v_norm);
	float s = smoothstep(0.0, 1.0, rim);
	vec4 lightColor = vec4(vec3(s), 1.0);

	//Calculate fog color
	
	

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

	vec4 finalcolor = mix(lightColor, color, 0.5);

	//Calculate rim lighting
	
	float dist = abs(v_pos.z);
	float fogFactor = (maxDist - dist) / (maxDist - minDist);
	fogFactor = clamp(fogFactor, 0.0, 1.0);


	FragColor = mix(fogColor, finalcolor, fogFactor);
}