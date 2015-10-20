
uniform sampler2D u_texture0;

varying vec3 v_color;

uniform vec4 u_constantColor;

void main(void)
{
	vec4 texColor = texture2D(u_texture0, gl_PointCoord.st);
	gl_FragColor = texColor;
//	gl_FragColor = texColor * vec4(v_color, 1.0) * u_constantColor;
//	gl_FragColor = vec4(v_color, 1.0) * u_constantColor;
//  	gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);
}