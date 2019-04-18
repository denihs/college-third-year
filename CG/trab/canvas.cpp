////////////////////////////////////////////////////////////////////////////////////        
// canvas.cpp
//
// This program allows the user to draw simple shapes on a canvas.
//
// Interaction:
// Left click on a box on the left to select a primitive.
// Then left click on the drawing area: once for point, twice for line or rectangle.
// Right click for menu options.
//
//  Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////// 

#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define POLYLINE 4
#define CIRCLE 5
#define HEXAGON 6
#define NUMBERPRIMITIVES 6
#define PI 3.14159265358979324

// Use the STL extension of C++.
using namespace std;

// Globals.
static GLsizei width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int tempX, tempY, tempX2, tempY2; // Co-ordinates of clicked point.
static int isGrid = 1; // Is there grid?
static int stipple = 0xFFFF;
static int drawBack = GL_LINE;
static int sizeGrid = 1;
static long font = (long)GLUT_BITMAP_TIMES_ROMAN_24; // Font selection.

static float color[3] = {0.0, 0.0, 0.0};


// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{
    char *c;

    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Point class.
class Point
{
public:
   Point(int xVal, int yVal)
   {
	  x = xVal; y = yVal;
   }
   void drawPoint(void); // Function to draw a point.
private:
   int x, y; // x and y co-ordinates of point.
   static float size; // Size of point.
};

float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{  
   glPointSize(size);
   glBegin(GL_POINTS);
      glColor3fv(color);
      glVertex3f(x, y, 0.0);
   glEnd();   
}

// Vector of points.
vector<Point> points;

// Iterator to traverse a Point array.
vector<Point>::iterator pointsIterator; 

// Function to draw all points in the points array.
void drawPoints(void)
{
   // Loop through the points array drawing each point.
   pointsIterator = points.begin();
   while(pointsIterator != points.end() )
   {
      pointsIterator->drawPoint();
	  pointsIterator++;
   }
}

// Line class.
class Line
{
public:
   Line(int x1Val, int y1Val, int x2Val, int y2Val)
   {
	  x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
   }
   void drawLine();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of endpoints.
};


// Function to draw a line.
void Line::drawLine()
{
   glBegin(GL_LINES);
      glColor3fv(color);
      glVertex3f(x1, y1, 0.0);
      glVertex3f(x2, y2, 0.0);
   glEnd();
}

// Vector of lines.
vector<Line> lines;

// Iterator to traverse a Line array.
vector<Line>::iterator linesIterator;

// Function to draw all lines in the lines array.
void drawLines(void)
{
   // Loop through the lines array drawing each line.
   linesIterator = lines.begin();
   while(linesIterator != lines.end() )
   {
      linesIterator->drawLine();
	  linesIterator++;
   }
}

// Rectangle class.
class Rectangle
{
public:
   Rectangle(int x1Val, int y1Val, int x2Val, int y2Val)
   {
	  x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
   }
   void drawRectangle();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices.
};

// Function to draw a rectangle.
void Rectangle::drawRectangle()
{
   glPolygonMode(GL_FRONT_AND_BACK, drawBack);
   glColor3fv(color);
   glRectf(x1, y1, x2, y2);
}

// Vector of rectangles.
vector<Rectangle> rectangles;

// Iterator to traverse a Rectangle array.
vector<Rectangle>::iterator rectanglesIterator;

// Function to draw all rectangles in the rectangles array.
void drawRectangles(void)
{
   // Loop through the rectangles array drawing each rectangle.
   rectanglesIterator = rectangles.begin();
   while(rectanglesIterator != rectangles.end() )
   {
      rectanglesIterator->drawRectangle();
	  rectanglesIterator++;
   }
}
// Circle class.
class Circle
{
public:
    Circle(int x1Val, int y1Val, int x2Val, int y2Val, int nv)
   {
	  x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val; numVertices = nv;
   }
   void drawCircle(int fixedColor = 0);
private:
   int x1, y1, x2, y2, numVertices; // x and y co-ordinates of diagonally opposite vertices.
};

// Function to draw a circle.
void Circle::drawCircle(int fixedColor)
{

    float t = 0;
    float R = sqrtl(pow((float) (x2 - x1), 2) + pow((float) (y2 - y1), 2));

    glPolygonMode(GL_FRONT_AND_BACK, drawBack);
    glBegin(drawBack == GL_FILL ? GL_POLYGON : GL_LINE_LOOP);
        for(int i = 0; i < numVertices; ++i)  {
            if (fixedColor) glColor3f(0.0, 0.0, 0.0);
            else glColor3fv(color);

            glVertex3f(x1 + R * cos(t), y1 + R * sin(t), 0.0);
            t += 2 * PI / numVertices;
        }
    glEnd();
}

// Vector of rectangles.
vector<Circle> circles;

// Iterator to traverse a Rectangle array.
vector<Circle>::iterator circlesIterator;

// Function to draw all circles in the circles array.
void drawCircles(void)
{
   // Loop through the circles array drawing each circle.
    circlesIterator = circles.begin();
   while(circlesIterator != circles.end() )
   {
       circlesIterator->drawCircle();
       circlesIterator++;
   }
}

// Function to draw point selection box in left selection area.
void drawPointSelectionBox(void)
{  
   if (primitive == POINT) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.9*height, 0.1*width, height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0); 
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.9*height, 0.1*width, height); 

   // Draw point icon.
   glPointSize(pointSize);
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_POINTS);
      glVertex3f(0.05*width, 0.95*height, 0.0);
   glEnd();  
}

// Function to draw line selection box in left selection area.
void drawLineSelectionBox(void)
{  
   if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Draw line icon.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINES);
      glVertex3f(0.025*width, 0.825*height, 0.0);
      glVertex3f(0.075*width, 0.875*height, 0.0);
   glEnd();  
}

// Function to draw rectangle selection box in left selection area.
void drawRectangleSelectionBox(void)
{  
   if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Draw rectangle icon.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, drawBack);
   glRectf(0.025*width, 0.735*height, 0.075*width, 0.765*height);
   glEnd();  
}


// Function to draw polyline selection box in left selection area.
void drawPolylineSelectionBox(void)
{

   float w1 = 0.0,
         w2 = 0.1*width,
         h1 = 0.6*height,
         h2 = 0.7*height;

   if (primitive == POLYLINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(w1, h1, w2, h2);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(w1, h1, w2, h2);

   // Draw line icon.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINE_STRIP);
   glVertex3f(0.005*width, 0.67*height, 0.0);
   glVertex3f(0.04*width, 0.62*height, 0.0);
   glVertex3f(0.04*width, 0.67*height, 0.0);
   glVertex3f(0.08*width, 0.61*height, 0.0);
   glEnd();
}

// Function to draw circle selection box in left selection area.
void drawCircleSelectionBox(void)
{

   float w1 = 0.0,
         w2 = 0.1*width,
         h1 = 0.5*height,
         h2 = 0.6*height;

   if (primitive == CIRCLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(w1, h1, w2, h2);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(w1, h1, w2, h2);

   Circle c = Circle(0.05 * width, 0.55 * height, 0.09 * width , 0.55 * height, 360);
   c.drawCircle(1);

}
// Function to draw hexagon selection box in left selection area.
void drawHexagonSelectionBox(void)
{

   float w1 = 0.0,
         w2 = 0.1*width,
         h1 = 0.4*height,
         h2 = 0.5*height;

   if (primitive == HEXAGON) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(w1, h1, w2, h2);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(w1, h1, w2, h2);

   Circle c = Circle(0.05 * width, 0.45 * height, 0.09 * width , 0.45 * height, 6);
   c.drawCircle(1);

}

// Function to draw unused part of left selection area.
void drawInactiveArea(void)
{
   glColor3f(0.6, 0.6, 0.6);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);

   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);
}

// Function to draw temporary point.
void drawTempPoint(void)
{
   glColor3f(1.0, 0.0, 0.0);
   glPointSize(pointSize);
   glBegin(GL_POINTS);
      glVertex3f(tempX, tempY, 0.0);
   glEnd(); 
}

// Function to draw a grid.
void drawGrid(void)
{
   int i;

   int w, z;
   float p, q;

   if (sizeGrid == 1) w = 9, z = 9, p = 0.1, q = 0.1;
   if (sizeGrid == 2) w = 4, z = 4, p = 0.2, q = 0.2;
   if (sizeGrid == 3) w = 3, z = 3, p = 0.3333333, q = 0.3333333;

   glEnable(GL_LINE_STIPPLE);
   glLineStipple(1, 0x5555); 
   glColor3f(0.75, 0.75, 0.75);

   glBegin(GL_LINES);
	  for (i=1; i <= w; i++)
	  {
         glVertex3f((width * 0.1) + i*p*(width * 0.9), 0.0, 0.0);
         glVertex3f((width * 0.1) + i*p*(width * 0.9), height, 0.0);
	  }
	  for (i=1; i <= z; i++)
	  {
         glVertex3f(0.1*width, i*q*height, 0.0);
         glVertex3f(width, i*q*height, 0.0);
	  }
   glEnd();
   glDisable(GL_LINE_STIPPLE);
}

void dynamicDraw() {
    // Update the location of the current point as the mouse moves with no button pressed.
    if (pointCount){
        switch (primitive) {
            case CIRCLE: {
                Circle c = Circle(tempX, tempY, tempX2 , tempY2, 360);
                c.drawCircle();
                break;
            }
            case LINE:
            case POLYLINE: {
                Line l = Line(tempX, tempY, tempX2 , tempY2);
                l.drawLine();
                break;
            }
            case RECTANGLE: {
                Rectangle x = Rectangle(tempX, tempY, tempX2 , tempY2);
                x.drawRectangle();
                break;
            }
            case HEXAGON: {
                Circle x = Circle(tempX, tempY, tempX2 , tempY2, 6);
                x.drawCircle();
                break;
            }
            case POINT: {
                Point x = Point(tempX, tempY);
                x.drawPoint();
                break;
            }
            default:
                break;
        }
    }
}

char * getDrawName() {
    switch (primitive) {
        case POINT:
            return (char *) "Point";
        case RECTANGLE:
            return (char *) "Rectangle";
        case LINE:
            return (char *) "Line";
        case POLYLINE:
            return (char *) "Polyline";
        case HEXAGON:
            return (char *) "Hexagon";
        case CIRCLE:
            return (char *) "Circle";
        default:
            return (char *) "Nada selecionado";
    }
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);

   drawPointSelectionBox();
   drawLineSelectionBox();
   drawRectangleSelectionBox();
   drawPolylineSelectionBox();
   drawCircleSelectionBox();
   drawHexagonSelectionBox();
   drawInactiveArea();

    glEnable(GL_LINE_STIPPLE);
        glLineStipple(5, stipple);
       drawPoints();
       drawLines();
       drawRectangles();
       drawCircles();
       dynamicDraw();
    glDisable(GL_LINE_STIPPLE);

   if ( (primitive != INACTIVE) && (primitive != POINT) &&
	   (pointCount == 1) ) drawTempPoint();
   if (isGrid) drawGrid();

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(width * 0.1, height * 0.01, 0.0);
    writeBitmapString((void*)font, getDrawName());

   glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
   if ( y < (1- NUMBERPRIMITIVES*0.1)*height ) primitive = INACTIVE;
   else if ( y < (1 - 5*0.1)*height ) primitive = HEXAGON;
   else if ( y < (1 - 4*0.1)*height ) primitive = CIRCLE;
   else if ( y < (1 - 3*0.1)*height ) primitive = POLYLINE;
   else if ( y < (1 - 2*0.1)*height ) primitive = RECTANGLE;
   else if ( y < (1 - 1*0.1)*height ) primitive = LINE;
   else primitive = POINT;
}

void startDrawing(int x, int y) {
    tempX = x; tempY = y; tempX2 = x; tempY2 = y;
    pointCount++;
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      y = height - y; // Correct from mouse to OpenGL co-ordinates.

	  // Click outside canvas - do nothing.
      if ( x < 0 || x > width || y < 0 || y > height ) ;

	  // Click in left selection area.
      else if ( x < 0.1*width ) 
	  {
	     pickPrimitive(y);
		 pointCount = 0;
	  }

	  // Click in canvas.
      else
	  {
	     if (primitive == POINT) points.push_back( Point(x,y) );
         else if (primitive == LINE) 
		 { 
	        if (pointCount == 0)
			{
                startDrawing(x, y);
			}
	        else 
			{
               lines.push_back( Line(tempX, tempY, x, y) );
                pointCount = 0;
			}
		 }
         else if (primitive == POLYLINE)
		 {
	        if (pointCount == 0)
			{
                startDrawing(x, y);
			}
	        else
			{
               lines.push_back( Line(tempX, tempY, x, y) );
               tempX = x;
               tempY = y;
			}
		 }
         else if (primitive == RECTANGLE) 
		 { 
	        if (pointCount == 0)
			{
                startDrawing(x, y);
			}
	        else 
			{
               rectangles.push_back( Rectangle(tempX, tempY, x, y) );
		       pointCount = 0;
			}
		 }
         else if (primitive == CIRCLE)
		 {
	        if (pointCount == 0)
			{
                startDrawing(x, y);
			}
	        else
			{
               circles.push_back( Circle(tempX, tempY, x, y, 360) );
		       pointCount = 0;
			}
		 }
         else if (primitive == HEXAGON)
		 {
	        if (pointCount == 0)
			{
                startDrawing(x, y);
			}
	        else
			{
               circles.push_back( Circle(tempX, tempY, x, y, 6) );
		       pointCount = 0;
			}
		 }
	  }
   }
   if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) pointCount = 0;
    glutPostRedisplay();
}

// Mouse motion callback routine.
void mousePassiveMotion(int x, int y)
{
    tempX2 = x; tempY2 = height - y;
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
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
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

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
   points.clear();
   lines.clear();
   rectangles.clear();
   circles.clear();
   primitive = INACTIVE;
   pointCount = 0;
}

// The right button menu callback function.
void rightMenu(int id)
{
   if (id==1) 
   {
	  clearAll();
	  glutPostRedisplay();
   }
   if (id==2) exit(0);
}

// The sub-menu callback function.
void grid_menu(int id)
{
   if (id==3) isGrid = 1;
   if (id==4) isGrid = 0;
   glutPostRedisplay();
}

void colorsMenu(int id) {
    if (id==5)
    {
        color[0] = 0.0; color[1] = 0.0; color[2] = 0.0;
    }
    if (id==6)
    {
        color[0] = 1.0; color[1] = 0.0; color[2] = 0.0;
    }
    if (id==7)
    {
        color[0] = 0.0; color[1] = 1.0; color[2] = 0.0;
    }
    if (id==8)
    {
        color[0] = 0.0; color[1] = 0.0; color[2] = 1.0;
    }
    glutPostRedisplay();
}

void getStipple(int id) {
    if (id==1)
    {
        stipple = 0xFFFF;
    }
    if (id==2)
    {
        stipple = 0x0101;
    }
    if (id==3)
    {
        stipple = 0x5555;
    }
    if (id==4)
    {
        stipple = 0x00FF;
    }
    glutPostRedisplay();
}

void getDrawBack(int id) {
    if (id==1)
    {
        drawBack = GL_FILL;
    }
    if (id==2)
    {
        drawBack = GL_LINE;
    }
    glutPostRedisplay();
}

void getSizeGrid(int id) {
    sizeGrid = id;
    glutPostRedisplay();
}

// Function to create menu.
void makeMenu(void)
{
   int sub_menu;
   sub_menu = glutCreateMenu(grid_menu);
   glutAddMenuEntry("On", 3);
   glutAddMenuEntry("Off",4);

   int colors = glutCreateMenu(colorsMenu);
    glutAddMenuEntry("Black", 5);
    glutAddMenuEntry("Red",6);
    glutAddMenuEntry("Green",7);
    glutAddMenuEntry("Blue",8);

    int subStipple = glutCreateMenu(getStipple);
    glutAddMenuEntry("0xFFFF", 1);
    glutAddMenuEntry("0x0101", 2);
    glutAddMenuEntry("0x5555", 3);
    glutAddMenuEntry("0x00FF", 4);

    int subDrawBack = glutCreateMenu(getDrawBack);
    glutAddMenuEntry("Filled", 1);
    glutAddMenuEntry("Outlined", 2);

    int subSizeGrid = glutCreateMenu(getSizeGrid);
    glutAddMenuEntry("1x", 1);
    glutAddMenuEntry("2x", 2);
    glutAddMenuEntry("3x", 3);

   glutCreateMenu(rightMenu);
   glutAddSubMenu("Grid", sub_menu);
   glutAddSubMenu("Size grid", subSizeGrid);
   glutAddSubMenu("Colors", colors);
   glutAddSubMenu("Stipple", subStipple);
   glutAddSubMenu("Outlined/Filled", subDrawBack);
   glutAddMenuEntry("Clear",1);
   glutAddMenuEntry("Quit",2);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Left click on a box on the left to select a primitive." << endl
        << "Then left click on the drawing area: once for point, twice for line or rectangle." << endl
        << "Right click for menu options." <<  endl; 
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_DOUBLE); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("canvas.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMouseFunc(mouseControl);
    // Register the mouse passive motion callback function.
    glutPassiveMotionFunc(mousePassiveMotion);

   makeMenu(); // Create menu.

   glutMainLoop(); 

   return 0;  
}