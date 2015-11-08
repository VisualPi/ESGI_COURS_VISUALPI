
#version 150

out vec4 Fragment;

void main(void)
{
    // auparavant en OpenGL (ES) 2 on ne pouvait specifier que 
    // gl_FragColor ou gl_FragData[]
    Fragment = vec4(1.0);
}