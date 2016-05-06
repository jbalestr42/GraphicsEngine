#version 410

layout(location=0) in vec3 Position;
layout(location=1) in vec2 TexCoord;
layout(location=2) in vec3 Normal;
layout(location=3) in vec4 Color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec4 Color0;
out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;

void main(void)
{
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * vec4(Position, 1.0);
	Color0 = Color;
	TexCoord0 = TexCoord;
	Normal0 = (ModelMatrix * vec4(Normal, 0.0)).xyz;		// Normal in world position
	WorldPos0 = (ModelMatrix * vec4(Position, 1.0)).xyz;	// Vertex in world position
}
