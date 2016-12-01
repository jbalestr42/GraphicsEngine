#version 410

in vec2 TexCoord0;

out vec4 FragColor;

uniform sampler2D screen_texture;

void main()
{
	FragColor = texture(screen_texture, TexCoord0);
}
