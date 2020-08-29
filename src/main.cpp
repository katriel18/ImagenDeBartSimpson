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

#include "Circulo.h"

GLuint renderingProgram;

using namespace std;

//metodos
vector<float> graficaPuntosBezier(float pcontrol[][2],int N);
float factorial(int n);
float CoefNewton(int n, int k);
float CurvaBezier(float u, int axis,float pcontrol[][2],int N);
vector<float> graficaCurvaBezier(float pcontrol[][2],int N);
void init (GLFWwindow* window) ;
void display(GLFWwindow* window, double currentTime);

//int N = 5;				//  <------------------

GLuint m_VAO;
GLuint m_VBO[2];


GLfloat pcontrol1[5][2]	//  <-----lado izquierdo-------

{
	{ -0.54, 0.8},//punto final
	{ -0.46, 0.4},
	{ -0.54, 0.0},
	{ -0.44, -0.4},
	{ -0.5,-0.8},//punto inicio

};

//9 vertices  <----- cabello-------

GLfloat pcontrol2[7][2]	//  <-----lado derecho-------

{

	{ 0.52,-0.61},//punto final //labio
	{ 0.48,-0.5},

	{ 0.5,-0.5},
	{ 0.46, -0.4},
	{ 0.52, 0.0},
	{ 0.42, 0.4},

	{ 0.52, 0.8},//punto inicio

};

GLfloat pcontrol3[5][2]	//  <-----barba 1-------

{

	{ -0.2, -0.6},//punto final
	{ -0.1, -0.69},
	{ 0.1, -0.69},

	{ 0.4, -0.71},
	{ 0.52,-0.62},//punto inicio

};

GLfloat pcontrol4[5][2]	//  <----- SONRISA-------

{

	{-0.15, -0.56},//punto final
	{ -0.20, -0.58},

	{ -0.22, -0.6},

	{ -0.20, -0.62},
	{ -0.18, -0.66},//punto inicio

};

GLfloat pcontrol5[5][2]	//  <----- barba2-------

{

	{ -0.1, -0.75},//punto final
	{ -0.05, -0.75},
	{ 0.1, -0.75},
	{ 0.05, -0.7},
	{ 0.08, -0.68},//punto inicio

};

GLfloat pcontrol6[7][2]	//  <----- nariz-------

{

	{ 0.36,-0.42 },//punto final
	{ 0.46 ,-0.40 },

	{ 0.60,-0.45},

	{ 0.48,-0.33},//punta

	{ 0.60,-0.19},

	{ 0.40 ,-0.30},
	{ 0.28 ,-0.28},//punto inicio

};

GLfloat pcontrol7[5][2]	//  <----- oreja-------

{

	{ -0.44,-0.40},//punto final


	{ -0.52,-0.52},
	{ -0.66,-0.37},
	{ -0.52,-0.18},


	{ -0.44,-0.29},//punto inicio

};

GLfloat pcontrol8[3][2]	//  <----- oreja interna 1-------

{
	{ -0.48,-0.39},
	{ -0.53,-0.37},
	{ -0.51,-0.33},//---------

};
GLfloat pcontrol9[3][2]	//  <----- oreja interna 2-------

{
	{-0.53,-0.35},
	{ -0.51,-0.32},
	{-0.47,-0.34},

};


GLfloat pcontrol10[3][2]	//  <----- frente-------

{
	{-0.53,-0.35},
	{ -0.51,-0.32},
	{-0.47,-0.34},

};


int numPointsBz;
int nPointsCurveBz;
vector<float> pCBezier;

vector<float> tempCBezier;
int cant1,cant2,cant3,cant4,cant5,cant6,cant7,cant8;
Circulo circulo;
vector<float> tempCirculo;

/********************/
vector<float> graficaPuntosBezier(float pcontrol[][2],int N) {
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
float CurvaBezier(float u, int axis,float pcontrol[][2],int N) {
	float suma = 0.0;
	for (int i = 0; i < N; i++) {
		suma += pcontrol[i][axis] * CoefNewton(N - 1, i) * pow(u, N - 1 - i)
				* pow(1.0 - u, i);
	}
	return suma;
}

/********************/
vector<float> graficaCurvaBezier(/*void,*/float pcontrol[][2],int N) {
	vector<float> temp;
	for (float u = 0.0; u <= 1; u += 0.01) {
		temp.push_back(CurvaBezier(u, 0,pcontrol, N));
		temp.push_back(CurvaBezier(u, 1,pcontrol, N));
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
	vector<float> pBezier = graficaPuntosBezier(pcontrol1,5);


	numPointsBz = pBezier.size()/2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);

	// Reserva memoria na GPU para um TARGET receber dados
	// Copia esses dados pra essa �rea de memoria
	glBufferData(
			GL_ARRAY_BUFFER,	// TARGET associado ao nosso buffer
			pBezier.size()*sizeof(GLfloat),	// tamanho do buffer
			(void*)&pBezier[0],			// Dados a serem copiados pra GPU
			GL_STATIC_DRAW				// Pol�tica de acesso aos dados, para otimiza��o
	);


    // Create a Vertex Buffer Para los puntos de la Curva
    // Vector que guarda los Puntos de la curva de Bezier

	//PUNTO CENTRO PARA PINTADO
	pCBezier.push_back(-0.22);
	pCBezier.push_back(-0.6);

	//  <-------- LADO IZQUIERDO ---------
	 tempCBezier = graficaCurvaBezier(pcontrol1,5);
	 pCBezier.insert(pCBezier.end(),tempCBezier.begin(),tempCBezier.end());

	//  <-------- PELO ---------
		pCBezier.push_back(-0.45);//inicio izquierdo
		pCBezier.push_back(0.72);
		pCBezier.push_back(-0.40);
		pCBezier.push_back(0.82);
		pCBezier.push_back(-0.35);
		pCBezier.push_back(0.72);
		pCBezier.push_back(-0.30);
		pCBezier.push_back(0.82);
		pCBezier.push_back(-0.25);
		pCBezier.push_back(0.72);

		pCBezier.push_back(-0.20);
		pCBezier.push_back(0.84);
		pCBezier.push_back(-0.15);
		pCBezier.push_back(0.74);
		pCBezier.push_back(-0.10);
		pCBezier.push_back(0.84);
		pCBezier.push_back(-0.05);
		pCBezier.push_back(0.74);
		pCBezier.push_back(0.00);
		pCBezier.push_back(0.84);
		pCBezier.push_back(0.05);
		pCBezier.push_back(0.74);
		pCBezier.push_back(0.10);
		pCBezier.push_back(0.84);
		pCBezier.push_back(0.15);
		pCBezier.push_back(0.74);
		pCBezier.push_back(0.20);
		pCBezier.push_back(0.84);

		pCBezier.push_back(0.25);
		pCBezier.push_back(0.72);
		pCBezier.push_back(0.30);
		pCBezier.push_back(0.82);
		pCBezier.push_back(0.35);
		pCBezier.push_back(0.72);
		pCBezier.push_back(0.40);
		pCBezier.push_back(0.82);
		pCBezier.push_back(0.45);
		pCBezier.push_back(0.72);//incio del pelo derecho

	//  <--------------------lado derecho

	tempCBezier = graficaCurvaBezier(pcontrol2,7);
	pCBezier.insert(pCBezier.end(),tempCBezier.begin(),tempCBezier.end());

	//  <-------------barba1

	tempCBezier = graficaCurvaBezier(pcontrol3,5);
	pCBezier.insert(pCBezier.end(),tempCBezier.begin(),tempCBezier.end());
	cant1=pCBezier.size()/2;

	//  <--------SONRISA-------

	tempCBezier = graficaCurvaBezier(pcontrol4,5);
	pCBezier.insert(pCBezier.end(),tempCBezier.begin(),tempCBezier.end());
	cant2=pCBezier.size()/2;

	//  <------------barba2

	tempCBezier = graficaCurvaBezier(pcontrol5,5);
	pCBezier.insert(pCBezier.end(),tempCBezier.begin(),tempCBezier.end());

	//  <-------- CUELLO ---------
	pCBezier.push_back(-0.1);
	pCBezier.push_back(-0.8);
	cant3=pCBezier.size()/2;


	//  <------------ojo  derecho
	tempCirculo=circulo.crearCirculo(0.20,200,0.40,-0.12);
	pCBezier.insert(pCBezier.end(),tempCirculo.begin(),tempCirculo.end());

	// <------------ojo  izquierdo
	tempCirculo=circulo.crearCirculo(0.20,200,0.16,-0.12);
	pCBezier.insert(pCBezier.end(),tempCirculo.begin(),tempCirculo.end());
	cant4=pCBezier.size()/2;


	//  <------------ojo peque�o  derecho
	tempCirculo=circulo.crearCirculo(0.02,100,0.40,-0.10);
	pCBezier.insert(pCBezier.end(),tempCirculo.begin(),tempCirculo.end());

	// <------------ojo peque�o  izquierdo
	tempCirculo=circulo.crearCirculo(0.02,100,0.10,-0.11);
	pCBezier.insert(pCBezier.end(),tempCirculo.begin(),tempCirculo.end());
	cant5=pCBezier.size()/2;


	//  <--------nariz-------
	tempCBezier = graficaCurvaBezier(pcontrol6,7);
	pCBezier.insert(pCBezier.end(),tempCBezier.begin(),tempCBezier.end());
	cant6=pCBezier.size()/2;

	//  <-------- oreja -------
	tempCBezier = graficaCurvaBezier(pcontrol7,5);
	pCBezier.insert(pCBezier.end(),tempCBezier.begin(),tempCBezier.end());
	cant7=pCBezier.size()/2;

	//  <-------- oreja intena -------
	tempCBezier = graficaCurvaBezier(pcontrol8,3);
	pCBezier.insert(pCBezier.end(),tempCBezier.begin(),tempCBezier.end());
	cant8=pCBezier.size()/2;

	//  <-------- oreja intena 2-------
	tempCBezier = graficaCurvaBezier(pcontrol9,3);
	pCBezier.insert(pCBezier.end(),tempCBezier.begin(),tempCBezier.end());

	//  <-------- triangulo de pintado de cuello -------

	pCBezier.push_back(-0.1);
	pCBezier.push_back(-0.8);
	pCBezier.push_back(-0.5);
	pCBezier.push_back(-0.8);
	pCBezier.push_back(-0.22);
	pCBezier.push_back(-0.6);

	//llenado de los vertices
	nPointsCurveBz = pCBezier.size()/2;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glBufferData(
			GL_ARRAY_BUFFER,
			pCBezier.size()*sizeof(GLfloat),
			(void*)&pCBezier[0],
			GL_STATIC_DRAW
	);

}

void display(GLFWwindow* window, double currentTime) {
    glUseProgram(renderingProgram);

    // cambiar color
    GLfloat colorVariable = glGetUniformLocation(renderingProgram, "colorVariable");

    GLfloat colorVariable2 = glGetUniformLocation(renderingProgram, "colorVariable2");

    // Clear the screen to black
    //float m = ((int) currentTime % 2 == 0) ? 0.5f : 1.0f;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);	// Habilita este atributo Layout 0

/*
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
*/

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

	//GL_LINE_STRIP,GL_LINES,GL_LINE_LOOP,GL_POINTS,GL_TRIANGLE_FAN(PINTA)

	//pintar de amarillo la base completa
	glProgramUniform1f(renderingProgram, colorVariable, 1.0f);
	glProgramUniform1f(renderingProgram, colorVariable2, 1.0f);
	glDrawArrays(GL_TRIANGLE_FAN, 0, cant3);
	//pintar de amarillo triangulo del cuello faltante
	glDrawArrays(GL_TRIANGLES, nPointsCurveBz-3, 3);


	//lados , cabello ,barba1
	glProgramUniform1f(renderingProgram, colorVariable, 0.0);
	glProgramUniform1f(renderingProgram, colorVariable2, 0.0);
	glDrawArrays(GL_LINE_STRIP,1,cant1-1);

	//sonrisa
	glDrawArrays(GL_LINE_STRIP,cant1,cant2-cant1);

	//barba2 y cuello
	glDrawArrays(GL_LINE_STRIP,cant2,cant3-cant2);

	//ojo1 derecho
	glProgramUniform1f(renderingProgram, colorVariable, 1.0f);
	glDrawArrays(GL_TRIANGLE_FAN,cant3+1,201-1);//+1,-1 elimina el punto extra

	glProgramUniform1f(renderingProgram, colorVariable, 0.0f);
	glDrawArrays(GL_LINE_LOOP,cant3+1,201-1);//+1,-1 elimina el punto centro

	//ojo2 grande izquierdo
	glProgramUniform1f(renderingProgram, colorVariable, 1.0f);
	glDrawArrays(GL_TRIANGLE_FAN,cant3+201,201);

	glProgramUniform1f(renderingProgram, colorVariable, 0.0f);
	glDrawArrays(GL_LINE_LOOP,cant3+201+1,201-1);//+1,-1 elimina el punto centro

	//ojos peque�os
	glDrawArrays(GL_TRIANGLE_FAN,cant4,101);
	glDrawArrays(GL_TRIANGLE_FAN,cant4+101,101);

	//nariz
	glProgramUniform1f(renderingProgram, colorVariable, 1.0f);
	glProgramUniform1f(renderingProgram, colorVariable2, 1.0f);
	glDrawArrays(GL_TRIANGLE_FAN, cant5,cant6-cant5);

	glProgramUniform1f(renderingProgram, colorVariable2, 0.0f);
	glProgramUniform1f(renderingProgram, colorVariable, 0.0f);
	glDrawArrays(GL_LINE_STRIP,cant5,cant6-cant5);

	//oreja
	glProgramUniform1f(renderingProgram, colorVariable, 1.0f);
	glProgramUniform1f(renderingProgram, colorVariable2, 1.0f);
	glDrawArrays(GL_TRIANGLE_FAN, cant6,cant7-cant6);

	glProgramUniform1f(renderingProgram, colorVariable2, 0.0f);
	glProgramUniform1f(renderingProgram, colorVariable, 0.0);
	glDrawArrays(GL_LINE_STRIP,cant6,cant7-cant6);

	//oreja interna 1
	glDrawArrays(GL_LINE_STRIP,cant7,cant8-cant7);

	//oreja interna 2
	glDrawArrays(GL_LINE_STRIP,cant8,nPointsCurveBz-cant8-3);






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

    GLFWwindow* window = glfwCreateWindow(750, 600, "Imagen De Bart Simpson", NULL, NULL);
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
