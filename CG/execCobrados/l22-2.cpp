//
// Created by denyhs on 03/07/19.
//

#include <GL/glut.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

static int year = 0, isAnimate = 0;

void increaseAngle(){
    year = (year + 1) % 360;
    glutPostRedisplay();
};

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    // DESENHA UM SOL E TRAZ ELE PARA A CENA
    glTranslatef(0.0, 0.0, -20.0);
    glColor3f(1.0,0.8,0.0);
    glutWireSphere(1.0, 20, 16); /* draw sun */

    // DESENHA O MENOR PLANETA FAZENDO ELE GIRAR MAIS RAPIDO
    glPushMatrix();
    glRotatef((GLfloat)year*3, 0.5, 1.0, 1.0);
    glTranslatef(2.0, 0.0, 0.0);
    glColor3f(0.0,0.4,1.0);
    glutWireSphere(0.2, 10, 8); /* draw smaller planet */
    glPopMatrix();

    // PLANETA DE TAMANHO MÉDIO
    glPushMatrix();
    glRotatef((GLfloat)year*2, 0.5, 1.0, 1.0);
    glTranslatef(6.0, 0.0, 0.0);
    glColor3f(0.0,0.4,1.0);
    glutWireSphere(0.5, 10, 8);  // draw average size planet
    glPopMatrix();


    // MAIOR PLANETA GIRANDO MAIS LENTAMENTE
    glRotatef(year, 0.5, 1.0, 1.0);
    glTranslatef(9.0, 0.0, 0.0);
    glColor3f(0.0,0.4,1.0);
    glutWireSphere(0.9, 10, 8);  // draw average biggest planet

    // LUA EM VOLTA DO MAIOR PLANETA
    glPushMatrix();
    glRotatef(year * (5), 0.0, 1.0, 0.0);
    glTranslatef(1.5, 0.0, 0.0);
    glColor3f(0.0,0.4,1.0);
    glutWireSphere(0.2, 10, 8); /* draw smaller planet */
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
    gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'y':
            year = (year + 5) % 360;
            glutPostRedisplay();
            break;
        case 'Y':
            year = (year - 5) % 360;
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