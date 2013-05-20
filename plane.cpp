#include "plane.h"

CPlane::CPlane(Vec3f p1, Vec3f p2, Vec3f p3, float distance) {
	
	this->p1 = p1;
	this->p2 = p2; 
	this->p3 = p3;

	Vec3f e1 = p3 - p2;
	Vec3f e3 = p2 - p1;
	
	//normal = e1%e3;
	//normal = Vec3f(0,0.8,0);
	normal = Vec3f(0,0.7,0);
	normal = normal.normalize();
	m_distance = distance;
}

short	CPlane::getPrimitiveType() {return PRIMITIVE_TYPE_PLANE;}

Vec3d	CPlane::getNorm(Vec3d point) {return normal;}

short	CPlane::isIntersect(CRay ray,double &distance) {
	
	short retVal = INTERSECTION_MISS;

	float intersection = normal | ray.direction ;
	if (intersection != 0)
	{
		float dist = - ( ( normal | ray.origin) + m_distance) / intersection;
		if (dist > 0)
		{
			if (dist < distance) 
			{
				distance = dist - 0.1;
				retVal = INTERSECTION_HIT;
			}
		}
	}
	return retVal;
}