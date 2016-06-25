#ifndef COLOR_H
#define COLOR_H

#include "Utils.h"
#include <istream>

class Color {
private:
	static uchar blendChannels(float c1, float c2, float alpha) {
		return static_cast<uchar>(c1 * alpha + c2 * (1 - alpha));
	}

	
public:
	uchar r, g, b;
	Color(){}
	Color(uchar r, uchar g, uchar b) : r(r), g(g), b(b) {}


	static uchar multiplyChannel(float c, float i) {
		if (c * i >= 255) {
			return 255;
		}
		return static_cast<uchar>(c * i);
	}

	uchar& operator[](int i) {
		if (i == 0) {
			return r;
		}
		else if (i == 1) {
			return g;
		}
		return b;
	}

	const uchar& operator[](int i) const {
		if (i == 0) {
			return r;
		}
		else if (i == 1) {
			return g;
		}
		return b;
	}

	void applyLight(float intensity) {
		for (int i = 0; i < 3; ++i) {
			(*this)[i] = multiplyChannel((*this)[i], intensity);
		}
	}

	void blend(const Color& t, float alpha) {
		for (int i = 0; i < 3; ++i) {
			(*this)[i] = blendChannels(t[i], (*this)[i], alpha);
		}
	}

	//Mix maximum of 255 colors
	template <class Iterator>
	static Color mix(Iterator begin, Iterator end) {
		short r = 0, g = 0, b = 0;
		uchar size = end - begin;
		while (begin != end) {
			r += begin->r;
			g += begin->g;
			b += begin->b;
			++begin;
		}
		return Color(r / size, g / size, b / size);
	}
};

#endif