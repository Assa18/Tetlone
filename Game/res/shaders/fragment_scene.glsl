#version 460 core
out vec4 FragColor;

in vec4 fColor;
in vec2 fTexCoords;
in float fTexIndex;

uniform sampler2D uTextures[16];

void main()
{
	FragColor = texture(uTextures[int(fTexIndex)], fTexCoords) * fColor;
}