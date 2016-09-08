#include <iostream>
#include <stdlib.h>

using namespace std;

long long p[1000010];

struct vert {
    int prior, sz;
    char key;
    long long hash, rev_hash;
    vert *l, *r;
    vert(){}
    vert(char key) : key(key), prior((rand() << 15) | rand()), sz(1), hash(key), rev_hash(key), l(NULL), r(NULL) {}
};

inline int size(vert *t) {
    return (t ? t->sz : 0);
}

inline int get_hash(vert *t) {
    return (t ? t->hash : 0);
}

inline int get_rev_hash(vert *t) {
    return (t ? t->rev_hash : 0);
}

inline bool good(vert *t) {
    return (t->hash == t->rev_hash);
}

inline void update(vert * &t) {
    if (!t)
        return;
    t->sz = size(t->l) + size(t->r) + 1;
    t->hash = get_hash(t->r) + t->key * p[size(t->r)] + get_hash(t->l) * p[size(t->r) + 1];
    t->rev_hash = get_rev_hash(t->l) + t->key * p[size(t->l)] + get_rev_hash(t->r) * p[size(t->l) + 1];
}

void split(vert *t, vert * &l, vert * &r, int cnt) {
    if (!t) {
        l = r = NULL;
        return;
    }
    if (size(t->l) >= cnt) {
        split(t->l, l, t->l, cnt);
        r = t;
    }
    else {
        split(t->r, t->r, r, cnt - size(t->l) - 1);
        l = t;
    }
    update(l);
    update(r);
}

void merge(vert * &t, vert *l, vert *r) {
    if (!l || !r) {
        t = (l ? l : r);
        return;
    }
    if (l->prior > r->prior) {
        merge(l->r, l->r, r);
        t = l;
    }
    else {
        merge(r->l, l, r->l);
        t = r;
    }
    update(t);
}

int main()
{
    int n, k;
    cin >> n;
    char x;
    int l, r, t;
    cin >> x;
    vert *root = new vert(x), *t1 = NULL, *t2 = NULL, *t3 = NULL;
    p[0] = 1;
    for (int i = 1; i <= n; i++)
        p[i] = p[i - 1] * 239;
    for (int i = 1; i < n; i++) {
        cin >> x;
        vert *tmp = new vert(x);
        merge(root, root, tmp);
    }
    cin >> k;
    for (int i = 0; i < k; i++) {
        cin >> t;
        if (t == 1) {
            cin >> t >> x;
            split(root, t1, t2, t);
            split(t1, t1, t3, t - 1);
            t3->key = x;
            merge(t1, t1, t3);
            merge(root, t1, t2);
        }
        else {
            cin >> l >> r;
            split(root, t1, t2, l - 1);
            split(t2, t2, t3, r - l + 1);
            cout << (good(t2) ? "YES\n" : "NO\n");
            merge(t2, t2, t3);
            merge(root, t1, t2);
        }
    }
    return 0;
}
