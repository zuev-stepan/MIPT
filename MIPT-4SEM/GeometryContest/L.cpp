#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 100010;

struct query {
    int i, x, l, r;
    bool first;
};

bool operator<(const query& a, const query& b) {
    return a.x < b.x;
}

int a[MAXN], b[MAXN], f[MAXN];
int n;

void add(int x) {
    for (int i = x; i <= n; i += i&(-i)) {
        f[i]++;
    }
}

int get(int x) {
    int ans = 0;
    for (int i = x; i > 0; i -= i&(-i)) {
        ans += f[i];
    }
    return ans;
}

int sum(int l, int r) {
    return get(r) - get(l - 1);
}

int main()
{
    //freopen("input.txt", "rt", stdin);
    freopen("permutation.in", "rt", stdin);
    freopen("permutation.out", "wt", stdout);
    int m, x, y, l, r;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &x);
        a[x] = i;
    }
    vector<query> q(m * 2);
    for (int i = 0; i < m; i++) {
        scanf("%d%d%d%d", &l, &r, &x, &y);
        q[2 * i].l = q[2 * i + 1].l = l;
        q[2 * i].r = q[2 * i + 1].r = r;
        q[2 * i].i = q[2 * i + 1].i = i;
        q[2 * i].x = x - 1;
        q[2 * i + 1].x = y;
        q[2 * i].first = true;
        q[2 * i + 1].first = false;
    }
    stable_sort(q.begin(), q.end());
    int j = 0;
    for (int i = 1; i <= n; i++) {
        add(a[i]);
        while (j < m * 2 && q[j].x < i) {
            j++;
        }
        while (j < m * 2 && q[j].x == i) {
            if (q[j].first) {
                b[q[j].i] -= sum(q[j].l, q[j].r);
            } else {
                b[q[j].i] += sum(q[j].l, q[j].r);
            }
            j++;
        }
    }
    for (int i = 0; i < m; i++) {
        printf("%d\n", b[i]);
    }
    return 0;
}