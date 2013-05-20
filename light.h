#ifndef _CLIGHT_H_
#define _CLIGHT_H_
#pragma once

#include "ray.h"
#include "sphere.h"


#define LIGHT_TYPE_OMNI			200
#define LIGHT_TYPE_DIRECTIONAL	201
#define LIGHT_TYPE_SPOT_LIGHT	202

class CLight : public CSpehere {

protected: 

public:

	CLight(Vec3d center, double radius): CSpehere(center,radius) { 
		
	}
	virtual short			getType() = 0;
};

#endif