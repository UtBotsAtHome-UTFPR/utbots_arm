#pragma once
#include "mathematics.h"

class Joint {
public:
	Joint(float length, Point3D endPoint) {
		this->length = length;
		this->endPoint.u.x = endPoint.x;
		this->endPoint.u.y = endPoint.y;
		this->endPoint.u.z = endPoint.z;
		DefineOriginalEndPoint();
	}
	Joint(float length, double x, double y, double z) {
		this->length = length;
		this->endPoint.u.x = x;
		this->endPoint.u.y = y;
		this->endPoint.u.z = z;
		DefineOriginalEndPoint();
	}
	Joint(float length, float angleH, float angleV, Point3D rotAxis) {
		this->length = length;
		float i = cos(angleV);
		this->endPoint.u.x = length * cos(angleV) * cos(angleH);
		this->endPoint.u.y = length * cos(angleV) * sin(angleH);
		this->endPoint.u.z = length * sin(angleV);
		this->rotAxis = rotAxis;
		this->rotAxis.Unit();
		DefineOriginalEndPoint();
	}

	void DefineOriginalEndPoint() {
		this->endPointOriginal.u.x = this->endPoint.u.x;
		this->endPointOriginal.u.y = this->endPoint.u.y;
		this->endPointOriginal.u.z = this->endPoint.u.z;
	}

	void Rotate(Quaternion q) {
		endPoint.Rotate(q);

		Quaternion temp(rotAxis, 0);
		temp.Rotate(q);
		rotAxis.Equal(temp.u);
	}
	
	float length;
	Quaternion endPointOriginal;
	Quaternion endPoint;
	Point3D rotAxis;

	int rotationDirection = 1;
	void ReverseDirection() { rotationDirection *= -1; }
};