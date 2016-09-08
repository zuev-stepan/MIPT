#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

struct pt {
    int x, y, c;

    bool operator ==(const pt& a) {
        return this->x == a.x && this->y == a.y;
    }
};

bool byXI(const pt& a, const int& b) {
    return a.x < b;
}

bool byX(const pt& a, const pt& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y) || (a.x == b.x && a.y == b.y && a.c < b.c);
}

bool byYI(const pt& a, const int& b) {
    return a.y < b;
}

bool byY(const pt& a, const pt& b) {
    return a.y < b.y;
}

const int MAXN = 263000;
const int INF = 1e9 + 7;

//[x1, x2) x [y1, y2)
struct Node {
    int m;
    pt* p = NULL;
    int size = 0;
    int to_set = INF;
    int val = INF;
    Node* l = NULL, * r = NULL;
};

int counter;
void build(Node* t, pt* points, int n, int tx1 = -INF, int ty1 = -INF, int tx2 = INF, int ty2 = INF, bool split = 0) {
    counter++;
    if (counter > 1000) {
        return;
    }
    if (n < 1) {
        return;
    }
    t->p = points;
    t->size = n;

    if (n == 1) {
        t->val = points->c;
        return;
    }

    if (split) {
        sort(points, points + n, byY);

        t->m = points[n / 2].y;
        pt* j = lower_bound(points, points + n, t->m, byYI);
        if (j - points == 0) {
            t->m++;
            j = lower_bound(points, points + n, t->m, byYI);
        }
        build(t->l = new Node(), points, j - points, tx1, ty1, tx2, t->m - 1, !split);
        build(t->r = new Node(), j, n - (j - points), tx1, t->m, tx2, ty2, !split);
    } else {
        sort(points, points + n, byX);

        t->m = points[n / 2].x;
        pt* j = lower_bound(points, points + n, t->m, byXI);
        if (j - points == 0) {
            t->m++;
            j = lower_bound(points, points + n, t->m, byXI);
        }
        build(t->l = new Node(), points, j - points, tx1, ty1, t->m - 1, ty2, !split);
        build(t->r = new Node(), j, n - (j - points), t->m, ty1, tx2, ty2, !split);
    }
    t->val = min(t->l->val, t->r->val);
}

void push(Node* t) {
    if (t == NULL || t->to_set == INF) {
        return;
    }
    if (t->size == 1) {
        t->p->c = t->to_set;
        t->to_set = INF;
        return;
    }
    if (t->l && t->l->size > 0) {
        t->l->to_set = t->l->val = t->to_set;
    }
    if (t->r && t->r->size > 0) {
        t->r->to_set = t->r->val = t->to_set;
    }
    t->to_set = INF;
}

int getMin(Node* t, int x1, int y1, int x2, int y2, int tx1 = -INF, int ty1 = -INF, int tx2 = INF, int ty2 = INF, bool split = 0) {
    //check(t);
    if (x1 > tx2 || x2 < tx1 || y1 > ty2 || y2 < ty1 || t->size <= 0) {
        return INF;
    }
    push(t);
    if (x1 <= tx1 && x2 >= tx2 && y1 <= ty1 && y2 >= ty2) {
        return t->val;
    }
    if (t->size == 1) {
        if (t->p->x >= x1 && t->p->x <= x2 && t->p->y >= y1 && t->p->y <= y2) {
            return t->val;
        } else {
            return INF;
        }
    }
    if (split) {
        return min(getMin(t->l, x1, y1, x2, y2, tx1, ty1, tx2, t->m - 1, !split),
                   getMin(t->r, x1, y1, x2, y2, tx1, t->m, tx2, ty2, !split));
    } else {
        return min(getMin(t->l, x1, y1, x2, y2, tx1, ty1, t->m - 1, ty2, !split),
                   getMin(t->r, x1, y1, x2, y2, t->m, ty1, tx2, ty2, !split));
    }
}

void set(Node* t, int x1, int y1, int x2, int y2, int c, int tx1 = -INF, int ty1 = -INF, int tx2 = INF, int ty2 = INF, bool split = 0) {
    if (x1 > tx2 || x2 < tx1 || y1 > ty2 || y2 < ty1 || t->size <= 0) {
        return;
    }
    push(t);
    if (x1 <= tx1 && x2 >= tx2 && y1 <= ty1 && y2 >= ty2) {
        if (t->size == 1) {
            t->p->c = t->val = c;
            return;
        }
        t->to_set = t->val = c;
        return;
    }
    if (t->size == 1) {
        if (t->p->x >= x1 && t->p->x <= x2 && t->p->y >= y1 && t->p->y <= y2) {
            t->val = t->p->c = c;
        }
        return;
    }
    if (split) {
        set(t->l, x1, y1, x2, y2, c, tx1, ty1, tx2, t->m - 1, !split);
        set(t->r, x1, y1, x2, y2, c, tx1, t->m, tx2, ty2, !split);
    } else {
        set(t->l, x1, y1, x2, y2, c, tx1, ty1, t->m - 1, ty2, !split);
        set(t->r, x1, y1, x2, y2, c, t->m, ty1, tx2, ty2, !split);
    }

    t->val = min(t->l->val, t->r->val);
}

pt points[MAXN];
int n, m;
char c;
int X1, X2, Y1, Y2, k;

void test() {
    srand(time(NULL));
    for (int k = 0; k < 1000000; k++) {
        counter = 0;
        int n = rand() % 100;
        for (int i = 0; i < n; i++) {
            points[i].x = rand() % 100;
            points[i].y = rand() % 100;
        }
        sort(points, points + n, byX);
        n = unique(points, points + n) - points;
        if (counter > 1000) {
            for (int i = 0; i < n; i++) {
                printf("%d %d\n", points[i].x, points[i].y);
            }
            exit(0);
        }
        build(new Node(), points, n);
        if (k % 100000 == 0) {
            printf("%d\n", k);
        }
    }
}

int main()
{
    //freopen("input.txt", "rt", stdin);
    //freopen("rectmin.in", "rt", stdin);
    //freopen("rectmin.out", "wt", stdout);
    test();
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d%d%d", &points[i].x, &points[i].y, &points[i].c);
    }
    sort(points, points + n, byX);
    n = unique(points, points + n) - points;
    Node* root = new Node();

    build(root, points, n);
    scanf("%d ", &m);
    for (int i = 0; i < m; i++) {
        scanf("%c%d%d%d%d ", &c, &X1, &Y1, &X2, &Y2);
        if (X1 > X2) {
            swap(X1, X2);
        }
        if (Y1 > Y2) {
            swap(Y1, Y2);
        }
        if (c == '=') {
            scanf("%d ", &k);
            set(root, X1, Y1, X2, Y2, k);
        } else {
            int t = getMin(root, X1, Y1, X2, Y2);
            if (t == INF) {
                printf("NO\n");
            } else {
                printf("%d\n", t);
            }
        }
    }
    return 0;
}
