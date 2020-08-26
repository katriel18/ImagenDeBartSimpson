/*
 * Circulo.h
 *
 *  Created on: 26 ago. 2020
 *      Author: ASUS
 */


#ifndef PRIMITIVES_CIRCULO_H_
#define PRIMITIVES_CIRCULO_H_


#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <SOIL2/SOIL2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

using namespace std;

class Circulo {
public:
	Circulo();
	virtual ~Circulo();

private:

public:

	vector<float> crearCirculo(GLfloat radio,int puntos,GLfloat posX,GLfloat posY);
};

#endif /* PRIMITIVES_CIRCULO_H_ */
