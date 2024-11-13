#pragma once
#include <cmath>
#include <string>
constexpr double PI = 3.14159265358979323846;
#define DEG_TO_RAD PI/180
#define RAD_TO_DEG 180/PI

class Point3D {
public:
	double x, y, z;
	Point3D() {
		x = 0;
		y = 0;
		z = 0;
	}
	Point3D(double i, double j, double k) {
		x = i;
		y = j;
		z = k;
	}
	Point3D(Point3D a, Point3D b) {
		x = a.x + b.x;
		y = a.y + b.y;
		z = a.z + b.z;
	}
	inline void Equal(Point3D p) {
		x = p.x;
		y = p.y;
		z = p.z;
	}	inline double Norm() {
		return sqrt(x * x + y * y + z * z);
	}
	inline void Unit() {
		double norm = Norm();
		x /= norm;
		y /= norm;
		z /= norm;
	}
};

class Quaternion {
public:
	double w;
	Point3D u;
	Quaternion() {
		w = 0;
		u.x = 0;
		u.y = 0;
		u.z = 0;
	}
	Quaternion(double real, double i, double j, double k) {
		w = real;
		u.x = i;
		u.y = j;
		u.z = k;
	}
	Quaternion(Point3D v, double real) {
		w = real;
		u.x = v.x;
		u.y = v.y;
		u.z = v.z;
	}
	Quaternion(double angle, Point3D v, bool degree = true) {
		v.Unit();
		degree ? angle *= PI / (180 * 2) : angle /= 2;
		w = cos(angle);
		u.x = sin(angle) * v.x;
		u.y = sin(angle) * v.y;
		u.z = sin(angle) * v.z;
	}
	inline void Equal(Quaternion q) {
		w = q.w;
		u.x = q.u.x;
		u.y = q.u.y;
		u.z = q.u.z;
	}

	/*Quaternion operator*(const Quaternion& other) const {
		Quaternion tmp;
		tmp.w = ((w * other.w) - (u.x * other.u.x) - (u.y * other.u.y) - (u.z * other.u.z));
		tmp.u.x = ((w * other.u.x) + (u.x * other.w) + (u.y * other.u.z) - (u.z * other.u.y));
		tmp.u.y = ((w * other.u.y) - (u.x * other.u.z) + (u.y * other.w) + (u.z * other.u.x));
		tmp.u.z = ((w * other.u.z) + (u.x * other.u.y) - (u.y * other.u.x) + (u.z * other.w));
		return tmp;
	}*/

	inline void Multiply(const Quaternion q) {
		Quaternion tmp;
		tmp.w = ((w * q.w) - (u.x * q.u.x) - (u.y * q.u.y) - (u.z * q.u.z));
		tmp.u.x = ((w * q.u.x) + (u.x * q.w) + (u.y * q.u.z) - (u.z * q.u.y));
		tmp.u.y = ((w * q.u.y) - (u.x * q.u.z) + (u.y * q.w) + (u.z * q.u.x));
		tmp.u.z = ((w * q.u.z) + (u.x * q.u.y) - (u.y * q.u.x) + (u.z * q.w));
		*this = tmp;
	}

	void Rotate(Quaternion q) {
		Quaternion tmp;
		tmp.Equal(q);
		//tmp * (*this); tmp * q.Conjugate();
		tmp.Multiply(*this);
		tmp.Multiply(q.Conjugate());
		*this = tmp;
	}

	inline double Norm() {
		return sqrt(u.x * u.x + u.y * u.y + u.z * u.z + w * w);
	}

	Quaternion Conjugate() {
		Quaternion tmp;
		tmp.w = w;
		tmp.u.x = -u.x;
		tmp.u.y = -u.y;
		tmp.u.z = -u.z;
		return tmp;
	}

	inline void Inverse() {
		double norm = Norm();
		Conjugate();
		u.x /= norm;
		u.y /= norm;
		u.z /= norm;
		w /= norm;
	}

	float getAngle(bool degree = true) {
		if (w > 1) return 0;
		float angle = 2 * acosf(w);
		if (degree) angle *= PI / (180);
		return angle;
	}

	void ExportToMatrix(float matrix[16]) {
		float wx, wy, wz, xx, yy, yz, xy, xz, zz;

		// adapted from Shoemake
		xx = u.x * u.x;
		xy = u.x * u.y;
		xz = u.x * u.z;
		yy = u.y * u.y;
		zz = u.z * u.z;
		yz = u.y * u.z;

		wx = w * u.x;
		wy = w * u.y;
		wz = w * u.z;

		matrix[0] = 1.0f - 2.0f * (yy + zz);
		matrix[4] = 2.0f * (xy - wz);
		matrix[8] = 2.0f * (xz + wy);
		matrix[12] = 0.0;

		matrix[1] = 2.0f * (xy + wz);
		matrix[5] = 1.0f - 2.0f * (xx + zz);
		matrix[9] = 2.0f * (yz - wx);
		matrix[13] = 0.0;

		matrix[2] = 2.0f * (xz - wy);
		matrix[6] = 2.0f * (yz + wx);
		matrix[10] = 1.0f - 2.0f * (xx + yy);
		matrix[14] = 0.0;

		matrix[3] = 0;
		matrix[7] = 0;
		matrix[11] = 0;
		matrix[15] = 1;
	}
};
