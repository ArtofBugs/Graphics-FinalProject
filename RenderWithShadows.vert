#version 330 compatibility

uniform mat4  uLightSpaceMatrix;
uniform mat4  uModel;
uniform mat4  uView;
uniform mat4  uProj;
uniform float uLightX;
uniform float uLightY;
uniform float uLightZ;

out vec4 vFragPosLightSpace;
out vec3 vNs;
out vec3 vLs;
out vec3 vEs;

void
main()
{
	vec3 LightPosition = vec3(uLightX, uLightY, uLightZ);

	vec4 ECposition = uView * uModel * gl_Vertex;
    vec3 tnorm = normalize( mat3(uModel) * gl_Normal );
	vNs = tnorm;
	vLs = LightPosition      - ECposition.xyz;
	vEs = vec3( 0., 0., 0. ) - ECposition.xyz;
        
	vFragPosLightSpace = uLightSpaceMatrix  * uModel * gl_Vertex;
	gl_Position        = uProj * uView      * uModel * gl_Vertex;
}