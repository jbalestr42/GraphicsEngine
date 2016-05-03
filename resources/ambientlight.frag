#version 410

#define MAX_DIRECTIONAL_LIGHT 10

in vec4 ex_Color;
in vec2 ex_TexCoord;
in vec3 ex_Normal;
out vec4 out_Color;

struct DirectionalLight
{
	vec4	color;
	vec3	direction;
	float	ambient_intensity;
	float	diffuse_intensity;
};

uniform DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHT];
uniform uint directional_light_count;

uniform sampler2D sampler;

void main(void)
{
	out_Color = vec4(0.0);
	for (uint i = 0; i < directional_light_count; i++)
	{
		vec4 ambient_color = vec4(directional_lights[i].color.rgb * directional_lights[i].ambient_intensity, directional_lights[i].color.a);
		float diffuse_factor = dot(normalize(ex_Normal), -directional_lights[i].direction);
		vec4 diffuse_color = step(0.0, diffuse_factor) * vec4(directional_lights[i].color.rgb * directional_lights[i].diffuse_intensity * diffuse_factor, directional_lights[i].color.a);
		out_Color += texture2D(sampler, ex_TexCoord.xy) * (ambient_color + diffuse_color);
	}
}
