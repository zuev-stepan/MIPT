#ifndef TEXTURED_H
#define TEXTURED_H

#include "Material.h"
#include "Image.h"

class Textured : public Material {
public:
	float ux, uy, vx, vy, x0, y0;
	float reflectivity, transparency, refractivity, roughness;
	const Image* texture;

	Textured(const Image* image, float x0, float y0, float ux, float uy, float vx, float vy, 
			 float reflectivity = 0.0f, float transparency = 0.0f, float refractivity = 0.0f, float roughness = 0.0f) :
		Material(reflectivity, transparency, refractivity, roughness),
		texture(image), 
		x0(x0), y0(y0),
		ux(ux), uy(uy), vx(vx), vy(vy)
	{}

	Textured(const Image* image, float x0, float y0, float ux, float uy, float vx, float vy, Material* parent) :
		Material(parent->reflectivity, parent->transparency, parent->n, parent->roughness),
		texture(image),
		x0(x0), y0(y0),
		ux(ux), uy(uy), vx(vx), vy(vy)
	{}

	const Color& getColor(float u = 0, float v = 0) const {
		return (*texture)[static_cast<int>(x0 + ux * u + vx * v + 0.5f)][static_cast<float>(y0 + uy * u + vy * v + 0.5f)];
	}
};

#endif