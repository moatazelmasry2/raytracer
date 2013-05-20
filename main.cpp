#include "raytracer.h"
#include "imaget.h"
#include "camera.h"
#include "sphere.h"
#include "plane.h"
#include "omnilight.h"


#include <stdio.h>


#define IMAGE_WIDTH	 640
#define IMAGE_HEIGHT 480
#define PATH "./output/output"
#define PATH2 ""
#define GLASS_REFRACTION (1.9 + 1.4)/2

void start();



void start() {

	CCamera *camera = new CCamera(	Vec3d(0.0f,0.0f,-10.0f ),
					Vec3d(0.0f,-2.0f,0.0f ), 
					Vec3d(0.0f,0.0f,1.0f ) );

	
	CImageT<double> image(IMAGE_WIDTH,IMAGE_HEIGHT);
	CRayTracer tracer(camera,&image);
	tracer.setFrustrum(new Frustrum(-4,-5,4,5) );
	
	COmniLight *light = new COmniLight(Vec3d(10, 0, -25), 0.1);
	light->material.ambient = Vec3f(0.7f,0.7f,0.7f);
	tracer.addLight(light);
	COmniLight *light2 = new COmniLight(Vec3d(10, 0, -25), 0.1);
	light2->material.ambient = Vec3f(0.7,0.7,0.7);
	tracer.addLight(light2);

	
	CSpehere *sphere1 = new CSpehere( Vec3d(1, -2.5, 5),1 );
	sphere1->material.diffuse  = Vec3f(0.7,0.6,0.4);
	sphere1->material.specular = Vec3f(0.4,0.6,0.7);
	sphere1->material.refractiveIndex = GLASS_REFRACTION;

	CSpehere *sphere2 = new CSpehere( Vec3d(-3.5f, -2.5, 5),1 );
	sphere2->material.diffuse  = Vec3f(0.2,0.6,0.4);
	sphere2->material.specular = Vec3f(0.2,0.4,0.6);
	sphere2->material.refractiveIndex = GLASS_REFRACTION;

	CSpehere *sphere3 = new CSpehere( Vec3d(-3,2,2),1 );
	sphere3->material.diffuse  = Vec3f(0.2,0.6,0.4);
	sphere3->material.specular = Vec3f(0.1,0.6,0.4);
	sphere3->material.refractiveIndex = GLASS_REFRACTION;

	CSpehere *sphere4 = new CSpehere( Vec3d(7,0,-2),2 );
	sphere4->material.diffuse  = Vec3f(0.1,0.6,0.7);
	sphere4->material.specular = Vec3f(0.4,0.7,0.9);
	sphere4->material.refractiveIndex = GLASS_REFRACTION;

	CPlane *plane1 = new CPlane(Vec3f(0,2,0),Vec3f(0,2,0),Vec3f(0,2,0),-50.0);
	plane1->material.diffuse = Vec3f(0.2,0.3,0.5);
	plane1->material.specular = Vec3f(0.5,0.6,0.8);


	tracer.addMesh(sphere1);
	tracer.addMesh(sphere2);
	//tracer.addMesh(sphere3);
	//tracer.addMesh(sphere4);
	tracer.addMesh(plane1);


	int axis = 0;
	for (int i=0; i< 25; i++) {
		std::string path = PATH;
		char *c = new char();
		itoa(i,c,10);
		path = path.append(c);
		path = path.append(".ppm");

		light->center = Vec3d(axis, 5, -25);
		light2->center= Vec3d(-axis,5,-25);
		axis += 2;
		tracer.startRayTracing(path.c_str() );
	}
	//tracer.startRayTracing(PATH);
	
}

   
void main() {

	start();
}
