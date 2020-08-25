//============================================================================
// Name        : Draw Bezier Curve in OpenGL Modern
// Description : Imagen De Bart Simpson
//============================================================================

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include "Utils.h"

GLuint renderingProgram;

using namespace std;

//metodos
//void nuevaCurva(int n);
vector<float> graficaPuntosBezier(float pcontrol[][2]);
float factorial(int n);
float CoefNewton(int n, int k);
float CurvaBezier(float u, int axis,float pcontrol[][2]);
vector<float> graficaCurvaBezier(float pcontrol[][2]);
void init (GLFWwindow* window) ;
void display(GLFWwindow* window, double currentTime);

int N = 5;				//  <------------------

GLuint m_VAO;
GLuint m_VBO[2];


GLfloat pcontrol1[5][2]	//  <-----lado derecho-------

{
	{ -0.52, 0.8},//punto final
	{ -0.46, 0.4},
	{ -0.54, 0.0},
	{ -0.46, -0.4},
	{ -0.5,-0.8},//punto inicio

};

GLfloat pcontrol2[5][2]	//  <-----lado derecho-------

{
	{ 0.52, 0.8},//punto final
	{ 0.46, 0.4},
	{ 0.54, 0.0},
	{ 0.46, -0.4},
	{ 0.5,-0.8},//punto inicio

};




int numPointsBz;
int nPointsCurveBz;

/********************/
vector<float> graficaPuntosBezier(float pcontrol[][2]) {
  vector<float> temp;
  for (int i = 0; i < N; i++) {
    temp.push_back(pcontrol[i][0]);
    temp.push_back(pcontrol[i][1]);
  }
  return temp;
}

/********************/
float factorial(int n) {
  float p = 1;
  for (int i = 1; i <= n; i++)
    p = p * (float) i;
  return p;
}

/********************/
float CoefNewton(int n, int k) {
  return factorial(n) / (factorial(k) * factorial(n - k));
}

/********************/
float CurvaBezier(float u, int axis,float pcontrol[][2]) {
	float suma = 0.0;
	for (int i = 0; i < N; i++) {
		suma += pcontrol[i][axis] * CoefNewton(N - 1, i) * pow(u, N - 1 - i)
				* pow(1.0 - u, i);
	}
	return suma;
}

/********************/
vector<float> graficaCurvaBezier(/*void,*/float pcontrol[][2]) {
	vector<float> temp;
	for (float u = 0.0; u <= 1; u += 0.01) {
		temp.push_back(CurvaBezier(u, 0,pcontrol));
		temp.push_back(CurvaBezier(u, 1,pcontrol));
	}
	return temp;
}



void init (GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");

    // Create Vertex Array Object
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    glGenBuffers(2, m_VBO);


    // Create a Vertex Buffer Object and copy the vertex data to it
    // Vector que guarda los Puntos de Bezier
	vector<float> pBezier = graficaPuntosBezier(pcontrol1);


	numPointsBz = pBezier.size()/2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);

	// Reserva memoria na GPU para um TARGET receber dados
	// Copia esses dados pra essa área de memoria
	glBufferData(
			GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
			pBezier.size()*sizeof(GLfloat),	// tamanho do buffer
			(void*)&pBezier[0],			// Dados a serem copiados pra GPU
			GL_STATIC_DRAW);		// Política de acesso aos dados, para otimização


    // Create a Vertex Buffer Para los puntos de la Curva
    // Vector que guarda los Puntos de la curva de Bezier
	vector<float> pCBezier = graficaCurvaBezier(pcontrol1);

	//  <-------- PELO ---------
/*
	pCBezier.push_back(-0.4);
	pCBezier.push_back(0.7);
	pCBezier.push_back(-0.3);
	pCBezier.push_back(0.8);
	pCBezier.push_back(-0.2);
	pCBezier.push_back(0.7);
	pCBezier.push_back(-0.1);
	pCBezier.push_back(0.8);

	pCBezier.push_back(0.0);
	pCBezier.push_back(0.7);
	pCBezier.push_back(0.1);
	pCBezier.push_back(0.8);
	pCBezier.push_back(0.2);
	pCBezier.push_back(0.7);
	pCBezier.push_back(0.3);
	pCBezier.push_back(0.8);*/

	//  <---------------------

	nPointsCurveBz = pCBezier.size()/2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glBufferData(
			GL_ARRAY_BUFFER,
			pCBezier.size()*sizeof(GLfloat),
			(void*)&pCBezier[0],
			GL_STATIC_DRAW);
}

void display(GLFWwindow* window, double currentTime) {
    glUseProgram(renderingProgram);

    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);	// Habilita este atributo Layout 0

    // Draw Control Points
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glVertexAttribPointer(
			0,
			2,
			GL_FLOAT,
			GL_FALSE,
			2*sizeof(GLfloat),
			0
	);
	glPointSize(4);
	glDrawArrays(GL_POINTS, 0, numPointsBz);



	// Draw Curves Points
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glVertexAttribPointer(
			0,
			2,
			GL_FLOAT,
			GL_FALSE,
			2*sizeof(GLfloat),
			0
	);
	glPointSize(6);
	glDrawArrays(GL_LINE_STRIP, 0, nPointsCurveBz);
	//GL_LINE_STRIP,GL_LINES,GL_LINE_LOOP,GL_POINTS,GL_TRIANGLE_FAN(PINTA)

}


int main(void) {
    if (!glfwInit()) {
    	exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(800, 600, "Imagen De Bart Simpson", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
    	exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
