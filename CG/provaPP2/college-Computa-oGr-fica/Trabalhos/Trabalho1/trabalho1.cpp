/*
 *
 * Compilador e Biblioteca: gcc, OpenGl/GLUT
 * Comando para compilação: g++ -o trabalho1  trabalho1.cpp -lglut -lGL -lGLU -lm
 *
 *
 * Tabalho 1 de Computação grafica 3º Ano - Ciência da computação UEMS
 * Nome: Fernando Lima
*/




/// Inclusão de libs

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/// Inclusão de defineds
#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define POLY_LINE 4
#define CIRCLE 5
#define HEXAGON 6
#define PI 3.14159265358979324
#define NUMBER_PRIMITIVES 6


/// Variaveis globais

// Use the STL extension of C++.
using namespace std;

// Estruturas que contera as informações dos pontos da tela
typedef struct {
    float x;
    float y;
}Pnt;

Pnt currentPoint; // Estrutura que iram conter as coordenadas em tempo real do ponto
Pnt center; // Estrutura que ira armazenar as coordenadas do primeiro ponto clicado na tela


int drawingSize = 9; // Tamanho original do grid, 9 quadrados espacados igualmente na tela
float colorVector[4] = { 0.0 }; // Vetor para quadar a cor atual do desenho, cada posiçao do vetor contem o valor RGB respectivo
int nVertices = 100; // Numero de vertices do circulo
int nVerticesHexagon = 5; // NUmero de vertices do hexagono

static int TYPE = GL_LINE; // Variavel para controle do filled e outline
static GLsizei width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int isGrid = 1; // Is there grid?
static int stippleID = 0;


/// Definindo as Classes

// Point class.
class Point {
public:
    Point(int xVal, int yVal) {
        x = xVal;
        y = yVal;
    }

    void drawPoint(void); // Function to draw a point.

private:
    int x, y; // x and y co-ordinates of point.
    static float size; // Size of point.
};

// Line class.
class Line {
public:
    Line(int x1Val, int y1Val, int x2Val, int y2Val) {
        x1 = x1Val;
        y1 = y1Val;
        x2 = x2Val;
        y2 = y2Val;
    }

    void drawLine();

private:
    int x1, y1, x2, y2; // x and y co-ordinates of endpoints.
};

class Rectangle {
public:
    Rectangle(int x1Val, int y1Val, int x2Val, int y2Val) {
        x1 = x1Val;
        y1 = y1Val;
        x2 = x2Val;
        y2 = y2Val;
    }
    void drawRectangle();

private:
    int x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices.
};

// class polyline.
class PolyLine {
public:
    PolyLine(int x1Val, int y1Val, int x2Val, int y2Val) {
        x1 = x1Val;
        y1 = y1Val;
        x2 = x2Val;
        y2 = y2Val;
    }

    void drawPolyLine();


public:
    int x1, y1, x2, y2; // x and y co-ordinates of endpoints.
};

// class polyline.
class Circle {
public:
    Circle(int x1Val, int y1Val, int x2Val, int y2Val) {
        x1 = x1Val;
        y1= y1Val;
        x2= x2Val;
        y2= y2Val;

    }

    void drawCircle();

private:
    int x1, y1, x2, y2; // x and y co-ordinates of endpoints.
};

// class polyline.
class Hexagon {
public:
    Hexagon(int x1Val, int y1Val, int x2Val, int y2Val) {
        x1 = x1Val;
        y1= y1Val;
        x2= x2Val;
        y2= y2Val;

    }

    void drawHexagon();

private:
    int x1, y1, x2, y2; // x and y co-ordinates of endpoints.
};


void applyStipple(void) {
    // Defina o padrão de stipple
    glRasterPos3f(30.0, 40.0, 0.0);

    stippleID == 0 ? glDisable(GL_LINE_STIPPLE) : glEnable(GL_LINE_STIPPLE);
    // Caso stippleId seja igual a zero, a line stipple sera desativada, caso contrario é ativado;

    // A Variavel stippleID é global e alterada no menu, dependendo do seu valor, ela entrara em cases diferentes e
    // mudara o stipple da linha, o case 4 é para desativar o a line stipple
    switch (stippleID) {
        case 1:
            glLineStipple(1, 0x0101);
            break;

        case 2:
            glLineStipple(5, 0x5555);
            break;

        case 3:
            glLineStipple(1, 0x00FF);
            break;

        case 4:
            glDisable(GL_LINE_STIPPLE);
            break;
    }
}
void mousePassiveMotion(int x, int y) {
    // Essa funçao server para atualizar em tempo
    // real as coordenadas x e y da estrutura Ptn,
    // colocando em x e y o valor corrente

    if (pointCount) {
        currentPoint.x = x;
        currentPoint.y = height - y;
    }

    glutPostRedisplay();
}



/// Funções de classes
// Função da classe que Desenha o ponto
void Point::drawPoint() {
    applyStipple();
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex3f(x, y, 0.0);
    glEnd();
}

// Função da classe que Desenha a linha
void Line::drawLine() {
    applyStipple();
    glColor3f(colorVector[0], colorVector[1], colorVector[2]); // Aplica as cores RGB, todas contidas nas posições do vetor

    glBegin(GL_LINES);
    glVertex3f(x1, y1, 0.0);
    glVertex3f(x2, y2, 0.0);
    glEnd();
}

// Function to draw a rectangle.
void Rectangle::drawRectangle() {
    applyStipple();
    glColor3f(colorVector[0], colorVector[1], colorVector[2]);

    glPolygonMode(GL_FRONT_AND_BACK, TYPE);
    glRectf(x1, y1, x2, y2);
}

// Function to draw lines continua.
void PolyLine::drawPolyLine() {
    applyStipple();
    glColor3f(colorVector[0], colorVector[1], colorVector[2]);

    glBegin(GL_LINES);
    glVertex3f(x1, y1, 0.0);
    glVertex3f(x2, y2, 0.0);
    glEnd();
}

// Function to draw circle.
void Circle::drawCircle() {

    applyStipple();

    float t = 0.0; // Angle parameter.
    float R = x1-x2;
    /// Ira calcular o RAIO do circle, subtrai de x1 o x2, assim encontrando o raio do circle

    glPolygonMode(GL_FRONT_AND_BACK, TYPE);


    /* Aqui o circle será desenhado de fato, nVerticesHexagon é o numero de vertices que o circle tera,
       esta definido globalmente com valor padrão de 5 vertices, a formula:
       (x1 + R * cos(t)) e (y1 + R * sin(t)), é uma formulas matematicas para a criação
       de um circle e as coordenadas x, y respectivamente
     */

    TYPE == GL_FILL?  glBegin(GL_POLYGON) : glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 100; ++i) {
        glColor3f(colorVector[0], colorVector[1], colorVector[2]);
        glVertex3f( x1 + R * cos(t),   y1 + R * sin(t), 0.0);
        t += 2 * PI / 100;
    }
    glEnd();
}

// Function to draw hexagon
void Hexagon::drawHexagon() {
    applyStipple();

    float t = 0.0; // Angle parameter.
    float R = x1-x2;
    /// Ira calcular o RAIO do hexagono, subtrai de x1 o x2, assim encontrando o raio do hexagono

    glPolygonMode(GL_FRONT_AND_BACK, TYPE);


    /* Aqui o hexagono será desenhado de fato, nVerticesHexagon é o numero de vertices que o hexagono tera,
       esta definido globalmente com valor padrão de 5 vertices, a formula:
       (x1 + R * cos(t)) e (y1 + R * sin(t)), é uma formulas matematicas para a criação
       de um hexagono e as coordenadas x, y respectivamente
     */

    glBegin(GL_POLYGON);
    for(int i = 0; i < nVerticesHexagon; ++i) {
        glColor3f(colorVector[0], colorVector[1], colorVector[2]);
        glVertex3f( x1 + R * cos(t),   y1 + R * sin(t), 0.0);
        t += 2 * PI / nVerticesHexagon;
    }
    glEnd();
}

/// Interators, vectors,
float Point::size = pointSize; // Set point size.


/// Iterators
vector<Point>::iterator pointsIterator; // Iterator to traverse a Point array.
vector<Line>::iterator linesIterator; // Iterator to traverse a Line array.
vector<Rectangle>::iterator rectanglesIterator; // Iterator to traverse a Rectangle array.
vector<PolyLine>::iterator polylinesIterator; // Iterator to traverse a Line array.
vector<Hexagon>::iterator hexagonsIterator; // Iterator to traverse a Line array.
vector<Circle>::iterator circleIterator; // Iterator to traverse a Line array.


/// Vectors
vector<Point> points; // Vector of points.
vector<Line> lines; // Vector of lines.
vector<Rectangle> rectangles; // Vector of rectangles.
vector<PolyLine> polylines; // Vector of lines.
vector<Circle> circles;// Vector of lines.
vector<Hexagon> hexagons; // Vector of lines.


/// Funções para chamada da função da classe, função que intera no vetor

// Function to draw all points in the points array.
void drawPoints(void) {
    // Loop through the points array drawing each point.
    pointsIterator = points.begin();
    while(pointsIterator != points.end()) {
        pointsIterator->drawPoint();
        pointsIterator++;
    }
}

// Function to draw all lines in the lines array.
void drawLines(void) {
    // Loop through the lines array drawing each line.
    linesIterator = lines.begin();

    while(linesIterator != lines.end()) {
        linesIterator->drawLine();
        linesIterator++;
    }
}

// Função para desehar todos os retangulos no vetor
void drawRectangles(void) {
    rectanglesIterator = rectangles.begin();

    while(rectanglesIterator != rectangles.end()) {
        rectanglesIterator->drawRectangle();
        rectanglesIterator++;
    }
}

// Função para desehar todas as poli linhas no vetor
void drawPolyLines(void) {
    // Loop through the lines array drawing each line.
    polylinesIterator = polylines.begin();

    while(polylinesIterator != polylines.end()) {
        polylinesIterator->drawPolyLine();
        polylinesIterator++;
    }
}

// Função para desehar todos os circulos no vetor
void drawCircles(void) {
    // Loop through the lines array drawing each line.
    circleIterator = circles.begin();

    while(circleIterator  != circles.end()) {
        circleIterator->drawCircle();
        circleIterator++;
    }
}


// Função para desehar todos os hexagonos no vetor
void drawHexagons(void) {
    // Loop through the lines array drawing each line.
    hexagonsIterator = hexagons.begin();

    while(hexagonsIterator  != hexagons.end()) {
        hexagonsIterator->drawHexagon();
        hexagonsIterator++;
    }
}



// Function to draw point selection box in left selection area.
void drawPointSelectionBox(void) {
    glDisable(GL_LINE_STIPPLE);
    /// Independente se a linha stipple foi ativada, o menu não é para ser ativado também
    /// Então desativamos

    /// Se primitiva for igual ponto, a cor do quadrado do ponto é alterada, o Highlight
    primitive == POINT ? glColor3f(1.0, 1.0, 1.0) : glColor3f(0.8, 0.8, 0.8);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.9 * height, 0.1 * width, height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.9*height, 0.1*width, height);

    // Draw point icon.
    glPointSize(pointSize);
    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_POINTS);
    glVertex3f(0.05 * width, 0.95 * height, 0.0);
    glEnd();
}

// Function to draw line selection box in left selection area.
void drawLineSelectionBox(void) {
    glDisable(GL_LINE_STIPPLE);

    primitive == LINE ? glColor3f(1.0, 1.0, 1.0) : glColor3f(0.8, 0.8, 0.8);


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.8 * height, 0.1 * width, 0.9 * height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.8 * height, 0.1 * width, 0.9 * height);

    // Draw line icon.
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.025 * width, 0.825 * height, 0.0);
    glVertex3f(0.075 * width, 0.875 * height, 0.0);
    glEnd();
}

// Function to draw rectangle selection box in left selection area.
void drawRectangleSelectionBox(void) {
    glDisable(GL_LINE_STIPPLE);

    primitive == RECTANGLE ? glColor3f(1.0, 1.0, 1.0) : glColor3f(0.8, 0.8, 0.8);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);


    // Draw rectangle icon.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.025 * width, 0.735 * height, 0.075 * width, 0.765 * height);
    glEnd();
}

// Function to draw polyLine selection box in left selection area.
void drawPolyLineSelectionBox(void) {
    glDisable(GL_LINE_STIPPLE);

    primitive == POLY_LINE ? glColor3f(1.0, 1.0, 1.0) : glColor3f(0.8, 0.8, 0.8);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.6 * height, 0.1 * width, 0.7*height);

    // Draw black boundary.
    glColor3f(0.0,  0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.6 * height, 0.1 * width, 0.7 * height);

    // Desenha polyLine
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.025 * width, 0.625 * height, 0.0);
    glVertex3f(0.045 * width, 0.675 * height, 0.0);

    glVertex3f(0.045 * width, 0.675 * height, 0.0);
    glVertex3f(0.065 * width, 0.630 * height, 0.0);

    glVertex3f(0.065 * width, 0.630 * height, 0.0);
    glVertex3f(0.085 * width, 0.665 * height, 0.0);
    glEnd();
}

// Function to draw circle selection box in left selection area.
void drawCircleSelectionBox(void) {
    glDisable(GL_LINE_STIPPLE);

    float t = 0.0;

    primitive == CIRCLE ? glColor3f(1.0, 1.0, 1.0) : glColor3f(0.8, 0.8, 0.8);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.5 * height, 0.1 * width, 0.6*height);

    // Draw black boundary.
    glColor3f(0.0,  0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.5 * height, 0.1 * width, 0.6 * height);


    /// Vai desenhar o icon no quadrado especifico dele, a logica é a mesma da função drawCircle
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < nVertices; ++i) {
        glColor3ub(0.0, 0.0, 0.0);
        glVertex3f( (0.050 * width) + 15 * cos(t), (0.550 * height) + 15 * sin(t), 0.0);
        t += 2 * PI / nVertices;
    }
    glEnd();

}

// Function to draw hexagon selection box in left selection area.
void drawHexagonSelectionBox(void) {

    float t = 0.0;
    primitive == HEXAGON ? glColor3f(1.0, 1.0, 1.0) : glColor3f(0.8, 0.8, 0.8);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.4 * height, 0.1 * width, 0.5*height);

    // Draw black boundary.
    glColor3f(0.0,  0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.4 * height, 0.1 * width, 0.5 * height);

    /// Vai desenhar o icon no quadrado especifico dele, a logica é a mesma da função drawHexagon
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < nVerticesHexagon; ++i) {
        glColor3ub(0.0, 0.0, 0.0);
        glVertex3f( (0.040 * width) + 15 * cos(t), (0.450 * height) + 15 * sin(t), 0.0);
        t += 2 * PI / nVerticesHexagon;
    }
    glEnd();
}

// Function to draw unused part of left selection area.
void drawInactiveArea(void) {

    glColor3f(0.6, 0.6, 0.6);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.0, 0.1 * width, (1 - NUMBER_PRIMITIVES*0.1) * height);

    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.0, 0.1 * width, (1 - NUMBER_PRIMITIVES*0.1) * height);
}

// Function to draw temporary point.
void drawTempPoint(void) {
    glColor3f(1.0, 0.0, 0.0);

    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex3f(center.x, center.y, 0.0);
    glEnd();
}

// Function to draw a grid.
void drawGrid(void) {
    float sum = 0.0;
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x5555); /// Stipple default do grid

    glColor3f(0.75, 0.75, 0.75); /// Cor default do grid

    if(drawingSize == 3) {
        sum = 200.0;
        glBegin(GL_LINES);
        for (int i = 2; i <= 3; i++) {
            glVertex3f(sum, 0.0, 0.0);
            glVertex3f(sum, height, 0.0);
            sum += 150.0;
        }

        for (int i = 1; i <= 4; i++) {
            glVertex3f(0.1 * width, i * 0.2 * height, 0.0);
            glVertex3f(width, i * 0.2 * height, 0.0);
        }
        glEnd();
    }

    if(drawingSize == 9) {
        glBegin(GL_LINES);
        for (int i = 2; i <= 9; i++) {
            glVertex3f( ((i * 0.1 * width) ) , 0.0, 0.0);
            glVertex3f( ((i * 0.1 * width)), height, 0.0);
        }

        for (int i = 1; i <= 9; i++) {
            glVertex3f(0.1 * width, i * 0.1 * height, 0.0);
            glVertex3f(width, i * 0.1 * height, 0.0);
        }
        glEnd();
    }

    if(drawingSize == 6) {
        sum = 125.0;
        for(int i = 0; i < 6; ++i) {
            glBegin(GL_LINES);
            glVertex3f( sum, 0.0, 0.0);
            glVertex3f( sum, height, 0.0);
            sum += 75.0;
        }

        for (int i = 1; i <= 6; i++) {
            glVertex3f((0.1 * width), i * 0.2 * height, 0.0);
            glVertex3f(width, i * 0.2 * height, 0.0);
        }
        glEnd();
    }


    glDisable(GL_LINE_STIPPLE);
}

void writeBitmapString(void *font, char *string) {
    char *c;
    for(c = string; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);
}

/// Função desenha as figuras geometricas dinamicamente
void drawFigures(int primitive) {
    float
            x1 = center.x,
            y1 = center.y,
            x2 = currentPoint.x,
            y2 = currentPoint.y;

    PolyLine *dynamicPolyLine = new PolyLine(x1, y1, x2, y2);
    Circle *dynamicCircle = new Circle(x1, y1, x2, y2);
    Line *dynamicLine= new Line(x1, y1, x2, y2);
    Rectangle *dynamicRectangle = new Rectangle(x1, y1, x2, y2);
    Hexagon *dynamicHexagon = new Hexagon(x1, y1, x2, y2);

    switch (primitive) {
        case  CIRCLE:
            dynamicCircle->drawCircle();
            break;

        case LINE:
            dynamicLine->drawLine();
            break;

        case POLY_LINE:
            dynamicPolyLine->drawPolyLine();
            break;

        case RECTANGLE:
            dynamicRectangle->drawRectangle();
            break;

        case HEXAGON:
            dynamicHexagon->drawHexagon();
            break;

        default:
            break;
    }
}

/// Escreve na tela o texto de qual figura está selecioada, o parametro texto é passado
void drawText(char *text) {
    glColor3f(0.0, 0.0, 15.0);
    glRasterPos3f(80.0, 55.0, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, text);
}

/// Função que chama a função para desenhar o texto de acordo com qual primitiva está clidada no presente momento
void drawFonts(void) {
    switch (primitive) {
        case CIRCLE:
            drawText((char*)"CIRCLE");
            break;

        case POLY_LINE:
            drawText((char*)"POLYLINE");
            break;

        case LINE:
            drawText((char*)"LINE");
            break;

        case RECTANGLE:
            drawText((char*)"RECTANGLE");
            break;

        case POINT:
            drawText((char*)"POINT");
            break;

        case HEXAGON:
            drawText((char*)"HEXAGON");
            break;
    }
}

// Drawing routine.
void drawScene(void) {

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);



    if(pointCount != 0) {
        drawFigures(primitive);
    }

    drawRectangles();
    drawPolyLines();
    drawHexagons();
    drawPoints();
    drawLines();
    drawCircles();


    if (( (primitive == LINE) || (primitive == RECTANGLE) || (primitive == CIRCLE)  || (primitive == HEXAGON) )
        && (pointCount == 1)
    ) drawTempPoint();


    if (isGrid) {
        drawGrid();
    }

    drawPointSelectionBox();
    drawLineSelectionBox();
    drawRectangleSelectionBox();
    drawPolyLineSelectionBox();
    drawCircleSelectionBox();
    drawHexagonSelectionBox();

    drawInactiveArea();

    drawFonts();
    glutSwapBuffers();
}


// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y) {
    if ( y < (1 - NUMBER_PRIMITIVES*0.1)*height) primitive = INACTIVE;
    else if ( y < (1 - 5*0.1)*height ) primitive = HEXAGON;
    else if ( y < (1 - 4*0.1)*height ) primitive = CIRCLE;
    else if ( y < (1 - 3*0.1)*height ) primitive = POLY_LINE;
    else if ( y < (1 - 2*0.1)*height ) primitive = RECTANGLE;
    else if ( y < (1 - 1*0.1)*height ) primitive = LINE;

    else primitive = POINT;
}

void resetAndSetCoordinates(float x, float y) {
    currentPoint.x = x;
    currentPoint.y = y;
    center.x = x;
    center.y = y;
    pointCount = 1;
}

void mouseControl(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        y = height - y;  // Correct from mouse to OpenGL co-ordinates.

        // Click outside canvas - do nothing.
        if (x < 0 || x > width || y < 0 || y > height) ;

            // Click in left selection area.
        else if ( x < 0.1*width ) {
            pickPrimitive(y);
            pointCount = 0;
        }

            // Click in canvas.
        else {

            if (primitive == POINT) {
                points.push_back(Point(x,y));
            }

            else if (primitive == LINE) {
                if (pointCount == 0) {
                    resetAndSetCoordinates(x, y);
                } else {
                    lines.push_back( Line(center.x, center.y, x, y) );
                    pointCount = 0;
                }

            }

            else if (primitive == RECTANGLE) {
                if (pointCount == 0) {
                    resetAndSetCoordinates(x, y);
                } else {
                    rectangles.push_back( Rectangle(center.x, center.y, x, y) );
                    pointCount = 0;
                }
            }


            else if(primitive == POLY_LINE) {
                if (pointCount == 0) {
                    resetAndSetCoordinates(x, y);
                } else {

                    polylines.push_back( PolyLine(center.x, center.y, x, y));
                    resetAndSetCoordinates(x, y);

                }
            }

            else if(primitive == CIRCLE) {
                if (pointCount == 0){
                    resetAndSetCoordinates(x, y);
                } else {
                    circles.push_back( Circle(center.x, center.y, x, y) );
                    pointCount = 0;
                }
            }

            else if(primitive == HEXAGON) {
                if (pointCount  == 0) {
                    resetAndSetCoordinates(x, y);

                } else {
                    hexagons.push_back( Hexagon(center.x, center.y, x, y) );
                    pointCount = 0;
                }
            }

        }

    }

    if((button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)) pointCount = 0;
    glutPostRedisplay();
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

    // Set viewing box dimensions equal to window dimensions.
    glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

    // Pass the size of the OpenGL window to globals.
    width = w;
    height = h;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {

    switch (key) {
        case 27:
            exit(0);
            break;

        default:
            break;
    }
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void) {
    points.clear();
    lines.clear();
    rectangles.clear();
    polylines.clear();
    circles.clear();
    hexagons.clear();

    primitive = INACTIVE;
    pointCount = 0;
}

// The right button menu callback function.
void rightMenu(int id) {

    if (id == 1) {
        clearAll();
        glutPostRedisplay();
    }

    if (id==2)
        exit(0);
}

// The sub-menu callback function.
void grid_menu(int id) {

    if(id == 3)
        isGrid = 1;

    if(id == 4)
        isGrid = 0;

    glutPostRedisplay();
}
// Controla o line stipple
void keyStrippers(int key) {
    switch(key) {
        case 1:
            stippleID = 1;
            break;

        case 2:
            stippleID = 2;
            break;

        case 3:
            stippleID = 3;
            break;

        case 4:
            stippleID = 0;
            break;

        default:
            break;
    }
}

// Controla as cores das figuras
void add_colors(int id) {

    switch (id) {
        case 1:
            colorVector[0] = 255.0;
            colorVector[1] = 0.00;
            colorVector[2] = 0.00;
            break;

        case 2:
            colorVector[0] = 0.0;
            colorVector[1] = 0.0;
            colorVector[2] = 204.0;
            break;

        case 3:
            colorVector[0] = 255.0;
            colorVector[1] = 0.0;
            colorVector[2] = 255.0;
            break;

        case 4:
            colorVector[0] = 0.0;
            colorVector[1] = 51.0;
            colorVector[2] = 0.0;
            break;

        case 5:
            colorVector[0] = 0.0;
            colorVector[1] = 0.0;
            colorVector[2] = 0.0;
            break;
    }

    glutPostRedisplay();
};

// Controla o modo das figuras, filled ou outline
void mode_figures(int id) {
    if(id == 1)
        TYPE = GL_LINE;

    if(id == 2)
        TYPE = GL_FILL;

    glutPostRedisplay();
}

// Altera o  tamanho do grid
void sizeGrid(int id) {
    if(id == 1) {
        drawingSize = 3;
    }

    if(id == 2) {
        drawingSize = 9;
    }

    if(id == 3) {
        drawingSize = 6;
    }

    glutPostRedisplay();
}

// Function to create menu.
void makeMenu(void) {
    int sub_menu, options_add_colors, options_mode, options_size_grid, options_strippers_line;

    options_strippers_line = glutCreateMenu(keyStrippers);
    glutAddMenuEntry("Stripper -  0x0001", 1);
    glutAddMenuEntry("Stripper -  0x5555", 2);
    glutAddMenuEntry("Stripper -  0x00FF", 3);
    glutAddMenuEntry("Stripper -  Off", 4);


    options_size_grid = glutCreateMenu(sizeGrid);
    glutAddMenuEntry("Size 1", 1);
    glutAddMenuEntry("Size 2", 2);
    glutAddMenuEntry("Size 2", 3);


    options_mode = glutCreateMenu(mode_figures);
    glutAddMenuEntry("outLine", 1);
    glutAddMenuEntry("Filled", 2);

    options_add_colors = glutCreateMenu(add_colors);
    glutAddMenuEntry("Red",   1);
    glutAddMenuEntry("Blue",  2);
    glutAddMenuEntry("Pink",  3);
    glutAddMenuEntry("Green", 4);
    glutAddMenuEntry("Black", 5);


    sub_menu = glutCreateMenu(grid_menu);
    glutAddMenuEntry("On", 3);
    glutAddMenuEntry("Off", 4);

    glutCreateMenu(rightMenu);
    glutAddSubMenu("Grid", sub_menu);
    glutAddSubMenu("Colors", options_add_colors);
    glutAddSubMenu("Mode", options_mode);
    glutAddSubMenu("Size Grid", options_size_grid);
    glutAddSubMenu("Line Stripper", options_strippers_line);
    glutAddMenuEntry("Clear", 1);
    glutAddMenuEntry("Quit", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void) {
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
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);

    glutInitWindowPosition(100, 100);
    glutCreateWindow("canvas.cpp");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    // Register the mouse callback function.
    glutMouseFunc(mouseControl);

    // Register the mouse passive motion callback function.
    glutPassiveMotionFunc(mousePassiveMotion);

    makeMenu(); // Create menu.
    glutMainLoop();

    return 0;
}