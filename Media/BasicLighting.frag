uniform sampler2D ShadowMapSampler;
uniform vec2 InvShadowMapSize;

varying vec4 diffuse,ambient,shadowCoord;
varying float NdotL,NdotH;


float GetDepth(vec4 depthColor)
{
	float depth = depthColor.x * 255.0 / 256.0 + depthColor.y * 255.0 / 65536.0 + depthColor.z * 255.0 / 16777216.0;
	return depth;
}

float ComputeShadow()
{
	vec3 shadowCoordinateWdivide = shadowCoord.xyz / shadowCoord.w ;

	// Used to lower moiré pattern and self-shadowing
	shadowCoordinateWdivide.z -= 0.00009;

	float shadow = 1.0;
	for( float u = -1.5 ; u <= 1.5 ; u += 1.0f )
	{
		for( float v = -1.5 ; v <= 1.5 ; v += 1.0f )
		{
			vec4 depthColor = texture2D(ShadowMapSampler,shadowCoordinateWdivide.st + vec2(u,v) * InvShadowMapSize);
			float distanceFromLight = GetDepth(depthColor);
			
			shadow += distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
		}
	}
	
	shadow /= 16.0f;
	
	return shadow;
}


void main (void)  
{	
	float power = 0.0;
	
	if(NdotL > 0.0)
	{
		power = pow(NdotH, gl_FrontMaterial.shininess);
	}
	
	vec4 color = ambient;

	color += diffuse * NdotL;
	color += gl_FrontMaterial.specular * gl_LightSource[0].specular * power;

	gl_FragColor =  vec4(color.rgb,1.0) * ComputeShadow();
}    
