#include <string.h>
#include <glut.h>
#include "Tracing.h"
#include "ComplexShape.h"
#include "FileReader.h"

const int WIDTH = 1000, HEIGHT = 1000;
int prevX, prevY;
bool pressed = false;
//TracingEngine tr(Camera(WIDTH, HEIGHT, Vector3f(-100, -100, 0), Vector3f(100, -100, 0), Vector3f(-100, -100, 200), Vector3f(0, -500, 140)), );
TracingEngine tr = readSomething("models/room.rt");

void Key(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
	case 'w':
		tr.camera.move((tr.camera.y) * 5);
		break;
	case 's':
		tr.camera.move((tr.camera.y) * -5);
		break;
	case 'a':
		tr.camera.move((tr.camera.x) * -5);
		break;
	case 'd':
		tr.camera.move((tr.camera.x) * 5);
		break;
	case 'q':
		tr.camera.move((tr.camera.z) * 5);
		break;
	case 'e':
		tr.camera.move((tr.camera.z) * -5);
		break;
	}
	glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP) {
			pressed = false;
		}
		else {
			pressed = true;
			prevX = x;
			prevY = y;
		}
	}
}

void mouseMove(int x, int y) {
	if (pressed) {
		tr.camera.rotateZ((prevX - x) * 0.001f);
		tr.camera.rotateX((prevY - y) * 0.001f);
		prevX = x;
		prevY = y;
	}
	glutPostRedisplay();
}

void Draw(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	tr.trace();
	glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, tr.openGLpicture);

	glutSwapBuffers();
}

void Init(int argc, char** argv)
{
	if (argc > 1) {
		tr = readSomething(argv[1]);
	} else {
		/*vector<LightSource> lights;
		vector<Shape*> shapes;
		lights.push_back(LightSource(Vector3f(0, -100, 100), 100));
		shapes.push_back(
					new ComplexShape(
						new ComplexShape(
							new ComplexShape(new Sphere(Vector3f(0, 0, 100), 30)), 
							new ComplexShape(new Sphere(Vector3f(0, 0, 120), 30)),
							ComplexShape::AND
						),
						new ComplexShape(new Sphere(Vector3f(10, -10, 110), 20)),
						ComplexShape::SUB
					));
		tr = TracingEngine(Camera(WIDTH, HEIGHT, Vector3f(-100, -100, 200), Vector3f(-100, -100, 0), Vector3f(100, -100, 200), Vector3f(0, -500, 0)), shapes, lights);*/
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("ABGR extension");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(Key);
	glutDisplayFunc(Draw);
	glutMainLoop();
}

int main(int argc, char **argv)
{
	Init(argc, argv);
	return 0;            
}