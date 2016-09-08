#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <assert.h>

namespace Geometry {
    using std::swap;
    using std::max;
    using std::min;
    using std::abs;
    using std::vector;
    using std::istream;
    using std::ostream;
    using std::sort;

    const double EPS = 1e-16;
    const int INF = 1e9;
    const double PI = acos(-1);

    //Лежит ли t между x и y
    inline bool between(double x, double y, double t) {
        return (t >= x && t <= y);
    }

    //Сравнимы ли 2 вещественных числа
    inline bool equal(double x, double y) {
        return (abs(x - y) < EPS);
    }

    //Квадрат
    inline double sqr(double t) {
        return t * t;
    }

    //Знак числа
    inline int sign(double t) {
        if (abs(t) < EPS) {
            return 0;
        }
        return t < 0 ? -1 : 1;
    }

    //Возвращает угол в диапазоне [0; 2 * PI) сравнимый с данным
    double normalizeAngle(double a) {
        return a - ((int) (a / (2 * PI))) * 2 * PI;
    }

    //Пересекается ли отрезок [a1, a2] с [b1, b2]
    bool intersectBox(double a1, double a2, double b1, double b2) {
        if (a1 > a2) {
            swap(a1, a2);
        }
        if (b1 > b2) {
            swap(b1, b2);
        }
        return max(a1, b1) <= min(a2, b2);
    }

    //Второй элемент с конца вектора
    template <class T>
    T second_back(const vector<T>& v) {
        assert(v.size() > 1);
        return v[v.size() - 2];
    }

    //Вектор
    class Vector2d {
    public:
        double x, y;
        Vector2d(){}
        Vector2d(double x, double y) : x(x), y(y) {}
        Vector2d(const Vector2d & start, const Vector2d & end) : x(end.x - start.x), y(end.y - start.y) {}

        Vector2d operator -(Vector2d t) const {
            return Vector2d(x - t.x, y - t.y);
        }

        Vector2d operator +(Vector2d t) const {
            return Vector2d(x + t.x, y + t.y);
        }

        //Скалярное произведение
        double operator *(Vector2d t) const {
            return (x * t.x + y * t.y);
        }

        Vector2d operator *(double t) const {
            return Vector2d(x * t, y * t);
        }

        Vector2d operator /(double t) const {
            return Vector2d(x / t, y / t);
        }

        //Псевдовекторное произведение
        double operator ^(Vector2d t) const {
            return (x * t.y - y * t.x);
        }

        bool operator ==(Vector2d t) const {
            return equal(this->x, t.x) && equal(this->y, t.y);
        }

        bool operator !=(Vector2d t) const {
            return !((*this) == t);
        }

        //Угол наклона к оси Ox, [0; 2 * PI)
        double angle() const {
            double t = atan2(this->y, this->x);
            if (t < 0) {
                return t + 2 * PI;
            }
            return t;
        }

        double len2() const {
            return (*this) * (*this);
        }

        double len() const {
            return sqrt(this->len2());
        }

        //Проекция вектора t на этот
        Vector2d projection(Vector2d t) const {
            return (*this) * ((*this) * t) / this->len2();
        }

        //Длина проекции t на этот
        double projectionLen(Vector2d t) const {
            return (*this) * t / this->len();
        }

        //Нормализованный вектор
        Vector2d normalized() const {
            return Vector2d(this->x / len(), this->y / this->len());
        }

        //Перпендикулярный вектор
        Vector2d perpendicular() const {
            return Vector2d(this->y, -this->x);
        }

        //Вектор, повернутый на угол
        Vector2d rotated(double angle) const {
            return Vector2d(this->x * cos(angle) - this->y * sin(angle),
                            this->x * sin(angle) + this->y * cos(angle));
        }
    };
    typedef Vector2d Point;

    istream& operator >>(istream& in, Vector2d & t) {
        in >> t.x >> t.y;
        return in;
    }

    ostream& operator <<(ostream& out, const Vector2d & t) {
        out << t.x << ' ' << t.y;
        return out;
    }

    //Прямая
    class Line {
    public:
        double a, b, c;
        Line(){}
        Line(double a, double b, double c) : a(a), b(b), c(c) {}

        //Нормализованная прямая, первый коэффициент >= 0
        Line normalized() const {
            if (a > 0) {
                return Line(this->a / sqrt(sqr(this->a) + sqr(this->b)),
                            this->b / sqrt(sqr(this->a) + sqr(this->b)),
                            this->c / sqrt(sqr(this->a) + sqr(this->b)));
            }
            return Line(-this->a / sqrt(sqr(this->a) + sqr(this->b)),
                        -this->b / sqrt(sqr(this->a) + sqr(this->b)),
                        -this->c / sqrt(sqr(this->a) + sqr(this->b)));
        }
    };

    istream& operator >>(istream& in, Line& t) {
        in >> t.a >> t.b >> t.c;
        return in;
    }

    ostream& operator <<(ostream& out, const Line& t) {
        out << t.a << ' ' << t.b << ' ' << t.c;
        return out;
    }

    //Отрезок
    class Segment {
    public:
        Point pt1, pt2;
        Segment() {}
        Segment(double x1, double y1, double x2, double y2) : pt1(x1, y1), pt2(x2, y2) {}
        Segment(const Point& pt1, const Point& pt2) : pt1(pt1), pt2(pt2) {}

        //Серединный перпендикуляр
        Line bisector() const {
            return Line(2 * (pt2.x - pt1.x), 2 * (pt2.y - pt1.y), pt1.len2() - pt2.len2());
        }
    };

    istream& operator >>(istream& in, Segment& t) {
        in >> t.pt1 >> t.pt2;
        return in;
    }

    ostream& operator <<(ostream& out, const Segment& t) {
        out << t.pt1 << '\n' << t.pt2;
        return out;
    }

    //Окружность
    class Circle {
    public:
        Point pt;
        double r;
        Circle() {}
        Circle(double x, double y, double r) : pt(x, y), r(r) {}
        Circle(const Point& pt, double r) : pt(pt), r(r) {}
    };

    istream& operator >>(istream& in, Circle& t) {
        in >> t.pt >> t.r;
        return in;
    }

    ostream& operator <<(ostream& out, const Circle& t) {
        out << t.pt << ' ' << t.r;
        return out;
    }

    //Многоугольник
    class Polygon {
    public:
        vector<Point> vertices;
        Polygon(){}
        Polygon(const std::vector<Point>& vertices) : vertices(vertices) {}

        //Список вершин зациклен
        Point& operator [](size_t i) {
            return this->vertices[i % this->vertices.size()];
        }

        const Point& operator [](size_t i) const {
            return this->vertices[i % this->vertices.size()];
        }

        //Добавить вершину
        void add(const Point& p) {
            this->vertices.push_back(p);
        }

        size_t size() const {
            return vertices.size();
        }

        //Площадь
        double calcArea() const {
            if (this->vertices.size() < 3) {
                return 0;
            }
            double ans = (this->vertices[0] ^ this->vertices.back());
            for (size_t i = 1; i < this->vertices.size(); ++i) {
                ans += this->vertices[i] ^ this->vertices[i - 1];
            }
            return abs(ans) / 2;
        }

        //Периметр
        double calcPerimeter() const {
            double ans = (this->vertices[0] - this->vertices.back()).len();
            for (size_t i = 1; i < this->vertices.size(); ++i) {
                ans += (this->vertices[i] - this->vertices[i - 1]).len();
            }
            return ans;
        }

        //Выпуклый ли
        bool isConvex() const {
            int prev_sign = 0;
            for (size_t i = 0; i < this->vertices.size(); ++i) {
                Vector2d v1 = (i ? this->vertices[i - 1]
                                 : this->vertices.back()) - this->vertices[i];
                Vector2d v2 = (i + 1 < this->vertices.size()
                               ? this->vertices[i + 1]
                               : this->vertices.front()) - this->vertices[i];
                int t = sign(v1 ^ v2);
                if (prev_sign != 0 && t != prev_sign) {
                    return false;
                } else if (prev_sign == 0) {
                    prev_sign = t;
                }
            }
            return true;
        }

        //Выпуклая оболочка
        Polygon convexHull() const {
            if (this->vertices.size() < 3) {
                return Polygon(this->vertices);
            }

            Point lower(INF, INF);
            for (size_t i = 0; i < this->vertices.size(); ++i) {
                if (lower.y > this->vertices[i].y
                    || (lower.y == this->vertices[i].y && lower.x > this->vertices[i].x)) {
                    lower = this->vertices[i];
                }
            }

            vector<Point> tmp;
            for (size_t i = 0; i < this->vertices.size(); ++i) {
                if (this->vertices[i] != lower) {
                    tmp.push_back(this->vertices[i]);
                }
            }

            stable_sort(tmp.begin(), tmp.end(),
                        [lower](const Point& pt1, const Point& pt2) {
                            return (((pt1 - lower) ^ (pt2 - lower)) > 0)
                                   || ((((pt1 - lower) ^ (pt2 - lower)) == 0)
                                       && ((pt1 - lower).len2() < (pt2 - lower).len2()));
                        }
            );

            vector<Point> vertex_stack;
            vertex_stack.push_back(lower);
            vertex_stack.push_back(tmp[0]);
            for (size_t i = 1; i < tmp.size(); ++i) {
                while ((((tmp[i] - vertex_stack.back())
                         ^ (second_back(vertex_stack) - vertex_stack.back())) <= 0)
                       && (vertex_stack.size() > 1)) {
                    vertex_stack.pop_back();
                }
                vertex_stack.push_back(tmp[i]);
            }
            return Polygon(vertex_stack);
        }

        //Повернутый на угол
        Polygon rotated(double angle) const {
            Polygon tmp(vector<Point>(this->size()));
            for (size_t i = 0; i < this->size(); ++i) {
                tmp[i] = this->vertices[i].rotated(angle);
            }
            return tmp;
        }

        //С обратным порядком обхода
        Polygon reversed() const {
            Polygon tmp(vector<Point>(this->size()));
            for (size_t i = 0; i < this->size(); ++i) {
                tmp[this->size() - i - 1] = this->vertices[i];
            }
            return tmp;
        }

        //Ограничивающий прямоугольник
        Polygon boundingBox() const {
            double minX = INF, minY = INF, maxX = -INF, maxY = -INF;
            for (size_t i = 0; i < this->size(); ++i) {
                if (this->vertices[i].x < minX) {
                    minX = this->vertices[i].x;
                }
                if (this->vertices[i].y < minY) {
                    minY = this->vertices[i].y;
                }
                if (this->vertices[i].x > maxX) {
                    maxX = this->vertices[i].x;
                }
                if (this->vertices[i].y > maxY) {
                    maxY = this->vertices[i].y;
                }
            }
            Polygon tmp;
            tmp.add(Point(minX, minY));
            tmp.add(Point(maxX, minY));
            tmp.add(Point(maxX, maxY));
            tmp.add(Point(minX, maxY));
            return tmp;
        }
    };

    istream& operator >>(istream& in, Polygon& polygon) {
        polygon.vertices.clear();
        int n;
        in >> n;
        polygon.vertices.resize(n);
        for (size_t i = 0; i < n; ++i) {
            in >> polygon[i];
        }
        return in;
    }

    ostream& operator <<(ostream& out, const Polygon& polygon) {
        out << polygon.vertices.size() << '\n';
        for (size_t i = 0; i < polygon.size(); ++i) {
            out << polygon.vertices[i].x << ' ' << polygon.vertices[i].y << '\n';
        }
        return out;
    }

    //Расстояние между двумя точками
    double distance(const Point& pt1, const Point& pt2) {
        return (pt1 - pt2).len();
    }

    //Ближайшая точка на отрезке
    Point nearest(const Segment& segment, const Point& pt) {
        if (between(0, (segment.pt2 - segment.pt1).len(),
                    (segment.pt2 - segment.pt1).projectionLen(pt - segment.pt1))) {
            return (segment.pt2 - segment.pt1).projection(pt - segment.pt1) + segment.pt1;
        }
        if ((pt - segment.pt1).len2() < (pt - segment.pt2).len()) {
            return segment.pt1;
        }
        return segment.pt2;
    }

    //Расстояние от точки дл отрезка
    double distance(const Segment& segment, const Point& pt) {
        return distance(nearest(segment, pt), pt);
    }

    //Пересекаются ли отрезки
    bool isIntersect(const Segment& segment1, const Segment segment2) {
        return intersectBox(segment1.pt1.x, segment1.pt2.x, segment2.pt1.x, segment2.pt2.x)
               && intersectBox(segment1.pt1.y, segment1.pt2.y, segment2.pt1.y, segment2.pt2.y)
               && (((segment1.pt2 - segment1.pt1) ^ (segment2.pt1 - segment1.pt1))
                  * ((segment1.pt2 - segment1.pt1) ^ (segment2.pt2 - segment1.pt1)) <= 0)
               && (((segment2.pt2 - segment2.pt1) ^ (segment1.pt1 - segment2.pt1))
                  * ((segment2.pt2 - segment2.pt1) ^ (segment1.pt2 - segment2.pt1)) <= 0);
    }

    //Пересечение прямых
    //Возвращает количество точек пересечения и точки:
    //0 - прямые параллельны
    //1 - прямые пересекаются, ответ в pt1
    //2 - прямые совпадают
    int intersect(const Line& line1, const Line& line2, Point& pt1) {
        if (equal(line1.a * line2.b - line1.b * line2.a, 0)) {
            if (equal(line1.a * line2.c - line1.c * line2.a, 0)
                && equal(line1.b * line2.c - line1.c * line2.b, 0)) {
                return 2;
            } else {
                return 0;
            }
        }
        pt1.x = -(line1.c * line2.b - line1.b * line2.c)
                / (line1.a * line2.b - line1.b * line2.a);
        pt1.y = -(line1.a * line2.c - line1.c * line2.a)
                / (line1.a * line2.b - line1.b * line2.a);
        return 1;
    }

    //Пересечение окружностей
    //Возвращает количество точек пересечения и точки:
    //0 - окружности не пересекаются
    //1 - окружности касаются, ответ в pt1
    //2 - окружности пересекаются, ответ в pt1 и pt2
    //3 - окружности совпадают
    int intersect(const Circle& circle1, const Circle& circle2, Point& pt1, Point& pt2) {
        if ((sqr(circle1.r + circle2.r) + EPS < (circle1.pt - circle2.pt).len2())
            || (circle1.r > (circle1.pt - circle2.pt).len() + circle2.r)
            || (circle2.r > (circle1.pt - circle2.pt).len() + circle1.r)) {
            return 0;
        }

        if (circle1.pt == circle2.pt && equal(circle1.r, circle2.r)) {
            return 3;
        }

        Vector2d O1H = (circle2.pt - circle1.pt)
                       * (sqr(circle1.r) - sqr(circle2.r) + (circle2.pt - circle1.pt).len2())
                       / (2 * (circle2.pt - circle1.pt).len2());
        if (equal(sqr(circle1.r + circle2.r), (circle1.pt - circle2.pt).len2())
            || equal(sqr(circle1.r - circle2.r), (circle1.pt - circle2.pt).len2())) {
            return 1;
        }

        double h = sqrt(sqr(circle1.r) - O1H.len2());
        Vector2d HP = (circle2.pt - circle1.pt).perpendicular().normalized() * h;
        pt1 = circle1.pt + O1H + HP;
        pt2 = circle1.pt + O1H - HP;
        return 2;
    }

    //Сумма минковского двух выпуклых многоугольников
    Polygon mincowski(const Polygon& p1, const Polygon& p2) {
        size_t i = 0, j = 0;
        for (size_t k = 0; k < p1.size(); ++k) {
            if (p1[k].y < p1[i].y || (p1[k].y == p1[i].y && p1[k].x < p1[i].x)) {
                i = k;
            }
        }

        for (size_t k = 0; k < p2.size(); ++k) {
            if (p2[k].y < p2[j].y || (p2[k].y == p2[j].y && p2[k].x < p2[j].x)) {
                j = k;
            }
        }

        Polygon sum;
        sum.add(p1[i] + p2[j]);

        while (sum.size() < p1.size() + p2.size()) {
            if ((p1[i + 1] - p1[i]).angle() < (p2[j + 1] - p2[j]).angle()) {
                sum.add((p1[i + 1] - p1[i]) + sum.vertices.back());
                ++i;
            } else {
                sum.add((p2[j + 1] - p2[j]) + sum.vertices.back());
                ++j;
            }
        }
        return sum;
    }

    //Касательная к окружности из точки pt
    //Возвращает количество точек касания:
    //0 - касательной нет
    //1 - точка одна, ответ в pt1
    //2 - точки две, ответ в pt1 и pt2
    int tangent(const Circle& circle, const Point& pt, Point& pt1, Point& pt2) {
        double t = (circle.pt - pt).len();
        if (equal(t, circle.r)) {
            pt1 = pt;
            return 1;
        }
        if (t < circle.r) {
            return 0;
        }
        return intersect(Circle(pt, sqrt((pt - circle.pt).len2() - sqr(circle.r))),
                         circle, pt1, pt2);
    }
};

#endif