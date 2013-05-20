#include "sphere.h"


CSpehere::CSpehere(Vec3d center, double radius) {
	
	material.emission = 2;

	this->center = center;
	this->radius = radius;
	this->sqrRadius = SQR(radius);
	this->compRadius = 1/radius;
}

short CSpehere::getPrimitiveType() {return PRIMITIVE_TYPE_SPHERE;}

Vec3d	CSpehere::getNorm(Vec3d point) {

	//(point - center) * compRadius;
	Vec3d normal  = point - center;
	return normal.normalize();
}
short		CSpehere::isIntersect(CRay ray,double &distance) {

	Vec3d v = ray.origin - center;
	double A = ray.direction | ray.direction;
	double B = 2 * ( ray.direction | v );
	double C = (v | v) - sqrRadius;
	int retval = INTERSECTION_MISS;
	double det = SQR(B) - 4 * A * C;
	//double det = SQR(B) - C;
	if (det > 0 ) {
		det = sqrtf(det);
		double t1 = (-B - det)/2*A;
		double t2 = (-B + det)/2*A;
		/*if (t2 > 0)
		{
			if (t1 < 0) 
			{
				if (t2 < distance) 
				{
					distance = t2;
					retval = INTERSECTION_INSIDE;
				}
			}
			else
			{
				if (t1 < distance)
				{
					distance = t1;
					retval = INTERSECTION_HIT;
				}
			}
		}*/
		if (t1>0 && t1<distance) {
			distance = t1;
			retval = INTERSECTION_HIT;
			
		} else if (t2> 0 && t2<distance) {
			distance = t2;
			retval = INTERSECTION_INSIDE;
		}


	}
	distance -= 0.1;
	return retval;

	/*Vec3d v = ray.origin - center;
	double b = -( v | ray.direction );
	double det = (b * b) - ( v | v ) + sqrRadius;
	int retval = INTERSECTION_MISS;
	if (det > 0)
	{
		det = sqrtf( det );
		double i1 = b - det;
		double i2 = b + det;
		if (i2 > 0)
		{
			if (i1 < 0) 
			{
				if (i2 < distance) 
				{
					distance = i2;
					retval = INTERSECTION_INSIDE;
				}
			}
			else
			{
				if (i1 < distance)
				{
					distance = i1;
					retval = INTERSECTION_HIT;
				}
			}
		}
	}
	return retval;*/
}