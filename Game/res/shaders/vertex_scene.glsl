#version 460 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in float vTexIndex;

out vec4 fColor;
out vec2 fTexCoords;
out float fTexIndex;

uniform mat4 viewproj;

void main()
{
	fColor = vColor;
	fTexCoords = vTexCoords;
	fTexIndex = vTexIndex;
	gl_Position = viewproj * vec4(vPos, 1.0);
}