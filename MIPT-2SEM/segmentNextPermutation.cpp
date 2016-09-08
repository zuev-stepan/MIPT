#include <iostream>
#include <stdio.h>
#include <algorithm>

using namespace std;

const int inf = 2000000000;

struct vert {
    int value, prior, cnt, sum;
    int increasing_len, decreasing_len;//, min, max;
    bool reversed;
    vert *l, *r;
    vert *left, *right;
    vert(){l = r = NULL; left = right = this;}
    vert(int a) {
        value = a;
        decreasing_len = increasing_len = 1;
        reversed = false;
        cnt = 1, sum = a;
        prior = (rand() << 15) | rand();
        l = r = NULL;
        left = right = this;
    }
};

inline int val(vert* t) {
    return (t ? t->value : -inf);
}

inline int sum(vert* t) {
    return (t ? t->sum : 0);
}

inline int size(vert* t) {
    return (t ? t->cnt : 0);
}

inline int dec_len(vert* t) {
    return (t ? (t->reversed ? t->increasing_len : t->decreasing_len) : 0);
}

inline int inc_len(vert* t) {
    return (t ? (t->reversed ? t->decreasing_len : t->increasing_len) : 0);
}

inline vert* left(vert* t) {
    return (t ? t->left : NULL);
}

inline vert* right(vert* t) {
    return (t ? t->right : NULL);
}

inline void push(vert* t) {
    if (!t)
        return;
    if (t->l) {
        t->l->reversed ^= t->reversed;
    }
    if (t->r) {
        t->r->reversed ^= t->reversed;
    }
    if (t->reversed) {
        t->reversed = false;
        swap(t->l, t->r);
        swap(t->decreasing_len, t->increasing_len);
    }
}

inline void update(vert* t) {
    if (!t) return;
    push(t);
    t->left = t->right = t;
    if (t->l)
        t->left = t->l->left;
    if (t->r)
        t->right = t->r->right;
    t->cnt = size(t->l) + size(t->r) + 1;
    t->sum = t->value + sum(t->l) + sum(t->r);
    t->decreasing_len = dec_len(t->r);
    if (dec_len(t->r) == size(t->r)) {
        if (val(left(t->r)) <= t->value) {
            t->decreasing_len++;
            if (val(right(t->l)) >= t->value)
                t->decreasing_len += dec_len(t->l);
        }
    }
    t->increasing_len = inc_len(t->l);
    if (inc_len(t->l) == size(t->l)) {
        if (val(right(t->l)) <= t->value) {
            t->increasing_len++;
            if (val(left(t->r)) >= t->value)
                t->increasing_len += inc_len(t->r);
        }
    }
}

void merge(vert* &t, vert* l, vert* r) {
    if (!l || !r) {
        t = l ? l : r;
    }
    else if (l->prior > r->prior) {
        push(l);
        push(r);
        merge(l->r, l->r, r);
        t = l;
    }
    else {
        push(l);
        push(r);
        merge(r->l, l, r->l);
        t = r;
    }
    update(t);
}

void split(vert* t, vert* &l, vert* &r, int cnt) {
    if (!t) {
        l = r = NULL;
        return;
    }
    push(t);
    if (size(t->l) >= cnt) {
        split(t->l, l, t->l, cnt);
        r = t;
    }
    else {
        split(t->r, t->r, r, cnt - size(t->l) - 1);
        l = t;
    }
    update(t);
}

vert *root, *t1, *t2, *t3;
int n, m, x, y, a, b;

void insert(int x, int pos) {
    split(root, t1, t2, pos - 1);
    merge(t1, t1, new vert(x));
    merge(root, t1, t2);
}

void set(vert* t, int x, int pos) {
    push(t);
    if (pos == size(t->l) + 1)
        t->value = x;
    else if (pos <= size(t->l))
        set(t->l, x, pos);
    else
        set(t->r, x, pos - size(t->l) - 1);
    update(t);
}

vert* get(vert* t, int pos) {
    push(t);
    if (pos == size(t->l) + 1) {
        return t;
    }
    else if (pos <= size(t->l))
        return get(t->l, pos);
    else
        return get(t->r, pos - size(t->l) - 1);
}

int get_sum(int l, int r) {
    split(root, t1, t2, l - 1);
    split(t2, t2, t3, r - l + 1);
    int tmp = sum(t2);
    merge(t2, t2, t3);
    merge(root, t1, t2);
    return tmp;
}

vert* bin_search(vert* t, vert* target) {
    if (!t)
        return NULL;
    push(t);
    vert* tmp;
    if (t->value > target->value) {
        tmp = bin_search(t->r, target);
    }
    else
        tmp = bin_search(t->l, target);
    if (tmp) {
        update(t);
        return tmp;
    }
    else {
        if (t->value > target->value) {
            swap(t->value, target->value);
            update(t);
            update(target);
            return t;
        }
        else
            return NULL;
    }

}

void next_permutation(int l, int r) {
    split(root, t1, t2, l - 1);
    split(t2, t2, t3, r - l + 1);
    vert* tmp, *t4;
    if (dec_len(t2) == size(t2)) {
        t2->reversed = true;
        merge(t2, t2, t3);
        merge(root, t1, t2);
        return;
    }
    split(t2, t2, t4, size(t2) - dec_len(t2));
    split(t2, t2, tmp, size(t2) - 1);
    bin_search(t4, tmp);
    t4->reversed = true;
    merge(t2, t2, tmp);
    merge(t2, t2, t4);
    merge(t2, t2, t3);
    merge(root, t1, t2);
}

void print(vert* t) {
    if (!t)
        return;
    push(t);
    print(t->l);
    printf("%d ", t->value);
    print(t->r);
}

int main()
{
    freopen("input.txt", "rt", stdin);
    t1 = new vert(), t2 = new vert(), t3 = new vert();
    cin >> n >> m;
    root = new vert(1);
    for (int i = 2; i <= n; i++) {
        insert(i, i);
    }
    for (int i = 1; i <= m; i++) {
        scanf("%d", &y);
        if (y == 1) {
            scanf("%d %d", &a, &b);
            printf("%d\n", get_sum(a, b));
        }
        if (y == 2) {
            scanf("%d %d", &a, &x);
            insert(x, a);
        }
        if (y == 3) {
            scanf("%d %d", &a, &x);
            set(root, x, a);
        }
        if (y == 4) {
            scanf("%d %d", &a, &b);
            next_permutation(a, b);
            print(root);
            printf("\n");
        }
    }
    print(root);
    return 0;
}
