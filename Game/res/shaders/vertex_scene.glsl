#version 460 core
layout (location = 0) in vec3 vPos;

out vec3 fColor;

uniform mat4 model;
uniform mat4 viewproj;

void main()
{
	fColor = vPos;
	gl_Position = model * viewproj * vec4(vPos, 1.0);
}