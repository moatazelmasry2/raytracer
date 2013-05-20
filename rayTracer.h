#include "ray.h"
#include "primitive.h"
#include "colort.h"
#include "imaget.h"
#include "camera.h"
#include "matrixt.h"
#include "plane.h"
#include "sphere.h"
#include "light.h"

#include <math.h>
#include <list>
using namespace std;


//How many reflections of a light beam to calculate
#define RECURSION_DEPTH 3
//EPSILON will be usually used to create a direction vector only short distance from an origin vector of a ray
#define EPSILON 0.1
//If the distance between the primitive and the light source, the light souce wil not be 
//considered when calculating the illumination
#define MAX_DISTANCE 1000.0f
#define BLACK Vec3f(0,0,0);

class CRayTracer {

private:

		
	/*CRay *rays;
	CPrimitive *primitives;*/
	CCamera *camera;
	Frustrum *frustrum;
	list<CPrimitive*> sceneObjects;
	CImageT<double> *image;
	list<CLight*> lightsList;
	double m_indexX, m_indexY;
	double m_deltaX, m_deltaY;


public:
	//this method takes in all the light sources and primitives in the scene
	//CRayTracer(CRay *rays,CPrimitive *primitives);

	CRayTracer(CCamera *camera,CImageT<double> *image);

	//void					addPrimitive(CPrimitive primitive);
	void					addMesh(CPrimitive *primitive);
	Vec3d					calculateReflectedRay(CRay incidentRay, Vec3d normal);
	//CColor3T<double>			calculateColor(CPrimitive *primitive,CRay ray, int depth = 1);
	Vec3f					calculateColor(CRay a_Ray, int recDepth);
	//CRay					calculateNorm(CPrimitive primitive, Vec3d point);
	void					startRayTracing(const char *path);
	void					addLight(CLight *light);
	
	/*
	The method is used to calculate the refracted ray that travels from one medium to the other
	 using the incident ray and the normal to the surface
	*/
	Vec3d*					calculateRefractedVector(CRay incidentRay, Vec3d normal, double refractiveIndex1, double refractiveIndex2);
	/**
	Calculates the color based on Fresnel equation. See the slides for more details
	*/
	double					calculateFresnelReflection(double incidentAngle,double refractedAngle);
	double					calculateRefractedCosAngle(double cosThetaI, double refractiveIndex1, double refractiveIndex2);
	/*
	*/
	Vec3f					calculateShadowLight(CRay ray, CMaterial material, Vec3d intersectionPoint, Vec3d normal, int objRef);
	/**
	*/
	Vec3f					calculateBRDF(CMaterial material, Vec3d normal, Vec3d lightDirection, Vec3d incidentRayDirection);

	Frustrum*				getFrustrum(){return frustrum;}

	void					setFrustrum(Frustrum *frustrum);
};