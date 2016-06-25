#ifndef VECTOR_H
#define VECTOR_H

#include "Utils.h"
#include <istream>

class Vector3f {
public:
	float x, y, z;
	Vector3f(){}
	Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}

	//Get i-th axis
	float& operator[](const int i) {
		if (i == 0) {
			return x;
		}
		else if (i == 1) {
			return y;
		}
		else {
			return z;
		}
	}

	//Get i-th axis
	const float& operator[](int i) const {
		if (i == 0) {
			return x;
		}
		else if (i == 1) {
			return y;
		}
		return z;
	}

	//Are this vector equal to t
	bool operator ==(Vector3f t) {
		return abs(x - t.x) < 1 && abs(y - t.y) < 1 && abs(z - t.z) < 1;
	}

	Vector3f operator -(Vector3f t) const {
		return Vector3f(x - t.x, y - t.y, z - t.z);
	}

	void operator -=(Vector3f t) {
		x -= t.x;
		y -= t.y;
	}

	Vector3f operator +(Vector3f t) const {
		return Vector3f(x + t.x, y + t.y, z + t.z);
	}

	void operator +=(Vector3f t) {
		x += t.x;
		y += t.y;
	}

	//Dot product
	float operator *(Vector3f t) const {
		return (x * t.x + y * t.y + z * t.z);
	}

	Vector3f operator *(float t) const {
		return Vector3f(x * t, y * t, z * t);
	}

	void operator *=(float t) {
		x *= t;
		y *= t;
	}

	Vector3f operator /(float t) const {
		t = 1 / t;
		return Vector3f(x * t, y * t, z * t);
	}

	void operator /=(float t) {
		t = 1 / t;
		x *= t;
		y *= t;
		z *= t;
	}

	//Cross product
	Vector3f operator %(Vector3f t) const {
		return Vector3f(y * t.z - z * t.y, z * t.x - x * t.z, x * t.y - y * t.x);
	}

	void operator %=(Vector3f t) {
		Vector3f tmp = *this;
		x = tmp.y * t.z - tmp.z * t.y;
		y = tmp.z * t.x - tmp.x * t.z;
		z = tmp.x * t.y - tmp.y * t.x;
	}

	float len2() const {
		return x * x + y * y + z * z;
	}

	float len() const {
		return sqrtf(this->len2());
	}

	Vector3f normalized() const {
		float len = this->len();
		if (len < EPS) {
			return Vector3f(0, 0, 0);
		}
		len = 1 / len;
		return Vector3f(x * len, y * len, z * len);
	}

	void normalize() {
		float len = sqrt(x * x + y * y + z * z);
		if (len < EPS) {
			return;
		}
		len = 1 / len;
		x *= len;
		y *= len;
		z *= len;
	}

	static Vector3f vmin(const Vector3f& a, const Vector3f& b) {
		return Vector3f(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
	}

	static Vector3f vmax(const Vector3f& a, const Vector3f& b) {
		return Vector3f(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
	}

	static Vector3f random() {
		return Vector3f(rand(), rand(), rand()).normalized();
	}

	//Project this vector on vector t
	Vector3f operator()(const Vector3f& t) const {
		//this.len == 1
		return (*this) * (t * (*this));
	}
};



#endif