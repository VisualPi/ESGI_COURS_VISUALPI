
#version 150

layout(location=0) in vec4 a_position;

uniform mat4 u_worldMatrix;

uniform	mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void main(void)
{
	gl_Position = a_position;
}