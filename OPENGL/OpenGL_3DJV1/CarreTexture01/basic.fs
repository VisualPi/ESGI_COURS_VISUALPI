



uniform sampler2D u_texture0;

varying vec3 v_color;

varying vec2 v_texCoords;

uniform vec4 u_constantColor;

void main(void)
{
	vec4 texColor = texture2D(u_texture0, v_texCoords);
	gl_FragColor = texColor;
//	gl_FragColor = texColor * vec4(v_color, 1.0) * u_constantColor;
//	gl_FragColor = vec4(v_color, 1.0) * u_constantColor;
//  	gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);
}