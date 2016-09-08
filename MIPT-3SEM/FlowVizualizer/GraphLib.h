#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

using std::vector;

//Базовая вершина
template<typename CEdge>
class СBasicVertex {
protected:
	vector<CEdge*> edges;
	int num;
public:
	СBasicVertex() {
		SetNum(0);
	}
	СBasicVertex(int _num) : num(_num) {}

	//Добавить ребро
	void AddEdge(CEdge* edge) {
		edges.push_back(edge);
	}

	//Получить i-е ребро
	CEdge* GetEdge(int i) const {
		if (i >= 0 && i < edges.size())
			return edges[i];
		return NULL;
	}

	//Получить номер
	int GetNum() const {
		return num;
	}

	//Установить номер
	void SetNum(int _num) {
		num = _num;
	}
};

//Базовое ребро
template<typename CVertex>
class CBasicEdge {
protected:
	CVertex* from, *to;
public:
	CBasicEdge(){}
	CBasicEdge(CVertex* _from, CVertex* _to) : from(_from), to(_to) {}

	//Получить вершину, из которой выходит ребро
	CVertex* GetFrom() {
		return from;
	}

	//Получить вершину, в которую идет ребро
	CVertex* GetTo() {
		return to;
	}
};

//Граф от данного ребра и вершины
//В CVertex должен быть метод CEdge* GetEdge(int)
template<typename CEdge, typename CVertex>
class CGraph {
protected:
	vector<CVertex*> vertices;
	vector<CEdge*> edges;
public:
	CGraph(){}
	virtual ~CGraph() {
		for (int i = 0; i < vertices.size(); i++)
			delete vertices[i];
		for (int i = 0; i < edges.size(); i++) {
			delete edges[i];
		}
	}

	//Получить i-ю вершину, если есть, иначе NULL
	CVertex* GetVertex(int i) const {
		if (i >= 0 && i < vertices.size())
			return vertices[i];
		return NULL;
	}

	//Получить ребро из i-ой вершины в j-ю, если есть, иначе NULL
	CEdge* GetEdge(int i, int j) const {
		if (GetVertex(i) != NULL)
			return GetVertex(i)->GetEdge(j);
		return NULL;
	}

	//Получить i-е ребро, елси есть, иначе NULL
	CEdge* GetEdge(int i) const {
		if (i >= 0 && i < edges.size())
			return edges[i];
		return NULL;
	}

	//Получить количество вершин
	int GetSize() const {
		return vertices.size();
	}

	//Получить количество ребер
	int GetEdgesCount() const {
		return edges.size();
	}
};

#endif GRAPH_H