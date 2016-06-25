#ifndef TRACING
#define TRACING

#include <vector>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Utils.h"
#include "KDTree.h"
#include "Image.h"
#include "Lights.h"
#include "Camera.h"

using std::vector;

class TracingEngine {
private:
	//Constants
	static const uchar DEBUG = 0;
	static const uchar RELEASE = 1;

	//Options
	uchar antiAliasing = 2;
	uchar mode = 1;
	uchar recursionDepth = 3;
	bool enableLight = 1;
	bool enableSecondaryLight = 0;
	bool enableReflections = 1;
	bool enableTransparency = 1;
	bool enableMultithreading = 1;
	float minAlpha = 0.01f;

	//Scene
	const ShapeContainer* container;
	vector<LightSource> lights;
	float extraLight = 0.1f;
	Color background;

	//Result
	Image image;

	//SecondaryLights
	int randomRayCount = 100;
	int blending = 5;
	vector<vector<Light>> secondLight;
	
public:
	uchar* openGLpicture;
	Camera camera;

	TracingEngine(const Camera& cam, vector<Shape*> shapes, vector<LightSource> lights, char antiAliasing = 2) :
		camera(cam), 
		antiAliasing(antiAliasing), 
		image(cam.width * antiAliasing, cam.height * antiAliasing),
		lights(lights.begin(), lights.end())
	{
		openGLpicture = new uchar[camera.width * camera.height * 3];
		
		camera.width *= antiAliasing;
		camera.height *= antiAliasing;

		background = Color(0, 185, 255);
		container = new KDTree(shapes);
	}

	Light calcLight(const TraceResult& traceResult) {
		float ans = 0;
		Light result(extraLight);
		for (LightSource& light : lights) {
			Vector3f toLight = light.pos - traceResult.pos;
			float distToLight = toLight.len();
			
			float multiply = 1, dist = 0;
			TraceResult tmp = traceResult;
			while (dist < distToLight && multiply > EPS) {
				tmp = container->traceRay(Ray(tmp.pos + toLight * EPS, toLight), distToLight);
				dist += tmp.dist;
				if (dist >= distToLight) {
					break;
				}
				multiply *= tmp.shape->material->transparency;
			}
			if (dist >= distToLight) {
				Vector3f L = (light.pos - traceResult.pos) / distToLight;
				float c = L * traceResult.normal / sqr(distToLight) * multiply;
				if (c > 0) {
					result += light.light * c;
				}
			}
		}
		return result;
	}

	Light calcSecondaryLight(const Ray& ray) {
		TraceResult traceResult = container->traceRay(ray);
		Light result;
		for (int i = 0; i < randomRayCount; i++) {
			Ray tmp(traceResult.pos + traceResult.normal * EPS, Vector3f::random());
			if (tmp.direction * traceResult.normal < 0) {
				tmp.direction *= -1;
			}
			TraceResult tr = container->traceRay(tmp);
			if (!tr.shape || tr.shape->material->reflectivity < EPS) {
				continue;
			}
			Light add = calcLight(tr);
			add.r *= tr.color.r;
			add.g *= tr.color.g;
			add.b *= tr.color.b;
			result += add * (1 / 255.0f);
		}
		return result * (0.3f / randomRayCount);
	}


	Color calcTransparency(const Ray& ray, const TraceResult& traceResult, int depth) {
		float n1 = ray.space.n, n2 = traceResult.shape->material->n;
		if (!traceResult.face && ray.space.parent) {
			n2 = ray.space.parent->n;
		}
		Vector3f v1 = ray.direction * n1;
		float v1dotNormal = v1 * traceResult.normal;
		Vector3f direction = v1 + traceResult.normal * v1dotNormal * (sqrt((sqr(n2) - sqr(n1)) / sqr(v1dotNormal) + 1) - 1);
		Ray newRay(traceResult.pos + direction * EPS, direction, Space(traceResult.shape->material->n, &(ray.space)));
		if (!traceResult.face && ray.space.parent) {
			newRay.space = *ray.space.parent;
		}
		return calcColor(newRay, depth - 1);
	}
	
	Color calcReflection(const Ray& ray, const TraceResult& traceResult, int depth) {
		Ray newRay(traceResult.pos, (ray.direction - (traceResult.normal(ray.direction) * 2)), ray.space);
		return calcColor(newRay, depth - 1);
	}

	Color calcColor(const Ray& ray, int depth) {
		TraceResult traceResult = container->traceRay(ray);
		if (traceResult.shape) {
			Color pixel = traceResult.color;
			if (depth > 0) {
				if (enableTransparency && traceResult.shape->material->transparency > minAlpha) {
					pixel.blend(calcTransparency(ray, traceResult, depth), traceResult.shape->material->transparency);
				}
				if (enableReflections && traceResult.shape->material->reflectivity > minAlpha) {
					pixel.blend(calcReflection(ray, traceResult, depth), traceResult.shape->material->reflectivity);
				}
			}
			if (enableLight) {
				Light light = calcLight(traceResult);
				if (enableSecondaryLight) {
					light += calcSecondaryLight(ray);
				}
				light.apply(pixel);
			}
			return pixel;
		}
		return background;
	}

	void fillImage(int fromX, int toX) {
		for (int i = fromX; i < toX; ++i) {
			for (int j = 0; j < camera.height; ++j) {
				Ray ray = camera.getRay(i, j);
				image[i][j] = calcColor(ray, recursionDepth);
			}
		}
	}

	void trace() {
		if (enableMultithreading) {
			// TODO thread pool
			int threadCount = std::thread::hardware_concurrency();
			int step = camera.width / threadCount;
			vector<std::thread> threads;
			for (int i = 0; i < threadCount - 1; i++) {
				threads.emplace_back([=] {fillImage(i * step, (i + 1) * step); });
			}
			threads.emplace_back([=] {fillImage((threadCount - 1) * step, camera.width); });
			for (int i = 0; i < threads.size(); ++i) {
				threads[i].join();
			}
		}
		else {
			fillImage(0, camera.width);
		}
		fillOpenGLPicture(0, camera.width);
	}
	
	void fillOpenGLPicture(int fromX, int toX) {
		uchar* curr = openGLpicture;
		Color* cbuf = new Color[antiAliasing * antiAliasing];
		for (int i = 0; i < camera.width; i += antiAliasing) {
			for (int j = 0; j < camera.height; j += antiAliasing) {
				for (int k = 0; k < antiAliasing * antiAliasing; k++) {
					cbuf[k] = image[j + k / antiAliasing][i + k % antiAliasing];
				}
				Color mixed = Color::mix(cbuf, cbuf + antiAliasing * antiAliasing);
				curr[0] = mixed.r;
				curr[1] = mixed.g;
				curr[2] = mixed.b;
				curr += 3;
			}
		}
		delete[] cbuf;
	}
};

#endif