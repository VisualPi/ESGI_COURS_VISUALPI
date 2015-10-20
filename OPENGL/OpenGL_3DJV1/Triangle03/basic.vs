
attribute vec4 a_position;
attribute float a_intensity;

uniform vec3 u_offset;

uniform float u_time;

varying vec3 v_color;

void main(void)
{
	v_color = vec3(a_intensity);

	gl_Position = a_position + vec4(u_offset, 0.0);

	//gl_PointSize = 64.0 * abs(cos(u_time));	
	gl_PointSize = 64.0 * (cos(u_time)/2.0 + 0.5);
	gl_PointSize = 64.0 * (cos(u_time)*0.5 + 0.5);
	gl_PointSize = 64.0 * ((cos(u_time) + 1.0) / 2.0);
}