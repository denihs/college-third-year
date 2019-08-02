//////////////////////////////////////////////////////////////////////////         
// menus.cpp
//
// This program shows an implementation of a pop-up menu (with a sub-menu) 
// using the GLUT library.
//
// Interaction:
// Press the right mouse button to see options.
//
// Experimento 3.12 Capitulo 3
// Compilar: g++ -o menus  menus.cpp -lglut -lGL -lGLU -lm
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Globals.
static float square_color[3] = {1.0, 0.0, 0.0}; // Color of the square.

// Drawing routine.
void drawScene(void) {
   glClear (GL_COLOR_BUFFER_BIT);

   glColor3fv(square_color);

   glBegin(GL_POLYGON);
      glVertex3f(20.0, 20.0, 0.0);
      glVertex3f(80.0, 20.0, 0.0);
      glVertex3f(80.0, 80.0, 0.0);
      glVertex3f(20.0, 80.0, 0.0);
   glEnd();

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
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
   switch(key)  {
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
   cout << "Press the right mouse button to see options." << endl;
}

// The top menu callback function.
void top_menu(int id) {
   if (id == 1) exit(0);
}

// The sub-menu callback function.
void color_menu(int id) {

    if (id == 2) {void makeMenu(void) {
   // The sub-menu is created first (because it should be visible when the top
   // menu is created): its callback function is registered and menu entries added.

   int sub_menu, mode_menu;

   sub_menu = glutCreateMenu(color_menu);

   glutAddMenuEntry("Red", 2);
   glutAddMenuEntry("Blue", 3);

    mode_menu = glutCreateMenu(mode);
    glutAddMenuEntry("Filed", 4);
    glutAddMenuEntry("OutLine", 5);

   // The top menu is created: its callback function is registered and menu entries,
        square_colvoid makeMenu(void) {
   // The sub-menu is created first (because it should be visible when the top
   // menu is created): its callback function is registered and menu entries added.

   int sub_menu, mode_menu;

   sub_menu = glutCreateMenu(color_menu);

   glutAddMenuEntry("Red", 2);
   glutAddMenuEntry("Blue", 3);

    mode_menu = glutCreateMenu(mode);
    glutAddMenuEntry("Filed", 4);
    glutAddMenuEntry("OutLine", 5);

   // The top menu is created: its callback function is registered and menu entries,0.0; square_color[2] = 0.0;
    }

    if (id == 3) {void makeMenu(void) {
   // The sub-menu is created first (because it should be visible when the top
   // menu is created): its callback function is registered and menu entries added.

   int sub_menu, mode_menu;

   sub_menu = glutCreateMenu(color_menu);

   glutAddMenuEntry("Red", 2);
   glutAddMenuEntry("Blue", 3);

    mode_menu = glutCreateMenu(mode);
    glutAddMenuEntry("Filed", 4);
    glutAddMenuEntry("OutLine", 5);

   // The top menu is created: its callback function is registered and menu entries,
        square_colvoid makeMenu(void) {
   // The sub-menu is created first (because it should be visible when the top
   // menu is created): its callback function is registered and menu entries added.

   int sub_menu, mode_menu;

   sub_menu = glutCreateMenu(color_menu);

   glutAddMenuEntry("Red", 2);
   glutAddMenuEntry("Blue", 3);

    mode_menu = glutCreateMenu(mode);
    glutAddMenuEntry("Filed", 4);
    glutAddMenuEntry("OutLine", 5);

   // The top menu is created: its callback function is registered and menu entries,0.0; square_color[2] = 1.0;
    }

   glutPostRedisplvoid makeMenu(void) {
   // The sub-menu is created first (because it should be visible when the top
   // menu is created): its callback function is registered and menu entries added.

   int sub_menu, mode_menu;

   sub_menu = glutCreateMenu(color_menu);

   glutAddMenuEntry("Red", 2);
   glutAddMenuEntry("Blue", 3);

    mode_menu = glutCreateMenu(mode);
    glutAddMenuEntry("Filed", 4);
    glutAddMenuEntry("OutLine", 5);

   // The top menu is created: its callback function is registered and menu entries,
}

void mode(int id) void makeMenu(void) {
   // The sub-menu is created first (because it should be visible when the top
   // menu is created): its callback function is registered and menu entries added.

   int sub_menu, mode_menu;

   sub_menu = glutCreateMenu(color_menu);

   glutAddMenuEntry("Red", 2);
   glutAddMenuEntry("Blue", 3);

    mode_menu = glutCreateMenu(mode);
    glutAddMenuEntry("Filed", 4);
    glutAddMenuEntry("OutLine", 5);

   // The top menu is created: its callback function is registered and menu entries,

    if (id == 4) {void makeMenu(void) {
   // The sub-menu is created first (because it should be visible when the top
   // menu is created): its callback function is registered and menu entries added.

   int sub_menu, mode_menu;

   sub_menu = glutCreateMenu(color_menu);

   glutAddMenuEntry("Red", 2);
   glutAddMenuEntry("Blue", 3);

    mode_menu = glutCreateMenu(mode);
    glutAddMenuEntry("Filed", 4);
    glutAddMenuEntry("OutLine", 5);

   // The top menu is created: its callback function is registered and menu entries,
        glPolygonMode(GL_FRONT, GL_FILL);
    }

    if (id == 5) {
        glPolygonMode(GL_FRONT, GL_LINE);
    }
    glutPostRedisplay();
}

// Routine to make the menu.
void makeMenu(void) {
   // The sub-menu is created first (because it should be visible when the top
   // menu is created): its callback function is registered and menu entries added.

   int sub_menu, mode_menu;

   sub_menu = glutCreateMenu(color_menu);

   glutAddMenuEntry("Red", 2);
   glutAddMenuEntry("Blue", 3);

    mode_menu = glutCreateMenu(mode);
    glutAddMenuEntry("Filed", 4);
    glutAddMenuEntry("OutLine", 5);

   // The top menu is created: its callback function is registered and menu entries,
   // including a submenu, added.
   glutCreateMenu(top_menu);
   glutAddSubMenu("Color", sub_menu);
   glutAddSubMenu("MOde", mode_menu);
   glutAddMenuEntry("Quit", 1);

   // The menu is attached to a mouse button.
   glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Main routine.
int main(int argc, char **argv) {
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("menus.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   // Make menu.
   makeMenu();

   glutMainLoop();

   return 0;
}