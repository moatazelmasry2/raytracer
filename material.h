#ifndef _CMATERIAL_H_
#define _CMATERIAL_H_
#pragma once

#include "colort.h"
#include "vectort.h"

class CMaterial {

private:


public:

	CMaterial() {
		reflection = 0.7f;
		refractiveIndex = 1.0f;
		emission = 1;
		refraction = 0.4f;
	}

	Vec3f ambient;
	Vec3f specular;
	Vec3f diffuse;
	float reflection; //specular reflection
	int	  emission;   //mx
	float refractiveIndex;
	//This should determine how the light ray is reflected/broken
	float dispersion; //Grad der Struung der spekularen Reflektion
	//CColor3T<float> color;
	float refraction;

};

#endif