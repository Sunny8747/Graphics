#version 140
//#define GL_compatibility_profile 1
#extension GL_ARB_compatibility: enable

out vec3 normal, lightDir;
out vec4 p;
out float diffuse_level;

uniform int toon_Level;

void main()
{	
	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal).xyz;
	lightDir = normalize(gl_LightSource[0].position).xyz;
	diffuse_level = dot(lightDir, normal);
}
