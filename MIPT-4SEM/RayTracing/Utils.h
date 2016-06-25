#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <algorithm>
#include <iostream>
#include <istream>

using std::abs;
using std::min;
using std::max;
using std::swap;
using std::istream;

typedef unsigned char uchar;

const int DIMENSIONS = 3;
const float EPS = 1e-3f;

template <class T>
T sqr(T t) {
	return t * t;
}

#endif