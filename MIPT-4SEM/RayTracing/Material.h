#ifndef SHAPE_PROPERTIES_H
#define SHAPE_PROPERTIES_H

#include "Color.h"

class Material {
public:
	float reflectivity, transparency, roughness, n;

	virtual ~Material() {}
	virtual const Color& getColor(float u = 0, float v = 0) const = 0;
	
	Material(float reflectivity = 0.0f, float transparency = 0.0f, float n = 1.0f, float roughness = 0.0f) {
		this->reflectivity = reflectivity;
		this->transparency = transparency;
		this->roughness = roughness;
		this->n = n;
	}
};

#endif