///////////////////////////////////////////////////////
// helix.cpp
//
// This program approximates a helix with a line strip.
//
// g++ -o helix  helix.cpp -lglut -lGL -lGLU -lm
// Sumanta Guha.
///////////////////////////////////////////////////////

#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265
#define N 40.0 // Number of vertices on the boundary of the disc.
using namespace std;


void drawDisc(float R, float X, float Y, float Z) {
   float t;
   int i;

   glBegin(GL_TRIANGLE_FAN);
      glVertex3f( X, Y, Z);
      for(i = 0; i <= N; ++i) {
         t = 2 * PI * i / N;
         glVertex3f(X + cos(t) * R, Y + sin(t) * R, Z);
      }
   glEnd();
}

// Drawing routine.
void drawScene(void) {
   float R = 20.0; // Radius of helix.

   float t; // Angle parameter.

   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);

   glBegin(GL_LINE_STRIP);
      for(t = -10 * PI; t <= 10 * PI; t += PI/20.0)
         // glVertex3f(R * cos(t), R * sin(t), t - 60.0);
         glVertex3f(R * cos(t), t, R * sin(t) - 60.0);
	     // glVertex3f(R * cos(t), t, R * sin(t) - 60.0);
   glEnd();

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

   // glOrtho(-50.0, 50.0, -50.0, 50.0, 0.0, 100.0);
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
   switch(key) {
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
   glutInitWindowPosition(0, 0);
   glutCreateWindow("helix.cpp");

   setup();

   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutMainLoop();

   return 0;
}
