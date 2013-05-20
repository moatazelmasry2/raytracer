#ifndef _CSPHERE_H_
#define _CSPHERE_H_
#pragma once

#include "primitive.h"

class CSpehere : public CPrimitive {
 
protected:

	double sqrRadius;
	double compRadius;

public: 
	
	Vec3d center;
	double radius;


	CSpehere(Vec3d center, double radius);
	virtual short		getPrimitiveType();
	virtual Vec3d	getNorm(Vec3d point);
	virtual short		isIntersect(CRay ray,double &distance);
	
};

#endif