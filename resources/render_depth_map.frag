#version 410

in vec2 TexCoord0;

out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
	float depth = texture(screenTexture, TexCoord0).r;
	FragColor = vec4(vec3(depth), 1.0);
}
