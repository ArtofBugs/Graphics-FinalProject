#version 330 compatibility

uniform vec3      uColor;
uniform sampler2D uShadowMap;
uniform sampler2D uTexUnit1;
uniform int       uShadowsOn;

in vec4 vFragPosLightSpace;
in vec3 vNs;
in vec3 vLs;
in vec3 vEs;
in vec2 vST;

out vec4 fFragColor;

const float BIAS =  0.01;
const vec3  SPECULAR_COLOR = vec3( 1., 1., 1. );
const float SHININESS = 8;

const float KA = 0.20;
const float KD = 0.60;
const float KS = (1.-KA-KD);


bool
IsInShadow(vec4 fragPosLightSpace)
{
    // have to manually do homogenous division to make light space position in range of -1 to 1:
    vec3 projection = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //then make it from 0 to 1:
    projection = 0.5*projection + 0.5;
   
    //Get closest depth from light's perspective
    float closestDepth = texture(uShadowMap, projection.xy).r;

    //get current depth:
    float currentDepth = projection.z;    
    bool isInShadow = (currentDepth - BIAS) > closestDepth;    
	//if(projection.z > 1.0)
	//{
        //isInShadow = false;
	//}
    return isInShadow;
}

void
main()
{
    vec3 normal = normalize(vNs);
    vec3 light  = normalize(vLs);
    vec3 eye    = normalize(vEs);

    float d = 0.;
    float s = 0.;
    
    vec3 myColor = texture(uTexUnit1, vST).rgb;
	//myColor = vec3( 0.8 * dot(vNs, vEs) * dot(vNs, vEs) * 0.1, 0.7 * dot(vNs, vEs), 0.5 * dot(vNs, vEs) * dot(vNs, vEs) * 0.1);

    vec3 lighting = KA * myColor;

    bool isInShadow = IsInShadow(vFragPosLightSpace);
    if( uShadowsOn != 0 )
        isInShadow = false;     // if in ShadowOff mode, nothing should be cnsidered in a shadow
    if( ! isInShadow )
    {
        d = dot(normal,light);
        if(d > 0.)
        {
            vec3 diffuse = KD*d*myColor;
            lighting += diffuse;

            vec3 refl = normalize( reflect( -light, normal ) );
            float dd = dot(eye,refl);
            if( dd > 0. )
            {
                s = pow( dd, SHININESS );
                vec3 specular = KS*s*SPECULAR_COLOR;
                lighting += specular;
            }
        }
    }

    fFragColor = vec4( lighting, 1. );
}
