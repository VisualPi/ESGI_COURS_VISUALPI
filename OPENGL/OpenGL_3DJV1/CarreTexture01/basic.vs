
attribute vec4 a_position;
attribute float a_intensity;
attribute vec2 a_texCoords;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_worldMatrix;

uniform vec3 u_offset;

uniform float u_time;

varying vec3 v_color;
varying vec2 v_texCoords;

void main(void)
{
	v_color = vec3(a_intensity);
	v_texCoords = a_texCoords;
	gl_Position = u_projectionMatrix * u_viewMatrix * u_worldMatrix * (a_position + vec4(u_offset, 0.0));

}