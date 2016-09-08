#include <iostream>
#include <queue>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

const int INF = 1e9;

struct Edge {
    Edge* reverse;
    int capacity, flow, in, out;
    Edge(){}
    Edge(int in, int out, int c) : in(in), out(out), capacity(c), flow(0), reverse(NULL) {};
};

typedef vector<vector<Edge*> > Graph;

void bfs(int source, int destination, Graph& g, vector<Edge*>& p) {
    queue<int> q;
    q.push(source);
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        for (int i = 0; i < g[curr].size(); i++) {
            if (p[g[curr][i]->out] == NULL && g[curr][i]->out != source && g[curr][i]->capacity > g[curr][i]->flow && g[curr][i]->capacity > 0) {
                p[g[curr][i]->out] = g[curr][i];
                q.push(g[curr][i]->out);
                if (p[destination] != NULL)
                    break;
            }
        }
        if (p[destination] != NULL)
            break;
    }
}

int Edmonds(int source, int destination, Graph& g) {
    int maxFlow = 0;
    vector<Edge*> p(g.size());
    while (true) {
        for (int i = 0; i < p.size(); i++)
            p[i] = NULL;
        bfs(source, destination, g, p);
        if (p[destination] == NULL) {
            return maxFlow;
        }
        int flow = INF;
        for (Edge* e = p[destination]; e != NULL; e = p[e->in]) {
            flow = min(flow, e->capacity - e->flow);
        }
        for (Edge* e = p[destination]; e != NULL; e = p[e->in]) {
            e->flow += flow;
            e->reverse->flow -= flow;
        }
        maxFlow += flow;
    }
}

void buildGraph(Graph& g) {
    int n, m, x, y, z;
    cin >> n >> m;
    int c[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = 0;
        }
    }
    g.resize(n);
    for (int i = 0; i < m; i++) {
        cin >> x >> y >> z;
        c[x - 1][y - 1] = z;
    }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (c[i][j] || c[j][i]) {
                g[i].push_back(new Edge(i, j, c[i][j]));
                g[j].push_back(new Edge(j, i, c[j][i]));
                g[i].back()->reverse = g[j].back();
                g[j].back()->reverse = g[i].back();
            }
        }
    }
}

int main()
{
    freopen("input.txt", "rt", stdin);
    Graph g;
    buildGraph(g);
    cout << Edmonds(0, g.size() - 1, g);
    return 0;
}
