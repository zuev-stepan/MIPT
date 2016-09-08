#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

using std::vector;

//������� �������
template<typename CEdge>
class �BasicVertex {
protected:
	vector<CEdge*> edges;
	int num;
public:
	�BasicVertex() {
		SetNum(0);
	}
	�BasicVertex(int _num) : num(_num) {}

	//�������� �����
	void AddEdge(CEdge* edge) {
		edges.push_back(edge);
	}

	//�������� i-� �����
	CEdge* GetEdge(int i) const {
		if (i >= 0 && i < edges.size())
			return edges[i];
		return NULL;
	}

	//�������� �����
	int GetNum() const {
		return num;
	}

	//���������� �����
	void SetNum(int _num) {
		num = _num;
	}
};

//������� �����
template<typename CVertex>
class CBasicEdge {
protected:
	CVertex* from, *to;
public:
	CBasicEdge(){}
	CBasicEdge(CVertex* _from, CVertex* _to) : from(_from), to(_to) {}

	//�������� �������, �� ������� ������� �����
	CVertex* GetFrom() {
		return from;
	}

	//�������� �������, � ������� ���� �����
	CVertex* GetTo() {
		return to;
	}
};

//���� �� ������� ����� � �������
//� CVertex ������ ���� ����� CEdge* GetEdge(int)
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

	//�������� i-� �������, ���� ����, ����� NULL
	CVertex* GetVertex(int i) const {
		if (i >= 0 && i < vertices.size())
			return vertices[i];
		return NULL;
	}

	//�������� ����� �� i-�� ������� � j-�, ���� ����, ����� NULL
	CEdge* GetEdge(int i, int j) const {
		if (GetVertex(i) != NULL)
			return GetVertex(i)->GetEdge(j);
		return NULL;
	}

	//�������� i-� �����, ���� ����, ����� NULL
	CEdge* GetEdge(int i) const {
		if (i >= 0 && i < edges.size())
			return edges[i];
		return NULL;
	}

	//�������� ���������� ������
	int GetSize() const {
		return vertices.size();
	}

	//�������� ���������� �����
	int GetEdgesCount() const {
		return edges.size();
	}
};

#endif GRAPH_H