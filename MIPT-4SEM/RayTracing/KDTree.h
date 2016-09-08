#ifndef KDTREE_H
#define KDTREE_H

#include "ShapeContainer.h"

#include <vector>

using std::vector;

class KDTree : public ShapeContainer {
private:
	class Node;

	Node* root;
	vector<Shape*> shapes;
	
	void build(Node* t, vector<Shape*> shapes, int depth = 32, int minSize = 32);
	TraceResult collideRayWithNode(const Ray& ray, Node* t) const;
	TraceResult traceRay(const Ray& ray, Node* t, float tMin, float tMax) const;
public:
	KDTree(vector<Shape*> shapes);
	~KDTree();

	TraceResult traceRay(const Ray& ray, float dist = INFINITY) const;
};

#endif