#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape {
public:
	Vector3f p[3];
	Vector3f normal;

	Triangle(const Vector3f p[], const Vector3f& normal, const Material* material) :
		Shape(material, BoundingBox(p, 3)),
		normal(normal)
	{
		for (int i = 0; i < 3; ++i) {
			this->p[i] = p[i];
		}
	}

	Triangle(const Vector3f p[], const Material* material) :
		Triangle(p, ((p[1] - p[0]) % (p[2] - p[0])).normalized(), material)
	{}

	TraceResult intersect(const Ray& ray) const {
		Vector3f E1 = this->p[1] - this->p[0];
		Vector3f E2 = this->p[2] - this->p[0];
		Vector3f P = ray.direction % E2;
		float S = E1 * P;
		if (abs(S) < EPS) {
			return TraceResult();
		}
		S = 1 / S;

		Vector3f T = ray.pos - p[0];
		float u = T * P * S;
		if (u < -EPS || u > 1 + EPS) {
			return TraceResult();
		}

		Vector3f Q = T % E1;
		float v = ray.direction * Q * S;
		if (v < -EPS || u + v > 1 + EPS) {
			return TraceResult();
		}

		float t = E2 * Q * S;
		Vector3f norm = this->normal;
		bool face = true;
		if (norm * ray.direction > 0) {
			norm = norm * -1;
			face = false;
		}
		return TraceResult(this, material->getColor(u, v), ray.pointAt(t), norm, t, face);
	}
};

#endif