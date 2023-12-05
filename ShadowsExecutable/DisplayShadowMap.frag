#version 330 compatibility

uniform sampler2D uShadowMap;

in vec2 vST;

out vec4 fFragColor;

void
main( )
{
	float gray = texture(uShadowMap, vST ).r;
	fFragColor = vec4( gray, gray, gray, 1. );
}
