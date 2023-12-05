#version 330 compatibility

in vec3 vNormal;
in vec4 vFragPosLightSpace;
in vec3 vNs;
in vec3 vLs;
in vec3 vEs;

uniform vec3 uColor;
uniform sampler2D uShadowMap;

out vec4 fFragColor;

const float BIAS =  0.01;
const vec3 SPECULAR_COLOR = vec3( 1., 1., 1. );
const float SHININESS = 8;


bool
IsInShadow(vec4 fragPosLightSpace)
{
    //Have to manually do homogenous division to make light space position in range of -1 to 1
    vec3 projection = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //then make it from 0 to 1
    projection = 0.5*projection + 0.5;
   
    //Get closest depth from light's perspective
    float closestDepth = texture(uShadowMap, projection.xy).r;

    //get current depth
    float currentDepth = projection.z;    
    bool isInShadow = (currentDepth - BIAS) > closestDepth;    
	//if(projection.z > 1.0)
	//{
        //shadow = 0.0;
	//}
    return isInShadow;
}

void
main()
{
    vec3 Normal = normalize(vNs);
    vec3 Light  = normalize(vLs);
    vec3 Eye    = normalize(vEs);

    vec3 ambient = 0.15 * uColor;

    float d = 0.;
    float s = 0.;
    
    vec3 lighting = ambient;

    //float test = texture(uShadowMap, gl_FragCoord.xy/vec2(1024,-1024)+vec2(0,1)).r; 
    bool isInShadow = IsInShadow(vFragPosLightSpace);  
    if( ! isInShadow )
    {
        d = max(dot(Normal,Light), 0.);
        vec3 diffuse = 0.6*d*uColor;
        lighting += diffuse;

        if(dot(Normal,Light)> 0.)
        {
            vec3 ref = normalize(2.*Normal*dot(Normal,Light) - Light);
            s = pow(max(dot(Eye,ref),0.), SHININESS);
        }

        vec3 specular = 0.3*s*SPECULAR_COLOR;
        lighting += specular;
    }

    //lighting.b = test;
    //lighting.rg *= 0.1;
    fFragColor = vec4( lighting, 1. );
}
