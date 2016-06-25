#ifndef LIGHTS_H
#define LIGHTS_H

#include "Color.h"
#include "Vector3f.h"

class Light {
public:
	float r, g, b;
	Light() : r(0), g(0), b(0) {}
	Light(float t) : r(t), g(t), b(t) {}
	Light(float r, float g, float b) : r(r), g(g), b(b) {}

	void operator +=(const Light& t) {
		r += t.r;
		g += t.g;
		b += t.b;
	}

	void operator *=(float t) {
		r *= t, g *= t, b *= t;
	}

	Light operator *(float t) {
		return Light(r * t, g * t, b * t);
	}

	void apply(Color& color) {
		color.r = Color::multiplyChannel(color.r, r);
		color.g = Color::multiplyChannel(color.g, g);
		color.b = Color::multiplyChannel(color.b, b);
	}
};

class LightSource {
public:
	Vector3f pos;
	Light light;
	LightSource() {}
	LightSource(Vector3f pos, float distance) : pos(pos), light(sqr(distance)) {}
};

#endif