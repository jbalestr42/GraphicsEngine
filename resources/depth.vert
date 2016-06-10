#version 410

layout(location=0) in vec3 Position;

uniform mat4 LightViewProjMatrix;
uniform mat4 ModelMatrix;

void main(void)
{
	gl_Position = (LightViewProjMatrix * ModelMatrix) * vec4(Position, 1.0);
}
