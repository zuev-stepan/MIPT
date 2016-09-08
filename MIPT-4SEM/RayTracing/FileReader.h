#ifndef FILEREADER_H
#define FILEREADER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using std::vector;
using std::string;
using std::ifstream;

union readStruct {
	char header[80];
	unsigned int n;
	float vertex[3];
	short additional;
};

const int SPHERE = 0;
const int TRIANGLE = 1;
const int QUADRANGLE = 2;

class Shape;
class TracingEngine;
class Image;

TracingEngine readSTL(const char* path);
TracingEngine readRT(const char* path);
TracingEngine readSomething(const char* path);
Image readBMP(const char* path);

#endif