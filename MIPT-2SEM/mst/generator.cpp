#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>

using namespace std;

typedef vector<vector<double> > Graph;

struct edge {
    int to;
    double l;
    edge(){}
    edge(int to, double l) : to(to), l(l) {}
};

vector<vector<edge> > generate_graph(int n, int p) {
    vector<vector<edge> > a(n);
    for(int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (rand() % 100 < p) {
                a[i].push_back(edge(j, ((double)rand()) / RAND_MAX));
                a[j].push_back(edge(i, a[i].back().l));
            }
        }
    }
    return a;
}

void print_graph(vector<vector<edge> > a) {
    int n = a.size();
    int m = 0;
    for (int i = 0; i < n; i++)
        m += a[i].size();
    cout << n << ' ' << m << '\n';
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < a[i].size(); j++) {
            if (a[i][j].to < i)
                continue;
            cout << i << ' ' << a[i][j].to << ' ' << a[i][j].l << '\n';
        }
    }
}
int main()
{
    freopen("input.txt", "wt", stdout);
    int n, p;
    cin >> n >> p;
    print_graph(generate_graph(n, p));
    return 0;
}
