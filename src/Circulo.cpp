/*
 * Circulo.cpp
 *
 *  Created on: 26 ago. 2020
 *      Author: ASUS
 */

#include "Circulo.h"



Circulo::Circulo() {
	// TODO Auto-generated constructor stub

}

Circulo::~Circulo() {
	// TODO Auto-generated destructor stub
}



vector<float> Circulo::crearCirculo(GLfloat radio,int puntos,GLfloat posX,GLfloat posY) {

	  vector<float> temp;

	// Create vertices
		GLfloat x = posX;//0;
		GLfloat y = posY;//0;

		GLfloat radius = radio;//1
		GLint numberOfSides = puntos; // 50
		int numberOfVertices = numberOfSides+1; // points + one center point.
		GLfloat twicePi = 2.0f * M_PI;

		GLfloat verticesX[numberOfVertices];
		GLfloat verticesY[numberOfVertices];

		verticesX[0] = x;
		verticesY[0] = y;


		for (int i = 1; i < numberOfVertices; i++) {
			verticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
			verticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));//HORARIA//verticesY[i] = y + (-radius * sin(i * twicePi / numberOfSides));
			}



		for (int i = 0; i < numberOfVertices; i++) {

			temp.push_back(verticesX[i]);
			temp.push_back(verticesY[i]);

		}

		return temp;

}
