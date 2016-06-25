#ifndef COMPLEX_H
#define COMPLEX_H

#include "Sphere.h"
#include "Quadrangle.h"
#include <algorithm>

using std::upper_bound;

class ComplexShape : public Shape {
public:
	static const uchar AND = 0;
	static const uchar OR = 1;
	static const uchar SUB = 2;
	static const uchar BOX = 3;
	static const uchar SPHERE = 4;
	static const uchar QUAD = 5;
	
	uchar type;
	ComplexShape* left = NULL, *right = NULL;
	Sphere* sphere = NULL;
	Quadrangle* quadrangle = NULL;

	ComplexShape(Sphere* shape) : Shape(shape->material, shape->boundingbox), type(SPHERE), sphere(shape) {}
	ComplexShape(Quadrangle* shape) : Shape(shape->material, shape->boundingbox), type(QUAD), quadrangle(shape) {}
	ComplexShape(ComplexShape* left, ComplexShape* right, uchar type) : Shape(left->material), left(left), right(right), type(type) {
		boundingbox = left->boundingbox;
		boundingbox.expand(right->boundingbox);
	}

	vector<TraceResult> inOut(const Ray& ray) const {
		vector<TraceResult> result;
		if (type == BOX) {
			return vector<TraceResult>();
		}
		else if (type == SPHERE) {
			Vector3f k = ray.pos - sphere->pos;
			float b = k * ray.direction;
			float c = k * k - sqr(sphere->r);
			float d = b * b - c;

			if (d >= 0) {
				vector<TraceResult> result;
				float sqrtfd = sqrt(d);
				float t1 = -b + sqrtfd;
				float t2 = -b - sqrtfd;
				if (t1 > t2) {
					swap(t1, t2);
				}
				Vector3f tmp = ray.pointAt(t1);
				result.push_back(TraceResult(this, material->getColor(), tmp, (tmp - sphere->pos) * sphere->revR, t1));
				tmp = ray.pointAt(t2);
				result.push_back(TraceResult(this, material->getColor(), tmp, (sphere->pos - tmp) * sphere->revR, t2));
				return result;
			}
			return vector<TraceResult>();
		}
		else if (type == QUAD) {
			result = vector<TraceResult>();
			TraceResult tmp = quadrangle->intersect(ray);
			if (tmp.shape) {
				result.push_back(tmp);
			}
			return result;
		}
		else if (type == OR) {
			vector<TraceResult> leftInOut = left->inOut(ray);
			vector<TraceResult> rightInOut = right->inOut(ray);
			int i = 0, j = 0;
			vector<TraceResult> result;
			int balance = 0;
			while (i < leftInOut.size() && j < rightInOut.size()) {
				if (leftInOut[i].dist < rightInOut[j].dist) {
					balance -= (i % 2 != 0);
					if (balance == 0) {
						result.push_back(TraceResult(this, leftInOut[i].color, leftInOut[i].pos, leftInOut[i].normal, leftInOut[i].dist));
					}
					balance += (i % 2 == 0);
					i++;
				}
				else {
					balance -= (j % 2 != 0);
					if (balance == 0) {
						result.push_back(TraceResult(this, rightInOut[j].color, rightInOut[j].pos, rightInOut[j].normal, rightInOut[j].dist));
					}
					balance += (j % 2 == 0);
					j++;
				}
			}
			while (i < leftInOut.size()) {
				result.push_back(leftInOut[i++]);
				result.back().shape = this;
			}
			while (j < rightInOut.size()) {
				result.push_back(rightInOut[j++]);
				result.back().shape = this;
			}
			return result;
		}
		else if (type == AND) {
			vector<TraceResult> leftInOut = left->inOut(ray);
			vector<TraceResult> rightInOut = right->inOut(ray);
			int i = 0, j = 0;
			vector<TraceResult> result;
			bool inLeft = false, inRight = false;
			while (i < leftInOut.size() && j < rightInOut.size()) {
				if (leftInOut[i].dist < rightInOut[j].dist) {
					if (inRight) {
						result.push_back(TraceResult(this, leftInOut[i].color, leftInOut[i].pos, leftInOut[i].normal, leftInOut[i].dist));
					}
					inLeft = (i % 2 == 0);
					i++;
				}
				else {
					if (inLeft) {
						result.push_back(TraceResult(this, leftInOut[i - 1].color, rightInOut[j].pos, rightInOut[j].normal, rightInOut[j].dist));
					}
					inRight = (j % 2 == 0);
					j++;
				}
			}
			return result;
		}
		else if (type == SUB) {
			vector<TraceResult> leftInOut = left->inOut(ray);
			vector<TraceResult> rightInOut = right->inOut(ray);
			int i = 0, j = 0;
			vector<TraceResult> result;
			bool inLeft = false, inRight = false;
			while (i < leftInOut.size() && j < rightInOut.size()) {
				if (leftInOut[i].dist < rightInOut[j].dist) {
					if (!inRight) {
						result.push_back(TraceResult(this, leftInOut[i].color, leftInOut[i].pos, leftInOut[i].normal, leftInOut[i].dist));
					}
					inLeft = (i % 2 == 0);
					i++;
				}
				else {
					if (inLeft) {
						result.push_back(TraceResult(this, leftInOut[i - 1].color, rightInOut[j].pos, rightInOut[j].normal, rightInOut[j].dist));
					}
					inRight = (j % 2 == 0);
					j++;
				}
			}
			while (i < leftInOut.size()) {
				result.push_back(leftInOut[i++]);
				result.back().shape = this;
			}
			return result;
		}
	}

	TraceResult intersect(const Ray& ray) const {
		vector<TraceResult> t = inOut(ray);
		if (t.size() == 0 || t.back().dist <= 0) {
			return TraceResult();
		}
		return *upper_bound(t.begin(), t.end(), TraceResult(NULL, Color(), Vector3f(), Vector3f(), 0), [](const TraceResult& a, const TraceResult& b) {
			return a.dist < b.dist;
		});
	}
};

#endif