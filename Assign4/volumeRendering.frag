#version 140
#extension GL_ARB_compatibility: enable

in vec3 pixelPosition;
uniform vec3 eyePosition;
uniform vec3 objectMin;
uniform vec3 objectMax;
uniform vec3 up;
uniform sampler3D tex;
uniform sampler1D transferFunction;

#define MIP

void main(){
	//	Orthogonal Projection
	//	vec3 rayDirection=normalize(-eyePosition); 
    vec3 right=normalize(cross(-eyePosition,up));
    vec3 virtualScreenCoord=eyePosition*0.5+
                                right*pixelPosition.x+
                                up*pixelPosition.y;

	// Perspective Projection
    vec3 rayDirection=normalize(virtualScreenCoord-eyePosition); 
	int sampling_rate = 5000;		//sampling rate in MIP and alpha_comp
	int cnt=0;
	vec3 endPoint[2]; // 0 : entry point, 1 : exit point

	//
	// 1. find ray-cube intersect point 
	//
	// intersection testing
	// each face test
	//direction length
	//TopBottom
	float max_z = (objectMax.z - eyePosition.z)/rayDirection.z;
	float min_z = (objectMin.z - eyePosition.z)/rayDirection.z;
	//LeftRight
	float max_x = (objectMax.x - eyePosition.x)/rayDirection.x;
	float min_x = (objectMin.x - eyePosition.x)/rayDirection.x;
	//FrontBack
	float max_y = (objectMax.y - eyePosition.y)/rayDirection.y;
	float min_y = (objectMin.y - eyePosition.y)/rayDirection.y;
	//direction x coord and y coord to find line in the box
	vec3 testz1 = eyePosition + max_z*rayDirection;
	if(testz1.x > objectMin.x && testz1.x < objectMax.x && testz1.y > objectMin.y && testz1.y < objectMax.y){
		if(cnt == 0) {
			endPoint[0] = testz1;
		}
		else {
			endPoint[1] = testz1;
		}
		cnt += 1;
	}
	vec3 testz2 = eyePosition + min_z*rayDirection;
	if(testz2.x > objectMin.x && testz2.x < objectMax.x && testz2.y > objectMin.y && testz2.y < objectMax.y){
		if(cnt == 0) {
			endPoint[0] = testz2;
		}
		else {
			endPoint[1] = testz2;
		}
		cnt += 1;
	}
	vec3 testx1 = eyePosition + max_x*rayDirection;
	if(testx1.z > objectMin.z && testx1.z < objectMax.z && testx1.y > objectMin.y && testx1.y < objectMax.y){
		if(cnt == 0) {
			endPoint[0] = testx1;
		}
		else {
			endPoint[1] = testx1;
		}
		cnt += 1;
	}
	vec3 testx2 = eyePosition + min_x*rayDirection;
	if(testx2.z > objectMin.z && testx2.z < objectMax.z && testx2.y > objectMin.y && testx2.y < objectMax.y){
		if(cnt == 0) {
			endPoint[0] = testx2;
		}
		else {
			endPoint[1] = testx2;
		}
		cnt += 1;
	}
	vec3 testy1 = eyePosition + max_y*rayDirection;
	if(testy1.z > objectMin.z && testy1.z < objectMax.z && testy1.x > objectMin.x && testy1.x < objectMax.x){
		if(cnt == 0) {
			endPoint[0] = testy1;
		}
		else {
			endPoint[1] = testy1;
		}
		cnt += 1;
	}
	vec3 testy2 = eyePosition + min_y*rayDirection;
	if(testy2.z > objectMin.z && testy2.z < objectMax.z && testy2.x > objectMin.x && testy2.x < objectMax.x){
		if(cnt == 0) {
			endPoint[0] = testy2;
		}
		else {
			endPoint[1] = testy2;
		}
		cnt += 1;
	}

	
	///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MIP								//ifdefmip
	//
	// 2. Maximum intensity projection
	//
    vec4 composedColor=vec4(0,0,0,0);	// why all change?

    // .. ToDo
	//float maxval = 0;
	for(int i = 0; i < sampling_rate; i++){
		vec3 curPos = ((sampling_rate - i)*endPoint[0] + i*endPoint[1])/sampling_rate;
		curPos.x = (objectMax.x + curPos.x)/2/objectMax.x;	//object space to tex coord 0~1
		curPos.y = (objectMax.y + curPos.y)/2/objectMax.y;
		curPos.z = (objectMax.z + curPos.z)/2/objectMax.z;
		vec4 getAlpha = texture3D(tex, curPos);				// red color is only in tex green: 0 Blue:0 alpha:1??
		composedColor.r = max(composedColor.r, getAlpha.r);
	}
	gl_FragColor=vec4(composedColor.r,composedColor.r,composedColor.r,composedColor.r);
	
    
#else
	//
	// 3. alpha composition
	//
    float composedAlpha=0;
    vec4 composedColor=vec4(0,0,0,0);

    // .. ToDo
	//maxval = 0;
	float maxDist = pow((endPoint[1].x - eyePosition.x),2) + pow((endPoint[1].y - eyePosition.y),2) + pow((endPoint[1].z - eyePosition.z),2);
	float minDist = pow((endPoint[0].x - eyePosition.x),2) + pow((endPoint[0].y - eyePosition.y),2) + pow((endPoint[0].z - eyePosition.z),2);
	if(maxDist < minDist) {
		vec3 swap = endPoint[1];
		endPoint[1] = endPoint[0];
		endPoint[0] = swap;
	}
	for(int i = 0; i < sampling_rate; i++){
		vec3 curPos = ((sampling_rate - i)*endPoint[0] + i*endPoint[1])/sampling_rate;
		curPos.x = (objectMax.x + curPos.x)/2/objectMax.x;	//object space to tex coord 0~1
		curPos.y = (objectMax.y + curPos.y)/2/objectMax.y;
		curPos.z = (objectMax.z + curPos.z)/2/objectMax.z;
		vec4 getColor = texture3D(tex, curPos);
		vec4 newColor = texture1D(transferFunction, getColor.r);	// red color is only in tex
		float newAlpha = pow(newColor.a,2);
		composedColor += vec4((1-composedAlpha)*newAlpha*newColor.rgb,0);
		composedAlpha += (1-composedAlpha)*newAlpha;
		if(composedAlpha > 0.99) {
			break;
		}
	}	
	composedColor = vec4(composedColor.rgb,composedAlpha);
	gl_FragColor=composedColor;
    
#endif

}