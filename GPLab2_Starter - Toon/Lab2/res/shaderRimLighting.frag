#version 400

out vec4 FragColor;
in vec3 v_norm;

uniform vec3 viewDir;

void main() 
{
	float rim = 1.0- dot(viewDir, v_norm);
	float s = smoothstep(0.0, 1.0, rim);

	FragColor = vec4(vec3(s), 1.0);
}
