#include <bits/stdc++.h>

using std::cout;

template<typename T>
class AutoGradient {
private:
    T x, dx;
public:
    AutoGradient(T x, T dx) : x(x), dx(dx) {}
    AutoGradient(T x) : x(x), dx(0) {}

    const AutoGradient operator +(const AutoGradient& a) const {
        return AutoGradient(x + a.x, dx + a.dx);
    }
    const AutoGradient operator -(const AutoGradient& a) const {
        return AutoGradient(x - a.x, dx - a.dx);
    }
    const AutoGradient operator *(const AutoGradient& a) const {
        return AutoGradient(x * a.x, dx * a.x + a.dx * x);
    }

    const AutoGradient operator /(const AutoGradient& a) const {
        return AutoGradient(x / a.x, (dx * a.x - x * a.dx) / a.x / a.x);
    }

    AutoGradient& operator =(const T& a) {
        this.x = a;
        this.dx = 0;
        return *this;
    }

    const T& getX() const {
        return this->x;
    }

    const T& getDx() const {
        return this->dx;
    }

    static AutoGradient getConst(T x) {
        return AutoGradient(x, 0);
    }

    static AutoGradient getVariable(T x) {
        return AutoGradient(x, 1);
    }
};

template<typename T>
AutoGradient<T> sin(AutoGradient<T> t) {
    return AutoGradient<T>(sin(t.getX()), t.getDx() * cos(t.getX()));
}
template<typename T>
AutoGradient<T> cos(AutoGradient<T> t) {
    return AutoGradient<T>(cos(t.getX()), - t.getDx() * sin(t.getX()));
}
template<typename T>
AutoGradient<T> log(AutoGradient<T> t) {
    return AutoGradient<T>(log(t.getX()), t.getDx() / t.getX());
}

template <typename T>
T f(const T& t) {
    T t_res = t;
    t_res = sin(t_res) * sin(t_res) + cos(t_res) * cos(t_res);
    return t_res;
}

int main() {
    cout << f(AutoGradient<float>(1, 1)).getX() << ' ' << f(AutoGradient<float>(1, 1)).getDx();
    return 0;
}
