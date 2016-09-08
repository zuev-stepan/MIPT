#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
using std::vector;

class Image {
public:
	vector<vector<Color>> buffer;

	int width, height;
	Image() {}
	Image(int width, int height) : width(width), height(height) {
		buffer.resize(width);
		for (int i = 0; i < width; ++i) {
			buffer[i].resize(height);
		}
	}

	vector<Color>& operator[](int i) {
		return buffer[i];
	}

	const vector<Color>& operator[](int i) const {
		return buffer[i];
	}
};

#endif