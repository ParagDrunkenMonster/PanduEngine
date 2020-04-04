uniform mat4 PANDUModelShadowProjetionMatrix0;

varying vec4 diffuse,ambient,shadowCoord;
varying float NdotL,NdotH;

void main(void)
{
	vec3 n = normalize(gl_NormalMatrix * gl_Normal).xyz;
	vec4 vertexInViewSpace = gl_ModelViewMatrix * gl_Vertex;
	
	vec3 lightDir = normalize( gl_LightSource[0].position.xyz - vertexInViewSpace.xyz * gl_LightSource[0].position.w);
	
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;
			
	float NdotL = max(dot(n, lightDir), 0.0);
	float NdotH = max(dot(n, gl_LightSource[0].halfVector.xyz), 0.0);

	gl_Position = ftransform();
	shadowCoord = PANDUModelShadowProjetionMatrix0 * vec4(gl_Vertex.xyz,1.0);
}
