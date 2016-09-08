#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>

using namespace std;

typedef vector<vector<double> > Graph;

vector<int> go[100000];
struct edge
{
    int x, y;
    double l;
    edge(){}
    edge(int a, int b, double c) : x(a), y(b), l(c) {}
};

bool cmp(edge a, edge b){return a.l < b.l;}
void dfs(int x, int k, int* used) {
	used[x] = k;
	for (int i = 0; i < go[x].size(); i++) {
		int y = go[x][i];
		if (used[y] == 0)
			dfs(y, k, used);
	}
}

double boruvka(vector<edge> r, int n) {
    sort(r.begin(), r.end(), cmp);
    int used[n];
    int m = r.size();
    int ans[m], nr[n];
    for (int i = 0; i < n; i++)
        go[i].clear();
    for (int i = 0; i < m; i++)
        ans[i] = 0;
	int sz = 0;
	int cur = 0;
	double MST = 0;
	while (sz < n - 1 && cur < n) {
		cur++;
		for (int i = 0; i < n; i++)
			used[i] = 0;
		int k = 0;
		for (int i = 0; i < n; i++)
			if (used[i] == 0)
				dfs(i, ++k, used);
		for (int i = 1; i <= k; i++)
			nr[i] = -1;
		for (int i = 0; i < m; i++)
			if (used[r[i].x] != used[r[i].y]) {
				int x = used[r[i].x];
				int y = used[r[i].y];
				if (nr[x] == -1 || r[i].l < r[nr[x]].l)
					nr[x] = i;
				if (nr[y] == -1 || r[i].l < r[nr[y]].l)
					nr[y] = i;
			}

		for (int i = 1; i <= k; i++)
			if (nr[i] != -1 && ans[nr[i]] == 0) {
				int x = nr[i];
				ans[x] = 1;
				MST += r[x].l;
				go[r[x].x].push_back(r[x].y);
				go[r[x].y].push_back(r[x].x);
				sz++;
			}
	}

	return MST;
}

Graph readGraph() {
    int n, m;
    cin >> n >> m;
    Graph g(n);
    for (int i = 0; i < n; i++) {
        g[i].resize(n);
    }
    int x, y;
    double l;
    for(int i = 0; i < m; i++) {
        cin >> x >> y >> l;
        g[x][y] = g[y][x] = l;
    }
    return g;
}

double getTime() {
    return clock() / (CLOCKS_PER_SEC / 1000);
}

int main()
{
    freopen("input.txt", "rt", stdin);
    freopen("outB.txt", "wt", stdout);
    Graph g = readGraph();
    vector<edge> a;
    for (int i = 0; i < g.size(); i++)
        for (int j = i + 1; j < g.size(); j++)
            if (g[i][j] > 0)
                a.push_back(edge(i, j, g[i][j]));
    double t = getTime();
    for(int i = 0; i < 10; i++) {
        boruvka(a, g.size());
    }
    cout << boruvka(a, g.size()) << '\n' << getTime() - t;
    return 0;
}
