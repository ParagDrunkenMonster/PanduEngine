uniform mat4 PANDUModelShadowProjetionMatrix0;

attribute vec3 PANDUTangent;
attribute vec3 PANDUBinormal;

varying vec4 diffuse,ambient,shadowCoord;
varying vec3 lightDir,halfVector;

void main(void)
{
	vec3 n = normalize(gl_NormalMatrix * gl_Normal).xyz;
	vec3 t = normalize(gl_NormalMatrix * PANDUTangent);
	vec3 b = normalize(gl_NormalMatrix * PANDUBinormal);
	
	vec4 vertexInViewSpace = gl_ModelViewMatrix * gl_Vertex;
	
	lightDir = normalize( gl_LightSource[0].position.xyz - vertexInViewSpace.xyz * gl_LightSource[0].position.w);
	
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;
		
	mat3 rotmat = mat3(		t.x, b.x, n.x,
							t.y, b.y, n.y,
							t.z, b.z, n.z);
	lightDir = rotmat * lightDir;
	lightDir = normalize(lightDir); 
	
	halfVector = rotmat * gl_LightSource[0].halfVector.xyz;
	halfVector = normalize(halfVector); 

	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
	shadowCoord = PANDUModelShadowProjetionMatrix0 * vec4(gl_Vertex.xyz,1.0);
}
