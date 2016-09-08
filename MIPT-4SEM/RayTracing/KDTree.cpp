#include "KDTree.h"
#include <algorithm>

using std::sort;
using std::min;
using std::max;

const int EMPTY_COST = 15000;

class KDTree::Node {
public:
	vector<Shape*> shapes;
	BoundingBox box;
	uchar axis = 0;
	int size = 0;
	float middle = 0;
	Node* l = NULL, *r = NULL;
	Node(const BoundingBox& box) : box(box) {}
	~Node() {
		if (this->l) {
			delete this->l;
			delete this->r;
		}
	}
};

void KDTree::build(Node* t, vector<Shape*> shapes, int depth, int minSize) {
	if (shapes.size() < minSize || depth <= 0) {
		t->shapes = shapes;
		return;
	}
	float SAH = INFINITY;
	int axis = 0;
	for (int i = 1; i < 3; i++) {
		if (t->box.v1[i] - t->box.v0[i] > t->box.v1[axis] - t->box.v0[axis]) {
			axis = i;
		}
	}
	sort(shapes.begin(), shapes.end(), [axis](const Shape* a, const Shape* b) {return a->boundingbox.v0[axis] < b->boundingbox.v0[axis]; });
	float tMin = t->box.v0[axis], tMax = t->box.v1[axis];
	for (int i = 0; i < shapes.size(); ++i) {
		float tMid = shapes[i]->boundingbox.v0[axis] - EPS;
		float newSAH = (tMid - tMin) * i + (tMax - tMid) * (shapes.size() - i);
		if (newSAH < SAH) {
			t->middle = tMid;
			SAH = newSAH;
			t->axis = axis;
		}
	}

	sort(shapes.begin(), shapes.end(), [axis](const Shape* a, const Shape* b) {return a->boundingbox.v1[axis] < b->boundingbox.v1[axis]; });
	for (int i = 0; i < shapes.size(); ++i) {
		float tMid = shapes[i]->boundingbox.v1[axis] + EPS;
		float newSAH = (tMid - tMin) * (i + 1) + (tMax - tMid) * (shapes.size() - i - 1);
		if (newSAH < SAH) {
			t->middle = tMid;
			SAH = newSAH;
			t->axis = axis;
		}
	}
	vector<Shape*> left, right;

	for (Shape* shape : shapes) {
		const BoundingBox& box = shape->boundingbox;
		if (box.v0[t->axis] <= t->middle) {
			left.push_back(shape);
		}
		if (box.v1[t->axis] >= t->middle) {
			right.push_back(shape);
		}
	}
	//if (EMPTY_COST + left.size() * (t->middle - t->box.v0[t->axis]) + right.size() * (t->box.v1[t->axis] - t->middle) > shapes.size() * (t->box.v1[t->axis] - t->box.v0[t->axis])) {
	if (left.size() > 0.99 * shapes.size() || right.size() > 0.99 * shapes.size() || left.size() + right.size() > 1.6 * shapes.size()) {
		t->shapes = shapes;
		return;
	}
	build(t->l = new Node(t->box.left(t->axis, t->middle)), left, depth - 1);
	build(t->r = new Node(t->box.right(t->axis, t->middle)), right, depth - 1);
}

KDTree::KDTree(vector<Shape*> shapes) : shapes(shapes.begin(), shapes.end()) {
	BoundingBox box = shapes[0]->boundingbox;
	for (int i = 1; i < shapes.size(); ++i) {
		box.expand(shapes[i]->boundingbox);
	}
	root = new Node(box);
	build(root, shapes);
}

KDTree::~KDTree() {
	delete root;
}

TraceResult KDTree::collideRayWithNode(const Ray& ray, Node* t) const {
	TraceResult result;
	for (size_t i = 0; i < t->shapes.size(); ++i) {
		TraceResult tmp = t->shapes[i]->intersect(ray);
		if (tmp.dist > EPS && tmp.dist < result.dist) {
			result = tmp;
		}
	}
	return result;
}

TraceResult KDTree::traceRay(const Ray& ray, Node* t, float tMin, float tMax) const {	
	if (!t->l) {
		return collideRayWithNode(ray, t);
	}
	Node* l = t->l, *r = t->r;
	float tMid = ray.tAt(t->axis, t->middle);
	if (ray.direction[t->axis] < 0) {
		l = t->r, r = t->l;
	}
	if (tMid <= tMin) {
		return traceRay(ray, r, tMid, tMax);
	}
	if (tMid >= tMax) {
		return traceRay(ray, l, tMin, tMid);
	}
	TraceResult result = traceRay(ray, l, tMin, tMid);
	if (result.dist > tMid + EPS) {
		result = traceRay(ray, r, tMid, tMax);
	}
	return result;
}

TraceResult KDTree::traceRay(const Ray& ray, float dist) const {
	float tMin, tMax;
	if (!root->box.collide(ray, tMin, tMax)) {
		return TraceResult();
	}
	return traceRay(ray, root, tMin, min(dist, tMax));
}
