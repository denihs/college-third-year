w///////////////////////////////////////////////////////////////////////////////////////
// hemisphere.cpp
//
// This program approximates a hemisphere with an array of latitudinal triangle strips.
//
// Interaction:
// Press P/p to increase/decrease the number of longitudinal slices.
// Press Q/q to increase/decrease the number of latitudinal slices.
// Press x, X, y, Y, z, Z to turn the hemisphere.
//
// Sumanta Guha.
/////////////////////////////////////////////////////////////////////////////////////// 

#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

// Globals.
int r = 5;
static float R = 1.5; // Radius of hemisphere.
static int p = 6; // Number of longitudinal slices.
static int q = 4; // Number of latitudinal slices.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.

// Drawing routine.
void drawScene(void)
{
   int  i, j;

   glClear (GL_COLOR_BUFFER_BIT);

   glLoadIdentity();

   // Command to push the hemisphere, which is drawn centered at the origin, 
   // into the viewing frustum.
   glTranslatef(0.0, 0.0, -10.0);

   // Commands to turn the hemisphere.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);
   
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glColor3f(0.0, 0.0, 0.0);

   // Array of latitudinal triangle strips, each parallel to the equator, stacked one
   // above the other from the equator to the north pole.
    float u, u1, v1;
   for(j = 0; j < q; j++) {
      // One latitudinal triangle strip.
      glBegin(GL_TRIANGLE_STRIP);
         for(i = 0; i <= p; i++) {
            
          u1 = (float)(j+1)/q * 2;
          u  = (float)(j)/q * 2;
          v1 = (float)(i)/p * PI * 2.0;
            
            glVertex3f(
                u1 * cos(v1) ,
                u1 *u1,
                u1 * sin(v1)  
            );

             glVertex3f(
                u * cos(v1) ,
                u * u,
                u * sin(v1)  
            );
              
      }

      glEnd();
   }

   glFlush();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-7.0, 7.0, -7.0, 7.0, 5.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      case 'P':
         p += 1;
         glutPostRedisplay();
         break;
      case 'p':
         if (p > 3) p -= 1;
         glutPostRedisplay();
         break;
      case 'Q':
         q += 1;
         glutPostRedisplay();
         break;
      case 'q':
         if (q > 3) q -= 1;
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
   cout << "Press P/p to increase/decrease the number of longitudinal slices." << endl
        << "Press Q/q to increase/decrease the number of latitudinal slices." << endl
        << "Press x, X, y, Y, z, Z to turn the hemisphere." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("hemisphere.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMainLoop(); 

   return 0;  
}
