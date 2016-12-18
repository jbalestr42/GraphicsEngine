#version 410

layout(location=0) in vec3 Position;
layout(location=1) in vec4 Color;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec4 Color0;

void main(void)
{
	gl_Position = (ProjectionMatrix * ViewMatrix) * vec4(Position, 1.0);
	Color0 = Color;
}
