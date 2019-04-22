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
#define TRANSLATION 1
#define SCALE 2
#define ROTATE 3
#define NUMBERPRIMITIVES 3
#define PI 3.14159265358979324

#define SIDE_MENU_WIDTH 0.4285

// Use the STL extension of C++.
using namespace std;

// Globals.
static GLsizei width, height; // OpenGL window size.
static float pointSize = 6.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int translationX = 0, translationY = 0;
static int scaleX = 0, scaleY = 0;
static int degrees = 0;
static long font = (long)GLUT_BITMAP_9_BY_15; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string, int x = 1000) {
    char *c;
    int i[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    int aux = x;
    
    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
    
    if (x < 1000) {
        if (x < 0) {
            glutBitmapCharacter(font, 45);
            aux *= -1;
        }
        
        if (aux >= 0 && aux < 10) {
            glutBitmapCharacter(font, aux + 48);
            return;
        }

        int d = aux < 100 ? 10 : 100;
        
        for (int j = 0; j < 10, d >= 1; j++) {
            i[j] = aux / d;
            int rest = aux % d;
            aux = rest;
            d /= 10;
        }

        for (int h = 0; i[h] != 100; h++) glutBitmapCharacter(font, i[h] + 48);

    }
}

// Point class.
class Point {
public:
    Point(double xVal, double yVal) {
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

float getVariant(float V, int U) {
    return (1 + ((V * U) / 10));
}

// Circle class.
class Circle {
public:
    Circle(double x1Val, double y1Val, double r, int nv)
   {
	  x1 = x1Val; y1 = y1Val; R = r; numVertices = nv;
   }
   void drawCircle();
private:
   double x1, y1, R; 
   int numVertices;
};

// Function to draw a circle.
void Circle::drawCircle() {

    float t = 0;

    glBegin(GL_LINE_LOOP);
        for(int i = 0; i < numVertices; ++i)  {
            glColor3f(0.0, 0.0, 0.0);
            glVertex3f(x1 + R * cos(t), y1 + R * sin(t), 0.0);
            t += 2 * PI / numVertices;
        }
    glEnd();
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

// Function to draw rotate selection box in left selection area.
void drawRotateBox(void) {
    float w1 = 0.0,
            w2 = SIDE_MENU_WIDTH*width,
            h1 = 0.1*height,
            h2 = 0.4 * height;


    if (primitive == ROTATE) glColor3f(1.0, 1.0, 1.0); // Highlight.
    else glColor3f(0.8, 0.8, 0.8); // No highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(w1, h1, w2, h2);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(w1, h1, w2, h2);

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(width * 0.01, height * 0.35, 0.0);
    writeBitmapString((void*)font, "Rotate");

    glRasterPos3f(width * 0.01, height * 0.25, 0.0);
    writeBitmapString((void*)font, "G = ", degrees);

    double centerX = SIDE_MENU_WIDTH * 0.7 * width;
    double centerY = 0.25*height;

    double R = 60.0;
    double nVert = 360;

    Circle c = Circle(centerX, centerY, R, nVert);
    c.drawCircle();

    Line l = Line(centerX, centerY, centerX + R, centerY);
    l.drawLine();

    double t = (2 * PI / nVert) * degrees;
    double xPoint = centerX + R * cos(t), yPoint = centerY + R * sin(t);

    glColor3f(1.0, 0.0, 0.0);
    Point p = Point(xPoint, yPoint);
    p.drawPoint();

    Line lr = Line(centerX, centerY, xPoint, yPoint);
    lr.drawLine();
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

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);

    drawCartesianPlace();
    drawTranslationBox();
    drawScaleBox();
    drawRotateBox();
    drawInactiveArea();

    glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
    if ( y < (1- NUMBERPRIMITIVES*0.3)*height ) primitive = INACTIVE;
    else if ( y < (1 - 2*0.3)*height ) primitive = ROTATE;
    else if ( y < (1 - 1*0.3)*height ) primitive = SCALE;
    else primitive = TRANSLATION;
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        y = height - y; // Correct from mouse to OpenGL co-ordinates.

        // Click outside canvas - do nothing.
        if ( x < 0 || x > width || y < 0 || y > height ) ;

            // Click in left selection area.
        else if ( x < SIDE_MENU_WIDTH*width ) {
            pickPrimitive(y);
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
            if (degrees == 360 && primitive == ROTATE) degrees = 0;
            if (degrees < 360 && primitive == ROTATE) degrees += 15;
            break;
        case GLUT_KEY_DOWN:
            if (translationY > -10 && primitive == TRANSLATION) translationY--;
            if (scaleY > 0 && primitive == SCALE) scaleY--;
            if (degrees == 0 && primitive == ROTATE) degrees = 360;
            if (degrees > 0 && primitive == ROTATE) degrees = degrees -= 15;
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

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Use the side menu to iteract with the square" << endl;
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
    glutMainLoop();

    return 0;
}