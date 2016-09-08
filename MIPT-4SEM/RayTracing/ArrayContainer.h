#ifndef ARRAYCONTAINER_H
#define ARRAYCONTAINER_H

#include "ShapeContainer.h"
#include "Tracing.h"
#include "Premitives.h"
#include <vector>

using std::vector;

class ArrayContainer : public ShapeContainer {
private:
	vector<Shape*> shapes;
public:
	ArrayContainer(vector<Shape*> shapes) : shapes(shapes.begin(), shapes.end()) {}
	~ArrayContainer(){}

	TraceResult ArrayContainer::traceRay(const Ray& ray, float dist = INFINITY) const {
		TraceResult result;
		for (size_t i = 0; i < shapes.size(); ++i) {
			TraceResult t = shapes[i]->intersect(ray);
			if (t.dist > 0 && t.dist <= dist && t.dist < result.dist) {
				result = t;
			}
		}
		return result;
	}
};

#endif