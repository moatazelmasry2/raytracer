#ifndef _CCAMERA_H_
#define _CCAMERA_H_
#pragma once


#include "vectort.h"
#include "frustrum.h"

class CCamera {
	
	

public: 
	
	CCamera(Vec3d origin,Vec3d direction, Vec3d upVector){
		this->position = origin;
		this->direction = direction;
		this->upVector = upVector;
	}

	Vec3d position;
	Vec3d direction;
	Vec3d upVector;

};

#endif