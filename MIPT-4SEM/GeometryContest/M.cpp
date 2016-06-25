#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

struct pt {
    int x, y, c;

    int& operator[](int i) {
        if (i == 0) {
            return x;
        } else {
            return y;
        }
    }

    const int& operator[](int i) const {
        if (i == 0) {
            return x;
        } else {
            return y;
        }
    }

    bool operator ==(const pt& a) {
        return this->x == a.x && this->y == a.y;
    }

    pt(){}
    pt(int a, int b) : x(a), y(b) {}

    pt cut(pt& t, int i) {
        pt tmp = *this;
        tmp[i] = t[i];
        return tmp;
    }
};

const int MAXN = 262144;
const int DIMENSIONS = 2;
const int INF = 1e9 + 7;

//[x1, x2) x [y1, y2)
struct Node {
    pt* points = NULL;
    int size = 0;
    int to_set = INF;
    int val = INF;
    Node* l = NULL, * r = NULL;
};

void build(Node* t, pt* points, int n, pt p1 = pt(-INF, -INF), pt p2 = pt(INF, INF), int axis = 0) {
    if (n < 1) {
        return;
    }
    nth_element(points, points + n / 2, points + n, [axis](const pt& a, const pt& b) {
        return a[axis] < b[axis];
    });
    t->points = points;
    t->size = n;
    t->val = points[n / 2].c;
    if (n == 1) {
        return;
    }
    if (n / 2 > 0) {
        build(t->l = new Node(), points, n / 2, p1, p2.cut(points[n / 2], axis), (axis + 1) % DIMENSIONS);
        t->val = min(t->val, t->l->val);
    }
    if (n / 2 + 1 < n) {
        build(t->r = new Node(), points + n / 2 + 1, n - n / 2 - 1, p1.cut(points[n / 2], axis), p2, (axis + 1) % DIMENSIONS);
        t->val = min(t->val, t->r->val);
    }
}

void push(Node* t) {
    //check(t);
    if (t == NULL || t->to_set == INF) {
        return;
    }
    t->points[t->size / 2].c = t->to_set;
    if (t->l && t->l->size > 0) {
        t->l->to_set = t->l->val = t->to_set;
    }
    if (t->r && t->r->size > 0) {
        t->r->to_set = t->r->val = t->to_set;
    }
    t->to_set = INF;
}

int getMin(Node* t, pt q1, pt q2, pt p1 = pt(-INF, -INF), pt p2 = pt(INF, INF), int axis = 0) {
    for (int i = 0; i < DIMENSIONS; i++) {
        if (q1[i] > p2[i] || q2[i] < p1[i]) {
            return INF;
        }
    }
    if (t == NULL || t->size <= 0) {
        return INF;
    }
    push(t);
    bool inside = true;
    for (int i = 0; i < DIMENSIONS; i++) {
        if (q1[i] > p1[i] || q2[i] < p2[i]) {
            inside = false;
            break;
        }
    }
    if (inside) {
        return t->val;
    }

    int tmp = t->points[t->size / 2].c;
    for (int i = 0; i < DIMENSIONS; i++) {
        if (q1[i] > t->points[t->size / 2][i] || q2[i] < t->points[t->size / 2][i]) {
            tmp = INF;
        }
    }

    return min(tmp,
               min(getMin(t->l, q1, q2, p1, p2.cut(t->points[t->size / 2], axis), (axis + 1) % DIMENSIONS),
               getMin(t->r, q1, q2, p1.cut(t->points[t->size / 2], axis), p2, (axis + 1) % DIMENSIONS)));
}

int set(Node* t, pt q1, pt q2, int c, pt p1 = pt(-INF, -INF), pt p2 = pt(INF, INF), int axis = 0) {
    if (t == NULL || t->size <= 0) {
        return INF;
    }
    push(t);
    for (int i = 0; i < DIMENSIONS; i++) {
        if (q1[i] > p2[i] || q2[i] < p1[i]) {
            return t->val;
        }
    }

    bool inside = true;
    for (int i = 0; i < DIMENSIONS; i++) {
        if (q1[i] > p1[i] || q2[i] < p2[i]) {
            inside = false;
            break;
        }
    }
    if (inside) {
        t->points[t->size/2].c = t->to_set = t->val = c;
        return c;
    }

    inside = true;
    for (int i = 0; i < DIMENSIONS; i++) {
        if (q1[i] > t->points[t->size / 2][i] || q2[i] < t->points[t->size / 2][i]) {
            inside = false;
        }
    }
    if (inside) {
        t->points[t->size / 2].c = c;
    }
    t->val = min(t->points[t->size / 2].c,
                 min(set(t->l, q1, q2, c, p1, p2.cut(t->points[t->size / 2], axis), (axis + 1) % DIMENSIONS),
                 set(t->r, q1, q2, c, p1.cut(t->points[t->size / 2], axis), p2, (axis + 1) % DIMENSIONS)));
}

pt points[MAXN];
int n, m;
char c;
int X1, X2, Y1, Y2, k;

int main()
{
    //freopen("input.txt", "rt", stdin);
    freopen("rectmin.in", "rt", stdin);
    freopen("rectmin.out", "wt", stdout);
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d%d%d", &points[i].x, &points[i].y, &points[i].c);
    }
    stable_sort(points, points + n, [](const pt& a, const pt& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y) || (a.x == b.x && a.y == b.y && a.c < b.c);
    });
    n = unique(points, points + n) - points;
    Node* root = new Node();

    build(root, points, n);
    scanf("%d ", &m);
    for (int i = 0; i < m; i++) {
        scanf("%c%d%d%d%d ", &c, &X1, &Y1, &X2, &Y2);
        if (c == '=') {
            scanf("%d ", &k);
            set(root, pt(X1, Y1), pt(X2, Y2), k);
        } else {
            int t = getMin(root, pt(X1, Y1), pt(X2, Y2));
            if (t == INF) {
                printf("NO\n");
            } else {
                printf("%d\n", t);
            }
        }
    }
    return 0;
}