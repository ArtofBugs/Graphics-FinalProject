#version 330 compatibility

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 vST;

void
main()
{
	vST = gl_MultiTexCoord0.st;
	gl_Position = uProj * uView  * uModel * gl_Vertex;
}