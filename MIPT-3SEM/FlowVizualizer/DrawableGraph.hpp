#ifndef DRAWGRAPH_H
#define DRAWGRAPH_H

#include "Engine.hpp"
#include <vector>

using std::vector;
using std::string;
using std::swap;
using std::pair;
using std::make_pair;

//Интерфейс объекта, который можно нарисовать
class IDrawable {
public:
	virtual void Draw() = 0;
};

class CDrawableEdge;

class CDrawableVertex : public СBasicVertex<CDrawableEdge>, public IDrawable {
private:
	float x, y, prevX, prevY;
	string text;
public:
	CDrawableVertex() : x(0), y(0), prevX(0), prevY(0), text("") {
		SetNum(0);
	}
	CDrawableVertex(int _x, int _y, string _text) : x(_x), y(_y), prevX(_x), prevY(prevY), text(_text) {
		SetNum(0);
	}
	~CDrawableVertex() {
		text.clear();
	}
	void Draw() {
		CEngine::DrawVertex(x, y, num, text);
	}
	void SavePos() {
		prevX = x;
		prevY = y;
	}
	float GetPrevX() {
		return prevX;
	}
	float GetPrevY() {
		return prevY;
	}
	float GetX() {
		return x;
	}
	void AddX(float dx) {
		x += dx;
	}
	float GetY() {
		return y;
	}
	void AddY(float dy) {
		y += dy;
	}
	void SetText(string _text) {
		text = _text;
	}
	void SetNum(int i) {
		num = i;
	}
};

class CDrawableEdge : public CBasicEdge<CDrawableVertex>, public IDrawable {
private:
	string text;
public:
	CDrawableEdge(CDrawableVertex* _from, CDrawableVertex* _to, string _text) : CBasicEdge<CDrawableVertex>(_from, _to), text(_text) {}
	~CDrawableEdge() {
		text.clear();
	}

	//Отрисовать
	void Draw() {
		CEngine::DrawEdge(from->GetX(), from->GetY(), to->GetX(), to->GetY(), text);
	}

	//Установить текст
	void SetText(string _text) {
		text = _text;
	}
};

//Граф, который мы умеем рисовать, при этом его вершины - магниты, а ребра
class CDrawableGraph : public IDrawable, public CGraph<CDrawableEdge, CDrawableVertex> {
private:
	const float edgeLength = 200;
	const float forceLength = 200;
	const float forceStrength = 1;
	const float edgeStrength = 1;
	const float friction = 0.99;
	int width, height;

	float length2(CDrawableVertex* v1, CDrawableVertex* v2) {
		return (v1->GetX() - v2->GetX()) * (v1->GetX() - v2->GetX()) + (v1->GetY() - v2->GetY()) * (v1->GetY() - v2->GetY());
	}

	float length2(float x, float y) {
		return x * x + y * y;
	}
	
public:
	CDrawableGraph(int _width, int _height) : width(_width), height(_height) {}
	~CDrawableGraph() {
		for (int i = 0; i < GetEdgesCount(); ++i) {
			delete GetEdge(i);
		}
		for (int i = 0; i < GetSize(); ++i) {
			delete GetVertex(i);
		}
		vertices.clear();
		edges.clear();
	}

	void Clear() {
		for (int i = 0; i < GetEdgesCount(); ++i) {
			delete GetEdge(i);
		}
		for (int i = 0; i < GetSize(); ++i) {
			delete GetVertex(i);
		}
		vertices.clear();
		edges.clear();
	}
	
	//Шаг физики
	void Step(float dt) {
		for (int i = 0; i < GetSize(); ++i) {
			float dx = GetVertex(i)->GetX() - GetVertex(i)->GetPrevX();
			float dy = GetVertex(i)->GetY() - GetVertex(i)->GetPrevY();
			GetVertex(i)->SavePos();
			GetVertex(i)->AddX(dx * friction * dt);
			GetVertex(i)->AddY(dy * friction * dt);
		}
		for (int i = 0; i < GetSize(); ++i) {
			for (int j = i + 1; j < GetSize(); ++j) {
				if (length2(GetVertex(i), vertices[j]) < forceLength * forceLength) {
					float dx = GetVertex(i)->GetX() - vertices[j]->GetX();
					float dy = GetVertex(i)->GetY() - vertices[j]->GetY();
					float magicMultiplier = ((forceLength * forceLength) / (dx * dx + dy * dy + forceLength * forceLength) - 0.5f) * forceStrength * dt;
					dx *= magicMultiplier;
					dy *= magicMultiplier;
					GetVertex(i)->AddX(dx);
					GetVertex(i)->AddY(dy);
					vertices[j]->AddX(-dx);
					vertices[j]->AddY(-dy);
				}
			}
		}
		for (int i = 0; i < GetEdgesCount(); ++i) {
			float dx = GetEdge(i)->GetFrom()->GetX() - GetEdge(i)->GetTo()->GetX();
			float dy = GetEdge(i)->GetFrom()->GetY() - GetEdge(i)->GetTo()->GetY();
			float magicMultiplier = ((edgeLength * edgeLength) / (dx * dx + dy * dy + edgeLength * edgeLength) - 0.5f) * edgeStrength * dt;
			dx *= magicMultiplier;
			dy *= magicMultiplier;
			GetEdge(i)->GetFrom()->AddX(dx);
			GetEdge(i)->GetFrom()->AddY(dy);
			GetEdge(i)->GetTo()->AddX(-dx);
			GetEdge(i)->GetTo()->AddY(-dy);
		}
		for (int i = 0; i < GetSize(); ++i) {
			int x = GetVertex(i)->GetX();
			int y = GetVertex(i)->GetY();
			if (x < 0) {
				GetVertex(i)->AddX(-x);
			}
			if (x > width) {
				GetVertex(i)->AddX(width - x);
			}
			if (y < 0) {
				GetVertex(i)->AddY(-y);
			}
			if (y > height) {
				GetVertex(i)->AddY(height - y);
			}
		}
	}

	void Draw() {
		for (int i = 0; i < GetEdgesCount(); ++i) {
			GetEdge(i)->Draw();
		}
		for (int i = 0; i < GetSize(); ++i) {
			GetVertex(i)->Draw();
		}
	}

	pair<float, float> GetVertexPos(int i) {
		return make_pair(GetVertex(i)->GetX(), GetVertex(i)->GetY());
	}
	
	void AddVertex(float x, float y, string text) {
		vertices.push_back(new CDrawableVertex(x, y, text));
		vertices.back()->SetNum(GetSize() - 1);
	}
	
	void AddEdge(int x, int y, string text) {
		edges.push_back(new CDrawableEdge(GetVertex(x), GetVertex(y), text));
	}
	
	void ChangeVertex(int i, string text) {
		GetVertex(i)->SetText(text);
	}
	
	void ChangeEdge(int i, string text) {
		GetEdge(i)->SetText(text);
	}
};

#endif DRAWGRAPH_H
