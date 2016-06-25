#ifndef SOLID_COLOR_H
#define SOLID_COLOR_H

#include "Material.h"
#include "Color.h"


class SolidColor : public Material {
public:
	Color color;

	SolidColor(const Color& color = Color(100, 100, 100), 
			   float reflectivity = 0.0f, float transparency = 0.0f, float refractivity = 1.0f, float roughness = 0.0f) :
		Material(reflectivity, transparency, refractivity, roughness),
		color(color)
	{}

	const Color& getColor(float u = 0, float v = 0) const {
		return color;
	}
};

#endif