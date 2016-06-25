#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <algorithm>

namespace Geometry {
    using std::swap;
    using std::max;
    using std::min;
    using std::abs;
    using std::vector;
    using std::istream;
    using std::ostream;
    using std::sort;
    using std::cout;

    const float EPS = 1e-14;
    const int INF = 1e9;

    bool between(float x, float y, float t) {
        return (t >= x && t <= y);
    }

    bool equal(float x, float y) {
        return (std::abs(x - y) < EPS);
    }

    float sqr(float x) {
        return x * x;
    }

    int sign(float x) {
        if (abs(x) < EPS) {
            return 0;
        }
        return x < 0 ? -1 : 1;
    }

    bool intersect_box(float a1, float a2, float b1, float b2) {
        if (a1 > a2) {
            swap(a1, a2);
        }
        if (b1 > b2) {
            swap(b1, b2);
        }
        return max(a1, b1) <= min(a2, b2);
    }

    template <class T>
    T second_back(const vector<T>& v) {
        return v[v.size() - 2];
    }

    class Vector {
    public:
        float x, y;
        Vector(){}
        Vector(float x, float y) : x(x), y(y) {}
        Vector(const Vector& start, const Vector& end) : x(end.x - start.x), y(end.y - start.y) {}

        Vector operator -(Vector t) const {
            return Vector(x - t.x, y - t.y);
        }

        Vector operator +(Vector t) const {
            return Vector(x + t.x, y + t.y);
        }

        float operator *(Vector t) const {
            return (x * t.x + y * t.y);
        }

        Vector operator *(float t) const {
            return Vector(x * t, y * t);
        }

        Vector operator /(float t) const {
            return Vector(x / t, y / t);
        }

        float operator ^(Vector t) const {
            return (x * t.y - y * t.x);
        }

        bool operator ==(Vector t) const {
            return equal(this->x, t.x) && equal(this->y, t.y);
        }

        bool operator !=(Vector t) const {
            return !((*this) == t);
        }

        float len() const {
            return sqrt(len2());
        }

        float len2() const {
            return (*this) * (*this);
        }

        float projection(Vector t) const {
            return (*this) * t / len();
        }

        Vector normalized() const {
            return Vector(this->x / len(), this->y / len());
        }

        Vector perpendicular() const {
            return Vector(this->y, -this->x);
        }

        Vector rotated(float angle) const {
            return Vector(this->x * cos(angle) - this->y * sin(angle), this->x * sin(angle) + this->y * cos(angle));
        }
    };
    typedef Vector Point;

    istream& operator >>(istream& in, Vector& t) {
        in >> t.x >> t.y;
        return in;
    }

    ostream& operator <<(ostream& out, const Vector& t) {
        out << t.x << ' ' << t.y;
        return out;
    }

    class Segment {
    public:
        Point pt1, pt2;
        Segment() {}
        Segment(float x1, float y1, float x2, float y2) : pt1(x1, y1), pt2(x2, y2) {}
        Segment(const Point& pt1, const Point& pt2) : pt1(pt1), pt2(pt2) {}
    };

    istream& operator >>(istream& in, Segment& t) {
        in >> t.pt1 >> t.pt2;
        return in;
    }

    ostream& operator <<(ostream& out, const Segment& t) {
        out << t.pt1 << '\n' << t.pt2;
        return out;
    }

    class Line {
    public:
        float a, b, c;
        Line(){}
        Line(float a, float b, float c) : a(a), b(b), c(c) {}
    };

    istream& operator >>(istream& in, Line& t) {
        in >> t.a >> t.b >> t.c;
        return in;
    }

    ostream& operator <<(ostream& out, const Line& t) {
        out << t.a << ' ' << t.b << ' ' << t.c;
        return out;
    }

    class Circle {
    public:
        Point pt;
        float r;
        Circle() {}
        Circle(float x, float y, float r) : pt(x, y), r(r) {}
        Circle(const Point& pt, float r) : pt(pt), r(r) {}
    };

    istream& operator >>(istream& in, Circle& t) {
        in >> t.pt >> t.r;
        return in;
    }

    ostream& operator <<(ostream& out, const Circle& t) {
        out << t.pt << ' ' << t.r;
        return out;
    }

    class Polygon {
    public:
        vector<Point> vertices;
        Polygon(){}
        Polygon(const std::vector<Point>& vertices) : vertices(vertices) {}

        Point& operator [](size_t i) {
            return this->vertices[i];
        }

        size_t size() const {
            return vertices.size();
        }

        float calcArea() const {
            if (this->vertices.size() < 3) {
                return 0;
            }
            float ans = (this->vertices[0] ^ this->vertices.back());
            for (size_t i = 1; i < this->vertices.size(); i++) {
                ans += this->vertices[i] ^ this->vertices[i - 1];
            }
            return abs(ans) / 2;
        }

        float calcPerimeter() const {
            float ans = (this->vertices[0] - this->vertices.back()).len();
            for (size_t i = 1; i < this->vertices.size(); i++) {
                ans += (this->vertices[i] - this->vertices[i - 1]).len();
            }
            return ans;
        }

        bool isConvex() const {
            int prev_sign = 0;
            for (size_t i = 0; i < this->vertices.size(); i++) {
                Vector v1 = (i ? this->vertices[i - 1] : this->vertices.back()) - this->vertices[i];
                Vector v2 = (i + 1 < this->vertices.size() ? this->vertices[i + 1] : this->vertices.front()) - this->vertices[i];
                int t = sign(v1 ^ v2);
                if (prev_sign != 0 && t != prev_sign) {
                    return false;
                } else if (prev_sign == 0) {
                    prev_sign = t;
                }
            }
            return true;
        }

        Polygon convexHull() const {
            if (this->vertices.size() < 3) {
                return Polygon(this->vertices);
            }

            Point lower(INF, INF);
            for (size_t i = 0; i < this->vertices.size(); i++) {
                if (lower.y > this->vertices[i].y
                || (lower.y == this->vertices[i].y && lower.x > this->vertices[i].x)) {
                    lower = this->vertices[i];
                }
            }

            vector<Point> tmp;
            for (size_t i = 0; i < this->vertices.size(); i++) {
                if (this->vertices[i] != lower) {
                    tmp.push_back(this->vertices[i]);
                }
            }

            stable_sort(tmp.begin(), tmp.end(),
                [lower](const Point& pt1, const Point& pt2) {
                    return (((pt1 - lower) ^ (pt2 - lower)) > 0)
                           || ((((pt1 - lower) ^ (pt2 - lower)) == 0) && ((pt1 - lower).len2() < (pt2 - lower).len2()));
                }
            );

            vector<Point> vertex_stack;
            vertex_stack.push_back(lower);
            vertex_stack.push_back(tmp[0]);
            for (size_t i = 1; i < tmp.size(); ++i) {
                while ( (((tmp[i] - vertex_stack.back()) ^ (second_back(vertex_stack) - vertex_stack.back())) <= 0)
                        && (vertex_stack.size() > 1)) {
                    vertex_stack.pop_back();
                }
                vertex_stack.push_back(tmp[i]);
            }
            return Polygon(vertex_stack);
        }
    };

    istream& operator >>(istream& in, Polygon& polygon) {
        polygon.vertices.clear();
        int n;
        in >> n;
        polygon.vertices.resize(n);
        for (int i = 0; i < n; i++) {
            in >> polygon[i];
        }
        return in;
    }

    ostream& operator <<(ostream& out, const Polygon& polygon) {
        out << polygon.vertices.size() << '\n';
        for (size_t i = 0; i < polygon.size(); i++) {
            //out << polygon.vertices[i] << '\n';
            out << (int) polygon.vertices[i].x << ' ' << (int) polygon.vertices[i].y << '\n';
        }
        return out;
    }

    float distance(const Point& pt1, const Point& pt2) {
        return (pt1 - pt2).len();
    }

    float distance(const Segment& segment, const Point& pt) {
        if (between(0, (segment.pt2 - segment.pt1).len(), (segment.pt2 - segment.pt1).projection(pt - segment.pt1))) {
            return abs(((segment.pt2 - segment.pt1) ^ (pt - segment.pt1)) / (segment.pt2 - segment.pt1).len());
        }
        return min((pt - segment.pt1).len(), (pt - segment.pt2).len());
    }



    bool intersect(const Segment& segment1, const Segment segment2) {
        return  intersect_box(segment1.pt1.x, segment1.pt2.x, segment2.pt1.x, segment2.pt2.x)
                && intersect_box(segment1.pt1.y, segment1.pt2.y, segment2.pt1.y, segment2.pt2.y)
                && ((segment1.pt2 - segment1.pt1) ^ (segment2.pt1 - segment1.pt1)) * ((segment1.pt2 - segment1.pt1) ^ (segment2.pt2 - segment1.pt1)) <= 0
                && ((segment2.pt2 - segment2.pt1) ^ (segment1.pt1 - segment2.pt1)) * ((segment2.pt2 - segment2.pt1) ^ (segment1.pt2 - segment2.pt1)) <= 0;
    }

    int intersect(const Line& line1, const Line& line2, Point& pt1) {
        if (equal(line1.a * line2.b - line1.b * line2.a, 0)) {
            if (equal(line1.a * line2.c - line1.c * line2.a, 0) && equal(line1.b * line2.c - line1.c * line2.b, 0)) {
                return 3;
            } else {
                return 0;
            }
        }
        pt1.x = -(line1.c * line2.b - line1.b * line2.c) / (line1.a * line2.b - line1.b * line2.a);
        pt1.y = -(line1.a * line2.c - line1.c * line2.a) / (line1.a * line2.b - line1.b * line2.a);
        return true;
    }

    int intersect(const Circle& circle1, const Circle& circle2, Point& pt1, Point& pt2) {
        if ((sqr(circle1.r + circle2.r) + EPS < (circle1.pt - circle2.pt).len2())
            || (circle1.r > (circle1.pt - circle2.pt).len() + circle2.r)
            || (circle2.r > (circle1.pt - circle2.pt).len() + circle1.r)) {
            return 0;
        }

        if (circle1.pt == circle2.pt && equal(circle1.r, circle2.r)) {
            return 3;
        }

        Vector O1H = (circle2.pt - circle1.pt) * ((sqr(circle1.r) - sqr(circle2.r) + (circle2.pt - circle1.pt).len2()) / 2 / (circle2.pt - circle1.pt).len2());
        if (equal(sqr(circle1.r + circle2.r), (circle1.pt - circle2.pt).len2())
            || equal(sqr(circle1.r - circle2.r), (circle1.pt - circle2.pt).len2())) {
            return 1;
        }

        float h = sqrt(sqr(circle1.r) - O1H.len2());
        Vector HP = (circle2.pt - circle1.pt).perpendicular().normalized() * h;
        pt1 = circle1.pt + O1H + HP;
        pt2 = circle1.pt + O1H - HP;
        return 2;
    }

    int tangent(const Circle& circle, const Point& pt, Point& pt1, Point& pt2) {
        float t = (circle.pt - pt).len();
        if (equal(t, circle.r)) {
            pt1 = pt;
            return 1;
        }
        if (t < circle.r) {
            return 0;
        }
        return intersect(Circle(pt, sqrt((pt - circle.pt).len2() - sqr(circle.r))), circle, pt1, pt2);
    }
};

using namespace Geometry;
using std::cin;
using std::cout;
using std::iostream;
using std::fixed;
using std::setprecision;

const int steps1 = 100000;
const int steps2 = 5000;
float da = 0.5 * acos(-1) / steps1;
float dda = da / steps2 / 3;
float cosa[steps1], sina[steps1];
Point a[102];
Point center;

float rotateX(Vector& t, int i) {
    return t.x * cosa[i] - t.y * sina[i];
}

float rotateY(Vector& t, int i) {
    return t.x * sina[i] + t.y * cosa[i];
}

float rotateXa(Vector& t, float a) {
    return t.x * cos(a) - t.y * sin(a);
}

float rotateYa(Vector& t, float a) {
    return t.x * sin(a) + t.y * cos(a);
}

int main()
{
    freopen("gift.in", "rt", stdin);
    //freopen("gift.out", "wt", stdout);
    for (int i = 0; i < steps1; i++) {
        cosa[i] = cos(da * i);
        sina[i] = sin(da * i);
    }
    int j = 1;
    Polygon a;
    while (true) {
        center = Vector(0, 0);
        cin >> a;
        if (a.size() == 0) {
            break;
        }
        a = a.convexHull();

        int n = a.size();
        for (int i = 0; i < n / 2; i++) {
            swap(a[i], a[n - i - 1]);
        }
        for (int i = 0; i < n; i++) {
            center = center + a[i];
        }
        center = center / n;

        int minX = 0, maxX = 0, minY = 0, maxY = 0;
        for (int i = 0; i < n; i++) {
            a[i] = a[i] - center;
            if (a[i].x < a[minX].x) {
                minX = i;
            }
            if (a[i].x > a[maxX].x) {
                maxX = i;
            }
            if (a[i].y < a[minY].y) {
                minY = i;
            }
            if (a[i].y > a[maxY].y) {
                maxY = i;
            }
        }
        float  minArea = (a[maxX].x - a[minX].x) * (a[maxY].y - a[minY].y), maxArea = (a[maxX].x - a[minX].x) * (a[maxY].y - a[minY].y);
        float  angle = 0;
        int minstep = 0, maxstep = 0;
        float aminX = minX, aminY = minY, amaxX = maxX, amaxY = maxY;
        float bminX = minX, bminY = minY, bmaxX = maxX, bmaxY = maxY;
        for (int i = 0; i < steps1; i++) {
            while (rotateX(a[minX], i) >= rotateX(a[(minX + 1) % n], i)) {
                minX = (minX + 1) % n;
            }
            while (rotateX(a[maxX], i) <= rotateX(a[(maxX + 1) % n], i)) {
                maxX = (maxX + 1) % n;
            }
            while (rotateY(a[minY], i) >= rotateY(a[(minY + 1) % n], i)) {
                minY = (minY + 1) % n;
            }
            while (rotateY(a[maxY], i) <= rotateY(a[(maxY + 1) % n], i)) {
                maxY = (maxY + 1) % n;
            }

            float  area = (rotateX(a[maxX], i) - rotateX(a[minX], i)) * (rotateY(a[maxY], i) - rotateY(a[minY], i));
            if (minArea > area) {
                minArea = area;
                minstep = i;
                aminX = minX, aminY = minY, amaxX = maxX, amaxY = maxY;
            }
            if (maxArea < area) {
                maxArea = area;
                maxstep = i;
                bminX = minX, bminY = minY, bmaxX = maxX, bmaxY = maxY;
            }
        }
        angle = da * (minstep) - da / 2;
        for (int i = 0; i < steps2 * 2; i++, angle += dda) {
            float aarea = (rotateXa(a[amaxX], angle) - rotateXa(a[aminX], angle)) * (rotateYa(a[amaxY], angle) - rotateYa(a[aminY], angle));
            float barea = (rotateXa(a[bmaxX], angle) - rotateXa(a[bminX], angle)) * (rotateYa(a[bmaxY], angle) - rotateYa(a[bminY], angle));
            if (minArea > aarea) {
                minArea = aarea;
            }
            if (maxArea < barea) {
                maxArea = barea;
            }
        }

        cout << "Gift " << j << '\n';
        cout << fixed << setprecision(3) << "Minimum area = " << minArea << '\n';
        cout << fixed << setprecision(3) << "Maximum area = " << maxArea << "\n\n";
        j++;
    }
    return 0;
}