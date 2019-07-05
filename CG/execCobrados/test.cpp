// Comando para compilação: g++ -o 4e  4e.cpp -lglut -lGL -lGLU -lm

////////////////////////////////////////////////////////////////
// ballAndTorus.cpp
//
// This program draws a ball that flies around a torus.
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the up/down arrow keys to speed up/slow down animation.
// Press the x, X, y, Y, z, Z keys to rotate the scene.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////

#include <iostream>
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Globals.
static float latAngle = 0.0; // Latitudinal angle.
static float longAngle = 0.0; // Longitudinal angle.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int isAnimate = 0; // Animated?
static int animationPeriod = 20; // Time interval between frames.

// Drawing routine.
void drawScene(void) {

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -25.0);


    /* Desenhando o sol com cor amarela */
    glColor3f(1.0, 1.0, 0.0);

    glTranslatef(0.0, 0.0, -14.0);
    glutWireSphere(10.0, 50.0, 50.0);


    glColor3f(0.5f, 0.35f, 0.05f); // Seta Planeta cinza
    glRotatef(-latAngle, 0, 1, 0); // Rotaciona em volta do eixo y no sentido anti horario por causa do -latAngle
    /* Caso queira deixa o planeta fixo, mover o glTranslatef para cima do rotate, assim o rotare vai rotacionar no
     no proprio eixo do objeto */

    glTranslatef(20, 0, 0);
    /* Translada o planeta em 20 do eixo do x, ou seja, move a esfera originaria no centro do Sol para o lado */

    glutWireSphere(2.0, 10, 10); //// Desenha platena cinza

    /* Acelera em 25 vezes a velocidade de rotacao da lua menor em orbita do planeta */

    glPushMatrix();
    glColor3f(0, 1, 100); /* Lua com cor azul, anã azul*/
    glRotatef(latAngle * (-5), 0, 1, 0); // Lua vai rotacionar em volta do planeta no sentido horario
    glTranslatef(3, 0, 0); //// Transalada a esfera em 2 para deixa o centro do planeta
    glutWireSphere(0.5, 5, 5);
    glPopMatrix();


    glColor3f(0, 0, 0); /* Lua com cor azul, anã azul*/
    glRotatef(latAngle, 1, 0, 0); // Lua vai rotacionar em volta do planeta no sentido horario
    glTranslatef(0, 3, 0); //// Transalada a esfera em 2 para deixa o centro do planeta
    glutWireSphere(0.5, 10, 10);

    glutSwapBuffers();

}

void animate(int value) { // Timer function.

    if (isAnimate)
        latAngle += 1;

    glutTimerFunc(animationPeriod, animate, 1);
    glutPostRedisplay();
}

void setup(void) { // Initialization routine.
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
}

void resize(int w, int h) { // OpenGL window reshape routine.
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void keyInput(unsigned char key, int x, int y) { // Keyboard input processing routine.
    switch(key) {
        case 27:
            exit(0);
            break;

        case ' ':
            isAnimate ? isAnimate = 0 : isAnimate = 1;
            glutPostRedisplay();
            break;

        case 'x':
            Xangle += 5.0;
            if(Xangle > 360.0)
                Xangle -= 360.0;

            glutPostRedisplay();
            break;

        case 'X':
            Xangle -= 5.0;

            if(Xangle < 0.0)
                Xangle += 360.0;

            glutPostRedisplay();
            break;

        case 'y':
            Yangle += 5.0;

            if(Yangle > 360.0)
                Yangle -= 360.0;

            glutPostRedisplay();
            break;

        case 'Y':
            Yangle -= 5.0;

            if(Yangle < 0.0)
                Yangle += 360.0;

            glutPostRedisplay();
            break;

        case 'z':
            Zangle += 5.0;

            if(Zangle > 360.0)
                Zangle -= 360.0;

            glutPostRedisplay();
            break;

        case 'Z':
            Zangle -= 5.0;

            if(Zangle < 0.0)
                Zangle += 360.0;

            glutPostRedisplay();
            break;

        default:
            break;
    }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y) {
    if(key == GLUT_KEY_DOWN)
        animationPeriod += 5;

    if (key == GLUT_KEY_UP) if (animationPeriod > 5) animationPeriod -= 5;

    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void) {
    cout << "Interaction:" << endl;
    cout << "Press space to toggle between animation on and off." << endl
         << "Press the up/down arrow keys to speed up/slow down animation." << endl
         << "Press the x, X, y, Y, z, Z keys to rotate the scene." << endl;
}

// Main routine.
int main(int argc, char **argv) {

    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ballAndTorus.cpp");

    setup();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);
    glutTimerFunc(5, animate, 1);
    glutMainLoop();

    return 0;
}

