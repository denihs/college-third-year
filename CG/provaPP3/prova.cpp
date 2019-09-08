//
// Created by denyhs on 05/09/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>

static long font = (long)GLUT_BITMAP_9_BY_15; // Font selection.

using namespace std;

#define TRANSLATE 1
#define SCALE 2
#define ROTATION 3

int mode = 1;

int scaleX = 0;
int scaleY = 0;
int scaleZ = 0;

int rotateX = 0;
int rotateY = 0;
int rotateZ = 0;

int firsClick = 0;
double objx = 0, objy=0, objz=0;
double angle_x = 0, angle_y = 0;
double move_x = 1;
double move_y = 0;
double move_z = 0;

int w, h;

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string, int x = 1000) {
    char *c;
    int i[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    int aux = x;

    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);

    if (x < 1000) {
        if (x < 0) {
            glutBitmapCharacter(font, 45);
            aux *= -1;
        }

        if (aux >= 0 && aux < 10) {
            glutBitmapCharacter(font, aux + 48);
            return;
        }

        int d = aux < 100 ? 10 : 100;

        for (int j = 0; j < 10, d >= 1; j++) {
            i[j] = aux / d;
            int rest = aux % d;
            aux = rest;
            d /= 10;
        }

        for (int h = 0; i[h] != 100; h++) glutBitmapCharacter(font, i[h] + 48);

    }
}

char * getMode() {
    if (mode == SCALE) return "Scale";
    if (mode == ROTATION) return "Rotate";
    return "Translate";
}

char * getAxis() {
    if (move_x) return "X";
    if (move_y) return "Y";
    return "Z";
}

char * getMovementStatus() {
    if (firsClick) {
        return "ON";
    }
    return "OFF";
}

void infos() {
        glColor3f(1.0, 1.0, 0.0);
        glRasterPos3f(-150, -80, 0.0);
        writeBitmapString((void*)font, "mode: ");
        glRasterPos3f(-143, -81, 0.0);
        writeBitmapString((void*)font, getMode());


        glRasterPos3f(-151, -85, 0.0);
        writeBitmapString((void*)font, "axis: ");
        glRasterPos3f(-143, -86, 0.0);
        writeBitmapString((void*)font, getAxis());

        glRasterPos3f(-152, -90, 0.0);
        writeBitmapString((void*)font, "movement: ");
        glRasterPos3f(-140, -92, 0.0);
        writeBitmapString((void*)font, getMovementStatus());
}

/*THE FUNCTION TO DRAW THE STUFF ON THE SCREEN*/
void display()
{
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

    infos();

    //glRotatef(angle_y, 1.0, 0.0, 0.0);
    glPushMatrix();
        glRotatef(angle_x, 0.0, 1.0, 0.0);
        glRotatef(angle_y, 1.0, 0.0, 0.0);
        // Desenhando o grid
        glBegin( GL_LINES );
            glColor3f(0, 0, 1);
            glVertex3f( 0, 0,  100  );
            glVertex3f( 0, 0, -100  );
        glEnd( );
        glBegin( GL_LINES );
            glColor3f(1, 0, 0);
            glVertex3f( -80, 0,  0  );
            glVertex3f( 80, 0,  0  );
        glEnd( );
        glColor3f(0.8, 0.8, 0.8);
        for (int i = 0; i < 170 ; i+=10){
            glBegin( GL_LINES );
                glVertex3f( -80+i, 0,  100  );
                glVertex3f( -80+i, 0, -100  );
            glEnd( );
        }
        for (int i = 0; i < 220 ; i+= 20){
            glBegin( GL_LINES );
                glVertex3f( -80, 0,  100-i  );
                glVertex3f(  80, 0,  100-i  );
            glEnd( );
        }

        // Desenhando cubo e linhas
        glColor3f(0.8, 0.8, 0.8);
        glPushMatrix( );
            glTranslatef( objx, objy, objz);
            glRotatef(rotateX, 1.0, 0.0, 0.0);
            glRotatef(rotateY, 0.0, 1.0, 0.0);
            glRotatef(rotateZ, 0.0, 0.0, 1.0);
            glTranslatef(0, 0, 0);

            glBegin( GL_LINES );
                if (move_z) glColor3f(1, 1, 0);
                else glColor3f(1, 1, 1);
                glVertex3f( 0, 0,  0  );
                glVertex3f( 0, 0, 40  );
            glEnd( );

            glBegin( GL_LINES );
                if (move_x) glColor3f(1, 1, 0);
                else glColor3f(1, 1, 1);
                glVertex3f( 0, 0,  0  );
                glVertex3f( 40, 0,  0  );
            glEnd( );

            glBegin( GL_LINES );
                if (move_y) glColor3f(1, 1, 0);
                else glColor3f(1, 1, 1);
                glVertex3f( 0, 0,  0  );
                glVertex3f( 0, 40,  0  );
            glEnd( );
            float scale = 5.0;
            //Lado FRENTE
            glBegin(GL_POLYGON);
                glColor3f(1.0, 0.0, 0.0); glVertex3f(  scale + scaleX, -scale - scaleY, -scale - scaleZ );
                glColor3f(0.0, 1.0, 0.0); glVertex3f(  scale + scaleX,  scale + scaleY, -scale - scaleZ );
                glColor3f(0.0, 0.0, 1.0); glVertex3f( -scale - scaleX,  scale + scaleY, -scale - scaleZ );
                glColor3f(1.0, 1.0, 0.0); glVertex3f( -scale - scaleX, -scale - scaleY, -scale - scaleZ );
            glEnd();

            // Lado TRASEIRA
            glBegin(GL_POLYGON);
                glColor3f(1.0, 0.0, 0.0);glVertex3f(  scale + scaleX, -scale - scaleY, scale + scaleZ );
                glColor3f(0.0, 1.0, 0.0);glVertex3f(  scale + scaleX,  scale + scaleY, scale + scaleZ );
                glColor3f(0.0, 0.0, 1.0);glVertex3f( -scale - scaleX,  scale + scaleY, scale + scaleZ );
                glColor3f(1.0, 1.0, 0.0);glVertex3f( -scale - scaleX, -scale - scaleY, scale + scaleZ );
            glEnd();

            // Lado DIREITA
            glBegin(GL_POLYGON);
                glColor3f(1.0, 0.0, 0.0);glVertex3f( scale + scaleX, -scale - scaleY, -scale - scaleZ );
                glColor3f(0.0, 1.0, 0.0);glVertex3f( scale + scaleX,  scale + scaleY, -scale - scaleZ );
                glColor3f(0.0, 0.0, 1.0);glVertex3f( scale + scaleX,  scale + scaleY,  scale + scaleZ );
                glColor3f(1.0, 1.0, 0.0);glVertex3f( scale + scaleX, -scale - scaleY,  scale + scaleZ );
            glEnd();

            // Lado ESQUERDA
            glBegin(GL_POLYGON);
                glColor3f(1.0, 0.0, 0.0);glVertex3f( -scale - scaleX, -scale - scaleY, scale + scaleZ );
                glColor3f(0.0, 1.0, 0.0);glVertex3f( -scale - scaleX,  scale + scaleY,  scale + scaleZ );
                glColor3f(0.0, 0.0, 1.0);glVertex3f( -scale - scaleX,  scale + scaleY, -scale - scaleZ );
                glColor3f(1.0, 1.0, 0.0);glVertex3f( -scale - scaleX, -scale - scaleY, -scale - scaleZ );
            glEnd();

            // Lado TOPO
            glBegin(GL_POLYGON);
                glColor3f(1.0, 0.0, 0.0);glVertex3f(  scale + scaleX,  scale + scaleY,  scale + scaleZ );
                glColor3f(0.0, 1.0, 0.0);glVertex3f(  scale + scaleX,  scale + scaleY, -scale - scaleZ );
                glColor3f(0.0, 0.0, 1.0);glVertex3f( -scale - scaleX,  scale + scaleY, -scale - scaleZ );
                glColor3f(1.0, 1.0, 0.0);glVertex3f( -scale - scaleX,  scale + scaleY,  scale + scaleZ );
            glEnd();

            // Lado BASE
            glBegin(GL_POLYGON);
                glColor3f(1.0, 0.0, 0.0);glVertex3f(  scale + scaleX, -scale - scaleY, -scale - scaleZ );
                glColor3f(0.0, 1.0, 0.0);glVertex3f(  scale + scaleX, -scale - scaleY,  scale + scaleZ );
                glColor3f(0.0, 0.0, 1.0);glVertex3f( -scale - scaleX, -scale - scaleY, scale + scaleZ );
                glColor3f(1.0, 1.0, 0.0);glVertex3f( -scale - scaleX, -scale - scaleY, -scale - scaleZ );
            glEnd();

    glPopMatrix( );
    glPopMatrix();

    glPushMatrix();
    glTranslatef( 0, 0, 0);

    glPopMatrix();

    glFlush( );
    glutSwapBuffers( );
}

double scaleAux = 0.0;
double rotateAux = 0.0;


void resetVariables() {
    scaleAux = 0.0;
    rotateAux = 0.0;
}


void mouse( int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {       
            firsClick = 1 - firsClick;
            if (!firsClick) {
                resetVariables();
            }
            glutPostRedisplay();
        }
    }
}

// Mouse motion callback routine.
void mousePassiveMotion(int x, int y){ 
    double modelview[16], projection[16], auxX, auxY, auxZ;
    int viewport[4];
    float z = 1 - 0.0001;

   if (firsClick && (move_x || move_y || move_z)) {
        /*Read the projection, modelview and viewport matrices
        using the glGet functions.*/
        glGetDoublev( GL_PROJECTION_MATRIX, projection );
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
        glGetIntegerv( GL_VIEWPORT, viewport );

        //Read the window z value from the z-buffer
        //glReadPixels( x, viewport[3]-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z );
        //if (z == 1) z -= 0.00001;
        //Use the gluUnProject to get the world co-ordinates of
        //the point the user clicked and save in objx, objy, objz.
        gluUnProject( x, viewport[3]-y, z, modelview, projection, viewport, &auxX, &auxY, &auxZ );

        if (mode == TRANSLATE) {
            if (move_x) objx = auxX;
            if (move_y) objy = auxY;
            if (move_z) objz = -auxY;
        }

        if (mode == SCALE) {

            if(move_x) {
                auxX > scaleAux ? scaleX++ : scaleX--;
                scaleAux = auxX;
            }
            if(move_y) {
                auxY > scaleAux ? scaleY++ : scaleY--;
                scaleAux = auxY;
            }
            if(move_z) {
                auxZ > scaleAux ? scaleZ++ : scaleZ--;
                scaleAux = auxZ;
            }
        }

        if (mode == ROTATION) {
            if(move_x) {
                auxX > rotateAux ? rotateX++ : rotateX--;
                rotateAux = auxX;
            }
            if(move_y) {
                auxY > rotateAux ? rotateY++ : rotateY--;
                rotateAux = auxY;
            }
            if(move_z) {
                auxZ > rotateAux ? rotateZ++ : rotateZ--;
                rotateAux = auxZ;
            }
        }



        glutPostRedisplay();
   }
}


void keyboard( int key, int x, int y )
{


    if (key == GLUT_KEY_RIGHT){
        angle_x += 2.0;
        glutPostRedisplay();
        //cout << "aqui" << endl;
    }

    else if(key == GLUT_KEY_LEFT){
        angle_x -= 2.0;
        glutPostRedisplay();
        //cout << "aqui-1" << endl;
    }

    else if(key == GLUT_KEY_UP){
        angle_y += 2.0;
        glutPostRedisplay();
        //cout << "aqui-3" << endl;
    }

    else if (key ==  GLUT_KEY_DOWN) {
        angle_y -= 2.0;
        glutPostRedisplay();
        //cout << "aqui-4" << endl;
    }

}

void setMove(int x, int y, int z) {
    move_x = x;
    move_y = y;
    move_z = z;
}

void keyInput(unsigned char key, int x, int y)
{
    switch (key) {
        case 'x':
        case 'X':
            setMove(1, 0, 0);
            resetVariables();
            break;
        case 'y':
        case 'Y':
            setMove(0, 1, 0);
            resetVariables();
            break;
        case 'z':
        case 'Z':
            setMove(0, 0, 1);
            resetVariables();
            break;
        case 't':
        case 'T':
            mode = 1;
            resetVariables();
            break;
        case 's':
        case 'S':
            mode = 2;
            resetVariables();
            break;
        case 'r':
        case 'R':
            mode = 3;
            resetVariables();
            break;
        case 27:
            exit(0);
            break;

    }
    glutPostRedisplay();
}

void init( int width, int height )
{
    w = width;
    h = height;

    glClearColor( 0.3, 0.3, 0.3, 1 );
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 45, 1.33, 0.1, 400 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    gluLookAt( -50, 100, 250, 0, 0, 0, 0, 1, 0 );
}

int main( int argc, char **argv )
{
    srand(time(NULL));
    glutInit( &argc, argv );
    //The most important part specify the things your
    //glut window should provide
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 1920, 1200 );
    glutCreateWindow("Transformações lineares");

    //enable z buffer
    glEnable( GL_DEPTH_TEST );
    //set the value in z-buffer as 1.0
    glClearDepth( 1.0 );
    init( 1920, 1200 );
    glutDisplayFunc( display );
    glutReshapeFunc( init );
    //glutIdleFunc( display );
    glutMouseFunc( mouse );
    glutKeyboardFunc(keyInput);
    glutSpecialFunc( keyboard );
    glutPassiveMotionFunc(mousePassiveMotion);
    glutMainLoop( );
}