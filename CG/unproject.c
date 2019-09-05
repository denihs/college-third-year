#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

GLdouble wxN = -50, wyN = 10, wzN = 0;  /*  returned world x, y, z coords  */
GLdouble wxF = 50, wyF = 0, wzF = 0;  /*  returned world x, y, z coords  */

int isTouched(float x, float y, float z, float r) {
   // Vetor diretor de R (dr)
   float xR = (wxF - wxN);
   float yR = (wyF - wyN);
   float zR = (wzF - wzN);

   // Ponto qualquer de R
   float xQ = wxN * 1;
   float yQ = wyN * 1;
   float zQ = wzN * 1;

   // vetor QP
    float xQP = x - xQ;
    float yQP = y - yQ;
    float zQP = z - zQ;

    // Produto vetorial entre QP e dr
    float i = (yQP * zR) - (zQP * yR);
    float j = (zQP * xR) - (xQP * zR);
    float k = (xQP * yR) - (yQP * xR);

    // d(P, r) = ||QP x dr|| / || dr ||

    float modQPdr = sqrt(pow(i, 2) + pow(j, 2) + pow(k, 2));
    float modDr = sqrt(pow(xR, 2) + pow(yR, 2) + pow(zR, 2));

    float dist = modQPdr / modDr;

    printf("Dist: %f\n", dist);
    return dist <= r ? 1 : 0;
}

void display(void)
{
    glPointSize(5);
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();

        if (isTouched(0, 0, -11, 10)) glColor3f(1, 1, 0);   // Yellow
        else glColor3f(1, 1, 1); // White

        glTranslatef(0, 0, -11);
        glutWireSphere(10, 30, 30);

    glPopMatrix();

    glPushMatrix();

        if (isTouched(0, 0, -11, 10)) glColor3f(1, 1, 0);   // Yellow
        else glColor3f(1, 1, 1); // White

        glTranslatef(11, 11, -11);
        glutWireSphere(5, 30, 30);
    glPopMatrix();

    glFlush();
}

/* Change these values for a different transformation  */
void reshape(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set viewing box dimensions equal to window dimensions.
    glOrtho(0.0, w, h, 0.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y)
{
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];
    GLint realy;  /*  OpenGL y coordinate position  */


    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                glGetIntegerv (GL_VIEWPORT, viewport);
                glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
                glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
/*  note viewport[3] is height of window in pixels  */
                realy = viewport[3] - (GLint) y - 1;
                printf ("Coordinates at cursor are (%4d, %4d)\n", x, realy);
                gluUnProject ((GLdouble) x, (GLdouble) realy, 0.0,
                              mvmatrix, projmatrix, viewport, &wxN, &wyN, &wzN);
                printf ("World coords at z=0.0 are (%f, %f, %f)\n", wxN, wyN, wzN);
                gluUnProject ((GLdouble) x, (GLdouble) realy, 1.0,
                              mvmatrix, projmatrix, viewport, &wxF, &wyF, &wzF);
                printf ("World coords at z=1.0 are (%f, %f, %f)\n", wxF, wyF, wzF);
                glutPostRedisplay();
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
                exit(0);
            break;
        default:
            break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
    }
}

/* 
 *  Open window, register input callback functions
 */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (800, 800);
    glutInitWindowPosition (100, 100);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc (keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}