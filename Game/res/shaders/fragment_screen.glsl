#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec2 offsets[9];
uniform float edge_kernel[9];

uniform int colorMode;

void main()
{
	FragColor = vec4(0.0f);
	vec3 texSample[9];
	if (colorMode == 0)
	{
		FragColor = vec4(texture(screenTexture, TexCoords).rgb, 1.0);
	}
	else if (colorMode == 1)
	{
		FragColor = vec4(1 - texture(screenTexture, TexCoords).rgb, 1.0);
	}
	else
	{
		for (int i = 0; i < 9; i++)
			texSample[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));

		for(int i = 0; i < 9; i++)
			FragColor += vec4(texSample[i] * int(edge_kernel[i]), 0.0f);
		FragColor.a = 1.0f;
	}
}