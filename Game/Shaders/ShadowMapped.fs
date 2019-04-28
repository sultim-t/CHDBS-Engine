#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D t_diff0;
uniform sampler2D t_shdw0;
uniform samplerCube t_cube0;

#define MAX_LIGHTS 4
uniform int u_LightCount;
uniform struct Light {
   vec4 Position;
   vec3 Color;
   vec3 ConeDirection;
   float Attenuation;
   float ConeAngle;
} u_Lights[MAX_LIGHTS];

uniform vec3 u_CameraPosition;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(t_shdw0, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    float bias = max(0.005 * (1.0 - dot(normal, u_Lights[0].Position.xyz)), 0.005);
   
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(t_shdw0, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(t_shdw0, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{           
    vec3 color = texture(t_diff0, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 ambient = vec3(0.4);

   
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      

	vec3 diffuse = vec3(0.0,0.0,0.0);

	for (int i=0; i<u_LightCount; i++)
	{
		vec3 surfToLight;
		float atten = 1.0;

		// directional
		if (u_Lights[i].Position.w == 0.0)
		{
			surfToLight = normalize(u_Lights[i].Position.xyz);
			atten = 1.0;
		}
		else
		{
			float lightAtten = u_Lights[i].Attenuation;

			// point light
			vec3 toLight = u_Lights[i].Position.xyz - fs_in.FragPos;
			surfToLight = normalize(toLight);
			
			float sqDistToLight = dot(toLight, toLight);
			atten = 1.0 / (1.0 + lightAtten * sqDistToLight);
		
			// cone

			float coneAngle = u_Lights[i].ConeAngle;

			if (coneAngle <= 90.0)
			{
				vec3 coneDir = u_Lights[i].ConeDirection;

				float lightToSurfAngle = degrees(acos(dot(-surfToLight, normalize(coneDir))));
				if (lightToSurfAngle > coneAngle)
				{
					atten = 0.0;
				}
			}
		}

		// diffuse
		float diff = max(dot(surfToLight, normal), 0.0);
		vec3 curDiffuse = diff * u_Lights[i].Color;

		diffuse += atten * curDiffuse;
	}


    vec3 lighting = (ambient + (1.0 - shadow) * diffuse) * color;    
	
	if (textureSize(t_cube0, 0).x != 1)
	{
		vec3 I = normalize(fs_in.FragPos - u_CameraPosition);
		vec3 R = reflect(I, normal);

		vec3 reflection = texture(t_cube0, R).rgb;
		lighting *= pow(reflection * 2, 1.0 - diffuse);
	}

    FragColor = vec4(lighting, 1.0);
}