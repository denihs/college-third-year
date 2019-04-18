#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265358979324
#define N 40.0 // Number of vertices on the boundary of the disc.

//using namespace std;

static GLsizei width, height; // OpenGL window size.

typedef struct{
        float x;
        float y;
}Pnt;

// Currently drag point.
Pnt currentPoint;
Pnt center;

int flagcenter=0;

void drawPoint(Pnt p){
   glPointSize(10);
   glBegin(GL_POINTS);
      glVertex3f(p.x, p.y, 0.0);
   glEnd();   
}

void drawCircle(float R, float X, float Y){
   float t;
   int i;
   glBegin(GL_LINE_LOOP);
      for(i = 0; i <= N; ++i){
         t = 2 * PI * i / N; 
         glVertex3f(X+ R * cos(t), Y+ R*sin(t), 0.0);
	  }
   glEnd();
}

float distance (Pnt P1, Pnt P2){
      float dist;
      dist= sqrt((P1.x-P2.x)*(P1.x-P2.x)+(P1.y-P2.y)*(P1.y-P2.y));
      
      return(dist);
}
void drawScene(void) {
     
     float R;
     glClear(GL_COLOR_BUFFER_BIT);
	 glColor3f(0.0,0.0,0.0);
	
	 drawPoint(center);
	
	 drawCircle (distance(center,currentPoint),center.x, center.y);
	
	glFlush();
}


// Mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
   // Store the clicked point in the center variable when left button is pressed the first time.
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
      if (!flagcenter){
          center.x=currentPoint.x=x;
          center.y=currentPoint.y=height-y;
          flagcenter=1;
      }else{
         flagcenter=0; 
      }
   if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) exit(0);
   
   glutPostRedisplay();
}

// Mouse motion callback routine.
void mousePassiveMotion(int x, int y)
{ 
   // Update the location of the current point as the mouse moves with no button pressed.
   if (flagcenter){
      currentPoint.x=x;
      currentPoint.y= height - y;   
      }
   glutPostRedisplay();
}


// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Set viewing box dimensions equal to window dimensions.
   glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

   // Pass the size of the OpenGL window to globals.
   width = w; 
   height = h; 

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
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
   printf( "Interaction:\n");
   printf( "Left mouse click to draw (first) a cicle's center,\n and after keep drag a second point for defining the circle's radio. \nPress again left button for accept second point\n"); 
   printf ("right mouse click to exit.");
}

int main (int argc,char** argv){
    printInteraction();
	glutInit(&argc,argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE );
	glutInitWindowSize(500,500);
	/*Set the posotion of window*/
	glutInitWindowPosition(0,0);
	glutCreateWindow("Interactive Circle in C");
	setup();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);  
    glutKeyboardFunc(keyInput);
   // Register the mouse callback function.
    glutMouseFunc(mouseControl); 
   // Register the mouse passive motion callback function.
    glutPassiveMotionFunc(mousePassiveMotion);

	glutMainLoop();
	
	return 0;  
} 