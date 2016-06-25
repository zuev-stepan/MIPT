#ifndef ENGINE_H
#define ENGINE_H

#include <glut.h>
#include <cmath>
#include <cstring>
#include <sstream>
#include <string>

using std::string;
using std::to_string;
using std::stringstream;

const float pi = 3.1415926535897932;
const float vertexR = 30;
const float fontWidth = 104.76 / 9 / 2;
const float fontHeight = 104.76 / 9 / 2 + 1;

const float cosX[] = {
	1.000000,
	0.923880,
	0.707107,
	0.382683,
	-0.000000,
	-0.382684,
	-0.707107,
	-0.923880,
	-1.000000,
	-0.923880,
	-0.707107,
	-0.382683,
	0.000000,
	0.382684,
	0.707107,
	0.923880
};

const float sinX[] = {
	0.000000,
	0.382683,
	0.707107,
	0.923880,
	1.000000,
	0.923880,
	0.707107,
	0.382683,
	-0.000000,
	-0.382683,
	-0.707107,
	-0.923880,
	-1.000000,
	-0.923879,
	-0.707107,
	-0.382683
};

class CEngine {
private:
	//Сколько нужно отступить от центра вершины, чтобы текст на ребре не залез на нее
	static float Normalize(float x, float y) {
		return (vertexR + fontWidth * 5) / sqrt(x * x + y * y);
	}
public:
	//Инициализация движка, необходимо вызвать перед началом работы с ним
	static void Init(int argc, char** argv, int width, int height) {
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
		glutInitWindowSize(width, height);
		glutInitWindowPosition(100, 100);
		glutCreateWindow("FlowViz");
		glClearColor(1, 1, 1, 1);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, height, 0, -1, 1);
	}

	//Запуск цикла отрисовки
	static void StartLoop() {
		glutMainLoop();
	}

	//Очистить экран
	static void Clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	//Нарисовать красный кружок
	static void DrawWarning(float x, float y, float r) {
		glColor3f(1, 0.5, 0.5);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 16; ++i) {
			glVertex2f(r * cosX[i] + x, r * sinX[i] + y);
		}
		glEnd();
	}

	//Нарисовать строку
	static void DrawString(float x, float y, string s) {
		glColor3f(0, 0, 0);
		glPushMatrix();
		glTranslatef(x, y, 0);
		glScalef(1 / 9.0, -1 / 9.0, 1 / 9.0);
		for (int i = 0; i < s.length(); ++i)
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, (int)s[i]);
		glPopMatrix();
	}

	//Нарисовать вершину графа (кружок с текстом в нем)
	static void DrawVertex(float x, float y, int i, string s) {
		glColor3f(0.7, 0.7, 0.7);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 16; ++i) {
			glVertex2f(vertexR * cosX[i] + x, vertexR * sinX[i] + y);
		}
		glEnd();
		DrawString(x - fontWidth * s.length(), y + fontHeight * 2, s);
		DrawString(x - fontWidth * to_string(i).length(), y, to_string(i));
	}

	//Нарисовать ребро графа (прямая с текстом)
	static void DrawEdge(float x1, float y1, float x2, float y2, string s) {
		glColor3f(0.5, 0.5, 0.5);
		double angle = 0;
		glPushMatrix();
		glTranslatef(x1, y1, 0);
		glRotatef(atan2(-(y2 - y1), (x2 - x1)) / pi * 180, 0, 0, -1);
		float l = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		glBegin(GL_POLYGON);
		glVertex2f(0, -1);
		glVertex2f(0, 1);
		glVertex2f(l - vertexR - 5, 1);
		glVertex2f(l - vertexR - 5, -1);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(l - vertexR - 5, -5);
		glVertex2f(l - vertexR, 0);
		glVertex2f(l - vertexR - 5, 5);
		glEnd();
		float sl = s.length() * fontWidth;
		if (x1 <= x2) {
			DrawString(l / 2 - sl / 2 - vertexR, -2, s);
		} else {
			glTranslatef(l / 2, 0, 0);
			glRotatef(180, 0, 0, -1);
			glTranslatef(-l / 2, 0, 0);
			DrawString(l / 2 - sl / 2 - vertexR, -2, s);
		}
		glPopMatrix();
	}

	//Нарисовать ребро графа (прямая с текстом)
	static void DrawButton(float x1, float y1, float x2, float y2, string s) {
		glColor3f(0.7, 0.7, 0.7);
		glBegin(GL_POLYGON);
		glVertex2f(x1, y1);
		glVertex2f(x2, y1);
		glVertex2f(x2, y2);
		glVertex2f(x1, y2);
		glEnd();
		DrawString(x1 + fontWidth, y1 + fontHeight * 2, s);
	}

	//Отрисовать все (свапнуть буферы и продолжить выполнение)
	static void Update() {
		glutSwapBuffers();
		glutPostRedisplay();
	}
};

#endif ENGINE_H