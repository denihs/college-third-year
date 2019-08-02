/////////////////////////////////          
// box.cpp
//
// This program draws a wire box.
//
// Sumanta Guha.
/////////////////////////////////

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Drawing routine.
void drawScene(void) {
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);
   glLoadIdentity(); 

   // Segundo pé é esquerdo da mesa
    glPushMatrix();
        glTranslatef(-8.5, -1.0, -35.0);
        glScalef(0.3, 3.0, 0.3); // Along x, y e profundidade no z
        glutWireCube(5.0); // Box torso.
    glPopMatrix();

    // Segundo pé direito da mesa
    glPushMatrix();
        glTranslatef(8.5, -1.0, -35.0);
        glScalef(0.3, 3.0, 0.3);
        glutWireCube(5.0);
    glPopMatrix();

    // Primeiro pé direito da mesa
    glPushMatrix();
        glTranslatef(10.0, 0.0, -20.0);
        glScalef(0.3, 3.0, 0.3);
        glutWireCube(5.0);
    glPopMatrix();

    // Primeiro pé esquedo da mesa
    glPushMatrix();
        glTranslatef(-10.0, 0.0, -20.0);
        glScalef(0.3, 3.0, 0.3);
        glutWireCube(5.0);
    glPopMatrix();

    // Parte de cima da mesa
    glPushMatrix();
        glTranslatef(0.0, 9.0, -35.0);
        glScalef(5.0, 0.05, 5.0);
        glutWireCube(5.0);
    glPopMatrix();

   glFlush();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0);  
}

// OpenGL window reshape routine.
void resize(int w, int h) {
   glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
   switch(key) 
   {
      case 27:
         exit(0);
         break;

      default:
         break;
   }
}

// Main routine.
int main(int argc, char **argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("box.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMainLoop(); 

   return 0;  
}

