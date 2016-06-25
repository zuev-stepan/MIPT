#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	Vector3f pos;
	float r, revR;

	Sphere(const Vector3f& pos, float r, const Material* material) :
		Shape(material, BoundingBox(Vector3f(pos.x - r, pos.y - r, pos.z - r), Vector3f(pos.x + r, pos.y + r, pos.z + r))),
		pos(pos),
		r(r), 
		revR(1 / r)
	{}

	TraceResult intersect(const Ray& ray) const {
		Vector3f k = ray.pos - this->pos;
		float b = k * ray.direction;
		float d = sqr(b) - k * k + sqr(r);

		if (d >= 0) {
			float sqrtfd = sqrtf(d);
			// t, a == 1
			float t1 = -b + sqrtfd;
			float t2 = -b - sqrtfd;
			if (t1 < t2 && t1 >= 0) {
				swap(t1, t2);
			}
			if (t2 < 0) {
				return TraceResult();
			}
			bool face = true;
			Vector3f ans = ray.pointAt(t2);
			Vector3f norm = ans - this->pos;
			if (norm * ray.direction > 0) {
				face = false;
				norm *= -1;
			}
			return TraceResult(this, material->getColor(), ans, norm * revR, t2, face);
		}
		return TraceResult();
	}
};

#endif