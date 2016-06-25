#include <bits/stdc++.h>

const long long MAXN = 100010;

class Circle {
public:
    long long id;
    long long x, y;
    long long r;
    Circle() {}
    Circle(long long x, long long y, long long r) : x(x), y(y), r(r) {}
};

bool operator <(const Circle& c1, const Circle& c2) {
    return (c1.y < c2.y) || (c1.y == c2.y && c1.r < c2.r) || (c1.y == c2.y && c1.r == c2.r && c1.id < c2.id);
}

class Event {
public:
    long long x, r;
    long long id;
    bool open;
    Event(){}
    Event(long long x, long long r, long long id, bool open) : x(x), r(r), id(id), open(open) {}
};

bool operator <(const Event& e1, const Event& e2) {
    return (e1.x < e2.x) || (e1.x == e2.x && !e1.open && e2.open) || (e1.x == e2.x && e1.open && e2.open && e1.r > e2.r);
}

long long len2(const Circle& c1, const Circle& c2) {
    return (c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y);
}

using std::cin;
using std::cout;
using std::iostream;
using std::set;
using std::sort;
using std::fixed;
using std::setprecision;
using std::fill;

Circle circles[MAXN];
Event events[MAXN * 2];
set<Circle> current;
int n;

int main()
{
    freopen("circles.in", "rt", stdin);
    freopen("circles.out", "wt", stdout);


    cin >> n;
    for (int i = 0; i < n; ++i) {
        circles[i].id = i;
        cin >> circles[i].x >> circles[i].y >> circles[i].r;

        events[i * 2].x = circles[i].x - circles[i].r;
        events[i * 2].id = i;
        events[i * 2].r = circles[i].r;
        events[i * 2].open = true;

        events[i * 2 + 1].x = circles[i].x + circles[i].r;
        events[i * 2 + 1].id = i;
        events[i * 2 + 1].r = circles[i].r;
        events[i * 2 + 1].open = false;
    }

    sort(events, events + n * 2);
    long long ans = 0;

    for (int i = 0; i < n * 2; i++) {
        Event& e = events[i];
        if (e.open) {
            auto i = current.lower_bound(circles[e.id]);
            for (int k = 0; k < 200 && i != current.begin(); ++k, --i);
            bool ok = true;

            for (int k = 0; k < 200 && i != current.end(); ++k, ++i) {
                if (len2(*i, circles[e.id]) <= i->r * i->r) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                ans += e.r * e.r;
                current.insert(circles[e.id]);
            }
        } else {
            current.erase(circles[e.id]);
        }
    }

    cout.setf(std::ios::fixed);
    cout.precision(10);
    cout << acos(-1) * ans;
    return 0;
}