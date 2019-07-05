//
// Created by denyhs on 03/07/19.
//

#include <GL/glut.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

static int angle = 0, isAnimate = 0;

static double x = -0.0, z = -0.0;

void increaseAngle(){
    angle = (angle + 1) % 360;
    if (x > 1) x = -0.0;
    if (z > 20) z = -0.0;
    x += 0.1;
    z += 0.01;
    glutPostRedisplay();
};

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);


    // BOLA GIRANDO EM ESPIRAL
    glPushMatrix();
    glRotatef((GLfloat)angle*5, 0.0, 0.0, 1.0);
    glTranslatef(0.5, 0.0, -z);
    glColor3f(0.0,0.4,1.0);
    glutSolidSphere(0.1, 10, 8);  // draw average size planet
    glPopMatrix();


    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAH
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 35.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'y':
            angle = (angle + 5) % 360;
            glutPostRedisplay();
            break;
        case 'Y':
            angle = (angle - 5) % 360;
            glutPostRedisplay();
            break;
        case ' ':
            glutIdleFunc(isAnimate ? increaseAngle : NULL);
            isAnimate = !isAnimate;
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}