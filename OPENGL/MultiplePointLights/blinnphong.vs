#version 150

#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_texcoords;
layout(location = 2) in vec3 a_normal;

// note: a partir d'OpenGL 4.3 ou si l'extension GL_ARB_explicit_uniform_location 
//		est disponible, on peut egalement affecter un location aux variable uniformes

uniform mat4 u_worldMatrix;

// note: a partir d'OpenGL 4.2 ou si l'extension GL_ARB_shading_language_420pack
//		est disponible, on peut affecter un binding point aux UBOs avec layout(binding=?)

layout(std140) uniform ViewProj
{
	mat4 u_viewMatrix;
	mat4 u_projectionMatrix;
};

out Vertex
{
	vec3 normal;
	vec2 texcoords;
	vec3 positionVS;	// VS = View Space
} OUT;

void main(void)
{	
	vec4 positionVS = u_viewMatrix * u_worldMatrix * a_position;
	vec3 N = mat3(u_worldMatrix) * a_normal;
	OUT.normal = N;
	OUT.texcoords = a_texcoords;
	OUT.positionVS = positionVS.xyz;

	gl_Position = u_projectionMatrix * positionVS;
}