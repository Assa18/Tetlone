#version 460 core
layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 vTexCoords;

out vec2 TexCoords;

uniform bool shake;
uniform float time;

void main()
{
	vec2 position = vPos;
	if (shake)
	{
		float strength = 0.01;
		position.x += cos(time * 10) * strength;
		position.y += cos(time * 15) * strength;
	}
	TexCoords = vTexCoords;
	gl_Position = vec4(position, 0.0, 1.0);
}