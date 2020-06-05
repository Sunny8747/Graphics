#version 140
//#define GL_compatibility_profile 1
#extension GL_ARB_compatibility: enable

out vec3 normal, lightDir;
out vec4 p;
uniform float m_Scale;
void main()
{	
    // ToDo
	
	vec4 P_obj = gl_Vertex;
	P_obj.x = P_obj.x * m_Scale;
	P_obj.y = P_obj.y * m_Scale;
	P_obj.z = P_obj.z * m_Scale;

	gl_Position = gl_ModelViewProjectionMatrix * (P_obj);
	normal = normalize(gl_NormalMatrix * gl_Normal).xyz;

	gl_BackColor = vec4(0,0,0,1);
}
