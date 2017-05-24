#version 410

#define MAX_DIRECTIONAL_LIGHT 10
#define MAX_POINT_LIGHT 10
#define MAX_SPOT_LIGHT 10

in vec4 Color0;
in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;
in vec4 LightPos0;

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
	float	inner_angle;
	float	outer_angle;
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
	vec4		ka;				// the alpha channel store whether there is a texture or not
	vec4		kd;				// the alpha channel store whether there is a texture or not
	vec4		ks;				// the alpha channel store whether there is a texture or not
	float		shininess;
	sampler2D	diffuse_tex;
	sampler2D	specular_tex;
};

uniform vec3 view_position;
uniform Material material;

// Needed for shadow
uniform vec3 light_position;
uniform sampler2D shadow_map;

vec3 compute_light(vec4 light_color, vec3 light_dir, vec3 normal, vec3 view_dir, float ambient_intensity)
{
	light_dir = normalize(light_dir); // TODO Normalize before entering the function
	// Diffuse shading
	float lambertian = max(dot(normal, light_dir), 0.0);
	// Specular shading
	vec3 reflect_dir = reflect(light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
	// Combine results
	//vec3 ambient_diffuse = (ambient_intensity + lambertian) * light_color.rgb * texture2D(material.diffuse_tex, TexCoord0).rgb;
	vec3 ambient = material.ka.a * light_color.rgb * ambient_intensity * texture2D(material.diffuse_tex, TexCoord0).rgb;
	vec3 diffuse = material.kd.a * light_color.rgb * lambertian * texture2D(material.diffuse_tex, TexCoord0).rgb;
	vec3 specular = material.ks.a * spec * texture2D(material.specular_tex, TexCoord0).rgb;
	//vec3 ambient = light_color.rgb * ambient_intensity * texture(material.diffuse_tex, TexCoord0).rgb;
	//vec3 diffuse = light_color.rgb * lambertian * texture(material.diffuse_tex, TexCoord0).rgb;
	//vec3 specular = spec * texture(material.specular_tex, TexCoord0).rgb;
	return (ambient + diffuse + specular);
}

void main(void)
{
	vec3 result = vec3(0.0);
	vec3 normal = normalize(Normal0);
	vec3 view_dir = normalize(view_position - WorldPos0);

	for (uint i = 0u; i < directional_light_count; i++)
		result += compute_light(directional_lights[i].color, -directional_lights[i].direction, normal, view_dir, directional_lights[i].ambient_intensity);

	for (uint i = 0u; i < point_light_count; i++)
	{
		// Attenuation
		float distance = length(point_lights[i].position - WorldPos0);
		float attenuation = 1.0 / (point_lights[i].constant_attenuation + point_lights[i].linear_attenuation * distance + point_lights[i].quadratic_attenuation * distance * distance);
		result += compute_light(point_lights[i].color, point_lights[i].position - WorldPos0, normal, view_dir, point_lights[i].ambient_intensity) * attenuation;
	}

	for (uint i = 0u; i < spot_light_count; i++)
	{
		// Compute whether the pixel is the radius of the spotlight
		vec3 light_dir = normalize(spot_lights[i].position - WorldPos0);
		float theta = dot(-light_dir, spot_lights[i].direction); // Direction is normalized before the shader
		float epsilon = spot_lights[i].outer_angle - spot_lights[i].inner_angle;
		float intensity = clamp((spot_lights[i].inner_angle + theta) / epsilon, 0.0, 1.0);
		// Attenuation
		float distance = length(spot_lights[i].position - WorldPos0);
		float attenuation = 1.0 / (spot_lights[i].constant_attenuation + spot_lights[i].linear_attenuation * distance + spot_lights[i].quadratic_attenuation * distance * distance);
		result += compute_light(spot_lights[i].color, light_dir, normal, view_dir, spot_lights[i].ambient_intensity) * attenuation * intensity;
	}

	// Calculate shadow
	// perform perspective divide
	vec3 projCoords = LightPos0.xyz / LightPos0.w;
	// Transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(shadow_map, projCoords.xy).r;
	// Get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// Check whether current frag pos is in shadow
	float shadow = currentDepth > closestDepth  ? 0.5 : 1.0;

	FragColor = vec4(result/* * shadow*/, 1.0);
}
