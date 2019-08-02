///////////////////////////////////////////////////////////////////////////////////////////
// circularAnnuluses.cpp
//
// This program draws three identical-looking circular annuluses in three different ways -
// see comments below.
//
// Interaction:
// Press the space bar to toggle between wirefrime and filled for the lower annulus.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265
#define N 8.0 // Number of vertices on the boundary of the disc.

using namespace std;

// Globals.
static int isWire = 0; // Is wireframe?
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Function to draw a disc with center at (X, Y, Z), radius R, parallel to the
// xy-plane.
void drawDisc(float R, float X, float Y, float Z) {
   float t;
   int i;

   glBegin(GL_TRIANGLE_FAN);
      glVertex3f(0, 30, -10);

      for(i = 0; i <= N; ++i) {
         t = 2 * PI * i / N;
         glVertex3f(cos(t) * R, 0, sin(t) * R - 10);
      }
   glEnd();
}

// Drawing routine.
void drawScene(void) {
   float angle;
   int i;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers including
                                                       // the depth buffer.

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   // Upper left circular annulus: the white disc overwrites the red disc.
   glColor3f(1.0, 0.0, 0.0);
   drawDisc(10.0, 25.0, 75.0, 0.0);

   glFlush();
}

// Initialization routine.
void setup(void) {
   glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h) {
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-25.0, 25.0, -25.0, 25.0, 2.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key)
   {
      case ' ':
         if (isWire == 0) isWire = 1;
         else isWire = 0;
         glutPostRedisplay();
         break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the space bar to toggle between wirefrime and filled for the lower annulus." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // Initialize the buffers
                                                             // including the depth buffer.
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("circularAnnuluses.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutMainLoop();

   return 0;
}
