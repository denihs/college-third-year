////////////////////////////////////////////////////////////////
// floweringPlant.cpp
//
// This program shows the animation of a plant with a flower -
// from a wilted state to blooming.
// The stem consists of 4 straight line segments and the flower
// of a hemisphere-like sepal and 6 elliptic petals.
//
// Interaction:
// Press space to toggle between animation on and off.
// Press delete to reset.
// Press the x, X, y, Y, z, Z keys to rotate the scene.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265358979324

using namespace std;

// Globals.
static float t = 0.0; // Animation parameter.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.
static unsigned int base; // Display lists base index.


// Initialization routine.
void setup(void) {
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   base = glGenLists(3); 
   glListBase(base); 
}

// Drawing routine.
void drawScene(void) {

    /* t é usado para interpolar linearmente entre o
     * valor inicial e final de cada parâmetro de
     * configuração. */

   float angleFirstSegment  = (1 - t) *  90.0 + t * 90.0; // Angle of first plant segment interpolating between 60 and 90.
   float angleSecondSegment = (1 - t) * -90.0 + t * 0.0;  // Angle of second plant segment.
   float angleThirdSegment  = (1 - t) * -90.0 + t * 0.0;  // Angle of third plant segment.
   float angleFourthSegment = (1 - t) * -90.0 + t*0.0;    // Angle of fourth plant segment.

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   glTranslatef(0.0, -10.0, -30.0);

   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

   // Stem segment display list.
   glNewList(base, GL_COMPILE);
   glColor3f(0.0, 1.0, 0.0);
   glLineWidth(3.0);

   glBegin(GL_LINES);
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(5.0, 0.0, 0.0);
   glEnd();
   glEndList();

   // Rotate scene.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0); 
   
   // First stem segment.
   glRotatef(angleFirstSegment, 0.0, 0.0, 1.0);
   glCallList(base);

   // Second stem segment.
   glTranslatef(5.0, 0.0, 0.0);
   glRotatef(angleSecondSegment, 0.0, 0.0, 1.0);
   glCallList(base);

   // Third stem segment.
   glTranslatef(5.0, 0.0, 0.0);
   glRotatef(angleThirdSegment, 0.0, 0.0, 1.0);
   glCallList(base);

   // Fourth stem segment.
   glTranslatef(5.0, 0.0, 0.0);
   glRotatef(angleFourthSegment, 0.0, 0.0, 1.0);
   glCallList(base);

   glutSwapBuffers();
}

// Timer function.
void animate(int value) {

    if (isAnimate) {
      t += 0.1;

	  if (t >= 1.0)
	      isAnimate = 0;
    }

   glutTimerFunc(animationPeriod, animate, 1);
   glutPostRedisplay();
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
   switch(key) {
      case 27:
         exit(0);
         break;

	  case ' ':
	      isAnimate ? isAnimate = 0 : isAnimate = 1;
          glutPostRedisplay();
		 break;

	  case 127: 
         if (isAnimate) isAnimate = 0;
		 t = 0.0;
         glutPostRedisplay();
		 break;

      case 'x':
         Xangle += 5.0;
		 if (Xangle > 360.0) Xangle -= 360.0;
         glutPostRedisplay();
         break;

      case 'X':
         Xangle -= 5.0;
		 if (Xangle < 0.0) Xangle += 360.0;
         glutPostRedisplay();
         break;

      case 'y':
         Yangle += 5.0;
		 if (Yangle > 360.0) Yangle -= 360.0;
         glutPostRedisplay();
         break;

      case 'Y':
         Yangle -= 5.0;
		 if (Yangle < 0.0) Yangle += 360.0;
         glutPostRedisplay();
         break;

      case 'z':
         Zangle += 5.0;
		 if (Zangle > 360.0) Zangle -= 360.0;
         glutPostRedisplay();
         break;

      case 'Z':
         Zangle -= 5.0;
		 if (Zangle < 0.0) Zangle += 360.0;
         glutPostRedisplay();
         break;

      default:
         break;
   }
}


// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off." << endl
	    << "Press delete to reset." << endl
        << "Press the x, X, y, Y, z, Z keys to rotate the scene." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow ("floweringPlant.cpp"); 
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutTimerFunc(5, animate, 1);
   glutMainLoop(); 

   return 0;  
}

