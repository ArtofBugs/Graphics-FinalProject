#version 330 compatibility

uniform vec3 uColor;

void main()
{             
    gl_FragColor = vec4(uColor, 1.);    // really doesn't matter...
}  