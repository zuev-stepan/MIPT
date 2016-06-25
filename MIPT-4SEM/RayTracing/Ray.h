#ifndef RAY_H
#define RAY_H

#include "Vector3f.h"

class Space {
public:
	float n;
	const Space* parent;
	Space() : n(1), parent(NULL) {}
	Space(float n, const Space* parent) : n(n), parent(parent) {}
};

class Ray {
public:
	Vector3f pos, direction, invDirection;
	Space space;

	Ray() {}
	Ray(const Vector3f& pos, const Vector3f& direction, const Space& space = Space()) : pos(pos), direction(direction.normalized()), space(space) {
		this->invDirection = Vector3f(1.0f / this->direction.x, 1.0f / this->direction.y, 1.0f / this->direction.z);
	}
	Vector3f pointAt(float t) const {
		return this->pos + this->direction * t;
	}

	float tAt(int axis, float p) const {
		return (p - this->pos[axis]) * this->invDirection[axis];
	}
};

#endif