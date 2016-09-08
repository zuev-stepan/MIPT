#ifndef SHAPE3D_H
#define SHAPE3D_H

#include "Shape.h"
#include "Ray.h"
#include <vector>

using std::vector;

// TODO
class Shape3d : public Shape {
public:
	virtual ~Shape3d() {}

	virtual vector<TraceResult> inOut(const Ray& ray) const = 0;
};

#endif