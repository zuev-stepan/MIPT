#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>

using namespace std;

typedef vector<vector<double> > Graph;

double prima(Graph a, int n)
{
    double min_e[n];
    bool used[n];
	double ans = 0;
    for (int i = 0; i < n; i++)
        min_e[i] = 1000000000, used[i] = false;
    min_e[0] = 0;
    for (int i = 0; i < n; i++)
    {
        int v = -1;
        for (int j = 0; j < n; j++)
            if (!used[j] && (v == -1 || min_e[j] < min_e[v]))
                v = j;
        if (v == -1)
            return -1;
		ans += min_e[v];
        used[v] = true;
        for (int i = 0; i < n; i++)
            if (a[i][v] < min_e[i] && (a[i][v] > 0))
                min_e[i] = a[i][v];//, out[i] = v;
    }
    return ans;
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
    freopen("out.txt", "wt", stdout);
    Graph g = readGraph();
    double t = getTime();
    for(int i = 0; i < 10; i++) {
        prima(g, g.size());
    }
    cout << prima(g, g.size()) << '\n' << getTime() - t;
    return 0;
}
