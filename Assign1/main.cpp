/*
 * Skeleton code for CSE471 Intro to Computer Graphics
 *
 * Won-Ki Jeong, wkjeong@unist.ac.kr
 * 20131329 Shin Sunwoo
 */

#define _USE_MATH_DEFINES

#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <fstream>

GLdouble bunny_vert[104505] = { 0 }; // 104505
GLdouble bunny_normal[104505] = { 0 };
int bunny_ind[208419] = { 0 }; // 208419
//test cube
/*
float vertices_coords[] = {
	-0.5,  0.5, 0.0, 0.5, //0   0 1		up
	0.5, 0.5, 0.0, 0.5, //1     3 2
	0.5, -0.5, 0.0, 0.5, //2 
	-0.5, -0.5, 0.0, 0.5, //3 
	- 0.5,  0.5, 0.0, -0.5, //4 4 5		down
	0.5, 0.5, 0.0, -0.5, //5    7 6
	0.5, -0.5, 0.0, -0.5, //6
	- 0.5, -0.5, 0.0, -0.5 //7
};

float vertices_color[] = {
	1, 0, 0, 1,//R (1,0,0,1)
	0, 1, 0, 1,//G (0,1,0,1) 
	0, 0, 1, 1,//B (0,0,1,1)
	1, 0, 0, 1,//R (1,0,0,1)
	1, 0, 0, 1,//R (1,0,0,1)
	0, 0, 1, 1,//B (0,0,1,1)
	0, 1, 0, 1,//G (0,1,0,1) 
	1, 0, 0, 1 //R (1,0,0,1)
};

int vertices_index[] = {
	0,1,2,3,
	0,1,5,4,
	1,2,6,5,
	3,2,6,7,
	0,3,7,4,
	7,6,5,4
};*/

GLfloat lightPos1[] = {600, 500, 600, 1};
GLfloat diffuse1[] =  {1, 1, 0, 3};
GLfloat specular1[] = {1, 1, 1, 1};
GLfloat ambient1[] =  {0, 0, 1, 1};

GLfloat lightPos2[] = { -50, -50, -50, 1};
GLfloat diffuse2[] = { 0, 0, 1, 1 };
GLfloat specular2[] = { 1, 1, 1, 1 };
GLfloat ambient2[] = { 1, 1, 0, 1 };

float lastPos[3] = { 0 };
float axis[3] = { 0 };
GLfloat angle = 0;
GLfloat m_transform[16] = { 0 };
const double DegreesToRadians = M_PI / 180.0;
bool trackingMouse = false;
bool panningMouse = false;
bool zoomingMouse = false;
float curPos[3] = { 0 };

void off_file_read(char* arg_path) {
	char* mesh_path = "mesh-data";
	char* bunny_name = "bunny.off";
	int count = 0;
	while (true) {
		if ((arg_path[count] == 's') && (arg_path[count + 7] == 'n') && (arg_path[count + 9] == 'c') && (arg_path[count + 12] == 'e')) { //checking skeleton-code and discard this part
			break;
		}
		count++;
	}
	char* path = (char*)malloc((sizeof(char)) * (count + 20)); //20
	strncpy(path, arg_path, count);
	for (int i = 0; i < 9; i++) {
		path[count + i] = mesh_path[i];
	}
	path[count + 9] = path[count - 1];
	for (int i = 0; i < 9; i++) { //9
		path[count + 10 + i] = bunny_name[i];
	}
	path[count + 19] = NULL; //19
	printf("%s\n", path);

	int no_ver = 0;
	int no_ind = 0;
	int no_temp = 0;

	FILE* off_file = fopen(path, "r");

	if (off_file == NULL) { //check file open
		printf("Open file fail\n");
	}

	char temp[10] = { 0 }; //temp record only OFF
	fscanf(off_file, "%s\n", temp); //reading off	

	fscanf(off_file, "%d %d %d\n", &no_ver, &no_ind, &no_temp); //reading no of vert and ind and 0
	count = 0;

	while (true) {
		if (count == 34835) { // reading vertex coordinate and put data to buffer
			break;
		}
		fscanf(off_file, "%lf %lf %lf\n", &bunny_vert[count * 3], &bunny_vert[count * 3 + 1], &bunny_vert[count * 3 + 2]);
		count++;
	}
	count = 0;
	while (true) {
		if (count == 69473) { // reading faces index and put data to buffer
			break;
		}
		fscanf(off_file, "%d %d %d %d\n", &no_temp, &bunny_ind[count * 3], &bunny_ind[count * 3 + 1], &bunny_ind[count * 3 + 2]); //no temp is always 3
		count++;
	}

	for (int i = 0; i < 104505; i++) { //it makes more accurate prevent ceil but makes bunny large size
		bunny_vert[i] = bunny_vert[i] * 10000;
	}
	//GLdouble face_normal[208419] = { 0 };
	GLdouble x1, y1, z1, x2, y2, z2, cx,cy,cz,tempDevide = 0;
	int nor_count[34835] = { 0 }; //vertex face normal count
	int indnum[3] = { 0 };
	GLdouble tempNorm[3] = { 0 };

	for (int i = 0; i < 69473; i++) {
		indnum[0] = bunny_ind[i * 3];
		indnum[1] = bunny_ind[i * 3 + 1];
		indnum[2] = bunny_ind[i * 3 + 2];

		nor_count[indnum[0]]++;
		nor_count[indnum[1]]++;
		nor_count[indnum[2]]++;

		x1 = bunny_vert[indnum[1]  ] - bunny_vert[indnum[0]  ]; // 1.x - 0.x
		y1 = bunny_vert[indnum[1]+1] - bunny_vert[indnum[0]+1]; // 1.y - 0.y
		z1 = bunny_vert[indnum[1]+2] - bunny_vert[indnum[0]+2]; // 1.z - 0.z
		x2 = bunny_vert[indnum[2]  ] - bunny_vert[indnum[0]  ]; // 2 - 0
		y2 = bunny_vert[indnum[2]+1] - bunny_vert[indnum[0]+1];
		z2 = bunny_vert[indnum[2]+2] - bunny_vert[indnum[0]+2];

		tempNorm[0] = y2 * z1 - z2 * y1;
		tempNorm[1] = z2 * x1 - x2 * z1;
		tempNorm[2] = x2 * y1 - y2 * x1;

		cx = (bunny_vert[indnum[0]] + bunny_vert[indnum[1]] + bunny_vert[indnum[2]]) / 3;
		cy = (bunny_vert[indnum[0] + 1] + bunny_vert[indnum[1] + 1] + bunny_vert[indnum[2] + 1]) / 3;
		cz = (bunny_vert[indnum[0] + 2] + bunny_vert[indnum[1] + 2] + bunny_vert[indnum[2] + 2]) / 3;

		if ((tempNorm[0] * cx + tempNorm[1] * cy + tempNorm[2] * cz) < 0) {
			tempNorm[0] = -tempNorm[0];
			tempNorm[1] = -tempNorm[1];
			tempNorm[2] = -tempNorm[2];
		}

		//tempDevide = sqrt(pow(bunny_normal[indnum[k] * 3] + tempNorm[0], 2) + pow(bunny_normal[indnum[k] * 3 + 1] + tempNorm[1], 2) + pow(bunny_normal[indnum[k] * 3 + 2] + tempNorm[2], 2));

		for (int k = 0; k < 3; k++) { //indnum 0 1 2 -> three vertex
			tempDevide = sqrt(pow(bunny_normal[indnum[k] * 3] + tempNorm[0], 2) + pow(bunny_normal[indnum[k] * 3 + 1] + tempNorm[1], 2) + pow(bunny_normal[indnum[k] * 3 + 2] + tempNorm[2], 2));
			bunny_normal[indnum[k] * 3    ] = bunny_normal[indnum[k] * 3    ] + tempNorm[0] / tempDevide; //x
			bunny_normal[indnum[k] * 3 + 1] = bunny_normal[indnum[k] * 3 + 1] + tempNorm[1] / tempDevide; //y
			bunny_normal[indnum[k] * 3 + 2] = bunny_normal[indnum[k] * 3 + 2] + tempNorm[2] / tempDevide; //z
		}
	}
	
	fclose(off_file);

	//checking part
	printf("last vert is : %lf\n", bunny_vert[104504]);
	printf("first vert is  : %lf\n", bunny_vert[0]);
	printf("first ind is : %d\n", bunny_ind[0]);
	printf("last ind is : %d\n", bunny_ind[208418]);
	double check1 = bunny_vert[104504];
	double check = bunny_vert[104504] * 1000000000;
	printf("%lf\n", check1);
}

void reshape(int w, int h)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0, 0, -10000,
		0, 0, 0,
		0, 1, 0
	);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1500, 2000, -1000, 2500, 100000, -100000);
}

void Lightfunc() {
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient1);
	
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
}

void renderScene(void) 
{
	glClearColor(1, 1, 1, 1);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Lightfunc();

	// Draw something here!
	GLuint array_buffer;
	GLuint normal_buffer;
	GLuint index_buffer;

	glColor3f(1, 1, 1);

	glGenBuffers(1, &array_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, array_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bunny_vert), bunny_vert, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, array_buffer);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);

	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bunny_normal), bunny_normal, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, 0);

	glGenBuffers(1, &index_buffer); //indexbuffer don't need attribarray
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bunny_ind), bunny_ind, GL_STATIC_DRAW);

	//glDrawArrays(GL_TRIANGLES, 0, 1);
	glDrawElements(GL_TRIANGLES, 208418, GL_UNSIGNED_INT, 0); //same with no of ind not no of triangle

	//old one no buffer use. CPU use
	/*glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices_coords);
	glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);
	glIndexPointer(GL_FLOAT, 0, vertices_index);*/
	
	//glutSolidTeapot(2); //teapot check
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'q': //quit
		case 'Q':
			exit(0);
			break;
		case 'o': //orthogonal
		case 'O':
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-1500, 2000, -1000, 2500, 100000, -100000);
			glutPostRedisplay();
			break;
		case 'p': //perspective
		case 'P':
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(40, 1, 0.1, 100000);
			glutPostRedisplay();
			break;
		case 's': //smooth
		case 'S':
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glutPostRedisplay();
			break;
		case 'w': //wire
		case 'W':
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glutPostRedisplay();
			break;
	}
}

void trackball_ptov(int x, int y, int width, int height, float v[3]) {
	float d, a;
	//project x,y onto a hemi sphere centered within width, height
	v[0] = (2.0F * x - width) / width;
	v[1] = (height - 2.0F * y) / height;
	d = (float)sqrt(v[0] * v[0] + v[1] * v[1]);
	v[2] = (float)cos((M_PI / 2.0F) * ((d < 1.0F) ? d : 1.0F));
	a = 1.0F / (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] *= a;
	v[1] *= a;
	v[2] *= a;
}

void mouseMotion(int x, int y) {
	float curPos[3], dx, dy, dz;
	// compute position on hemisphere
	trackball_ptov(x, y, 800, 800, curPos);
	if (trackingMouse) {
		// compute the change in position on the hemisphere
		dx = curPos[0] - lastPos[0];
		dy = curPos[1] - lastPos[1];
		dz = curPos[2] - lastPos[2];
		printf("dx is : %f dy is : %f dz is : %f\n", dx, dy, dz);
		if (dx || dy || dz) { // compute theta and cross product
			angle = 90.0 * sqrt(dx * dx + dy * dy + dz * dz);
			axis[0] = lastPos[1] * curPos[2] - lastPos[2] * curPos[1];
			axis[1] = lastPos[2] * curPos[0] - lastPos[0] * curPos[2];
			axis[2] = lastPos[0] * curPos[1] - lastPos[1] * curPos[0];
			lastPos[0] = curPos[0];
			lastPos[1] = curPos[1];
			lastPos[2] = curPos[2];
		}
		glRotatef(dx*5, 1, 0, 0);
		glRotatef(dy*5, 0, 1, 0);
		glRotatef(dz*5, 0, 0, 1);
		//glMultMatrixf(m_transform);
		glutPostRedisplay();
	}
	if (panningMouse) {
		// compute the change in position on the hemisphere
		dx = curPos[0] - lastPos[0];
		dy = curPos[1] - lastPos[1];
		dz = curPos[2] - lastPos[2];
		printf("dx is : %f dy is : %f dz is : %f\n", dx, dy, dz);
		if (dx || dy || dz) { // compute theta and cross product
			angle = 90.0 * sqrt(dx * dx + dy * dy + dz * dz);
			axis[0] = lastPos[1] * curPos[2] - lastPos[2] * curPos[1];
			axis[1] = lastPos[2] * curPos[0] - lastPos[0] * curPos[2];
			axis[2] = lastPos[0] * curPos[1] - lastPos[1] * curPos[0];
			lastPos[0] = curPos[0];
			lastPos[1] = curPos[1];
			lastPos[2] = curPos[2];
		}
		glTranslatef(dx * 1000, dy * 1000, dz * 1000);
		glutPostRedisplay();
	}
	if (zoomingMouse) {
		// compute the change in position on the hemisphere
		dx = curPos[0] - lastPos[0];
		dy = curPos[1] - lastPos[1];
		dz = curPos[2] - lastPos[2];
		printf("dx is : %f dy is : %f dz is : %f\n", dx, dy, dz);
		if (dx || dy || dz) { // compute theta and cross product
			angle = 90.0 * sqrt(dx * dx + dy * dy + dz * dz);
			axis[0] = lastPos[1] * curPos[2] - lastPos[2] * curPos[1];
			axis[1] = lastPos[2] * curPos[0] - lastPos[0] * curPos[2];
			axis[2] = lastPos[0] * curPos[1] - lastPos[1] * curPos[0];
			lastPos[0] = curPos[0];
			lastPos[1] = curPos[1];
			lastPos[2] = curPos[2];
		}
		glTranslatef(0, 0, dz * 5000);
		glutPostRedisplay();
	}
}

void mouse (int button, int state, int x, int y) {
	//GLfloat* m_transform;
	if (button == GLUT_LEFT_BUTTON) {
		switch (state) {
			case GLUT_DOWN:
				trackingMouse = true;
				break;
			case GLUT_UP:
				trackingMouse = false;
				break;
		}
	}
	if (button == GLUT_MIDDLE_BUTTON ) {
		switch (state) {
			case GLUT_DOWN:
				panningMouse = true;
				break;
			case GLUT_UP:
				panningMouse == false;
				break;
		}
	}
	if (button == GLUT_RIGHT_BUTTON) {
		switch (state) {
		case GLUT_DOWN:
			zoomingMouse = true;
			break;
		case GLUT_UP:
			zoomingMouse = false;
			break;
		}
	}
}


int main(int argc, char** argv)
{
	char* arg_path = *argv;
	off_file_read(arg_path);
	
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glEnable(GL_DEPTH_TEST);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CSE471 - Assignment 1");

	glewInit();
	GLint max_ind;
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &max_ind);
	printf("max ind is : %d\n", max_ind);
	// register callbacks

	glEnable(GL_DEPTH_TEST);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutReshapeFunc(reshape);
	glutDisplayFunc(renderScene);

	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	glutMainLoop();

	return 1;
}