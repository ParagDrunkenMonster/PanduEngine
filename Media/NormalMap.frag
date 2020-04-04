uniform sampler2D NormalMapSampler;
uniform sampler2D ParallaxMapSampler;
uniform sampler2D OcclusionMapSampler;
uniform sampler2D ShadowMapSampler;

uniform float ParallaxScale;
uniform float ParallaxBias;

varying vec4 diffuse,ambient,shadowCoord;
varying vec3 lightDir,halfVector;

float prcntOfDiffusOnOppositeLitDir = 0.25f;

const float InvShadowMapSize = 1.0f/4096.0f;

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
			vec4 depthColor = texture2D(ShadowMapSampler,shadowCoordinateWdivide.st + vec2(u,v) * vec2(InvShadowMapSize,InvShadowMapSize));
			float distanceFromLight = GetDepth(depthColor);
			
			//if (shadowCoord.w > 0.0)
			{
				shadow += distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
			}
		}
	}
	
	shadow /= 16.0f;
	
	return shadow;
}


void main (void)  
{	
	vec3 h = normalize(halfVector);
	float height = texture2D(ParallaxMapSampler, gl_TexCoord[0].st).r;
	height = height * ParallaxScale + ParallaxBias;
	
	vec2 newTexCoord = gl_TexCoord[0].st + (height * h.xy);

	vec3 textureNormal = texture2D(NormalMapSampler, newTexCoord.xy).xyz; 	
	vec3 n = normalize((textureNormal * 2.0 ) - 1.0 );
	vec3 l = normalize(lightDir);

	float NdotL = max(dot(n, l), 0.0);
	float NdotLOpposite = max(dot(n,-l),0.0);
	float NdotH = max(dot(n, h), 0.0);
	float power = 0.0;
	
	if(NdotL > 0.0)
	{
		power = pow(NdotH, gl_FrontMaterial.shininess);
	}
	
	vec4 occlusion = texture2D(OcclusionMapSampler, newTexCoord.xy);
	vec4 color = ambient * vec4(occlusion.rgb,1.0);


	color += diffuse * NdotL + diffuse * prcntOfDiffusOnOppositeLitDir * NdotLOpposite;
	color += gl_FrontMaterial.specular * gl_LightSource[0].specular * power;

	gl_FragColor =  vec4(color.rgb,1.0) * ComputeShadow();
}    
