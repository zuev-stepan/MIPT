#ifndef MATRIX_H
#define MATRIX_H

#include "Vector3f.h"

class Matrix {
public:
	float a[3][3];
	Matrix(){
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				a[i][j] = 0;
			}
		}
	}

	Vector3f operator *(Vector3f t) {
		Vector3f result(0, 0, 0);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result[i] += a[i][j] * t[j];
			}
		}
		return result;
	}

	Matrix operator*(Matrix t) {
		Matrix result;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					result.a[i][j] += a[i][k] * t.a[k][j];
				}
			}
		}
		return result;
	}

	static Matrix rotation(Vector3f t, float angle) {
		Matrix rot;
		float cosa = cos(angle), sina = sin(angle);
		rot.a[0][0] = cosa + (1 - cosa) * t.x * t.x;
		rot.a[0][1] = (1 - cosa) * t.x * t.y - sina * t.z;
		rot.a[0][2] = (1 - cosa) * t.x * t.z + sina * t.y;
		rot.a[1][0] = (1 - cosa) * t.x * t.y + sina * t.z;
		rot.a[1][1] = cosa + (1 - cosa) * t.y * t.y;
		rot.a[1][2] = (1 - cosa) * t.y * t.z - sina * t.x;
		rot.a[2][0] = (1 - cosa) * t.x * t.z - sina * t.y;
		rot.a[2][1] = (1 - cosa) * t.y * t.z + sina * t.x;
		rot.a[2][2] = cosa + (1 - cosa) * t.z * t.z;
		return rot;
	}
};

#endif