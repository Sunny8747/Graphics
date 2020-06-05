#version 140
#extension GL_ARB_compatibility: enable

in vec4 p;
in vec3 normal, lightDir;
uniform int toon_Level;
in float diffuse_level;

void main()
{	
	vec4 color;
	// ToDo

	vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	vec4 specular =  gl_FrontMaterial.specular * gl_LightSource[0].specular;
	float level_scale = 1.0 / (toon_Level - 1);

	if(toon_Level == 2) {
		if(diffuse_level > 0.98) {
			gl_FragColor = specular;
		}
		else {
			gl_FragColor = 0.5 * diffuse;
		}
	}
	else if(toon_Level == 3) {
		if(diffuse_level > 0.98) {
			gl_FragColor = specular;
		}
		else if(diffuse_level > level_scale){
			gl_FragColor = (1 - level_scale) * diffuse;
		}
		else {
			gl_FragColor = pow(level_scale,2) * diffuse;
		}
	}
	else if(toon_Level == 4) {
		if(diffuse_level > 0.98) {
			gl_FragColor = specular;
		}
		else if(diffuse_level > 2 * level_scale){
			gl_FragColor = (1 - level_scale) * diffuse;
		}
		else if(diffuse_level > level_scale){
			gl_FragColor = (1 - 2 * level_scale) * diffuse;
		}
		else {
			gl_FragColor = pow(level_scale,2) * diffuse;
		}
	}
	else if(toon_Level == 5) {
		if(diffuse_level > 0.98) {
			gl_FragColor = specular;
		}
		else if(diffuse_level > 3 * level_scale){
			gl_FragColor = (1 - level_scale) * diffuse;
		}
		else if(diffuse_level > 2 * level_scale){
			gl_FragColor = (1 - 2 * level_scale) * diffuse;
		}
		else if(diffuse_level > level_scale){
			gl_FragColor = (1 - 3 * level_scale) * diffuse;
		}
		else {
			gl_FragColor = pow(level_scale,2) * diffuse;
		}
	}
	else if(toon_Level == 6) {
		if(diffuse_level > 0.98) {
			gl_FragColor = specular;
		}
		else if(diffuse_level > 4 * level_scale){
			gl_FragColor = (1 - level_scale) * diffuse;
		}
		else if(diffuse_level > 3 * level_scale){
			gl_FragColor = (1 - 2 * level_scale) * diffuse;
		}
		else if(diffuse_level > 2 * level_scale){
			gl_FragColor = (1 - 3 * level_scale) * diffuse;
		}
		else if(diffuse_level > level_scale){
			gl_FragColor = (1 - 4 * level_scale) * diffuse;
		}
		else {
			gl_FragColor = pow(level_scale,2) * diffuse;
		}
	}
	else if(toon_Level == 7) {
		if(diffuse_level > 0.98) {
			gl_FragColor = specular;
		}
		else if(diffuse_level > 5 * level_scale){
			gl_FragColor = (1 - level_scale) * diffuse;
		}
		else if(diffuse_level > 4 * level_scale){
			gl_FragColor = (1 - 2 * level_scale) * diffuse;
		}
		else if(diffuse_level > 3 * level_scale){
			gl_FragColor = (1 - 3 * level_scale) * diffuse;
		}
		else if(diffuse_level > 2 * level_scale){
			gl_FragColor = (1 - 4 * level_scale) * diffuse;
		}
		else if(diffuse_level > level_scale){
			gl_FragColor = (1 - 5 * level_scale) * diffuse;
		}
		else {
			gl_FragColor = pow(level_scale,2) * diffuse;
		}
	}
	else if(toon_Level == 8) {
		if(diffuse_level > 0.98) {
			gl_FragColor = specular;
		}
		else if(diffuse_level > 6 * level_scale){
			gl_FragColor = (1 - level_scale) * diffuse;
		}
		else if(diffuse_level > 5 * level_scale){
			gl_FragColor = (1 - 2 * level_scale) * diffuse;
		}
		else if(diffuse_level > 4 * level_scale){
			gl_FragColor = (1 - 3 * level_scale) * diffuse;
		}
		else if(diffuse_level > 3 * level_scale){
			gl_FragColor = (1 - 4 * level_scale) * diffuse;
		}
		else if(diffuse_level > 2 * level_scale){
			gl_FragColor = (1 - 5 * level_scale) * diffuse;
		}
		else if(diffuse_level > level_scale){
			gl_FragColor = (1 - 6 * level_scale) * diffuse;
		}
		else {
			gl_FragColor = pow(level_scale,2) * diffuse;
		}
	}
	else {
		if(diffuse_level > 0.98) {
			gl_FragColor = specular;
		}
		else if(diffuse_level > 7 * level_scale){
			gl_FragColor = 0.875 * diffuse;
		}
		else if(diffuse_level > 6 * level_scale){
			gl_FragColor = (1 - 2 * level_scale) * diffuse;
		}
		else if(diffuse_level > 5 * level_scale){
			gl_FragColor = (1 - 3 * level_scale) * diffuse;
		}
		else if(diffuse_level > 4 * level_scale){
			gl_FragColor = (1 - 4 * level_scale) * diffuse;
		}
		else if(diffuse_level > 3 * level_scale){
			gl_FragColor = (1 - 5 * level_scale) * diffuse;
		}
		else if(diffuse_level > 2 * level_scale){
			gl_FragColor = (1 - 6 * level_scale) * diffuse;
		}
		else if(diffuse_level > level_scale){
			gl_FragColor = (1 - 7 * level_scale) * diffuse;
		}
		else {
			gl_FragColor = pow(level_scale,2) * diffuse;
		}
	}
}