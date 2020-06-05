/*
 * Skeleton code for CSE471 Fall 2019
 *
 * Won-Ki Jeong, wkjeong@unist.ac.kr
 */


#include <stdio.h>
#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <assert.h>
#define _USE_MATH_DEFINES
#include "math.h"

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>


//
// Definitions
//

typedef struct{
    unsigned char x, y, z, w;
} uchar4;
typedef unsigned char uchar;

// BMP loader
void LoadBMPFile(uchar4 **dst, int *width, int *height, const char *name);

#define screenSize 512
#define	imageSize 256

//
// Variables
//
bool trackingMouse = false;
float angle = 0.0;
float axis[3];
float trans[3];
float lastPos[3] = { 0.0, 0.0, 0.0 };
int curx, cury;
int startX, startY;
GLfloat m_transform[16];

// array to store synthetic cubemap raw image
static GLubyte image1[4][4][4];
static GLubyte image2[4][4][4];
static GLubyte image3[4][4][4];
static GLubyte image4[4][4][4];
static GLubyte image5[4][4][4];
static GLubyte image6[4][4][4];

// texture object handles, FBO handles
GLuint cube_tex, color_tex;
GLuint fb, depth_rb;

//
// Functions
//

// create synthetic data for static cubemap
void makeSyntheticImages(void)
{
	int i, j, c;
    
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			c = (( ((i&0x1)==0)^((j&0x1)==0)))*255;
			image1[i][j][0] = (GLubyte) c;
			image1[i][j][1] = (GLubyte) c;
			image1[i][j][2] = (GLubyte) c;
			image1[i][j][3] = (GLubyte) 255;
			
			image2[i][j][0] = (GLubyte) c;
			image2[i][j][1] = (GLubyte) c;
			image2[i][j][2] = (GLubyte) 0;
			image2[i][j][3] = (GLubyte) 255;
			
			image3[i][j][0] = (GLubyte) c;
			image3[i][j][1] = (GLubyte) 0;
			image3[i][j][2] = (GLubyte) c;
			image3[i][j][3] = (GLubyte) 255;
			
			image4[i][j][0] = (GLubyte) 0;
			image4[i][j][1] = (GLubyte) c;
			image4[i][j][2] = (GLubyte) c;
			image4[i][j][3] = (GLubyte) 255;
			
			image5[i][j][0] = (GLubyte) 255;
			image5[i][j][1] = (GLubyte) c;
			image5[i][j][2] = (GLubyte) c;
			image5[i][j][3] = (GLubyte) 255;
			
			image6[i][j][0] = (GLubyte) c;
			image6[i][j][1] = (GLubyte) c;
			image6[i][j][2] = (GLubyte) 255;
			image6[i][j][3] = (GLubyte) 255;
		}
	}
}

void init(void)
{
	// You need to ues glew
	glewInit();
	
	GLfloat diffuse[4] = {1.0, 1.0, 1.0, 1.0};
	
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// make synthetic cubemap data
	makeSyntheticImages();
	
	//
	// Creating a 2D texture from image
	// 
	int width, height;
	uchar4 *dst;
	LoadBMPFile(&dst, &width, &height, "../mob.bmp"); // this is how to load image
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &color_tex);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dst);
	//glBindTexture(GL_TEXTURE_2D, 0);

	
#define STATIC_CUBEMAP //STATIC_CUBEMAP 

	//
	// creating cube map texture (either static or dynamic)
	//
	//uchar4* cdst;
	//LoadBMPFile(&cdst, &width, &height, "../mob.bmp");
#ifdef STATIC_CUBEMAP
	// create static cubemap from synthetic data
	glGenTextures(1, &cube_tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_tex);
	// ToDo...
	uchar4* cdst1;
	//LoadBMPFile(&cdst1, &width, &height, "../mob.bmp");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	//LoadBMPFile(&cdst1, &width, &height, "../mob.bmp");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	//LoadBMPFile(&cdst1, &width, &height, "../stone.bmp");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image3);
	//LoadBMPFile(&cdst1, &width, &height, "../stone.bmp");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image4);
	//LoadBMPFile(&cdst1, &width, &height, "../grass.bmp");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image5);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image6);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif


// generate cubemap on-the-fly
#ifdef DYNAMIC_CUBEMAP
	//RGBA8 Cubemap texture, 24 bit depth texture, 256x256
	glGenTextures(1, &cube_tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_tex);
	
	// ToDo...


	//
	// creating FBO and attach cube map texture
	//

	//-------------------------
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	//Attach one of the faces of the Cubemap texture to this FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, cube_tex, 0);
	//-------------------------
	glGenRenderbuffers(1, &depth_rb);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_rb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, imageSize, imageSize);
	//-------------------------
	//Attach depth buffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rb);
	//-------------------------
	//Does the GPU support current FBO configuration?
	GLenum status;
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		std::cout<<"good"<<std::endl; break;
	default:
		assert(false); break;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
#endif
}

void idle()
{
	// do something for animation here b/c this will be called when idling

	glutPostRedisplay();
}

// dynamically update cubemap
void update_cubemap(GLuint temp_tex)
{
	// bind FBO to render to texture
	glBindFramebuffer(GL_FRAMEBUFFER, fb);

	// render to +x face
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, temp_tex, 0);

	// render the entire scene here...

	// render to -x face
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, temp_tex, 0);

	// render the entire scene here...

	// render to +y face
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, temp_tex, 0);

	// render the entire scene here...

	// render to -y face
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, temp_tex, 0);

	// render the entire scene here...

	// render to +z face
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, temp_tex, 0);

	// render the entire scene here...

	// render to -z face
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, temp_tex, 0);

	// render the entire scene here...
	glutSolidCube(10);
	// unbind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void render_cube(float s, GLuint texture) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	//front
	glBegin(GL_QUADS);
	glTexCoord2f(0.33333, 0.5);		glVertex3f(-s, s, s);
	glTexCoord2f(0.66666, 0.5);		glVertex3f(s, s, s);
	glTexCoord2f(0.66666, 0.25);	glVertex3f(s, -s, s);
	glTexCoord2f(0.33333, 0.25);	glVertex3f(-s, -s, s);
	glEnd();
	//back
	glBegin(GL_QUADS);
	glTexCoord2f(0.33333, 0.75);	glVertex3f(-s, s, -s);
	glTexCoord2f(0.66666, 0.75);	glVertex3f(s, s, -s);
	glTexCoord2f(0.66666, 1);		glVertex3f(s, -s, -s);
	glTexCoord2f(0.33333, 1);		glVertex3f(-s, -s, -s);
	glEnd();
	//up
	glBegin(GL_QUADS);
	glTexCoord2f(0.33333f, 0.5);	glVertex3f(-s, s, s);
	glTexCoord2f(0.66666f, 0.5);	glVertex3f(s, s, s);
	glTexCoord2f(0.66666f, 0.75);	glVertex3f(s, s, -s);
	glTexCoord2f(0.33333f, 0.75);	glVertex3f(-s, s, -s);
	glEnd();
	//down
	glBegin(GL_QUADS);
	glTexCoord2f(0.33333f, 0.25f);	glVertex3f(-s, -s, s);
	glTexCoord2f(0.66666f, 0.25f);	glVertex3f(s, -s, s);
	glTexCoord2f(0.66666f, 0);		glVertex3f(s, -s, -s);
	glTexCoord2f(0.33333f, 0);		glVertex3f(-s, -s, -s);
	glEnd();
	//left
	glBegin(GL_QUADS);
	glTexCoord2f(0.33333f, 0.75f);	glVertex3f(-s, s, -s);
	glTexCoord2f(0.33333f, 0.5f);	glVertex3f(-s, s, s);
	glTexCoord2f(0, 0.5);			glVertex3f(-s, -s, s);
	glTexCoord2f(0, 0.75);			glVertex3f(-s, -s, -s);
	glEnd();
	//right
	glBegin(GL_QUADS);
	glTexCoord2f(0.66666f, 0.5f);	glVertex3f(s, s, s);
	glTexCoord2f(0.66666f, 0.75f);	glVertex3f(s, s, -s);
	glTexCoord2f(1, 0.75);			glVertex3f(s, -s, -s);
	glTexCoord2f(1, 0.5);			glVertex3f(s, -s, s);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void render_cubemap(float s, GLuint texture) {
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);	
	/*glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);*/
	
	//glutSolidCube(4);

	//front
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);//red
	glTexCoord2f(0.33333, 0.5);		glVertex3f(-s, s, s);
	glTexCoord2f(0.66666, 0.5);		glVertex3f(s, s, s);
	glTexCoord2f(0.66666, 0.25);	glVertex3f(s, -s, s);
	glTexCoord2f(0.33333, 0.25);	glVertex3f(-s, -s, s);
	glEnd();
	//back
	glBegin(GL_QUADS);
	glColor3f(0, 1, 0);//g
	glTexCoord3f(0.33333, 1,0);		glVertex3f(-s, -s, -s);
	glTexCoord3f(0.66666, 1,0);		glVertex3f(s, -s, -s);
	glTexCoord3f(0.66666, 0.75,0);	glVertex3f(s, s, -s);
	glTexCoord3f(0.33333, 0.75,0);	glVertex3f(-s, s, -s);	
	glEnd();
	//up
	glBegin(GL_QUADS);
	glColor3f(0, 0, 1);//b
	glTexCoord2f(0.33333f, 0.75);	glVertex3f(-s, s, -s);
	glTexCoord2f(0.66666f, 0.75);	glVertex3f(s, s, -s);
	glTexCoord2f(0.66666f, 0.5);	glVertex3f(s, s, s);
	glTexCoord2f(0.33333f, 0.5);	glVertex3f(-s, s, s);
	glEnd();
	//down
	glBegin(GL_QUADS);
	glColor3f(1, 1, 0);//rg
	glTexCoord2f(0.33333f, 0.25f);	glVertex3f(-s, -s, s);
	glTexCoord2f(0.66666f, 0.25f);	glVertex3f(s, -s, s);
	glTexCoord2f(0.66666f, 0);		glVertex3f(s, -s, -s);
	glTexCoord2f(0.33333f, 0);		glVertex3f(-s, -s, -s);
	glEnd();
	//left
	glBegin(GL_QUADS);
	glColor3f(0, 1, 1);//gb
	glTexCoord2f(0.33333f, 0.75f);	glVertex3f(-s, s, -s);
	glTexCoord2f(0.33333f, 0.5f);	glVertex3f(-s, s, s);
	glTexCoord2f(0, 0.5);			glVertex3f(-s, -s, s);
	glTexCoord2f(0, 0.75);			glVertex3f(-s, -s, -s);
	glEnd();
	//right
	glBegin(GL_QUADS);
	glColor3f(1, 0, 1);//rb
	glTexCoord2f(0.66666f, 0.5f);	glVertex3f(s, s, s);
	glTexCoord2f(0.66666f, 0.75f);	glVertex3f(s, s, -s);
	glTexCoord2f(1, 0.75);			glVertex3f(s, -s, -s);
	glTexCoord2f(1, 0.5);			glVertex3f(s, -s, s);
	glEnd();

	glDisable(GL_TEXTURE_CUBE_MAP);
}

void display(void)
{
	// update dynamic cubemap per frame
#ifdef DYNAMIC_CUBEMAP
	update_cubemap();
#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	printf("image1[4][4][4] = %d %d %d %d\n", image1[0][0][0], image1[1][1][1], image1[2][2][2], image1[3][3][3]);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	render_cubemap(5, cube_tex);
	glDisable(GL_CULL_FACE);

	// render something here...
	if (trackingMouse) {
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0, 0.0, -20.0);
		glRotatef(angle, axis[0], axis[1], axis[2]);
		glTranslatef(0, 0, 20);
		glMultMatrixf(m_transform);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)m_transform);
		glColor3f(1, 1, 1);

		render_cube(1, color_tex);
		//glutSolidTorus(2, 0.5, 100, 100);
		//glutSolidCube(1);

		glPopMatrix();
	}
	else {
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(m_transform);

		glColor3f(1, 1, 1);

		render_cube(1, color_tex);
		//glutSolidTorus(2, 0.5, 100, 100);
		//glutSolidCube(1);

		glPopMatrix();
	}
	//glutSolidCube(2);
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)m_transform);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -20);
	glMultMatrixf(m_transform);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)m_transform);
	glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q') {
		printf("Quit Window\n");
		exit(0);
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
	trackball_ptov(x, y, screenSize, screenSize, curPos);
	if (trackingMouse) {
		// compute the change in position on the hemisphere
		dx = curPos[0] - lastPos[0];
		dy = curPos[1] - lastPos[1];
		dz = curPos[2] - lastPos[2];
		printf("dx is : %f dy is : %f dz is : %f\n", dx, dy, dz);
		if (dx || dy || dz) { // compute theta and cross product
			angle = 110.0 * sqrt(dx * dx + dy * dy + dz * dz);
			axis[0] = lastPos[1] * curPos[2] - lastPos[2] * curPos[1];
			axis[1] = lastPos[2] * curPos[0] - lastPos[0] * curPos[2];
			axis[2] = lastPos[0] * curPos[1] - lastPos[1] * curPos[0];
			lastPos[0] = curPos[0];
			lastPos[1] = curPos[1];
			lastPos[2] = curPos[2];
		}
		glutPostRedisplay();
	}
}


void Start(int x, int y)
{
	trackingMouse = true;
	startX = x;
	startY = y;
	curx = x;
	cury = y;
	trackball_ptov(x, y, screenSize, screenSize, lastPos);
	trackingMouse = true;
}


void mouse(int button, int state, int x, int y) {
	//GLfloat* m_transform;
	if (button == GLUT_LEFT_BUTTON) {
		switch (state) {
		case GLUT_DOWN:
			y = screenSize - y;
			Start(x, y);
			break;
		case GLUT_UP:
			angle = 0;
			trackingMouse = false;
			break;
		}
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenSize, screenSize);
	glutInitWindowPosition(100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	glutMainLoop();
	return 0;
}