#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3f.h"
#include "Matrix3f.h"
#include "Ray.h"

class Camera {
public:
	Vector3f p[3], pos;
	Vector3f x, y, z;
	int width, height;
	Camera(){}
	Camera(int width, int height, Vector3f p0, Vector3f p1, Vector3f p2, Vector3f pos) : width(width), height(height), pos(pos) {
		p[0] = p0;
		p[1] = p1;
		p[2] = p2;
		x = (p2 - p0).normalized();
		y = ((p2 - p0) % (p1 - p0)).normalized();
		z = (p0 - p1).normalized();
	}

	void rotateX(float angle) {
		Matrix rot = Matrix::rotation(x, angle);
		z = rot * z;
		y = rot * y;
		for (int i = 0; i < 3; i++) {
			p[i] = rot * (p[i] - pos) + pos;
		}
	}

	void rotateZ(float angle) {
		Matrix rot = Matrix::rotation(z, angle);
		x = rot * x;
		y = rot * y;
		for (int i = 0; i < 3; i++) {
			p[i] = rot * (p[i] - pos) + pos;
		}
	}

	void move(const Vector3f& d) {
		pos = pos + d;
		for (int i = 0; i < 3; ++i) {
			p[i] += d;
		}
	}

	Ray getRay(int x, int y) {
		return Ray(pos, (p[2] - p[0]) * (static_cast<float>(x) / static_cast<float>(width)) + (p[1] - p[0]) * ((height - static_cast<float>(y)) / static_cast<float>(height)) + p[0] - pos);
	}

	Vector3f getMiddle() {
		return (p[1] - p[0]) * 0.5f + (p[2] - p[0]) * 0.5f + p[0];
	}
};

#endif