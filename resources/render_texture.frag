#version 410

in vec2 TexCoord0;

out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
	FragColor = texture(screenTexture, TexCoord0);
}
