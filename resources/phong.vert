#version 410

#define MAX_SHADOW_MAP 10

layout(location=0) in vec3 Position;
layout(location=1) in vec2 TexCoord;
layout(location=2) in vec3 Normal;
layout(location=3) in vec4 Color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 LightViewProjMatrix[MAX_SHADOW_MAP];
uniform uint cascade_shadow_map_count;

out vec4 Color0;					// Vertex color
out vec2 TexCoord0;					// Texture coordinate
out vec3 Normal0;					// Normal in world space
out vec3 WorldPos0;					// Vertex in world space
out vec4 LightPos0[MAX_SHADOW_MAP];	// Vertex in light space
out float ClipSpacePosZ;

void main(void)
{
	//TODO compute MVP before the shader
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * vec4(Position, 1.0);
	Color0 = Color;
	TexCoord0 = TexCoord;
	Normal0 = (ModelMatrix * vec4(Normal, 0.0)).xyz;
	WorldPos0 = (ModelMatrix * vec4(Position, 1.0)).xyz;
	for (uint i = 0; i < cascade_shadow_map_count; i++)
		LightPos0[i] = LightViewProjMatrix[i] * vec4(WorldPos0, 1.0);
	ClipSpacePosZ = gl_Position.z;
}
