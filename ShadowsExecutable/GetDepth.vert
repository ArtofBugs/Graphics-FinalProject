#version 330 compatibility

uniform mat4 uLightSpaceMatrix;
uniform mat4 uModel;

void
main()
{
    gl_Position = uLightSpaceMatrix * uModel * gl_Vertex;
} 