#ifndef _CPLANE_H_
#define _CPLANE_H_
#pragma once

#include "primitive.h"

class CPlane : public CPrimitive {

protected: 
	
	Vec3f p1, p2, p3;
	float m_distance;
	Vec3d normal;

public: 

	CPlane(Vec3f p1, Vec3f p2, Vec3f p3, float distance);
	short		getPrimitiveType();
	Vec3d		getNorm(Vec3d point);
	short		isIntersect(CRay ray,double &distance);

};

#endif
