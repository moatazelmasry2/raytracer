#ifndef _CRAY_H_
#define _CRAY_H_
#pragma once

#include "VectorT.h"
#include "ColorT.h"

class CRay {

private: 

	

public: 
	
	Vec3d origin;
	Vec3d direction;
	CColor3T<double> lightColor;

	CRay(Vec3d origin,Vec3d direction);
	CRay();
	CRay flipRay();
};

#endif