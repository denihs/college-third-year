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

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

#define INACTIVE 0
#define TRANSLATION 1
#define POINT 1
#define SCALE 2
#define LINE 2
#define RECTANGLE 3
#define NUMBERPRIMITIVES 3

#define SIDE_MENU_WIDTH 0.4285

// Use the STL extension of C++.
using namespace std;

// Globals.
static GLsizei width, height; // OpenGL window size.
static float pointSize = 6.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int tempX, tempY; // Co-ordinates of clicked point.
static int isGrid = 1; // Is there grid?
static int translationX = 0, translationY = 0;
static int scaleX = 10, scaleY = 10;
static long font = (long)GLUT_BITMAP_9_BY_15; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string, int x = 100)
{
    char *c;
    int i[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    int aux = x;

    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);

    if (x < 100) {

        if (x < 0) {
            glutBitmapCharacter(font, 45);
            aux *= -1;
        }

        if (aux >= 0 && aux < 10) {
            glutBitmapCharacter(font, aux + 48);
            return;
        }

        i[0] = 1;
        i[1] = 0;
        for (int h = 0; i[h] != 100; h++) glutBitmapCharacter(font, i[h] + 48);

    }
}

// Point class.
class Point
{
public:
    Point(double xVal, double yVal)
    {
        x = xVal; y = yVal;
    }
    void drawPoint(void); // Function to draw a point.
private:
    double x, y; // x and y co-ordinates of point.
    static float size; // Size of point.
};

float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{
    glPointSize(size);
    glBegin(GL_POINTS);
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
    Line(double x1Val, double y1Val, double x2Val, double y2Val)
    {
        x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
    }
    void drawLine();
private:
    double x1, y1, x2, y2; // x and y co-ordinates of endpoints.
};


// Function to draw a line.
void Line::drawLine()
{
    glBegin(GL_LINES);
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

float getVariant(float V, int U) {
    return (1 + ((V * U) / 10));
}

// Function to draw translation selection box in left selection area.
void drawTranslationBox(void) {
    float w1 = 0.0,
            w2 = SIDE_MENU_WIDTH*width,
            h1 = 0.7*height,
            h2 = 1 * height;


    if (primitive == TRANSLATION) glColor3f(1.0, 1.0, 1.0); // Highlight.
    else glColor3f(0.8, 0.8, 0.8); // No highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(w1, h1, w2, h2);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(w1, h1, w2, h2);

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(width * 0.01, height * 0.95, 0.0);
    writeBitmapString((void*)font, "Translation");

    glRasterPos3f(width * 0.01, height * 0.85, 0.0);
    writeBitmapString((void*)font, "X = ", translationX);

    glRasterPos3f(width * 0.01, height * 0.80, 0.0);
    writeBitmapString((void*)font, "Y = ", translationY);

    double centerX = SIDE_MENU_WIDTH * 0.75 * width;
    double centerY = 0.85*height;

    Line lx = Line(SIDE_MENU_WIDTH * 0.5 * width, centerY, SIDE_MENU_WIDTH * width, centerY);
    lx.drawLine();

    Line ly = Line(centerX, h1, centerX, h2);
    ly.drawLine();

    // Max mult center X -> 1.33
    // Min mult center X -> 0.66

    float variantX = 0.33;
    glColor3f(0.0, 0.0, 1.0);
    Point px = Point(centerX * getVariant(variantX, translationX), centerY);
    px.drawPoint();


    // Max mult cent Y -> 1.17
    // Min mult cent Y -> 0.83
    float variantY = 0.17;
    glColor3f(1.0, 0.0, 0.0);
    Point py = Point(centerX, centerY * getVariant(variantY, translationY));
    py.drawPoint();

}
// Function to draw scale selection box in left selection area.
void drawScaleBox(void) {
    float w1 = 0.0,
            w2 = SIDE_MENU_WIDTH*width,
            h1 = 0.4*height,
            h2 = 0.7 * height;


    if (primitive == SCALE) glColor3f(1.0, 1.0, 1.0); // Highlight.
    else glColor3f(0.8, 0.8, 0.8); // No highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(w1, h1, w2, h2);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(w1, h1, w2, h2);

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(width * 0.01, height * 0.65, 0.0);
    writeBitmapString((void*)font, "Scale");

    glRasterPos3f(width * 0.01, height * 0.55, 0.0);
    writeBitmapString((void*)font, "W = ", scaleX);

    glRasterPos3f(width * 0.01, height * 0.50, 0.0);
    writeBitmapString((void*)font, "H = ", scaleY);

    double centerX = SIDE_MENU_WIDTH * 0.5 * width;
    double centerY = 0.42*height;

    Line lx = Line(SIDE_MENU_WIDTH * 0.5 * width, centerY, SIDE_MENU_WIDTH * width, centerY);
    lx.drawLine();

    Line ly = Line(centerX, h1 * 1.05, centerX, h2);
    ly.drawLine();


    float variantX = 0.98;
    glColor3f(0.0, 0.0, 1.0);
    Point px = Point(centerX * getVariant(variantX, scaleX), centerY);
    px.drawPoint();


    float variantY = 0.66;
    glColor3f(1.0, 0.0, 0.0);
    Point py = Point(centerX, centerY * getVariant(variantY, scaleY));
    py.drawPoint();

}

// Function to draw Cartesian plane selection
void drawCartesianPlace(void) {
    float w1 = 0.0,
            w2 = SIDE_MENU_WIDTH*width,
            h1 = 0.7*height,
            h2 = 1 * height;

    double boxWidth = SIDE_MENU_WIDTH * width;
    double centerX =  ((width  - boxWidth) / 2) + boxWidth;
    double centerY = 0.5*height;

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x5555);

    glColor3f(0.0, 0.0, 0.0);
    Line lx = Line(boxWidth, centerY, 1 * width, centerY);
    lx.drawLine();

    Line ly = Line(centerX, 0, centerX, 1 * height);
    ly.drawLine();
    glDisable(GL_LINE_STIPPLE);


    float variantX = 0.395;
    float variantY = 0.99;

    double coordX = centerX * getVariant(variantX, translationX);
    double coordY = centerY * getVariant(variantY, translationY);

    int cubeScale = 5;

    double  dist =

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(coordX - (float)(scaleX * cubeScale), coordY + (float)(scaleY * cubeScale), 0.0);

        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(coordX - (float)(scaleX * cubeScale), coordY - (float)(scaleY * cubeScale), 0.0);

        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(coordX + (float)(scaleX * cubeScale), coordY - (float)(scaleY * cubeScale), 0.0);

        glColor3f(1.0, 1.0, 0.0);
        glVertex3f(coordX + (float)(scaleX * cubeScale), coordY + (float)(scaleY * cubeScale), 0.0);
    glEnd();


    if (scaleX == 0 || scaleY == 0) glColor3f(0.0, 128.0, 0.0);
    else glColor3f(1.0, 1.0, 1.0);
    Point px = Point(coordX, coordY);
    px.drawPoint();

}

// Function to draw unused part of left selection area.
void drawInactiveArea(void)
{
    glColor3f(0.6, 0.6, 0.6);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.0, SIDE_MENU_WIDTH*width, (1 - NUMBERPRIMITIVES*0.3)*height);

    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.0, SIDE_MENU_WIDTH*width, (1 - NUMBERPRIMITIVES*0.3)*height);
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

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);

    drawCartesianPlace();
    drawTranslationBox();
    drawScaleBox();
    drawInactiveArea();

    drawPoints();
    drawLines();
    drawRectangles();
    if ( ((primitive == LINE) || (primitive == RECTANGLE)) &&
         (pointCount == 1) ) drawTempPoint();

    glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
    if ( y < (1- NUMBERPRIMITIVES*0.3)*height ) primitive = INACTIVE;
    else if ( y < (1 - 2*0.3)*height ) primitive = RECTANGLE;
    else if ( y < (1 - 1*0.3)*height ) primitive = SCALE;
    else primitive = TRANSLATION;
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
        else if ( x < SIDE_MENU_WIDTH*width )
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
                    tempX = x; tempY = y;
                    pointCount++;
                }
                else
                {
                    lines.push_back( Line(tempX, tempY, x, y) );
                    pointCount = 0;
                }
            }
            else if (primitive == RECTANGLE)
            {
                if (pointCount == 0)
                {
                    tempX = x; tempY = y;
                    pointCount++;
                }
                else
                {
                    rectangles.push_back( Rectangle(tempX, tempY, x, y) );
                    pointCount = 0;
                }
            }
        }
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

void  keyControl(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if (translationY < 10 && primitive == TRANSLATION) translationY++;
            if (scaleY < 10 && primitive == SCALE) scaleY++;
            break;
        case GLUT_KEY_DOWN:
            if (translationY > -10 && primitive == TRANSLATION) translationY--;
            if (scaleY > 0 && primitive == SCALE) scaleY--;
            break;
        case GLUT_KEY_LEFT:
            if (translationX > -10 && primitive == TRANSLATION) translationX--;
            if (scaleX > 0 && primitive == SCALE) scaleX--;
            break;
        case GLUT_KEY_RIGHT:
            if (translationX < 10 && primitive == TRANSLATION) translationX++;
            if (scaleX < 10 && primitive == SCALE) scaleX++;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
    points.clear();
    lines.clear();
    rectangles.clear();
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

// Function to create menu.
void makeMenu(void)
{
    int sub_menu;
    sub_menu = glutCreateMenu(grid_menu);
    glutAddMenuEntry("On", 3);
    glutAddMenuEntry("Off",4);

    glutCreateMenu(rightMenu);
    glutAddSubMenu("Grid", sub_menu);
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
    glutInitWindowSize(800, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Geometric Translation");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutMouseFunc(mouseControl);
    glutSpecialFunc(keyControl);
    makeMenu(); // Create menu.

    glutMainLoop();

    return 0;
}