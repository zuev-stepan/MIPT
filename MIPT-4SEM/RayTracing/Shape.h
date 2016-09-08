#ifndef SHAPE_H
#define SHAPE_H

#include "Material.h"
#include "BoundingBox.h"

class Shape {
public:
	class TraceResult {
	public:
		const Shape* shape;
		Vector3f pos, normal;
		Color color;
		float dist;
		bool face;
		TraceResult(const Shape* shape = NULL,
					const Color& color = Color(),
					const Vector3f& pos = Vector3f(),
					const Vector3f& normal = Vector3f(),
					float dist = INFINITY, bool face = true) : 
			shape(shape), color(color), pos(pos), normal(normal), dist(dist), face(face)
		{}
	};

	const Material* material;
	BoundingBox boundingbox;

	Shape(const Material* material) : material(material) {

	}

	Shape(const Material* material, const BoundingBox& boundingbox) :
		material(material), 
		boundingbox(boundingbox) 
	{}

	virtual ~Shape() {}

	virtual TraceResult intersect(const Ray& ray) const = 0;
	
	const Material* getMaterial() {
		return material;
	}
};

typedef Shape::TraceResult TraceResult;

#endif