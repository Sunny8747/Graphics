#version 140
#extension GL_ARB_compatibility: enable

in vec4 p;
in vec3 normal;
in float dist;
uniform float A_S;
uniform float D_S;
uniform float S_S;
uniform float Shiny_S;

void main()
{	
	vec4 n = normalize(vec4(normal, 0));
	vec4 l = normalize(gl_LightSource[0].position - p);
	vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;

	vec4 diffuse = D_S * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

	vec4 v = normalize(-p);
	vec4 r = normalize(reflect(-l,n));

	vec4 loc_ambient = A_S * gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
	vec4 glb_ambient = A_S * gl_LightModel.ambient * gl_FrontMaterial.ambient;

	float att = 8.0 / (gl_LightSource[0].constantAttenuation +
					gl_LightSource[0].linearAttenuation * dist +
					gl_LightSource[0].quadraticAttenuation * dist * dist);

	vec4 specular = S_S * gl_FrontMaterial.specular * gl_LightSource[0].specular;

	gl_FragColor = glb_ambient + att * (max(dot(l,n),0) * diffuse + loc_ambient + specular * pow(max(dot(r, n), 0), Shiny_S));
}