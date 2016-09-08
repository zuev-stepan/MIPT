#ifndef SHAPECONTAINER_H
#define SHAPECONTAINER_H

#include "Shape.h"

class ShapeContainer {
public:
	virtual ~ShapeContainer(){}
	virtual TraceResult traceRay(const Ray& ray, float dist = INFINITY) const = 0;
};

#endif	