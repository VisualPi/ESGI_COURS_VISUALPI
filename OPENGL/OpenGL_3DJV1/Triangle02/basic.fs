
varying vec3 v_color;

uniform vec4 u_constantColor;

void main(void)
{
	gl_FragColor = vec4(v_color, 1.0) * u_constantColor;
//  	gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);
}