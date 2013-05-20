#ifndef _OMNI_LIGHT_H_
#define _OMNI_LIGHT_H_

#include "light.h"

class COmniLight: public CLight {

public:

	COmniLight(Vec3d center, double radius): CLight(center,radius) {  }
	
	short getType(){return LIGHT_TYPE_OMNI;}


};

#endif