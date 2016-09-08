#include "FileReader.h"
#include "Tracing.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Quadrangle.h"
#include "SolidColor.h"
#include "Textured.h"
#include "Lights.h"
#include "Camera.h"
#include <map>

using std::istream;
using std::map;
using std::stof;
using std::stoi;

istream& operator>>(istream& in, Color& t) {
	short r, g, b;
	in >> r >> g >> b;
	t.r = r, t.g = g, t.b = b;
	return in;
}


istream& operator>>(istream& in, Vector3f& t) {
	in >> t.x >> t.y >> t.z;
	return in;
}

TracingEngine readSTL(const char* path) {
	ifstream myFile(path, std::ios::in | std::ios::binary);

	readStruct tmp;
	vector<Shape*> model;
	Vector3f normal, v[3];
	Vector3f minv(INFINITY, INFINITY, INFINITY), maxv(-INFINITY, -INFINITY, -INFINITY);

	if (myFile) {
		myFile.read(tmp.header, 80);
		myFile.read(tmp.header, 4);
		int n = tmp.n;

		for (int i = 0; i < n; i++) {
			myFile.read(tmp.header, 12);
			normal = Vector3f(tmp.vertex[0], tmp.vertex[1], tmp.vertex[2]);
			myFile.read(tmp.header, 12);
			v[0] = Vector3f(tmp.vertex[0], tmp.vertex[1], tmp.vertex[2]);
			myFile.read(tmp.header, 12);
			v[1] = Vector3f(tmp.vertex[0], tmp.vertex[1], tmp.vertex[2]);
			myFile.read(tmp.header, 12);
			v[2] = Vector3f(tmp.vertex[0], tmp.vertex[1], tmp.vertex[2]);
			myFile.read(tmp.header, 2);
			model.push_back(new Triangle(v, normal, new SolidColor()));
			for (int j = 0; j < 3; ++j) {
				minv = Vector3f::vmin(minv, v[j]);
				maxv = Vector3f::vmax(maxv, v[j]);
			}
		}
	}
	
	myFile.close();
	minv.x = min(minv.x, minv.z);
	minv.z = minv.x;
	maxv.x = max(maxv.x, maxv.z);
	maxv.z = maxv.x;
	minv.y -= 10;
	Vector3f topleft(minv.x, minv.y, maxv.z);
	Vector3f bottomleft(minv.x, minv.y, minv.z);
	Vector3f topright(maxv.x, minv.y, maxv.z);
	Vector3f origin((minv.x + maxv.x) / 2.0f, minv.y - 100, (minv.z + maxv.z) / 2.0f);
	vector<LightSource> lights;
	lights.push_back(LightSource(Vector3f(origin.x, origin.y, origin.z + 100), 150));
	return TracingEngine(Camera(1000, 1000, topleft, bottomleft, topright, origin), model, lights);
}

string next(ifstream& in) {
	string result;
	while (true) {
		in >> result;
		if (result[0] == '#') {
			getline(in, result);
		}
		else {
			break;
		}
	}
	return result;
}

TracingEngine readRT(const char* path) {
	ifstream in(path, std::ios::in);

	vector<Shape*> shapes;
	map<string, Material*> materials;
	vector<LightSource> lights;

	LightSource light;
	SolidColor* material = new SolidColor();
	Camera camera;
	
	Vector3f v[4];
	int i = 0;
	Color color;
	float power = 1, distance = 1, r;

	Image* img;
	float x0, y0, ux, uy, vx, vy;

	string name, command;

	map<string, int> vertex;
	vertex["origin"] = 0;
	vertex["topleft"] = 1;
	vertex["bottomleft"] = 2;
	vertex["topright"] = 3;
	vertex["coords"] = 0;
	vertex["vertex"] = 0;

	if (in) {
		while (true) {
			command = next(in);
			if (vertex.find(command) != vertex.end()) {
				int i = vertex[command];
				v[i].x = stof(next(in));
				v[i].y = stof(next(in));
				v[i].z = stof(next(in));
				if (command == "vertex") {
					vertex[command]++;
				}
			}
			if (command == "endviewport") {
				camera = Camera(1000, 1000, v[1], v[2], v[3], v[0]);
			}
			if (command == "name" || command == "material") {
				name = next(in);
			}
			if (command == "color") {
				int r = stoi(next(in)), g = stoi(next(in)), b = stoi(next(in));
				material->color = Color(r, g, b);
			}
			if (command == "alpha") {
				material->transparency = 1 - stof(next(in));
			}
			if (command == "reflect") {
				material->reflectivity = stof(next(in));
			}
			if (command == "refract") {
				material->n = stof(next(in));
			}
			if (command == "endentry") {
				materials[name] = material;
				material = new SolidColor();
			}
			if (command == "file") {
				img = new Image(readBMP(next(in).c_str()));
			}
			if (command == "relative") {
				x0 = stof(next(in));
				y0 = stof(next(in));
				ux = stof(next(in));
				uy = stof(next(in));
				vx = stof(next(in));
				vy = stof(next(in));
			}
			if (command == "endtexture") {
				materials[name] = new Textured(img, x0, y0, ux, uy, vx, vy, materials[name]);
			}
			if (command == "reference") {
				for (int i = 0; i < 2; ++i) {
					command = next(in);
					if (command == "power") {
						power = stof(next(in));
					}
					else {
						distance = stof(next(in));
					}
				}
			}
			if (command == "power") {
				light.light = Light(stof(next(in)) / power * sqr(distance));
			}
			if (command == "endpoint") {
				light.pos = v[0];
				lights.push_back(light);
			}
			if (command == "radius") {
				r = stof(next(in));
			}
			if (command == "endsphere") {
				shapes.push_back(new Sphere(v[0], r, materials[name]));
				vertex["coords"] = 0;
			}
			if (command == "endtriangle") {
				shapes.push_back(new Triangle(v, materials[name]));
				vertex["vertex"] = 0;
			}
			if (command == "endquadrangle") {
				shapes.push_back(new Quadrangle(v, materials[name]));
				vertex["vertex"] = 0;
			}
			if (command == "endgeometry") {
				break;
			}
		}

	}
	in.close();
	return TracingEngine(camera, shapes, lights);
}

TracingEngine readSomething(const char* path) {
	if (path[strlen(path) - 1] == 't') {
		return readRT(path);
	}
	return readSTL(path);
}

Image readBMP(const char* path) {
	int i;
	FILE* f = fopen(path, "rb");

	if (f == NULL)
		throw "Argument Exception";

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); 
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	int row_padded = (width * 3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded];
	unsigned char tmp;

	Image img(width, height);

	for (int i = 0; i < height; i++) {
		fread(data, sizeof(unsigned char), row_padded, f);
		for (int j = 0; j < width * 3; j += 3) {
			img.buffer[j / 3][i] = Color(data[j + 2], data[j + 1], data[j]);
		}
	}
	delete[] data;
	return img;
}