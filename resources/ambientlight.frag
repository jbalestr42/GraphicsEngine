#version 410

#define MAX_DIRECTIONAL_LIGHT 10

in vec4 ex_Color;
in vec2 ex_TexCoord;
out vec4 out_Color;

struct DirectionalLight
{
	vec4	color;
	float	ambient_intensity;
};
uniform DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHT];
uniform uint directional_light_count;

uniform sampler2D sampler;

void main(void)
{
	out_Color = vec4(0.0);
	for (uint i = 0; i < directional_light_count; i++)
		out_Color += texture2D(sampler, ex_TexCoord) * directional_lights[i].color * directional_lights[i].ambient_intensity;
}
