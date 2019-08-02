/*
 *
 * Compilador e Biblioteca: gcc, OpenGl/GLUT
 * Comando para compilação: g++ -o trabalho2  trabalho2.cpp -lglut -lGL -lGLU -lm
 *
 *
 * Tabalho 2 de Computação grafica 3º Ano - Ciência da computação UEMS
 * Nome: Fernando Lima
*/




/// Inclusão de libs

#include <sstream>
#include <string.h>



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

#define CONCEITO_VECTOR 1
#define ANGULO_VECTOR 2
#define SUM_VECTOR 3
#define SCALAR_PRODUCT 4
#define VECTOR_PRODUCT 5
#define PI 3.14159265358979324
#define NUMBER_PRIMITIVES 5


/// Variaveis globais

// Use the STL extension of C++.
using namespace std;

// Estruturas que contera as informações dos pontos da tela
typedef struct {
    float x;
    float y;
}Pnt;

int quadrant = 1, posQuadrant  = 0;
float wi = 0.0, hei = 0.0;

typedef struct  {
    float x1 = 500, x2, y1 = 250, y2;
}PntTemp;

PntTemp vectorOne, vectorTwo;

Pnt currentPoint; // Estrutura que iram conter as coordenadas em tempo real do ponto
Pnt center; // Estrutura que ira armazenar as coordenadas do primeiro ponto clicado na tela


int drawingSize = 9; // Tamanho original do grid, 9 quadrados espacados igualmente na tela
float colorVector[4] = { 0.0 }; // Vetor para quadar a cor atual do desenho, cada posiçao do vetor contem o valor RGB respectivo
int nVertices = 100; // Numero de vertices do circulo
int isConceitoVector = 1;
int Constant = 0;
int scalar = 0;

static int TYPE = GL_LINE; // Variavel para controle do filled e outline
static GLsizei width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int isGrid = 1; // Is there grid?
static int stippleID = 0;


/// Definindo as Classes

// Line class.
class ConceitoVector {
public:
    ConceitoVector(int x1Val, int y1Val, int x2Val, int y2Val) {
        x1 = x1Val;
        y1 = y1Val;
        x2 = x2Val;
        y2 = y2Val;
    }

    void drawConceitoVector();

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

class AnguloVector {
    public:
        AnguloVector(int x1Val, int y1Val, int x2Val, int y2Val) {
            x1 = x1Val;
            y1 = y1Val;
            x2 = x2Val;
            y2 = y2Val;
        }

        void drawAnguloVector();


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

char * itoa (int value, char *result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
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
// Função da classe que Desenha a linha
void ConceitoVector:: drawConceitoVector() {
    glColor3f(colorVector[0], colorVector[1], colorVector[2]); // Aplica as cores RGB, todas contidas nas posições do vetor

    glBegin(GL_LINES);
    glVertex3f(x1, y1, 0.0);
    glVertex3f(x2, y2, 0.0);
    glEnd();
}

// Function to draw a rectangle.
void Rectangle::drawRectangle() {
    glColor3f(colorVector[0], colorVector[1], colorVector[2]);

    glPolygonMode(GL_FRONT_AND_BACK, TYPE);
    glRectf(x1, y1, x2, y2);
}

// Function to draw lines continua.
void AnguloVector::drawAnguloVector() {
    glColor3f(colorVector[0], colorVector[1], colorVector[2]);

    glBegin(GL_LINES);
        glVertex3f(x1, y1, 0.0);
        glVertex3f(x2, y2, 0.0);
    glEnd();
}

// Function to draw circle.
void Circle::drawCircle() {

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(x1, y1 , 0.0);
    glVertex3f(x2 , y2, 0.0);
    glEnd();


    if(x2 > 500 && y2 > 250) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(x2 - 20 , y2 + 20, 0.0);
            glVertex3f(x2  + 20, y2 - 20, 0.0);
            glVertex3f( x2+15, y2 + 15, 0.0);
        glEnd();
    }



    if(x2 < 500 && y2 > 250) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(x2 - 20 , y2 + 20, 0.0);
            glVertex3f(x2  - 20, y2 - 20, 0.0);
            glVertex3f( x2+15, y2 + 15, 0.0);
        glEnd();
    }

    if(x2 < 500 && y2 > 250) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x2 - 20 , y2 + 20, 0.0);
        glVertex3f(x2  - 20, y2 - 20, 0.0);
        glVertex3f( x2+15, y2 + 15, 0.0);
        glEnd();
    }

    if(x2 < 500 && y2 < 250) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(x2 - 20, y2 + 20, 0.0);
            glVertex3f(x2 - 20, y2 - 20, 0.0);
            glVertex3f( x2+15, y2 - 20, 0.0);
        glEnd();
    }

    if(x2 > 500 && y2 < 250) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x2 + 20, y2 + 20, 0.0);
        glVertex3f(x2 - 20, y2 - 20, 0.0);
        glVertex3f( x2+15, y2 - 20, 0.0);
        glEnd();
    }
}


/// Iterators

vector<ConceitoVector>::iterator conceitoVectorIterator; // Iterator to traverse a Line array.
vector<Rectangle>::iterator rectanglesIterator; // Iterator to traverse a Rectangle array.
vector<AnguloVector>::iterator anguloVectorIterator; // Iterator to traverse a Line array.
vector<Circle>::iterator circleIterator; // Iterator to traverse a Line array.


/// Vectors

vector<ConceitoVector> conceitoVectors; // Vector of lines.
vector<Rectangle> rectangles; // Vector of rectangles.
vector<AnguloVector> anguloVectors; // Vector of lines.
vector<Circle> circles;// Vector of lines.


/// Funções para chamada da função da classe, função que intera no vetor


void clearAll(void) {
    conceitoVectors.clear();
    rectangles.clear();
    anguloVectors.clear();
    circles.clear();
    primitive = INACTIVE;
    pointCount = 0;
}

void drawArrow(float x1, float y1, float x2, float y2) {
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(x1, y1 , 0.0);
    glVertex3f(x2 , y2, 0.0);
    glEnd();

    if(x2 > 500 && y2 > 250) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x2 - 20 , y2 + 20, 0.0);
        glVertex3f(x2  + 20, y2 - 20, 0.0);
        glVertex3f( x2+15, y2 + 15, 0.0);
        glEnd();
    }

    if(x2 >= 500 && y2 ==  250) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x2, y2 +20, 0.0);
        glVertex3f(x2, y2 - 20, 0.0);
        glVertex3f(x2 + 20, y2 , 0.0);
        glEnd();
    }

    if(x2 < 500 && y2 ==  250) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x2  , y2 , 0.0);
        glVertex3f(x2  + 20, y2 - 20, 0.0);
        glVertex3f(x2+20, y2 + 20, 0.0);
        glEnd();
    }

    if(x2 < 500 && y2 > 250) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x2 - 20 , y2 + 20, 0.0);
        glVertex3f(x2  - 20, y2 - 20, 0.0);
        glVertex3f( x2+15, y2 + 15, 0.0);
        glEnd();
    }

    if(x2 < 500 && y2 > 250) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x2 - 20 , y2 + 20, 0.0);
        glVertex3f(x2  - 20, y2 - 20, 0.0);
        glVertex3f( x2+15, y2 + 15, 0.0);
        glEnd();
    }

    if(x2 < 500 && y2 < 250) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x2 - 20, y2 + 20, 0.0);
        glVertex3f(x2 - 20, y2 - 20, 0.0);
        glVertex3f( x2+15, y2 - 20, 0.0);
        glEnd();
    }

    if(x2 > 500 && y2 < 250) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(x2 + 20, y2 + 20, 0.0);
        glVertex3f(x2 - 20, y2 - 20, 0.0);
        glVertex3f( x2+15, y2 - 20, 0.0);
        glEnd();
    }

}

// Function to draw all lines in the lines array.
void drawConceitoVectors(void) {
    // Loop through the lines array drawing each line.
    conceitoVectorIterator = conceitoVectors.begin();

    while(conceitoVectorIterator != conceitoVectors.end()) {
        conceitoVectorIterator->drawConceitoVector();
        conceitoVectorIterator++;
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
void drawAnguloVectors(void) {
    // Loop through the lines array drawing each line.
    anguloVectorIterator = anguloVectors.begin();

    while(anguloVectorIterator != anguloVectors.end()) {
        anguloVectorIterator->drawAnguloVector();
        anguloVectorIterator++;
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

void writeBitmapString(void *font, char *string) {
    char *c;
    for(c = string; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);
}

// Function to draw point selection box in left selection area.
void drawConceitoVectorsSelectionBox(void) {
    glDisable(GL_LINE_STIPPLE);
    /// Independente se a linha stipple foi ativada, o menu não é para ser ativado também
    /// Então desativamos

    /// Se primitiva for igual ponto, a cor do quadrado do ponto é alterada, o Highlight

    primitive == CONCEITO_VECTOR ? glColor3f(0.8, 0.8, 0.8) : glColor3f(0.08, 0.2, 0.2);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.8 * height, 0.25 * width, height);

    // Draw black boundary.
    glColor3f(255.0, 255.0, 255.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.8*height, 0.25 *width, height);

    // Draw point icon.
    glPointSize(10);
    glColor3f(0.0, 0.0, 0.0);

    /// Desenha o icone (Seta) no box
    glColor3f(255.0, 255.0, 255.0);
    glBegin(GL_LINES);
        /// Desenha a linha do vetor
        glVertex3f(0.050 * width, 0.860 * height, 0.0);
        glVertex3f(0.20 * width, 0.930 * height, 0.0);

        /// Desenha a base da seta
        glVertex3f(0.21 * width, 0.900 * height, 0.0);
        glVertex3f(0.19 * width, 0.960 * height, 0.0);

        /// Desenha o lado superior da seta
        glVertex3f(0.19 * width, 0.960 * height, 0.0);
        glVertex3f(0.22 * width, 0.940 * height, 0.0);

        /// Desenha o lado inferior da seta
        glVertex3f(0.21 * width, 0.900 * height, 0.0);
        glVertex3f(0.22 * width, 0.940 * height, 0.0);
    glEnd();

    glColor3f(255.0, 255.0, 255.0);
    glRasterPos3f(25.00, 410.0, 0.0);
    writeBitmapString(GLUT_BITMAP_9_BY_15, "Conceito Vetor" );

}

// Function to draw line selection box in left selection area.
void drawAnguloVectorsSelectionBox(void) {
    glDisable(GL_LINE_STIPPLE);

    primitive == ANGULO_VECTOR ?   glColor3f(0.8, 0.8, 0.8) : glColor3f(0.08, 0.2, 0.2);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.6 * height, 0.25 * width, 0.8 * height);

    // Draw black boundary.
    glColor3f(255.0, 255.0, 255.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.6 * height, 0.25 * width, 0.8 * height);

    float t = 0.0;

    /// Desenha o icone (angulo) no box
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < nVertices; ++i) {
        glColor3ub(255.0, 255.0, 255.0);
        glVertex3f( (0.115 * width) + 35 * cos(t), (0.680 * height) + 35 * sin(t), 0.0);
        t += PI / nVertices;
    }
    glEnd();

    glColor3f(255.0, 255.0, 255.0);
    glRasterPos3f(25.00, 310.0, 0.0);
    writeBitmapString(GLUT_BITMAP_9_BY_15, "Angulo de Vetores" );

}

// Function to draw rectangle selection box in left selection area.
void drawSumVectorsSelectionBox(void) {
    glDisable(GL_LINE_STIPPLE);

    primitive == SUM_VECTOR ? glColor3f(0.8, 0.8, 0.8) : glColor3f(0.08, 0.2, 0.2);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.4 * height, 0.25 * width, 0.6 * height);

    // Draw black boundary.
    glColor3f(255.0, 255.0, 255.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.4 * height, 0.25 * width, 0.6 * height);

    glBegin(GL_LINES);
    /// Desenha a linha do vetor
        glVertex3f(0.050 * width, 0.550  * height, 0.0);
        glVertex3f(0.20 * width, 0.550 * height, 0.0);

        glVertex3f(0.20 * width, 0.500 * height, 0.0);
        glVertex3f(0.050 * width, 0.500 * height, 0.0);

    glEnd();


    glColor3f(255.0, 255.0, 255.0);
    glRasterPos3f(25.00, 210.0, 0.0);
    writeBitmapString(GLUT_BITMAP_9_BY_15, "Produto Escalar" );

}

// Function to draw polyLine selection box in left selection area.
void drawScalarProductSelectionBox(void) {
    glDisable(GL_LINE_STIPPLE);

    primitive == SCALAR_PRODUCT ? glColor3f(0.8, 0.8, 0.8) : glColor3f(0.08, 0.2, 0.2);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.2 * height, 0.25 * width, 0.4*height);

    // Draw black boundary.
    glColor3f(255.0, 255.0, 255.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.2 * height, 0.25 * width, 0.4 * height);

    glBegin(GL_LINES);
        glVertex3f(0.050 * width, 0.300 * height, 0.0);
        glVertex3f(0.10 * width, 0.300* height, 0.0);
    glEnd();

    glColor3f(255.0, 255.0, 255.0);
    glRasterPos3f(0.11 * width, 0.280 * height, 0.0);
    writeBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "*");

    glColor3f(255.0, 255.0, 255.0);
    glRasterPos3f(0.14  * width, 0.290 * height, 0.0);
    writeBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "C");

    glColor3f(255.0, 255.0, 255.0);
    glRasterPos3f(25.00, 110.0, 0.0);
    writeBitmapString(GLUT_BITMAP_9_BY_15, "Multip por Escalar");
}

// Function to draw circle selection box in left selection area.
void drawVectorProductSelectionBox(void) {
    glDisable(GL_LINE_STIPPLE);

    float t = 0.0;

    primitive == VECTOR_PRODUCT ? glColor3f(0.8, 0.8, 0.8) : glColor3f(0.08, 0.2, 0.2);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0, 0.25 * width, 0.2 * height);

    // Draw black boundary.
    glColor3f(255.0, 255.0, 255.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0, 0.25 * width, 0.2 * height);

    glBegin(GL_LINES);
        glVertex3f(0.050 * width, 0.100 * height, 0.0);
        glVertex3f(0.10 * width, 0.160* height, 0.0);

        glVertex3f(0.050 * width, 0.100 * height, 0.0);
        glVertex3f(0.10 * width, 0.100 * height, 0.0);
    glEnd();

    glColor3f(255.0, 255.0, 255.0);
    glRasterPos3f(25.00, 10.0, 0.0);
    writeBitmapString(GLUT_BITMAP_9_BY_15, "Soma De Vetores" );

}

// Function to draw unused part of left selection area.
void drawInactiveArea(void) { }

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
    float sum = 200.0;
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x5555); /// Stipple default do grid
    glColor3f(0.75, 0.75, 0.75); /// Cor default do grid


    glBegin(GL_LINES);
        for (int i = 0; i < 6; ++i) {
            glVertex3f( sum, 0.0, 0.0);
            glVertex3f( sum, height, 0.0);
            sum += 100.0;
        }

        sum = 0;
        for (int i = 0; i < 6; i++) {
            glVertex3f(0.0, sum, 0.0);
            glVertex3f(width, sum, 0.0);
            sum += 83.333333;
        }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}



/// Escreve na tela o texto de qual figura está selecioada, o parametro texto é passado
void drawText(char *text) {
    glColor3f(0.0, 0.0, 15.0);
    glRasterPos3f(80.0, 55.0, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, text);
}



void drawTextScala() {
    char msg[30];
    itoa(abs(scalar),msg, 10);

    glColor3f(0.0, 0.0, 15.0);
    glRasterPos3f(210.0, 80.0, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "Modulo do produto Escalar: ");

    glColor3f(0.0, 0.0, 15.0);
    glRasterPos3f(440.0, 80.0, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, msg);

}

/// Função para desenhar plano cartesiano
void drawCartesianPlane(void) {
    glColor3f(0.0,51.0, 0.0); // Aplica as cores RGB, todas contidas nas posições do vetor
    glBegin(GL_LINES);
        /// Linha da vertical
        glVertex3f(500, 0, 0.0);
        glVertex3f(500, 500, 0.0);

        /// Linha da horizontal
        glVertex3f(0.0, 250.0, 0.0);
        glVertex3f(width, 250.0, 0.0);
    glEnd();

}

void AdrawConceitoVector() {

    drawArrow(300,83.3333,700,420.6667);
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(430.0, 260.0, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "V ->");

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(230.0, 60.0, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "DIRECAO");

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(580.0, 450.0, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "SENTIDO");

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(520.0, 150.0, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "| X | -> MODULO DO VETOR");
}


void drawArrowQuadrant() {
    switch(posQuadrant) {
        case 0:
            drawArrow(500, 250, 600, 250);
            break;
        case 45:
            drawArrow(500,  250, 600, 333.3333);
            break;

        case 90:
            drawArrow(500, 250, 500, 333.3333);
            break;

        case 135:
            drawArrow(500, 250, 400, 333.3333);
            break;

        case 180:
            drawArrow(500, 250, 400, 250.0);
            break;

        case 225:
            drawArrow(500, 250, 400, 166.6667);
            break;

        case 270:
            drawArrow(500, 250, 500, 166.6667);
            break;

        case 315:
            drawArrow(500, 250, 600, 166.6667);
            break;

        case 360:
            drawArrow(500, 250, 600, 250);
            break;

    }

}


void print(float x1, float y1, float x2, float y2, char *text, float *colors) {

    glRasterPos3f(30.0, 40.0, 0.0);
    glDisable(GL_LINE_STIPPLE);

    glColor3f(colors[0], colors[1], colors[2]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(x1, y1, x2, y2);

    glColor3f(0.0, 0.0, 15.0);
    glRasterPos3f(x1 + 5, y1 + 10, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_12, text);
}

void drawOptions(void) {
    float colors[3];

    switch(posQuadrant) {
        case 0:
            colors[0] = 244.0, colors[1] = 0.0, colors[2] = 0.24;
            print(510, 83.3333,540, 120.3333, "0", colors);

            break;
        case 45:
            colors[0] = 12.0, colors[1] = 123.0, colors[2] = 0.24;
            print(550, 83.3333,580, 120.3333, "45", colors);
            break;

        case 90:
            colors[0] = 1.2, colors[1] = 0.0, colors[2] = 0.0;
            print(590, 83.3333,620, 120.3333, "90", colors);
            break;

        case 135:
            colors[0] = 12.2, colors[1] = 34.123, colors[2] = 0.01;
            print(630, 83.3333,660, 120.3333, "135", colors);
            break;

        case 180:
            colors[0] = 0.2, colors[1] = 34.10, colors[2] = 1.20;
            print(670, 83.3333,700, 120.3333, "180", colors);
            break;

        case 225:
            colors[0] = 0.0, colors[1] = 0.1, colors[2] = 0.24;
            print(340, 83.3333, 370, 120.3333, "225", colors);
            break;

        case 270:
            colors[0] = 1.1, colors[1] = 0.31, colors[2] = 0.24;
            print(380, 83.3333, 410, 120.3333, "270", colors);

            break;

        case 315:
            colors[0] = 0.1, colors[1] = 0.0, colors[2] = 0.2;
            print(420, 83.3333, 450, 120.3333, "315", colors);
            break;

        case 360:
            colors[0] = 1.1, colors[1] = 1.0, colors[2] = 0.2;
            print(460, 83.3333, 490, 120.3333, "360", colors);
            break;
    }

}


void drawCalc(void) {

    if(vectorOne.x2 < 500) vectorOne.x2 = -vectorOne.x2;
    if(vectorOne.y2 < 250) vectorOne.y2 = -vectorOne.y2;

    if(vectorTwo.x2 < 500) vectorTwo.x2 = -vectorTwo.x2;
    if(vectorTwo.y2 < 250) vectorTwo.y2 = -vectorTwo.y2;

    float
        x1 = (abs(vectorOne.x2) - 500),
        x2 = (abs(vectorTwo.x2) - 500),
        y1 = (abs(vectorOne.y2) - 250),
        y2 = (abs(vectorTwo.y2) - 250);

    circles.push_back( Circle(500, 250, 500 + (x1 + x2), 250 + (y1 + y2) ) );
}

void drawInfoScalar(int pos) {

    if(pos  == 0) {
        glColor3f(0.0, 0.0, 15.0);
        glRasterPos3f(220.0, 80.0, 0.0);
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, "Sentido do vetor -> NUlo" );
    } else if(pos < 10) {
        glColor3f(0.0, 0.0, 15.0);
        glRasterPos3f(220.0, 80.0, 0.0);
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, "Sentido do vetor -> NEGATIVO" );
    }

    if(pos >= 10) {
        glColor3f(0.0, 0.0, 15.0);
        glRasterPos3f(220.0, 80.0, 0.0);
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, "Sentido do vetor -> POSITIVO" );
    }

    glColor3f(0.0, 0.0, 15.0);
    glRasterPos3f(220.0, 30.0, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "Modulo do Vetor: " );

    char msg[30];
    itoa(abs(pos),msg, 10);

    glColor3f(0.0, 0.0, 15.0);
    glRasterPos3f(390.0, 30.0, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, msg);


}

// Drawing routine.
void drawScene(void) {

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);


    drawRectangles();
    drawAnguloVectors();
    drawCircles();
    drawConceitoVectors();


    if (( (primitive == CONCEITO_VECTOR) || (primitive == SUM_VECTOR) || (primitive == SCALAR_PRODUCT) )
        && (pointCount == 1)
    ) drawTempPoint();

    drawCartesianPlane();

    if (isGrid) {
        drawGrid();
    }

    if(primitive == CONCEITO_VECTOR) {
        AdrawConceitoVector();
    }

    if(primitive == SUM_VECTOR) {
        drawTextScala();
    }

    if(primitive == ANGULO_VECTOR) {
        circles.clear();
        drawArrowQuadrant();
        drawOptions();
    }

    if(primitive == SCALAR_PRODUCT) {
        circles.clear();
        drawArrow(500, 250, 500 + Constant, 250);
        drawInfoScalar(Constant);
    }

    drawSumVectorsSelectionBox();
    drawConceitoVectorsSelectionBox();
    drawScalarProductSelectionBox();
    drawAnguloVectorsSelectionBox();
    drawInactiveArea();
    drawVectorProductSelectionBox();
    glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y) {
    cout << y << endl;

    if(y > 400 && y < 500) primitive = CONCEITO_VECTOR;
    else if(y > 300 && y < 400) primitive = ANGULO_VECTOR;
    else if(y > 200 && y < 400) primitive = SUM_VECTOR;
    else if(y > 100 && y < 200) primitive = SCALAR_PRODUCT;
    else if(y > 0 && y < 100) primitive = VECTOR_PRODUCT;
    cout << primitive << endl;
}

void resetAndSetCoordinates(float x, float y) {
    currentPoint.x = x;
    currentPoint.y = y;
    center.x = x;
    center.y = y;
    pointCount = 1;
}


void drawPVetorial(void) {

    if(vectorOne.x2 < 500) vectorOne.x2 = -vectorOne.x2;
    if(vectorOne.y2 < 250) vectorOne.y2 = -vectorOne.y2;

    if(vectorTwo.x2 < 500) vectorTwo.x2 = -vectorTwo.x2;
    if(vectorTwo.y2 < 250) vectorTwo.y2 = -vectorTwo.y2;

    float
        x1 = (abs(vectorOne.x2) - 500),
        x2 = (abs(vectorTwo.x2) - 500),
        y1 = (abs(vectorOne.y2) - 250),
        y2 = (abs(vectorTwo.y2) - 250);

    scalar =  (x1 * x2) + (y1 * y2);
}


void mouseControl(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        y = height - y;  // Correct from mouse to OpenGL co-ordinates.

        // Click outside canvas - do nothing.
        if (x < 0 || x > width || y < 0 || y > height);

            // Click in left selection area.
        else if (x < 0.25 * width) {
            pickPrimitive(y);
            pointCount = 0;
        }

        if(primitive == SUM_VECTOR) {
            if(pointCount > 2) {
                if(pointCount == 3) {
                    drawPVetorial();
                    pointCount = 0;
                    circles.clear();
                }
            }

            if(pointCount != 0) {
                if(pointCount == 1) {
                    vectorOne.x2 = x;
                    vectorOne.y2 = y;
                }

                if(pointCount == 2) {
                    vectorTwo.x2 = x;
                    vectorTwo.y2 = y;
                }


                drawArrow(500, 250, x, y);
                circles.push_back( Circle(500, 250, x, y) );
                pointCount++;
            } else pointCount++;
        }


        if (primitive == VECTOR_PRODUCT) {
            if(pointCount > 2) {
                if(pointCount == 4) {
                    clearAll();
                } else {
                    drawCalc();

                    pointCount = 4;
                }

            } else if(pointCount != 0) {
                if(pointCount == 1) {
                    vectorOne.x2 = x;
                    vectorOne.y2 = y;
                }

                if(pointCount == 2) {
                    vectorTwo.x2 = x;
                    vectorTwo.y2 = y;
                }

                drawArrow(500, 250, x, y);
                circles.push_back( Circle(500, 250, x, y) );
                pointCount++;
            } else pointCount++;

        }
    }

    if ((button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)) pointCount = 0;
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

        case 49:
            posQuadrant = 0;
            break;

        case 50:
            posQuadrant = 45;
            break;

        case 51:
            posQuadrant = 90;
            break;

        case 52:
            posQuadrant = 135;
            break;

        case 53:
            posQuadrant = 180;
            break;

        case 54:
            posQuadrant = 225;
            break;

        case 55:
            posQuadrant = 270;
            break;

        case 56:
            posQuadrant = 315;
            break;

        case 57:
            posQuadrant = 360;
            break;

        case '-': /// Seta para esquerda
            Constant -= 10;
            break;
        case '+': /// Seta para direita
            Constant += 10;
            break;
        default:
            break;
    }
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
void keyGraus(int key) {
    switch(key) {
        case 1:
            posQuadrant = 0;
            break;

        case 2:
            posQuadrant = 45;
            break;

        case 3:
            posQuadrant = 90;
            break;

        case 4:
            posQuadrant = 135;
            break;

        case 5:
            posQuadrant = 180;
            break;

        case 6:
            posQuadrant = 225;
            break;

        case 7:
            posQuadrant = 270;
            break;

        case 8:
            posQuadrant = 315;
            break;

        case 9:
            posQuadrant = 360;
            break;


        default:
            break;
    }
}


void makeMenu(void) {
    int sub_menu, options_graus;

    options_graus = glutCreateMenu(keyGraus);
    glutAddMenuEntry("0  Graus",  1);
    glutAddMenuEntry("45  Graus", 2);
    glutAddMenuEntry("90  Graus", 3);
    glutAddMenuEntry("135 Graus", 4);
    glutAddMenuEntry("180 Graus", 5);
    glutAddMenuEntry("225 Graus", 6);
    glutAddMenuEntry("270 Graus", 7);
    glutAddMenuEntry("315 Graus", 8);
    glutAddMenuEntry("360 Graus", 9);



    sub_menu = glutCreateMenu(grid_menu);
    glutAddMenuEntry("On", 3);
    glutAddMenuEntry("Off", 4);

    glutCreateMenu(rightMenu);
    glutAddSubMenu("Grid", sub_menu);
    glutAddSubMenu("Alterar Graus", options_graus);
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
    glutInitWindowSize(800, 500);

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