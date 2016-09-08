#pragma once

#include "Shape.h"

class TraceResult {
public:
	const Shape* shape;
	Vector3f pos, normal;
	Color color;
	float dist;
	bool face;
	TraceResult(const Shape* shape = NULL, const Color& color = Color(), const Vector3f& pos = Vector3f(), const Vector3f& normal = Vector3f(), float dist = INFINITY, bool face = true) : shape(shape), color(color), pos(pos), normal(normal), dist(dist), face(face) {}
};