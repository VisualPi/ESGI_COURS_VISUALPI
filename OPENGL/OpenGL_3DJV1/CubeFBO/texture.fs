

varying vec2 v_texCoords;

uniform sampler2D u_texture;

out vec4 color;

void main(void)
{
    //gl_FragColor = texture2D(u_texture, v_texCoords);
    color = texture(u_texture, v_texCoords);
}