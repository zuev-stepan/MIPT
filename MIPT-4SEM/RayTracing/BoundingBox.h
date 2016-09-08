#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "Vector3f.h"
#include "Ray.h"
#include "Utils.h"

class BoundingBox {
public:
	Vector3f v0, v1;
	BoundingBox(){}
	BoundingBox(const Vector3f& v0, const Vector3f& v1) : v0(v0), v1(v1) {}
	BoundingBox(const Vector3f v[], int size) {
		v0 = v[0];
		v1 = v[1];
		for (int i = 0; i < size; ++i) {
			v0 = Vector3f::vmin(v0, v[i]);
			v1 = Vector3f::vmax(v1, v[i]);
		}
	}	

	bool collide(const Ray& ray, float& tMin, float& tMax) const {
		float t;
		tMin = 0, tMax = INFINITY;
		for (int i = 0; i < DIMENSIONS; i++) {
			float axisMin = ray.tAt(i, v0[i]);
			float axisMax = axisMin;
			t = ray.tAt(i, v1[i]);
			axisMin = min(axisMin, t);
			axisMax = max(axisMax, t);
			tMin = max(tMin, axisMin);
			tMax = min(tMax, axisMax);
		}
		return tMax >= 0 && tMin <= tMax;
	}

	bool onEdge(const Vector3f& t) const {
		int c = 0;
		for (int i = 0; i < 3; i++) {
			c += abs(v0[i] - t[i]) < 3;
			c += abs(v1[i] - t[i]) < 3;
		}
		return c > 1;
	}

	bool isInside(const Vector3f& t) const {
		return (v0.x - EPS < t.x && v0.y - EPS <= t.y && v0.z - EPS <= t.z 
				&& v1.x + EPS >= t.x && v1.y + EPS >= t.y && v1.z + EPS >= t.z);
	}

	void expand(const BoundingBox& t) {
		for (int i = 0; i < DIMENSIONS; i++) {
			v0[i] = min(v0[i], t.v0[i]);
			v1[i] = max(v1[i], t.v1[i]);
		}
	}

	void expand(const Vector3f& t) {
		for (int i = 0; i < DIMENSIONS; i++) {
			v0[i] = min(v0[i], t[i]);
			v1[i] = max(v1[i], t[i]);
		}
	}

	BoundingBox left(int axis, float split) const {
		Vector3f t = v1;
		t[axis] = split;
		return BoundingBox(v0, t);
	}

	BoundingBox right(int axis, float split) const {
		Vector3f t = v0;
		t[axis] = split;
		return BoundingBox(t, v1);
	}
};

#endif