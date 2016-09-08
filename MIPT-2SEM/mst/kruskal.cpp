#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>

using namespace std;

typedef vector<vector<double> > Graph;

int p[100000], r[100000];
struct edge
{
    int x, y;
    double l;
    edge(){}
    edge(int a, int b, double c) : x(a), y(b), l(c) {}
};

bool cmp(edge a, edge b){return a.l < b.l;}
int find(int x)
{
    return (x == p[x]) ? x : (p[x] = find(p[x]));
}
void unite(int x, int y)
{
    int a = find(x), b = find(y);
    if (a == b)
        return;
    if (r[a] < r[b])
        p[a] = b, r[b] += r[a];
    else
        p[b] = a, r[a] += r[b];
}

double kruskal(vector<edge> a, int n)
{
    double ans = 0;
    for (int i = 1; i <= n; i++)
        p[i] = i, r[i] = 1;
    sort(a.begin(), a.end(), cmp);
    for (int i = 0; i < a.size(); i++)
        if (find(a[i].x + 1) != find(a[i].y + 1))
            unite(a[i].x + 1, a[i].y + 1), ans += a[i].l;//, used[i] = true;
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
    freopen("outK.txt", "wt", stdout);
    Graph g = readGraph();
    vector<edge> a;
    for (int i = 0; i < g.size(); i++)
        for (int j = i + 1; j < g.size(); j++)
            if (g[i][j] > 0)
                a.push_back(edge(i, j, g[i][j]));
    double t = getTime();
    for(int i = 0; i < 10; i++) {
        kruskal(a, g.size());
    }
    cout << kruskal(a, g.size()) << '\n' << getTime() - t;
    return 0;
}
