#version 410

#define MAX_DIRECTIONAL_LIGHT 10
#define MAX_POINT_LIGHT 10
#define MAX_SPOT_LIGHT 10

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
};

struct PointLight
{
	vec3	position;
	vec3	direction;
	vec4	color;
	float	ambient_intensity;
	float	constant_attenuation;
	float	linear_attenuation;
	float	quadratic_attenuation;
};

struct SpotLight
{
	vec3	position;
	vec3	direction;
	vec4	color;
	float	inner_angle;
	float	outer_angle;
	float	ambient_intensity;
	float	constant_attenuation;
	float	linear_attenuation;
	float	quadratic_attenuation;
};

uniform DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHT];
uniform PointLight point_lights[MAX_POINT_LIGHT];
uniform SpotLight spot_lights[MAX_SPOT_LIGHT];

uniform uint directional_light_count;
uniform uint point_light_count;
uniform uint spot_light_count;

struct Material
{
	vec4		ka;
	vec4		kd;
	vec4		ks;
	float		shininess;
	sampler2D	diffuse_tex;
	sampler2D	specular_tex;
};

uniform vec3 view_position;
uniform Material material;

vec3 compute_light(vec4 light_color, vec3 light_dir, vec3 normal, vec3 view_dir, float ambient_intensity)
{
	light_dir = normalize(light_dir); // TODO Normalize before entering the function
	// Diffuse shading
	float lambertian = max(dot(normal, light_dir), 0.0);
	// Specular shading
	vec3 reflect_dir = reflect(light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
	// Combine results
	vec3 ambient = light_color.rgb * ambient_intensity * texture2D(material.diffuse_tex, TexCoord0).rgb;
	vec3 diffuse = light_color.rgb * lambertian * texture2D(material.diffuse_tex, TexCoord0).rgb;
	vec3 specular = spec * texture2D(material.specular_tex, TexCoord0).rgb;
	return (ambient + diffuse + specular);
}

void main(void)
{
	vec3 result = vec3(0.0);
	vec3 normal = normalize(Normal0);
	vec3 view_dir = normalize(view_position - WorldPos0);

	for (uint i = 0; i < directional_light_count; i++)
		result += compute_light(directional_lights[i].color, -directional_lights[i].direction, normal, view_dir, directional_lights[i].ambient_intensity);

	for (uint i = 0; i < point_light_count; i++)
	{
		// Attenuation
		float distance = length(point_lights[i].position - WorldPos0);
		float attenuation = 1.0 / (point_lights[i].constant_attenuation + point_lights[i].linear_attenuation * distance + point_lights[i].quadratic_attenuation * distance * distance);
		result += compute_light(point_lights[i].color, point_lights[i].position - WorldPos0, normal, view_dir, point_lights[i].ambient_intensity) * attenuation;
	}

	for (uint i = 0; i < spot_light_count; i++)
	{
		// Attenuation
		vec3 light_dir = normalize(spot_lights[i].position - WorldPos0);
		float theta = dot(light_dir, -spot_lights[i].direction); // Direction is normalized before the shader
		float epsilon = spot_lights[i].inner_angle - spot_lights[i].outer_angle;
		float intensity = clamp((theta - spot_lights[i].outer_angle) / epsilon, 0.0, 1.0);
		float distance = length(spot_lights[i].position - WorldPos0);
		float attenuation = 1.0 / (spot_lights[i].constant_attenuation + spot_lights[i].linear_attenuation * distance + spot_lights[i].quadratic_attenuation * distance * distance);
		result += compute_light(spot_lights[i].color, light_dir, normal, view_dir, spot_lights[i].ambient_intensity) * attenuation * (1.0 - intensity);
	}
	FragColor = vec4(result, 1.0);
}
