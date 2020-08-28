/*
 * PuntosControl.h
 *
 *  Created on: 27 ago. 2020
 *      Author: ASUS
 */

#ifndef PUNTOSCONTROL_H_
#define PUNTOSCONTROL_H_

#include "Utils.h"

class PuntosControl {
public:
	PuntosControl();
	virtual ~PuntosControl();


private:

	 GLfloat pcontrol1[5][2];

public :
	 GLfloat ladoIzquierdo();



};

#endif /* PUNTOSCONTROL_H_ */
