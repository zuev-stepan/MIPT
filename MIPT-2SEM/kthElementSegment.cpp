#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <ctime>

using namespace std;

struct Element {
    int val, pos;
    Element(){}
    Element(int a, int b) : val(a), pos(b) {}
};

struct Query {
    int l, r, k;
};

bool operator <(const Element &a, const Element &b) {
    return a.val < b.val || (a.val == b.val && a.pos < b.pos);
}


class Node {
public:
    int key;
    Node *left, *right;
    Node() : left(NULL), right(NULL), key(0) {}
    Node(Node* t) : left(t->left), right(t->right), key(t->key) {}
};

Node* add(Node *t, int l, int r, int pos) {
    Node* current = t ? new Node(t) : new Node();
    if (pos == l && pos == r) {
        current->key = 1;
        return current;
    }
    current->key++;
    if (pos <= (l + r) / 2) {
        current->left = add(current->left, l, (l + r) / 2, pos);
    }
    else {
        current->right = add(current->right, (l + r) / 2 + 1, r, pos);
    }
    return current;
}

int ask(Node *t, int l, int r, int tl, int tr) {
    if (tl > tr || !t)
        return 0;
    if (l == tl && r == tr)
        return t->key;
    return  ask(t->left, l, (l + r) / 2, tl, min(tr, (l + r) / 2)) +
            ask(t->right, (l + r) / 2 + 1, r, max((l + r) / 2 + 1, tl), tr);
}

int smart_ask(Node *TL, Node *TR, int l, int r, int k) {
    if (l == r) {
        return l;
    }
    int cnt = 0;
    if (TR && TR->left)
        cnt += TR->left->key;
    if (TL && TL->left)
        cnt -= TL->left->key;
    if (cnt >= k)
        return smart_ask(TL ? TL->left : NULL, TR->left, l, (l + r) / 2, k);
    else
        return smart_ask(TL ? TL->right : NULL, TR->right, (l + r) / 2 + 1, r, k - cnt);
}

double solveMlogN(vector<int> a, vector<Query> q) {
    double time = clock() / (CLOCKS_PER_SEC / 1000);
    int n = a.size(), m = q.size();
    vector<Element> sorted(a.size());
    for (int i = 0; i < n; i++)
        sorted[i].val = a[i], sorted[i].pos = i;
    sort(sorted.begin(), sorted.end());
    vector<Node*> trees(n);
    trees[0] = add(NULL, 0, n - 1, lower_bound(sorted.begin(), sorted.end(), Element(a[0], 0)) - sorted.begin());
    for (int i = 1; i < n; i++) {
        trees[i] = add(trees[i - 1], 0, n - 1, lower_bound(sorted.begin(), sorted.end(), Element(a[i], i)) - sorted.begin());
    }
    for (int i = 0; i < m; i++) {
        int pos = smart_ask(q[i].l > 0 ? trees[q[i].l - 1] : NULL, trees[q[i].r], 0, n - 1, q[i].k);
        //printf("%d\n", sorted[pos].val);
    }
    return clock() / (CLOCKS_PER_SEC / 1000) - time;
}

double solveMN(vector<int> a, vector<Query> q) {
    double time = clock() / (CLOCKS_PER_SEC / 1000);
    int n = a.size(), m = q.size();
    vector<int> buf(n);
    for (int i = 0; i < m; i++) {
        copy(a.begin() + q[i].l, a.begin() + q[i].r + 1, buf.begin());
        nth_element(buf.begin(), buf.begin() + q[i].k - 1, buf.begin() + (q[i].r - q[i].l + 1));
        //printf("%d\n", buf[q[i].k - 1]);
    }
    return clock() / (CLOCKS_PER_SEC / 1000) - time;
}

double solveMNlogN(vector<int> a, vector<Query> q) {
    double time = clock() / (CLOCKS_PER_SEC / 1000);
    int n = a.size(), m = q.size();
    vector<int> buf(n);
    for (int i = 0; i < m; i++) {
        copy(a.begin() + q[i].l, a.begin() + q[i].r + 1, buf.begin());
        sort(buf.begin(), buf.begin() + q[i].r - q[i].l + 1);
        //printf("%d\n", buf[q[i].k - 1]);
    }
    return clock() / (CLOCKS_PER_SEC / 1000) - time;
}

int main()
{
    freopen("input.txt", "rt", stdin);
    int n, m, l, r, k;
    scanf("%d", &n);
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        //scanf("%d", &a[i]);
        a[i] = rand();
    }
    scanf("%d", &m);
    vector<Query> q(m);
    for (int i = 0; i < m; i++) {
        //scanf("%d%d%d", &q[i].l, &q[i].r, &q[i].k);
        q[i].l = rand() % n;
        q[i].r = q[i].l + rand() % (n - q[i].l);
        q[i].k = 1 + rand() % (q[i].r - q[i].l + 1);
    }
    printf("MlogN: %f\n", solveMlogN(a, q));
    printf("MN: %f\n", solveMN(a, q));
    printf("MNlogN: %f\n", solveMNlogN(a, q));
    return 0;
}
