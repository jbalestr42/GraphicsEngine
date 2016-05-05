#version 410

#define MAX_DIRECTIONAL_LIGHT 10

in vec4 Color0;
in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

out vec4 FragColor;

struct DirectionalLight
{
	vec4	color;
	vec3	direction;
	float	ambient_intensity;
	float	diffuse_intensity;
};

uniform DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHT];
uniform uint directional_light_count;

struct Material
{
	float	specular_intensity;
	float	specular_power;
};

uniform sampler2D sampler;
uniform vec3 camera_position;

Material mat;

void main(void)
{
	mat.specular_intensity = 1.0;
	mat.specular_power = 32.0;
	FragColor = vec4(0.0);
	vec3 normal = normalize(Normal0);

	for (uint i = 0; i < directional_light_count; i++)
	{
		vec3 light_direction = -directional_lights[i].direction;

		// Ambient
		vec3 ambient_color = directional_lights[i].ambient_intensity * directional_lights[i].color.rgb;

		// Diffuse
		float diffuse_factor = max(0.0, dot(normal, light_direction));
		vec3 diffuse_color = diffuse_factor * directional_lights[i].diffuse_intensity * directional_lights[i].color.rgb;

		// Specular
		float specular_factor = ceil(diffuse_factor) * pow(max(0.0, dot(normalize(camera_position - WorldPos0), normalize(reflect(directional_lights[i].direction, normal)))), mat.specular_power);
		vec3 specular_color =  specular_factor * mat.specular_intensity * directional_lights[i].color.rgb;

		// Pixel Color
		FragColor += texture2D(sampler, TexCoord0.xy) * vec4(ambient_color + diffuse_color + specular_color, 1.0);
	}
}
