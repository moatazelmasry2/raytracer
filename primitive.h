#ifndef _CPRIMITIVE_H_
#define _CPRIMITIVE_H_
#pragma once



#include "colort.h"
#include "ray.h"
#include "material.h"

#include <vector>
#include <list>
using namespace std;

#define	NULL_VALUE -999
#define SQR(x) x*x

#define PRIMITIVE_TYPE_PRIMITIVE	99
#define PRIMITIVE_TYPE_POLYGON		100
#define PRIMITIVE_TYPE_BOX			101
#define PRIMITIVE_TYPE_SPHERE		102
#define PRIMITIVE_TYPE_CONE			103
#define	PRIMITIVE_TYPE_ELLIPSE		104
#define PRIMITIVE_TYPE_PLANE		105

#define INTERSECTION_HIT			1
#define INTERSECTION_MISS			0
#define INTERSECTION_INSIDE			-1


/*This is the building component of the object where the light will fall and reflected/broken
This is an abstract class, Other object classes should inherit it to write different shapes
*/
class CPrimitive {
	
	const static int startRef = 100;
	static int objRef;
	int curRef;

public:

	CMaterial material;

	CPrimitive() { 
		if (!objRef) objRef = startRef;
		curRef = objRef++;
	}

	int getObjRef(){return curRef;}
	
	virtual short		getPrimitiveType()=0;
	virtual Vec3d	getNorm(Vec3d point)=0;
	virtual short		isIntersect(CRay ray,double &dist)=0;
	
};

#endif