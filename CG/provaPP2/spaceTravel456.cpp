///////////////////////////////////////////////////////////////////////////////////
// spaceTravel.cpp
//
// This program draws a conical spacecraft that can travel and an array of
// fixed spherical asteroids. The view in the left viewport is from a fixed
// camera; the view in the right viewport is from the spacecraft.
// There is approximate collision detection.
//
// User-defined constants:
// ROW is the number of rows of  asteroids.
// COLUMN is the number of columns of asteroids.
// FILL_PROBABILITY is the percentage probability that a particular row-column slot
// will be filled with an asteroid.
//
// Interaction:
// Press the left/right arrow keys to turn the craft.
// Press the up/down arrow keys to move the craft.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

#define ROWS 8  // Number of rows of asteroids.
#define COLUMNS 5 // Number of columns of asteroids.
#define FILL_PROBABILITY 100 // Percentage probability that a particular row-column slot will be
#define GOLDEN_ASTEROID_NUMBER 20
// filled with an asteroid. It should be an integer between 0 and 100.

using namespace std;

// Globals.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static int width, height; // Size of the OpenGL window.
static float angle = 0.0; // Angle of the spacecraft.
static float xVal = 0, zVal = 0; // Co-ordinates of the spacecraft.
static int isCollision = 0; // Is there collision between the spacecraft and an asteroid?
static unsigned int spacecraft; // Display lists base index.
int variantGoldenColor = 0;
int maxGoldenColor = false;
int rowGolden = 0;
int columnGolden = 0;
int goldFind = 0;
bool animate = false;

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{
    char *c;

    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

int angles2[693] = {
0,
5,
10,
10,
15,
15,
20,
20,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
25,
20,
20,
20,
15,
10,
10,
10,
5,
5,
5,
0,
0,
355,
355,
355,
355,
355,
355,
355,
355,
355,
355,
355,
355,
355,
355,
355,
355,
355,
355,
355,
360,
360,
5,
5,
5,
10,
10,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
15,
20,
20,
25,
25,
30,
35,
35,
40,
40,
45,
45,
45,
50,
50,
55,
55,
60,
65,
65,
70,
70,
70,
75,
75,
70,
70,
70,
70,
65,
65,
60,
60,
55,
50,
50,
50,
50,
50,
50,
50,
50,
50,
45,
45,
45,
40,
35,
35,
35,
30,
25,
25,
25,
20,
20,
15,
15,
10,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
5,
0,
0,
355,
355,
350,
345,
345,
345,
340,
340,
340,
340,
340,
340,
340,
345,
345,
350,
350,
355,
355,
360,
360,
5,
5,
10,
10,
15,
15,
20,
20,
25,
25,
30,
30,
35,
35,
40,
40,
45,
45,
50,
50,
55,
55,
50,
50,
45,
45,
40,
40,
35,
35,
30,
25,
25,
25,
20,
15,
15,
10,
10,
5,
5,
5,
0,
355,
355,
355,
355,
355,
355,
355,
355,
355,
355,
350,
345,
345,
340,
340,
335,
335,
330,
330,
325,
325,
320,
320,
320,
315,
310,
310,
305,
305,
305,
300,
300,
295,
290,
290,
290,
285,
285,
290,
290,
290,
295,
295,
300,
305,
305,
305,
310,
305,
305,
300,
300,
295,
295,
290,
290,
285,
285,
280,
280,
280,
275,
275,
270,
270,
270,
270,
270,
270,
270,
270,
270,
270,
270,
270,
270,
270,
270,
270,
270,
270,
270,
270,
275,
280,
280,
280,
285,
290,
290,
290,
290,
290,
290,
285,
285,
280,
280,
280,
275,
270,
270,
265,
265,
260,
260,
260,
255,
255,
250,
250,
245,
245,
240,
235,
235,
230,
230,
230,
225,
225,
225,
225,
220,
215,
215,
210,
210,
210,
210,
210,
210,
210,
205,
205,
205,
210,
210,
215,
215,
220,
220,
220,
220,
220,
220,
220,
220,
220,
220,
220,
220,
220,
220,
220,
220,
220,
225,
225,
230,
230,
230,
235,
235,
240,
240,
245,
245,
250,
250,
250,
250,
250,
250,
245,
245,
240,
240,
235,
230,
230,
230,
225,
225,
220,
215,
215,
215,
210,
205,
205,
205,
200,
200,
195,
190,
190,
185,
185,
185,
180,
180,
175,
175,
170,
165,
165,
160,
160,
160,
155,
150,
150,
145,
145,
140,
140,
140,
135,
130,
130,
125,
125,
125,
125,
125,
125,
125,
125,
125,
125,
125,
125,
125,
125,
125,
130,
135,
135,
135,
135,
135,
135,
135,
135,
140,
145,
145,
145,
150,
150,
155,
155,
160,
160,
160,
160,
160,
160,
160,
155,
155,
155,
150,
150,
145,
145,
140,
140,
140,
140,
140,
140,
140,
135,
135,
135,
130,
125,
125,
125,
120,
115,
115,
115,
110,
110,
105,
105,
105,
105,
105,
105,
105,
105,
110,
110,
115,
120,
120,
120,
120,
120,
125,
125,
130,
130,
135,
135,
140,
140,
145,
145,
145,
150,
155,
155,
160,
160,
160,
160,
160,
160,
160,
160,
160,
165,
165,
170,
175,
175,
180,
180,
180,
185,
185,
190,
190,
190,
195,
200,
200,
205,
205,
210,
210,
210,
215,
220,
220,
220,
225,
230,
230,
230,
235,
235,
240,
245,
245,
245,
250,
250,
250,
250,
250,
250,
250,
250,
250,
250,
250,
250,
245,
245,
240,
240,
235,
235,
235,
230,
225,
225,
220,
220,
220,
215,
215,
210,
210,
210,
210,
210,
210,
210,
205,
205,
200,
200,
195,
195,
190,
185,
185,
185,
180,
180,
175,
175,
170,
170,
165,
165,
165,
165,
165,
165,
165,
165,
165,
165,
160,
160,
155,
155,
150,
145,
145,
145,
145,
140,
135,
135,
130,
130,
125,
125,
125,
120,
120,
115,
115,
110,
110,
105,
100,
100,
100,
95 };




// Asteroid class.
class Asteroid
{
public:
    Asteroid();
    Asteroid(float x, float y, float z, float r, unsigned char colorR,
             unsigned char colorG, unsigned char colorB, int golden, int objecT);
    float getCenterX() { return centerX; }
    float getCenterY() { return centerY; }
    float getCenterZ() { return centerZ; }
    float getRadius()  { return radius; }
    int isGolden() { return golden;}
    void draw();

private:
    float centerX, centerY, centerZ, radius;
    unsigned char color[3];
    int golden, object;
};

// Asteroid default constructor.
Asteroid::Asteroid()
{
    centerX = 0.0;
    centerY = 0.0;
    centerZ = 0.0;
    radius = 0.0; // Indicates no asteroid exists in the position.
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    golden = 0;
}

// Asteroid constructor.
Asteroid::Asteroid(float x, float y, float z, float r, unsigned char colorR,
                   unsigned char colorG, unsigned char colorB, int goldenX, int objecT)
{
    centerX = x;
    centerY = y;
    centerZ = z;
    radius = r;
    color[0] = colorR;
    color[1] = colorG;
    color[2] = colorB;
    golden = goldenX;
    object = objecT;
}

// Function to draw asteroid.
void Asteroid::draw()
{
    if (radius > 0.0) // If asteroid exists.
    {
        glPushMatrix();
        glTranslatef(centerX, centerY, centerZ);
        if (golden) {
            unsigned char goldenColor[3] = {255, 255, variantGoldenColor};
            glColor3ubv(goldenColor);
        } else{glColor3ubv(color);}
        if (golden) {
            glutWireSphere(radius, (int)radius*6, (int)radius*6);
        } else {
            switch (object) {
                case 0:
                    glutWireCube(radius);
                case 1:
                    glutWireCone(radius, 10, (int)radius*6, (int)radius*6);
                    break;
                case 2:
                    glutWireTorus(radius - 2, radius,  (int)radius*6, (int)radius*6);
                    break;
                case 3:
                    glutWireDodecahedron();
                    break;
                case 4:
                    glutWireOctahedron();
                    break;
                case 5:
                    glutWireTetrahedron();
                    break;
                case 6:
                    glutWireIcosahedron();
                    break;
                case 7:
                    glutWireTeapot(radius);
                    break;
                default:
                    glutWireSphere(radius, (int)radius*6, (int)radius*6);
            }
        }

        glPopMatrix();
    }
}

Asteroid arrayAsteroids[ROWS][COLUMNS]; // Global array of asteroids.

// Initialization routine.
void setup(void)
{
    int i, j, c = 0;

    spacecraft = glGenLists(1);
    glNewList(spacecraft, GL_COMPILE);
    glPushMatrix();
    glRotatef(180.0, 0.0, 1.0, 0.0); // To make the spacecraft point down the $z$-axis initially.
    glColor3f (1.0, 1.0, 1.0);
    glutWireCone(5.0, 10.0, 10, 10);

    glRotatef(-180.0, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 20.0, 0.0);
    glEnd();
    glLineWidth(1.0);

    glPopMatrix();
    glEndList();

    // Initialize global arrayAsteroids.
    for (j=0; j<COLUMNS; j++)
        for (i=0; i<ROWS; i++)
            if (rand()%100 < FILL_PROBABILITY)
                // If rand()%100 >= FILL_PROBABILITY the default constructor asteroid remains in the slot
                // which indicates that there is no asteroid there because the default's radius is 0.
            {
                int n = rand()%9;

                c += 1;
                // Position the asteroids depending on if there is an even or odd number of columns
                // so that the spacecraft faces the middle of the asteroid field.
                if (c == GOLDEN_ASTEROID_NUMBER) {
                    rowGolden = i;
                    columnGolden = j;
                    arrayAsteroids[i][j] = Asteroid( 30.0*(-COLUMNS/2 + j), 0.0, -40.0 - 30.0*i, 20.0,
                                                     255, 255, 0, 1, n);
                } else if (COLUMNS%2) // Odd number of columns.
                    arrayAsteroids[i][j] = Asteroid( 30.0*(-COLUMNS/2 + j), 0.0, -40.0 - 30.0*i, 3.0,
                                                     rand()%256, rand()%256, rand()%256, 0, n);
                else // Even number of columns.
                    arrayAsteroids[i][j] = Asteroid( 15 + 30.0*(-COLUMNS/2 + j), 0.0, -40.0 - 30.0*i, 3.0,
                                                     rand()%256, rand()%256, rand()%256, 0, n);
            }

    glEnable(GL_DEPTH_TEST);
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1,
                             float x2, float y2, float z2, float r2)
{
    return ( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) <= (r1+r2)*(r1+r2) );
}

// Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
int asteroidCraftCollision(float x, float z, float a)
{
    int i,j;

    // Check for collision with each asteroid.
    for (j=0; j<COLUMNS; j++)
        for (i=0; i<ROWS; i++)
            if (arrayAsteroids[i][j].getRadius() > 0 ) // If asteroid exists.
                if ( checkSpheresIntersection( x - 5 * sin( (PI/180.0) * a), 0.0,
                                               z - 5 * cos( (PI/180.0) * a), 7.072,
                                               arrayAsteroids[i][j].getCenterX(), arrayAsteroids[i][j].getCenterY(),
                                               arrayAsteroids[i][j].getCenterZ(), arrayAsteroids[i][j].getRadius() ) ) {
                    if (i == rowGolden && j == columnGolden) {
                        goldFind = 1;
                    }
                    return 1;
                }
    goldFind = 0;
    return 0;
}

void glowAsteroid( int value ) {
    if (maxGoldenColor) variantGoldenColor -= 1;
    if (!maxGoldenColor) variantGoldenColor += 1;

    if (variantGoldenColor == 250) maxGoldenColor = true;
    if (variantGoldenColor == 0) maxGoldenColor = false;

    glutTimerFunc(1, glowAsteroid, 0);
    glutPostRedisplay();
}
int i = 0;
void animateScene() {
    if (animate && i < 693) {

        angle = angles2[i];
        xVal = xVal - sin(angle * PI/180.0);
        zVal = zVal - cos(angle * PI/180.0);

        i++;
        glutPostRedisplay();
    }
}

// Drawing routine.
void drawScene(void)
{
    int i, j;
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Begin left viewport.
    glViewport (0, 0, width/2.0,  height);
    glLoadIdentity();

    // Write text in isolated (i.e., before gluLookAt) translate block.
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(-28.0, 25.0, -30.0);
    if (isCollision) {
        if ( goldFind )
            writeBitmapString((void*)font,"You have found gold!");
        else
            writeBitmapString((void*)font, "Cannot - will crash!");
    }
    glPopMatrix();

    // Locate the camera at the tip of the cone and pointing in the direction of the cone.
    gluLookAt(xVal - 10 * sin( (PI/180.0) * angle),
              0.0,
              zVal - 10 * cos( (PI/180.0) * angle),
              xVal - 11 * sin( (PI/180.0) * angle),
              0.0,
              zVal - 11 * cos( (PI/180.0) * angle),
              0.0,
              1.0,
              0.0);

    // Draw all the asteroids in arrayAsteroids.
    for (j=0; j<COLUMNS; j++)
        for (i=0; i<ROWS; i++)
            arrayAsteroids[i][j].draw();

    // Begin right viewport.
    glViewport(width/2.0, 0, width/2.0, height);
    glLoadIdentity();

    // Write text in isolated (i.e., before gluLookAt) translate block.
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(-28.0, 25.0, -30.0);
    if (isCollision) {
        if (goldFind)
            writeBitmapString((void *) font, "You have found gold!");
        else
            writeBitmapString((void *) font, "Cannot - will crash!");
    }
    glPopMatrix();

    // Draw a vertical line on the left of the viewport to separate the two viewports
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(-5.0, -5.0, -5.0);
    glVertex3f(-5.0, 5.0, -5.0);
    glEnd();
    glLineWidth(1.0);

    // Fixed camera.
    gluLookAt(
            arrayAsteroids[rowGolden][columnGolden].getCenterX(),
            arrayAsteroids[rowGolden][columnGolden].getCenterY() + 40,
            arrayAsteroids[rowGolden][columnGolden].getCenterZ(),
            xVal,
            0.0,
            zVal,
            0.0,
            1.0,
            0.0
    );

    // Draw all the asteroids in arrayAsteroids.
    for (j=0; j<COLUMNS; j++)
        for (i=0; i<ROWS; i++)
            arrayAsteroids[i][j].draw();
    // End right viewport.

    // Draw spacecraft.
    glPushMatrix();
    glTranslatef(xVal, 0.0, zVal);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glCallList(spacecraft);
    glPopMatrix();
    // End left viewport.

    glutSwapBuffers();
    animateScene();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
    glMatrixMode(GL_MODELVIEW);

    // Pass the size of the OpenGL window.
    width = w;
    height = h;
}



// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case ' ':
            animate = !animate;
        default:
            break;
    }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
    float tempxVal = xVal, tempzVal = zVal, tempAngle = angle;

    // Compute next position.
    if (key == GLUT_KEY_LEFT) tempAngle = angle + 5.0;
    if (key == GLUT_KEY_RIGHT) tempAngle = angle - 5.0;
    if( key == GLUT_KEY_UP)
    {
        tempxVal = xVal - sin(angle * PI/180.0);
        tempzVal = zVal - cos(angle * PI/180.0);
    }
    if( key == GLUT_KEY_DOWN)
    {
        tempxVal = xVal + sin(angle * PI/180.0);
        tempzVal = zVal + cos(angle * PI/180.0);
    }

    // Angle correction.
    if (tempAngle > 360.0) tempAngle -= 360.0;
    if (tempAngle < 0.0) tempAngle += 360.0;

    // Move spacecraft to next position only if there will not be collision with an asteroid.
    if (!asteroidCraftCollision(tempxVal, tempzVal, tempAngle) )
    {
        isCollision = 0;
        xVal = tempxVal;
        zVal = tempzVal;
        angle = tempAngle;
    }
    else isCollision = 1;
    cout << angle << endl;
    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Press the left/right arrow keys to turn the craft." << endl
         << "Press the up/down arrow keys to move the craft." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
    srand((unsigned)time(0));
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("spaceTravel.cpp");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);
    glutTimerFunc(10, glowAsteroid, 0);
    glutMainLoop();

    return 0;
}

