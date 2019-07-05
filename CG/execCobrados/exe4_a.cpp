#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

//globais
static int isAnimate = 0; // Animated?
static int animationPeriod = 25; // Time interval between frames.
static float Xangle = -16.0, Yangle = 0.0;
static float Xrot = 0.0, Yrot = 0.0;

void drawScene(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(1.0, 0.0, 0.0);

    glLoadIdentity();

    glTranslatef(0.0,0.0,-35.0);

    glRotatef(45.0,1.0,0.0,0.0);
    glRotatef(-115.0,0.0,1.0,0.0);
    glRotatef(17.0,1.0,1.0,0.0);

    glColor3f(0.0, 1.0, 1.0); 
    glBegin(GL_POLYGON); 
        glVertex3f(-20.0, 0.0, -6.0); 
        glVertex3f(0.0, 0.0, -6.0); 
        glVertex3f(0.0, 0.0, 6.0); 
        glVertex3f(-20.0, 0.0, 6.0); 
    glEnd();

    glPushMatrix();
        glColor3f(1.0, 1.0, 0.0); 
        glRotatef(25.0,0.0,0.0,1.0);
        glBegin(GL_POLYGON); 
            glVertex3f(0.0, 0.0, -6.0); 
            glVertex3f(25.0, 0.0, -6.0); 
            glVertex3f(25.0, 0.0, 6.0); 
            glVertex3f(0.0, 0.0, 6.0); 
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(Xangle,0.0,0.0);
        glTranslatef(0.0,Yangle,0.0);
        // glRotatef(Yrot, 0.0, 1.0, 0.0);
        glRotatef(Xrot, 0.0, 0.0, 1.0);
        //Desenha a bola
        // glutSolidSphere(2.0, 10, 10);
        glutWireSphere(2.0, 10, 10);
    glPopMatrix();

    glutSwapBuffers();//deixa a tela branca
}

// OpenGL window reshape routine.
void resize (int w, int h)
{
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
//    gluPerspective(60.0, (float)w/(float)h, 1.0, 50.0);
    glOrtho(-20.0, 20.0, -20.0, 20.0, 5.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
}

//função de animação
void animate(int value)
{
   if(isAnimate){
        if(Xangle < 16.0){
            if(Xangle >= 0) Yangle +=0.25;
            Xangle += 0.5;
            Xrot -= 5.0;
            Yrot += 1.0;
        }
        else isAnimate = 0;
   }
   glutTimerFunc(animationPeriod, animate, 1);
   glutPostRedisplay();
}

void keyInput(unsigned char key,int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
        break;
        case ' ': 
            if(Xangle >=16.0){
                Xangle = -16.0;
                Yangle = 0;
                Xrot = 0.0;
                Yrot = 0.0;
            }
            if (isAnimate) isAnimate = 0;
            else isAnimate = 1;
            glutPostRedisplay();
		break;
    }
}

void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0); 
}

int main(int argc, char **argv) 
{
//    printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("exe4_a.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutTimerFunc(5, animate, 1);
   glutMainLoop();
   
   return 0;
}