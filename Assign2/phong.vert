#version 140
//#define GL_compatibility_profile 1
#extension GL_ARB_compatibility: enable

out vec3 normal;
out vec4 p;
uniform float A_S;
uniform float D_S;
uniform float S_S;
uniform float Shiny_S;
out float dist;

void main()
{	
	// Clip Coordinate

	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal).xyz;
	p = gl_ModelViewMatrix * gl_Vertex;

	dist = length(vec3(gl_LightSource[0].position - p));

}
