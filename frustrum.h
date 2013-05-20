/**
This struct is used to describe the view port, i.e. How much from the objects infront of the camera
*/
struct Frustrum {
	
	double minX, minY, maxX, maxY; //The four points used to describe the area of the frustrum
	double nearestP, furthestP; //Nearest and furthest point the camera can see
	Frustrum(double minX,double minY, double maxX, double maxY,double nearestP=0, double furthestP=1000):
	minX(minX),maxX(maxX),minY(minY),maxY(maxY),nearestP(nearestP),furthestP(furthestP) {
	}

	Frustrum() {

	}
	Frustrum operator=(Frustrum frust) {

		Frustrum temp;
		temp.furthestP = frust.furthestP;
		temp.nearestP  = frust.nearestP;
		temp.maxX		= frust.maxX;
		temp.maxY		= frust.maxY;
		temp.minX		= frust.minX;
		temp.minY		= frust.minX;

		return temp;
	}
};