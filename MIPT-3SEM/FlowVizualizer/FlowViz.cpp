#include "stdafx.h"
#include "Engine.hpp"
#include "FlowLib.hpp"
#include "DrawableGraph.hpp"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>
#include <ctime>

using std::cin;
using std::string;
using std::pair;

///Без глобальных переменных и функций нельзя, я с OpenGL же работаю
const int width = 1280, height = 720;
const int maxVert = 15, minVert = 3, edgeMult = 1, maxCapacity = 100;
static bool canPress = true;
static vector<CStep> steps;
static CDrawableGraph* graph;
static int currentStep;
static bool needInit = true;

//Обновляем граф
void CorrectGraph() {
	//e(u)/h(u)
	for (int i = 0; i < steps[currentStep].GetNetwork().GetSize(); ++i) {
		string s = to_string(steps[currentStep].GetNetwork().GetVertex(i)->GetExtraFlow()) + '/' + to_string(steps[currentStep].GetNetwork().GetVertex(i)->GetHeight());
		graph->ChangeVertex(i, s);
	}
	//f(u, v)/c(u, v)/c(v, u)
	for (int i = 0; i < steps[currentStep].GetNetwork().GetEdgesCount(); ++i) {
		CFlowEdge* edge = steps[currentStep].GetNetwork().GetEdge(i);
		if (edge->GetFrom()->GetNum() > edge->GetTo()->GetNum()) {
			edge = edge->GetReverse();
		}
		string s = to_string(edge->GetCapacity()) + '>' + to_string(edge->GetFlow()) + '>' + to_string(edge->GetReverse()->GetCapacity());
		graph->ChangeEdge(i, s);
	}
}

//Генерим новую сеть, запускаем на ней алгоритм, удаляем старый граф, строим новый граф для отрисовки по сети 
void initGraph() {
	int vCount = minVert + rand() % maxVert;
	int eCount = vCount - 1 + min((vCount * (vCount - 1)) / 2 - vCount + 1, rand() % (vCount * edgeMult));
	CNetwork net(vCount, eCount, rand() % maxCapacity + 1);
	delete graph;
	graph = new CDrawableGraph(width, height);
	steps = RelabelToFront(net);
	currentStep = 0;
	for (int i = 0; i < steps[currentStep].GetNetwork().GetSize(); i++) {
		graph->AddVertex(rand() % width, rand() % height, "");
	}
	for (int i = 0; i < steps[currentStep].GetNetwork().GetEdgesCount(); i++) {
		CFlowEdge* edge = steps[currentStep].GetNetwork().GetEdge(i);
		if (edge->GetFrom()->GetNum() > edge->GetTo()->GetNum()) {
			edge = edge->GetReverse();
		}
		graph->AddEdge(edge->GetFrom()->GetNum(), edge->GetTo()->GetNum(), "");
	}
	CorrectGraph();
	needInit = false;
}

void Draw() {
	//Чистим экран
	CEngine::Clear();
	//Рисуем кнопки
	CEngine::DrawButton(0, 0, 100, 20, "Previous");
	CEngine::DrawButton(102, 0, 202, 20, "Next");
	CEngine::DrawButton(204, 0, 304, 20, "Random");
	//Обновляем граф
	if (needInit) {
		initGraph();
	}
	graph->Step(0.1);
	//Рисуем граф
	pair<float, float> pos = graph->GetVertexPos(steps[currentStep].GetChangedVertex());
	CEngine::DrawWarning(pos.first, pos.second, vertexR + 5);
	graph->Draw();
	//Рисуем подсказки
	CEngine::DrawString(0, height - fontHeight * 4 - 2, steps[currentStep].GetDescription());
	CEngine::DrawString(0, height - fontHeight * 2 - 2, "Vertex u format: e - extra flow, h - height: e/h");
	CEngine::DrawString(0, height - 2,					"Edge (u, v) format: u < v, f - flow, c - capacity: c(u, v)>f(u, v)>c(v, u)");
	CEngine::Update();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (canPress) {
			if (x < 100 && y < 20) {
				currentStep--;
				if (currentStep < 0)
					currentStep = 0;
				CorrectGraph();
			}
			if (x > 102 && x < 202 && y < 20) {
				currentStep++;
				if (currentStep >= steps.size())
					currentStep--;
				CorrectGraph();
			}
			if (x > 204 && x < 304 && y < 20) {
				needInit = true;
			}
		}
		canPress = false;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		canPress = true;
	}
}

int _tmain(int argc, char** argv) {
	freopen("input.txt", "rt", stdin);
	srand(time(NULL));
	CEngine::Init(argc, argv, width, height);
	glutDisplayFunc(Draw);
	glutMouseFunc(Mouse);
	CEngine::StartLoop();
}

