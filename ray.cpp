#include "ray.h"

CRay::CRay() {
}

CRay::CRay(Vec3d origin,Vec3d direction) {

	this->origin = origin;
	this->direction = direction;
}

CRay CRay::flipRay() {
	CRay ray(this->direction, this->origin);
	return ray;
}