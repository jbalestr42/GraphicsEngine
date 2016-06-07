#version 410

#define MAX_DIRECTIONAL_LIGHT 10
#define MAX_POINT_LIGHT 50

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

struct PointLight
{
	vec3	position;
	vec4	color;
	float	ambient_intensity;
	float	diffuse_intensity;
	float	constant_attenuation;
	float	linear_attenuation;
	float	quadratic_attenuation;
};

uniform DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHT];
uniform PointLight point_lights[MAX_POINT_LIGHT];

uniform uint directional_light_count;
uniform uint point_light_count;

struct Material
{
	vec4		ka;
	vec4		kd;
	vec4		ks;
	float		shininess;
	sampler2D	diffuse_tex;
	sampler2D	specular_tex;
};

uniform vec3 camera_position;
uniform Material material;

vec3 compute_directional_light(DirectionalLight light, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(-light.direction);
	// Diffuse shading
	float lambertian = max(dot(normal, light_dir), 0.0);
	// Specular shading
	vec3 reflect_dir = reflect(light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
	// Combine results
	vec3 ambient = light.color.rgb * light.ambient_intensity * texture2D(material.diffuse_tex, TexCoord0).rgb;
	vec3 diffuse = light.color.rgb * light.diffuse_intensity * lambertian * texture2D(material.diffuse_tex, TexCoord0).rgb;
	vec3 specular = spec * texture2D(material.specular_tex, TexCoord0).rgb;
	return (ambient + diffuse + specular);
}

vec3 compute_point_light(PointLight light, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(light.position - WorldPos0);
	// Diffuse shading
	float lambertian = max(dot(normal, light_dir), 0.0);
	// Specular shading
	vec3 reflect_dir = reflect(light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	// Attenuation
	float distance = length(light.position - WorldPos0);
	float attenuation = 1.0f / (light.constant_attenuation + light.linear_attenuation * distance + light.quadratic_attenuation * distance * distance);

	// Combine results
	vec3 ambient = light.color.rgb * light.ambient_intensity * texture2D(material.diffuse_tex, TexCoord0).rgb;
	vec3 diffuse = light.color.rgb * light.diffuse_intensity * lambertian * texture2D(material.diffuse_tex, TexCoord0).rgb;
	vec3 specular = spec * texture2D(material.specular_tex, TexCoord0).rgb;
	return ((ambient + diffuse + specular) * attenuation);
}

void main(void)
{
	vec3 result = vec3(0.0);
	vec3 normal = normalize(Normal0);
	vec3 view_dir = normalize(camera_position - WorldPos0);

	for (uint i = 0; i < directional_light_count; i++)
		result += compute_directional_light(directional_lights[i], normal, view_dir);
	for (uint i = 0; i < point_light_count; i++)
		result += compute_point_light(point_lights[i], normal, view_dir);
	FragColor = vec4(result, 1.0);
}
