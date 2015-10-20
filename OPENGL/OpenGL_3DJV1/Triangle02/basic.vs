
attribute vec4 a_position;
attribute float a_intensity;

uniform vec3 u_offset;

varying vec3 v_color;

void main(void)
{
	v_color = vec3(a_intensity);

	gl_Position = a_position + vec4(u_offset, 0.0);
}