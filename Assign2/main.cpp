/*
 * Skeleton code for CSE471 Fall 2019
 *
 * Won-Ki Jeong, wkjeong@unist.ac.kr
 */

#include <GL/glew.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <assert.h>

#include <iostream>

#include "textfile.h"

// Shader programs
GLuint p[3];

// light position
float lpos[4] = {1, 0.5, 1, 0};
float checkPhong = 0;
float param_diffuse = 0.5;
float param_ambient = 0.5;
float param_specular = 0.5;
float param_shiny = 30;  // alpha in phong eq
float param_scale = 1.2;
int param_level = 3;
float L_ambient[4] = {0, 1, 0, 0.1};
float L_diffuse[4] = {1, 0, 0, 1};
float L_specular[4] = {1, 1, 1, 1};
float M_ambient[4] = {1, 0, 0, 0.1};
float M_diffuse[4] = {1, 0, 0, 1};
float M_specular[4] = {1, 1, 1, 1};
float M_black[4] = {0, 0, 0, 1};
float vertex[6] = {-0.8f, -0.5f, 0.0f, 0.9f, 0.5f, -0.7f};

void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;

    float ratio = 1.0f * (float)w / (float)h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

void init_light()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, L_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, L_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, L_specular);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.1);
    glMaterialfv(GL_FRONT, GL_AMBIENT, M_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, M_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, M_specular);
    // glMaterialfv(GL_BACK, GL_DIFFUSE, M_black);
}

void param_init()
{
    GLuint A_Strength = glGetUniformLocation(p[0], "A_S");
    GLuint D_Strength = glGetUniformLocation(p[0], "D_S");
    GLuint S_Strength = glGetUniformLocation(p[0], "S_S");
    GLuint Shiny_Strength = glGetUniformLocation(p[0], "Shiny_S");
    GLuint Scale = glGetUniformLocation(p[1], "m_Scale");
    GLuint Level = glGetUniformLocation(p[2], "toon_Level");
    glUniform1f(A_Strength, param_ambient);
    glUniform1f(D_Strength, param_diffuse);
    glUniform1f(S_Strength, param_specular);
    glUniform1f(Shiny_Strength, param_shiny);
    glUniform1f(Scale, param_scale);
    glUniform1i(Level, param_level);
}

void keyboard(unsigned char key, int x, int y)
{
    if (checkPhong == 0) {  // phong shading
        if (key == 'p' || key == 'P') {
            printf("ToonShading\n");
            checkPhong = 1;
            p[2] = createGLSLProgram("../toon.vert", NULL, "../toon.frag");

            init_light();
            param_init();
        }
        else if (key == '1' || key == '3') {  // diffuse
            if (key == '1') {
                if (param_diffuse > 0.1) {
                    param_diffuse = param_diffuse - 0.1;
                }
                param_init();
            }
            else {
                if (param_diffuse < 1) {
                    param_diffuse = param_diffuse + 0.1;
                }
                param_init();
            }
        }
        else if (key == '4' || key == '6') {  // ambient
            if (key == '4') {
                if (param_ambient > 0.1) {
                    param_ambient = param_ambient - 0.1;
                }
                param_init();
            }
            else {
                if (param_ambient < 1) {
                    param_ambient = param_ambient + 0.1;
                }
                param_init();
            }
        }
        else if (key == '7' || key == '9') {  // specular
            if (key == '7') {
                if (param_specular > 0.1) {
                    param_specular = param_specular - 0.1;
                }
                param_init();
            }
            else {
                if (param_specular < 1) {
                    param_specular = param_specular + 0.1;
                }
                param_init();
            }
        }
        // shininess decrease parameter then more brighter and increase
        // parameter then more darker, bright dark is appending on area of
        // specular.
        // '+' in 10keypad worked but in left side of backspace key '+' and '='
        // is doesn't work so added '='key to prevent confuse to grading
        else if (key == '-' || key == '+' || key == '=') {
            if (key == '-') {
                if (param_shiny > 1) {
                    param_shiny = param_shiny - 1;
                }
                param_init();
            }
            else {
                if (param_shiny < 100) {
                    param_shiny = param_shiny + 1;
                }
                param_init();
            }
        }
        else if (key == 'q' || key == 'Q') {
            printf("Quit Window\n");
            exit(0);
        }
        else if (key == 's' || key == 'S') {  // solid
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else if (key == 'w' || key == 'W') {  // wire
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }
    else {
        if (key == 'p' || key == 'P') {
            // ToDo
            printf("PhongShading\n");
            checkPhong = 0;
            p[0] = createGLSLProgram("../phong.vert", NULL,
                                     "../phong.frag");  // Phong
            init_light();
            param_init();
        }
        else if (key == '2') {  // level2
            param_level = 2;
            param_init();
        }
        else if (key == '3') {  // level2
            param_level = 3;
            param_init();
        }
        else if (key == '4') {  // level2
            param_level = 4;
            param_init();
        }
        else if (key == '5') {  // level2
            param_level = 5;
            param_init();
        }
        else if (key == '6') {  // level2
            param_level = 6;
            param_init();
        }
        else if (key == '7') {  // level2
            param_level = 7;
            param_init();
        }
        else if (key == '8') {  // level2
            param_level = 8;
            param_init();
        }
        else if (key == '9') {  // level2
            param_level = 9;
            param_init();
        }
        else if (key == '-' || key == '+' || key == '=') {
            if (key == '-') {
                if (param_shiny > 1) {
                    param_shiny = param_shiny - 1;
                }
                param_init();
            }
            else {
                if (param_shiny < 100) {
                    param_shiny = param_shiny + 1;
                }
                param_init();
            }
        }
        else if (key == 'q' || key == 'Q') {
            printf("Quit Window\n");
            exit(0);
        }
        else if (key == 's' || key == 'S') {  // solid
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else if (key == 'w' || key == 'W') {  // wire
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }
    glutPostRedisplay();
}

void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(5.0, 5.0, 5.0, -1.0, -1.0, -1.0, 0.0f, 1.0f, 0.0f);

    // ToDo

    // for silhouette
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glUseProgram(p[1]);
    GLuint Scale = glGetUniformLocation(p[1], "m_Scale");
    glUniform1f(Scale, param_scale);
    glutSolidTeapot(1);
    glCullFace(GL_FRONT);
    glUseProgram(p[2]);
    glutSolidTeapot(1);
    glDisable(GL_CULL_FACE);

    // for non silhouette
    // glClearColor(1.0, 1.0, 1.0, 1.0);
    // glutSolidTeapot(1);
    // glFrontFace(GL_CW);

    glutSwapBuffers();
}

void idle() { glutPostRedisplay(); }

int main(int argc, char **argv)
{
    // init GLUT and create Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("CSE471 - Assignment 2");

    // register callbacks

    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glewInit();
    if (glewIsSupported("GL_VERSION_3_3"))
        printf("Ready for OpenGL 3.3\n");
    else {
        printf("OpenGL 3.3 is not supported\n");
        exit(1);
    }

    // Create shader program
    p[0] = createGLSLProgram("../phong.vert", NULL, "../phong.frag");  // Phong
    // p[1] = createGLSLProgram( "../silhouette.vert", NULL,
    // "../silhouette.frag" ); // Silhouette p[2] = createGLSLProgram(
    // "../toon.vert", NULL, "../toon.frag" ); // Cartoon
    glUseProgram(p[0]);

    init_light();
    param_init();

    // enter GLUT event processing cycle
    glutMainLoop();

    return 1;
}