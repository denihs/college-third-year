////////////////////////////////////////////////////
// square.cpp
//
// Stripped down OpenGL program that draws a square.
//
// Sumanta Guha.
////////////////////////////////////////////////////
#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

#define PI 3.14159265358979324
// Globals.
static float R = 17.6; // Radius of circle.
static float X = 45.5; // X-coordinate of center of circle.
static float Y = 28.0; // Y-coordinate of center of circle.
static int numVertices = 200000; // Number of vertices on circle.

// Drawing (display) routine.
   // Clear screen to background color.
void drawAnotherScene(int n, int v) {
   glClear(GL_COLOR_BUFFER_BIT);

   // Set foreground (or drawing) color.
   glColor3f(0.0, 0.0, 0.0);

   // Draw a polygon with specified vertices.
   glBegin(GL_POLYGON);
      glVertex3f(80.0, 20.0, 0.0);
      glVertex3f(20.0 + v, 20.0, 0.0);
      glVertex3f(80.0, 80.0, 0.0);
      glVertex3f(20.0, 80.0, 0.0);
   glEnd();

   // Flush created objects to the screen, i.e., force rendering.
   glFlush();
}

void drawScene(void) {
  glClear(GL_COLOR_BUFFER_BIT);

  // Set foreground (or drawing) color.
  glColor3f(0.0, 0.0, 0.0);

  // Set polygon mode.
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(30.0, 50.0, 0.0);
    glVertex3f(10.0, 10.0, 0.0);
    glVertex3f(30.0, 30.0, 0.0);
    glVertex3f(80.0, 10.0, 0.0);
    glVertex3f(60.0, 30.0, 0.0);
    glVertex3f(80.0, 70.0, 0.0);
    glVertex3f(60.0, 50.0, 0.0);
    glVertex3f(10.0, 70.0, 0.0);
    glVertex3f(30.0, 50.0, 0.0);
    glVertex3f(10.0, 10.0, 0.0);
  glEnd();

  glFlush();
}

// Drawing (display) routine.
void reliquias(void)
{
   // Clear screen to background color.
   glClear(GL_COLOR_BUFFER_BIT);

   // Set foreground (or drawing) color.
   glColor3f(0.0, 0.0, 0.0);

   // Draw a polygon with specified vertices.
   glBegin(GL_POLYGON);
      glVertex3f(45.9, 60.0, 0.0);
      glVertex3f(80.0, 10.0, 0.0);
      glVertex3f(10.0, 10.0, 0.0);
   glEnd();

   // Set foreground (or drawing) color.
   glColor3f(1.0, 1.0, 1.0);

   // Draw a polygon with specified vertices.
   glBegin(GL_LINES);
      glVertex3f(46.0, 60.0, 1.0);
      glVertex3f(46.0, 10.0, 1.0);
   glEnd();

   float t = 0; // Angle parameter.
    int i;

    // Draw a line loop with vertices at equal angles apart on a circle
    // with center at (X, Y) and radius R, The vertices are colored randomly.
    glBegin(GL_LINE_LOOP);
       for(i = 0; i < numVertices; ++i)
   	 {
          glVertex3f(X + R * cos(t), Y + R * sin(t), 0.0);
          t += 2 * PI / numVertices;
   	 }
    glEnd();

   // Flush created objects to the screen, i.e., force rendering.
   glFlush();
}

// Initialization routine.
void setup(void)
{
   // Set background (or clearing) color.
   glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   // Set viewport size to be entire OpenGL window.
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);

   // Set matrix mode to projection.
   glMatrixMode(GL_PROJECTION);

   // Clear current projection matrix to identity.
   glLoadIdentity();

   // Specify the orthographic (or perpendicular) projection,
   // i.e., define the viewing box.
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);

   // Set matrix mode to modelview.
   glMatrixMode(GL_MODELVIEW);

   // Clear current modelview matrix to identity.
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key)
   {
	  // Press escape to exit.
      case 27:
        exit(0);
        break;
      case 13:
        for(int i=1; i<1000; i++) {
          drawAnotherScene(i, i/2);
        }
        break;
        break;
      case 8:
        reliquias();
        break;
      default:
        break;
    }
}

// Main routine: defines window properties, creates window,
// registers callback routines and begins processing.
int main(int argc, char **argv)
{
   // Initialize GLUT.
   glutInit(&argc, argv);

   // Set display mode as single-buffered and RGB color.
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

   // Set OpenGL window size.
   glutInitWindowSize(600, 600);

   // Set position of OpenGL window upper-left corner.
   glutInitWindowPosition(600, 600);

   // Create OpenGL window with title.
   glutCreateWindow("square.cpp");

   // Initialize.
   setup();

   // Register display routine.
   glutDisplayFunc(drawScene);

   // Register reshape routine.
   glutReshapeFunc(resize);

   // Register keyboard routine.
   glutKeyboardFunc(keyInput);

   // Begin processing.
   glutMainLoop();

   return 0;
}
