#ifndef QUADRANGLE_H
#define QUADRANGLE_H

#include "Shape.h"

class Quadrangle : public Shape {
public:
	Vector3f p[4];
	Vector3f normal;

	Quadrangle(const Vector3f p[], const Vector3f& normal, const Material* material) :
		Shape(material, BoundingBox(p, 4)),
		normal(normal)
	{
		for (int i = 0; i < 4; ++i) {
			this->p[i] = p[i];
		}
	}

	Quadrangle(const Vector3f p[], const Material* material)
		: Quadrangle(p, ((p[1] - p[0]) % (p[2] - p[0])).normalized(), material) {
	}

	TraceResult intersect(const Ray& ray) const {
		float u[2], v[2];
		for (int i = 0; i <= 1; ++i) {
			Vector3f E1 = (this->p[1] - this->p[i * 2]);
			Vector3f E2 = (this->p[3] - this->p[i * 2]);
			Vector3f P = ray.direction % E2;
			float S = E1 * P;
			if (abs(S) < EPS) {
				continue;
			}
			S = 1 / S;

			Vector3f T = ray.pos - p[i * 2];
			u[i] = T * P * S;
			Vector3f Q = T % E1;
			v[i] = ray.direction * Q * S;
			if (u[i] < -EPS || u[i] > 1 + EPS) {
				continue;
			}
			if (v[i] < -EPS || u[i] + v[i] > 1 + EPS) {
				continue;
			}
			float t = E2 * Q * S;
			Vector3f norm = this->normal;
			bool face = true;
			if (norm * ray.direction > 0) {
				norm = norm * -1;
				face = false;
			}
			return TraceResult(this, material->getColor(u[0], v[0]), ray.pointAt(t), norm, t, face);
		}
		return TraceResult();
	}
};

#endif