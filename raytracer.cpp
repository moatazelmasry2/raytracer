#include "raytracer.h"
#include <stack>
#include <exception>
using namespace std;

#include <stdlib.h>
#include <float.h>


#define REFLECTION_DEPTH 3
#define VAKUM_REFRACTIVE_INDEX 1

CRayTracer::CRayTracer(CCamera *camera,CImageT<double> *image) {
	
	this->camera = camera;
	this->image  = image;
	this->frustrum = 0;
}

void CRayTracer::startRayTracing(const char *path) {
	
	//If there are no objects, then nothing to render
	if (sceneObjects.size() <1 ) return;

	CColor3T<double> *pixels = (CColor3T<double>*)image->getPixelPtr();

	//I'm not sure, but I think t in this case is the mean point of the Raster picture
	//double t = image->getWidth()/image->getHeight();
	double t = 0.9f;
	int index = 0;
	try {
	for (int i = 0; i < image->getHeight(); i++) {
		m_indexX = frustrum->minX;
		for (int j = 0; j<image->getWidth() ;j++) {

			CRay ray;
			ray.origin = camera->position;
			ray.direction = ( Vec3d(m_indexX, m_indexY,0) - camera->position ).normalize();

			list<CPrimitive*>::iterator it;

			Vec3f color = calculateColor(ray,1);

			int red = color.m_values[0] * 256;
			int green = color.m_values[1] * 256;
			int blue = color.m_values[2] * 256;
			if (red > 255 ) red = 255;
			if (green > 255 ) green = 255;
			if (blue > 255 ) blue = 255;
			pixels[index].m_rgb[0] = red;
			pixels[index].m_rgb[1] = green;
			pixels[index].m_rgb[2] = blue;
			index++;
			m_indexX+= m_deltaX;
		}
		m_indexY += m_deltaY;
	}
	}catch (exception& e) {
		std::cout<<e.what()<<"\n";
		getchar();
	}
	
	m_indexY = frustrum->minY;

	CImageT<unsigned char> image2 = *image;
	image2.save(path);
	
	
}


/*This function will calculate the reflected ray
I don't knoq until now, how the material will affect the angle of reflection, so I'll also consider it
perfect reflection
*/
Vec3d CRayTracer::calculateReflectedRay(CRay incidentRay, Vec3d normal) {
	
	//incidentRay.direction = incidentRay.direction.normalize();
	Vec3d direction = incidentRay.direction - (2 * (incidentRay.direction | normal) * normal) ;
	return direction;
}


//This is the function that implements the Phong model (BRDF)
//It takes many factors into calculations like the material class(diffusion,reflection and dispersion)
//And the return value will be multiplied by the cousin factor 
Vec3f	CRayTracer::calculateColor(CRay a_Ray, int recDepth) {
	bool defineFresnel = 0;
	Vec3f shadowColor(0,0,0), reflectedColor(0,0,0), refractedColor(0,0,0);

	if (recDepth > REFLECTION_DEPTH) return BLACK;
	
	list<CLight*>::iterator lightsIt;
	for (lightsIt = lightsList.begin(); lightsIt!= lightsList.end(); lightsIt++) {
		CLight *light = (CLight*)(*lightsIt);
		double distance = MAX_DISTANCE;
		int result = light->isIntersect(a_Ray,distance);
		if (result == INTERSECTION_HIT || result == INTERSECTION_INSIDE ) return Vec3f(1,1,1);
	}

	Vec3f calcColor(0,0,0);
	CPrimitive *prim = 0;
	int result = 0;
	double distance = MAX_DISTANCE;
	
	list<CPrimitive*>::iterator objIt;
	for (objIt = sceneObjects.begin(); objIt != sceneObjects.end(); objIt++) {
		CPrimitive *tmpPrim = (CPrimitive*)(*objIt);
		int res = tmpPrim->isIntersect(a_Ray,distance);
		if ( res == INTERSECTION_HIT || res == INTERSECTION_INSIDE ) {
			result = res;
			prim = tmpPrim;
		}
	}
	

	if (!prim) return BLACK;

	//This is the intersection time
	//calculating the Phong color
	Vec3d pi = a_Ray.origin + a_Ray.direction*distance;
	//CSpehere *sphere = (CSpehere*)prim;
	//double diff = (pi - sphere->center).norm();
	Vec3d normal = prim->getNorm(pi);

	//If this is the first round of raytrace flip he vector. That's because its from the camera, 
	//otherwise it will be fliped upon itself and you get reflection on the same object
	a_Ray.direction = a_Ray.direction.normalize();
	calcColor = calculateShadowLight(a_Ray,prim->material,pi,normal ,prim->getObjRef() );

	
	//Adding reflection if applicable
	if (prim->material.reflection>0) {
		Vec3d reflectedVector = calculateReflectedRay(a_Ray, normal);
		CRay reflectedRay(pi,reflectedVector);
		calcColor += prim->material.reflection * calculateColor(reflectedRay, recDepth + 1);
	}

	if (prim->material.reflection > 0 ) {
		double cosTheta = ( normal | a_Ray.direction );
		Vec3d *refractedVector = 0;

		// < 90° , we are going from outside to inside
		if (cosTheta>0) {
			refractedVector = 	calculateRefractedVector(a_Ray,normal,1.0,prim->material.refractiveIndex);
		}
		// > 90° , Going from inside to outside
		else {
			refractedVector = 	calculateRefractedVector(a_Ray,normal,prim->material.refractiveIndex,1.0);
		}
		if (refractedVector != 0) {
			CRay refractedRay(pi,*refractedVector);
			calcColor += prim->material.refraction * calculateColor(refractedRay,recDepth + 1);
		}
	}
	return calcColor;
}

Vec3f	CRayTracer::calculateShadowLight(CRay ray, CMaterial material, Vec3d intersectionPoint, Vec3d normal, int objRef) {

	Vec3f color(0,0,0);
	ray.direction = -ray.direction;
	//Now Iterating through the lights and add the BRDF/Reflection/refraction
	list<CLight*>::iterator lightsIt;
	for (lightsIt = lightsList.begin(); lightsIt!= lightsList.end(); lightsIt++) {
		bool isIntersect = false;
		CLight *light = (CLight*)(*lightsIt);
		//Creating a light ray from the intersection point to the light
		//We want to make sure there|s nothing in the way, that|s why we don|t do it in the opposite direction
		//Otherwise we will hit our object
		//CRay lightRay( intersectionPoint, intersectionPoint + light->center * EPSILON);
		CRay lightRay(light->center, light->center + intersectionPoint* EPSILON);
		lightRay.direction = lightRay.direction.normalize();

		CRay testRay(intersectionPoint,intersectionPoint + light->center * EPSILON); 
		testRay.direction = testRay.direction.normalize();
		list<CPrimitive*>::iterator objIt;
		for (objIt = sceneObjects.begin(); objIt!= sceneObjects.end(); objIt++) {
			CPrimitive *prim = (CPrimitive*)(*objIt);
			//Sometimes the intersection point is a few millimetres inside the Sphere, and it will be considered as intersection
			//we use the objref to avoid this case, since close to the surface = on the surface
			if (prim->getObjRef() == objRef) continue;
			double distance = MAX_DISTANCE;
			int result = prim->isIntersect(testRay,distance);
			if ( (result == INTERSECTION_HIT ) && distance > EPSILON ) {
				isIntersect = true;
				break;
			}
		}//end of for loop on objects
		//Now clauclating the phong model
		if (!isIntersect) {
			//CRay lightRay(light->center, light->center + intersectionPoint*EPSILON);
			
			double cosTheta = normal | lightRay.direction;
			if (cosTheta > 0 ) {
				color += light->material.ambient* cosTheta * calculateBRDF(material,normal,lightRay.direction,ray.direction);
			}
		}
	}
	return color;
}


//This method is to calculate the Blinn-Phong model using the equation given in the sides
Vec3f	CRayTracer::calculateBRDF(CMaterial material, Vec3d normal, Vec3d lightDirection, Vec3d incidentRayDirection) {

	Vec3d H = (lightDirection + incidentRayDirection).normalize();
	double heighlight = powf( normal | H , material.emission);
	return material.diffuse + material.specular * heighlight;
}

Vec3d*	CRayTracer::calculateRefractedVector(CRay incidentRay, Vec3d normal, double refractiveIndex1, double refractiveIndex2) {
	
	//incidentRay.direction = incidentRay.direction.normalize();
	double cosThetaI = incidentRay.direction | normal ;
	double thetaI = acosf(cosThetaI);

	Vec3d B = ( incidentRay.direction + normal * cosThetaI )/( sinf(thetaI) );

	
	/*double thetaT = refractiveIndex1 * sinf(thetaI) / refractiveIndex2;
	double cosThetaT = cosf(thetaT);*/
	double cosThetaT = sqrtl( 1 - ( SQR(refractiveIndex1) * (1 - SQR(cosThetaI) ) )/SQR(refractiveIndex2) );
	double thetaT = acosl(cosThetaT);
	
	Vec3d  *refractedVector = new Vec3d( ( (double)sinf(thetaT) * B) 
		- ( cosThetaT * normal ) );
	
	return refractedVector;

}

void	CRayTracer::addMesh(CPrimitive *primitive) {
	this->sceneObjects.push_back(primitive);
}

void CRayTracer::addLight(CLight *light) {
	lightsList.push_back(light);
}

double CRayTracer::calculateFresnelReflection(double incidentAngle,double refractedAngle) {

	double fresnel = 1/2 * ( ( SQR(sin(incidentAngle - refractedAngle ) ) )/SQR(sin( incidentAngle + refractedAngle ) ) + 
		SQR(tan(incidentAngle - refractedAngle) )/SQR(tan(incidentAngle + refractedAngle) )  );
	return fresnel;
}

void CRayTracer::setFrustrum(Frustrum *frustrum) {
	this->frustrum = frustrum;
	m_deltaX = (frustrum->maxX - frustrum->minX)/ image->getWidth();
	m_deltaY = ( frustrum->maxY - frustrum->minY)/image->getHeight();
	m_indexY = frustrum->minY;
}

double	CRayTracer::calculateRefractedCosAngle(double cosThetaI, double refractiveIndex1, double refractiveIndex2) {
	
	return sqrtl( 1 - ( SQR(refractiveIndex1) * (1 - SQR(cosThetaI) ) )/SQR(refractiveIndex2) );
}