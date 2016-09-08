#include <iostream>
#include <vector>
#include <stdio.h>
#include <list>

using namespace std;

const int INF = 2000000000;

class CBasicEdge;

class CBasicVertex {
protected:
    vector<CBasicEdge*> edges;
    int currentEdge;
public:
    CBasicVertex() : currentEdge(0) {}
    void AddEdge(CBasicEdge* edge) {
        edges.push_back(edge);
    }
    CBasicEdge* GetEdge(int i) {
        if (i >= 0 && i < edges.size())
            return edges[i];
        return NULL;
    }
    CBasicEdge* GetCurrentEdge() {
        if (currentEdge >= 0 && currentEdge < edges.size())
            return edges[currentEdge];
        return NULL;
    }
    void IncCurrent() {
        currentEdge++;
    }
    void SetCurrent(int i) {
        currentEdge = i;
    }
};

class CBasicEdge {
protected:
    CBasicVertex* from, * to;
public:
    CBasicEdge(){}
    CBasicEdge(CBasicVertex* _from, CBasicVertex* _to) : from(_from), to(_to) {}
    CBasicVertex* GetFrom() {
        return from;
    }
    CBasicVertex* GetTo() {
        return to;
    }
};

template<typename CEdge, typename CVertex>
class CGraph {
protected:
    vector<CVertex*> vertices;
    vector<CEdge*> edges;
public:
    CGraph(){}
    CVertex* GetVertex(int i) {
        if (i >= 0 && i < vertices.size())
            return vertices[i];
        return NULL;
    }
    CEdge* GetEdge(int i, int j) {
        if (GetVertex(i) != NULL)
            return GetVertex(i)->GetEdge(j);
        return NULL;
    }
    CEdge* GetEdge(int i) {
        if (i >= 0 && i < edges.size())
            return edges[i];
        return NULL;
    }
    int GetSize() {
        return vertices.size();
    }
    int GetEdgesCount() {
        return edges.size();
    }
};

class CFlowVertex;

class CFlowEdge : public CBasicEdge {
private:
    CFlowEdge* reverse;
    long long flow, capacity;
public:
    CFlowEdge() : flow(0), capacity(0), reverse(NULL){}
    CFlowEdge(CFlowVertex* from, CFlowVertex* to, int c) : CBasicEdge((CBasicVertex*) from, (CBasicVertex*) to), flow(0), capacity(c) {
        reverse = new CFlowEdge();
        reverse->from = (CBasicVertex*) to;
        reverse->to = (CBasicVertex*) from;
        reverse->reverse = this;
    }
    CFlowEdge* GetReverse() {
        return reverse;
    }
    void SetReverse(CFlowEdge* reverse) {
        reverse = reverse;
    }
    long long GetCapacity() {
        return capacity;
    }
    void AddCapacity(long long c) {
        capacity += c;
    }
    long long GetFlow () {
        return flow;
    }
    void AddFlow(long long df) {
        flow += df;
    }
    bool IsFull() {
        return capacity - flow <= 0;
    }
};

class CFlowVertex : public CBasicVertex {
private:
    int height;
    long long extraFlow;
public:
    CFlowVertex() : extraFlow(0), height(0) {}
    CFlowVertex(int _extraFlow, int _height) : extraFlow(_extraFlow), height(_height) {}
    long long GetExtraFlow() {
        return extraFlow;
    }
    void AddFlow(long long flow) {
        extraFlow += flow;
    }
    int GetHeight() {
        return height;
    }
    void SetHeight(int h) {
        height = h;
    }
    void Relabel() {
        int d = INF;
        for (int i = 0; i < edges.size(); ++i) {
            CFlowEdge* curr = (CFlowEdge*) edges[i];
            if (!curr->IsFull())
                d = min(d, ((CFlowVertex*) curr->GetTo())->GetHeight());
        }

        if (d == INF)
            return;
        height = d + 1;
    }
};

class CNetwork : public CGraph<CFlowEdge, CFlowVertex> {
public:
    CNetwork(){}
    CNetwork(const vector<CFlowVertex*>& _vertices) {
        for (int i = 0; i < _vertices.size(); ++i) {
            vertices.push_back(_vertices[i]);
        }
    }
    CNetwork(istream& in) {
        int n, m, from, to, capacity;
        in >> n >> m;
        for (int i = 0; i < n; ++i) {
            vertices.push_back(new CFlowVertex());
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
    void Push(CFlowEdge* edge) {
        CFlowVertex* from = (CFlowVertex*) edge->GetFrom();
        CFlowVertex* to = (CFlowVertex*) edge->GetTo();
        int d = min(from->GetExtraFlow(), 0ll + edge->GetCapacity() - edge->GetFlow());
        edge->AddFlow(d);
        edge->GetReverse()->AddFlow(-d);
        from->AddFlow(-d);
        to->AddFlow(d);
    }
    void Discharge(CFlowVertex* vertex) {
        while (vertex->GetExtraFlow() > 0) {
            CFlowEdge* edge = (CFlowEdge*) (vertex->GetCurrentEdge());
            if (edge == NULL) {
                vertex->Relabel();
                vertex->SetCurrent(0);
            }
            else {
                CFlowVertex* from = (CFlowVertex*) edge->GetFrom();
                CFlowVertex* to = (CFlowVertex*) edge->GetTo();
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

long long RelabelToFront(CNetwork& network) {
    CFlowVertex* start = network.GetVertex(0);
    start->SetCurrent(0);
    CFlowEdge* edge = (CFlowEdge*) start->GetCurrentEdge();
    while(edge) {
        edge->AddFlow(edge->GetCapacity());
        edge->GetReverse()->AddFlow(-edge->GetCapacity());
        CFlowVertex* to = (CFlowVertex*) edge->GetTo();
        to->AddFlow(edge->GetCapacity());
        start->AddFlow(-edge->GetCapacity());
        start->IncCurrent();
        edge = (CFlowEdge*) start->GetCurrentEdge();
    }
    start->SetHeight(network.GetSize());
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
        current++;
    }
    return network.GetVertex(network.GetSize() - 1)->GetExtraFlow();
}

int main()
{
    //freopen("input.txt", "rt", stdin);
    CNetwork network(cin);
    cout << RelabelToFront(network) << '\n';
    for (int i = 0; i < network.GetEdgesCount(); ++i) {
        cout << network.GetEdge(i)->GetFlow() << '\n';
    }
    return 0;
}
