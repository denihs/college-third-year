#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

GLdouble wxN = -50, wyN = 10, wzN = 0;  /*  returned world x, y, z coords  */
GLdouble wxF = 50, wyF = 0, wzF = 0;  /*  returned world x, y, z coords  */

#define TAM 20
int xs[TAM], ys[TAM], zs[TAM], rs[TAM];

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

    return dist <= r ? 1 : 0;
}

void display(void)
{
    glPointSize(5);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

    int x, y, z, r;
    for (int i = 0; i < TAM; i++) {

        x = xs[i];
        y = ys[i];
        z = zs[i];
        r = rs[i];

        glPushMatrix();
            if (isTouched(x, y, z, r)) glColor3f(1, 1, 0);   // Yellow
            else glColor3f(1, 1, 1); // White

            glTranslatef(x, y, z);
            glutWireSphere(r, 30, 30);

        glPopMatrix();
    }

    glFlush();
    glutSwapBuffers( );
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
void init( int width, int height )
{
    glClearColor( 0.3, 0.3, 0.3, 1 );
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 45, 1.33, 0.1, 400 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    gluLookAt( -0, 0, 100, 0, 0, 0, 0, 1, 0 );
}

float moreRandom() {
    return -1 + rand()%3;
}

int main( int argc, char **argv )
{
    srand(time(NULL));


    for(int i = 0; i < TAM; i++) {
        xs[i] = (rand()%80) * moreRandom();
        ys[i] = (rand()%80) * moreRandom();
        zs[i] = (20 + rand()%80) * -1;
        rs[i] = 5 + rand()%10;
    }
    for(int i = 0; i < TAM; i++) {
            printf("X: %d, Y: %d, Z: %d, R: %d\n", xs[i], ys[i], zs[i], rs[i]);
    }

    glutInit( &argc, argv );
    //The most important part specify the things your
    //glut window should provide
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 1280, 720 );
    glutCreateWindow("Teste usando gluUnProject");

    //enable z buffer
    glEnable( GL_DEPTH_TEST );
    //set the value in z-buffer as 1.0
    glClearDepth( 1.0 );
    init( 1280, 720 );
    glutDisplayFunc( display );
    glutReshapeFunc( init );
    //glutIdleFunc( display );
    glutMouseFunc( mouse );
    glutMainLoop( );
}
