#ifndef FLOW_H
#define FLOW_H

#include "GraphLib.h"
#include <iostream>
#include <stdio.h>
#include <list>
#include <algorithm>
#include <unordered_set>

using std::vector;
using std::istream;
using std::list;
using std::pair;
using std::string;
using std::make_pair;
using std::random_shuffle;
using std::unordered_set;
using std::min;
using std::to_string;

const int INF = 2000000000;

//��� �� ����, ��� �������, ��� �������� ���� �� 10000 - �������
//����� ��� ��������� ���������� �����
struct Hash {
	size_t operator()(pair<int, int> a) const {
		return a.first * 10000 + a.second;
	}
};

class CFlowVertex;

//����� ����. ������������� ������ �����, �������� ����� � ���������� �����������
class CFlowEdge : public CBasicEdge<CFlowVertex> {
private:
	CFlowEdge* reverse;
	long long flow, capacity;
public:
	CFlowEdge() : flow(0), capacity(0), reverse(NULL) {}
	CFlowEdge(CFlowVertex* from, CFlowVertex* to, int c) : CBasicEdge(from, to), flow(0), capacity(c) {
		reverse = new CFlowEdge();
		reverse->from = to;
		reverse->to = from;
		reverse->reverse = this;
	}
	~CFlowEdge(){}
	
	//�������� �������� �����
	CFlowEdge* GetReverse() const {
		return reverse;
	}

	//���������� �������� �����
	void SetReverse(CFlowEdge* reverse) {
		reverse = reverse;
	}

	//�������� ���������� �����������
	long long GetCapacity() const {
		return capacity;
	}

	//��������� ���������� ����������� �� dc
	void AddCapacity(long long dc) {
		capacity += dc;
	}

	//�������� �����
	long long GetFlow() const {
		return flow;
	}

	//��������� ����� �� df
	void AddFlow(long long df) {
		flow += df;
	}

	//�������� �� �����
	bool IsFull() const {
		return capacity - flow <= 0;
	}
};

//������� ����
class CFlowVertex : public �BasicVertex<CFlowEdge> {
private:
	int height, currentEdge;
	long long extraFlow;
public:
	CFlowVertex(int _num) : currentEdge(0), extraFlow(0), height(0), �BasicVertex<CFlowEdge>(_num) {}
	CFlowVertex(int _extraFlow, int _height, int _num) : currentEdge(0), extraFlow(_extraFlow), height(_height), �BasicVertex<CFlowEdge>(_num) {}
	~CFlowVertex(){}

	//�������� ������ �����
	long long GetExtraFlow() const {
		return extraFlow;
	}

	//�������� ������ �����
	void AddFlow(long long flow) {
		extraFlow += flow;
	}

	//�������� ������
	int GetHeight() const {
		return height;
	}

	//���������� ������
	void SetHeight(int h) {
		height = h;
	}

	//������� �������
	void Relabel() {
		int d = INF;
		for (int i = 0; i < edges.size(); ++i) {
			CFlowEdge* curr = edges[i];
			if (!curr->IsFull())
				d = min(d, curr->GetTo()->GetHeight());
		}

		if (d == INF)
			return;
		height = d + 1;
	}

	//�������� ������� �����
	CFlowEdge* GetCurrentEdge() const {
		if (currentEdge >= 0 && currentEdge < edges.size())
			return edges[currentEdge];
		return NULL;
	}

	//��������� ��������� �� ������� �����
	void IncCurrent() {
		currentEdge++;
	}

	//���������� ��������� �� ������� ����� � ������� i
	void SetCurrent(int i) {
		currentEdge = i;
	}
};

//����� ����
class CNetwork : public CGraph<CFlowEdge, CFlowVertex> {
public:
	CNetwork(){}
	~CNetwork(){}
	//��������� ������� ���� �� vertexCount ������, edgeCount >= vertexCount - 1 �����, � ������������ ���������� ������������ maxCapacity
	//�������� �� O(V^2)
	//������� ������� ��������� ������, ����� ��������� ��� ��������� �����
	CNetwork(int vertexCount, int edgesCount, int maxCapacity) {
		vector<int> vertex;
		vector<pair<int, int> > edge;
		unordered_set<pair<int, int>, Hash > used;
		for (int i = 0; i < vertexCount; ++i) {
			vertex.push_back(i);
			vertices.push_back(new CFlowVertex(i));
		}
		for (int i = 0; i < vertexCount; ++i) {
			for (int j = 0; j < vertexCount; ++j) {
				if (i != j)
					edge.push_back(make_pair(i, j));
			}
		}
		random_shuffle(vertex.begin() + 1, vertex.end());
		random_shuffle(edge.begin(), edge.end());
		for (int i = 1; i < vertexCount; ++i) {
			int from = vertex[rand() % i];
			int to = vertex[i];
			int capacity = rand() % maxCapacity + 1;
			CFlowEdge* newEdge = new CFlowEdge(vertices[from], vertices[to], capacity);
			vertices[from]->AddEdge(newEdge);
			vertices[to]->AddEdge(newEdge->GetReverse());
			used.insert(make_pair(from, to));
			edges.push_back(newEdge);
		}
		int added = vertexCount - 1;
		for (int i = 0; added < edgesCount; ++i) {
			int from = edge[i].first;
			int to = edge[i].second;
			if (used.find(make_pair(from, to)) != used.end())
				continue;
			int capacity = rand() % maxCapacity + 1;
			if (used.find(make_pair(to, from)) != used.end()) {
				while (vertices[from]->GetCurrentEdge()->GetTo()->GetNum() != to) {
					vertices[from]->IncCurrent();
				}
				vertices[from]->GetCurrentEdge()->AddCapacity(capacity);
				added++;
				used.insert(make_pair(from, to));
				continue;
			}
			CFlowEdge* newEdge = new CFlowEdge(vertices[from], vertices[to], capacity);
			vertices[from]->AddEdge(newEdge);
			vertices[to]->AddEdge(newEdge->GetReverse());
			edges.push_back(newEdge);
			used.insert(make_pair(from, to));
			added++;
		}
	}

	//����������� �����������
	CNetwork(const CNetwork& _network) {
		for (int i = 0; i < _network.GetSize(); ++i) {
			vertices.push_back(new CFlowVertex(_network.GetVertex(i)->GetExtraFlow(), _network.GetVertex(i)->GetHeight(), i));
		}
		for (int i = 0; i < _network.GetEdgesCount(); ++i) {
			CFlowEdge* oldEdge = _network.GetEdge(i);
			CFlowVertex* from = vertices[_network.GetEdge(i)->GetFrom()->GetNum()];
			CFlowVertex* to = vertices[_network.GetEdge(i)->GetTo()->GetNum()];
			CFlowEdge* edge = new CFlowEdge(from, to, oldEdge->GetCapacity());
			edge->AddFlow(oldEdge->GetFlow());
			edge->GetReverse()->AddFlow(oldEdge->GetReverse()->GetFlow());
			from->AddEdge(edge);
			to->AddEdge(edge->GetReverse());
			edges.push_back(edge);
		}
	}

	//������� ���� �� ������
	CNetwork(istream& in) {
		int n, m, from, to, capacity;
		in >> n >> m;
		for (int i = 0; i < n; ++i) {
			vertices.push_back(new CFlowVertex(i));
		}
		for (int i = 0; i < m; ++i) {
			in >> from >> to >> capacity;
			from--;
			to--;
			CFlowEdge* edge = new CFlowEdge(vertices[from], vertices[to], capacity);
			vertices[from]->AddEdge(edge);
			vertices[to]->AddEdge(edge->GetReverse());
			edges.push_back(edge);
		}
	}

	//���������� �������� ������ �� �����
	void Push(CFlowEdge* edge) {
		CFlowVertex* from = edge->GetFrom();
		CFlowVertex* to = edge->GetTo();
		int d = min(from->GetExtraFlow(), 0ll + edge->GetCapacity() - edge->GetFlow());
		edge->AddFlow(d);
		edge->GetReverse()->AddFlow(-d);
		from->AddFlow(-d);
		to->AddFlow(d);
	}

	//��������� �������
	void Discharge(CFlowVertex* vertex) {
		while (vertex->GetExtraFlow() > 0) {
			CFlowEdge* edge = vertex->GetCurrentEdge();
			if (edge == NULL) {
				vertex->Relabel();
				vertex->SetCurrent(0);
			}
			else {
				CFlowVertex* from = edge->GetFrom();
				CFlowVertex* to = edge->GetTo();
				if (!edge->IsFull() && from->GetHeight() == to->GetHeight() + 1) {
					Push(edge);
				}
				else {
					vertex->IncCurrent();
				}
			}
		}
	}
};

class CStep {
private:
	CNetwork network;
	int changedVertex;
	string description;
public:
	CStep(const CNetwork& network, int changedVertex, string description) : network(network), changedVertex(changedVertex), description(description) {}
	
	CNetwork& GetNetwork() {
		return network;
	}
	
	int GetChangedVertex() const {
		return changedVertex;
	}

	string GetDescription() const {
		return description;
	}
};

//�������� ������� � ������ � �������� �� O(V^3)
//���������� ��������� ���� �� ������ ���� � ��� ��������
vector<CStep> RelabelToFront(CNetwork& network) {
	vector<CStep> steps;
	steps.push_back(CStep(network, 0, "Initial network (0 - source, " + to_string(network.GetSize() - 1) + " - sink)"));
	CFlowVertex* start = network.GetVertex(0);
	start->SetCurrent(0);
	CFlowEdge* edge = start->GetCurrentEdge();
	while (edge) {
		edge->AddFlow(edge->GetCapacity());
		edge->GetReverse()->AddFlow(-edge->GetCapacity());
		CFlowVertex* to = edge->GetTo();
		to->AddFlow(edge->GetCapacity());
		start->AddFlow(-edge->GetCapacity());
		start->IncCurrent();
		edge = start->GetCurrentEdge();
	}
	start->SetHeight(network.GetSize());
	steps.push_back(CStep(network, 0, "Init flow from vertex 0"));
	list<CFlowVertex*> goodList;
	for (int i = 0; i < network.GetSize(); ++i) {
		network.GetVertex(i)->SetCurrent(0);
		if (i > 0 && i < network.GetSize() - 1)
			goodList.push_back(network.GetVertex(i));
	}
	list<CFlowVertex*>::iterator current = goodList.begin();
	while (current != goodList.end()) {
		CFlowVertex* vertex = *current;
		int oldHeight = vertex->GetHeight();
		network.Discharge(vertex);
		if (vertex->GetHeight() > oldHeight) {
			goodList.erase(current);
			current = goodList.insert(goodList.begin(), vertex);
		}
		steps.push_back(CStep(network, vertex->GetNum(), "Discharge(" + to_string(vertex->GetNum()) + ")"));
		current++;
	}
	steps.push_back(CStep(network, network.GetSize() - 1, "Finished! Max flow to vertex " + to_string(network.GetSize() - 1) + " equals " + to_string(network.GetVertex(network.GetSize() - 1)->GetExtraFlow())));
	return steps;
}

#endif FLOW_H
