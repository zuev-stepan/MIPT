#include <bits/stdc++.h>

using namespace std;

const int INF = 1e9;

int k;

struct State {
    char operation;
    State* left, * right;
    int* mem;
    State() : mem(NULL) {}
    State(State* s1, State* s2, char c) {
        left = s1, right = s2, operation = c;
        mem = new int[k + 1];
        fill(mem, mem + k + 1, -1);
    }
    ~State() {
        delete[] mem;
    }
    //ленивой динамикой находит длину минимальное слова, подходящего под часть регулярки, которое имеет суффикс x^k
    int min_len(char x, int k) {
        //если уже считали - просто ответим за O(1)
        if (mem[k] >= 0)
            return mem[k];
        //если регулярка имеет вид s1+s2, то возьмем минимальную длину из подходящих под s1 и s2
        if (operation == '+') {
            mem[k] = min(left->min_len(x, k), right->min_len(x, k));
        //за O(k), но мб вызывается до k раз
        //если регулярка имеет вид s1.s2, то переберем сколько из букв x нам даст s2 и остальные возьмем из s1
        } else if (operation == '.') {
            int ans = INF;
            for (int i = 0; i < k; i++) {
                //s2 состоит только из x
                if (right->min_len(x, i) == i) {
                    ans = min(ans, left->min_len(x, k - i) + i);
                }
            }
            //или возьмем все буквы из s2
            if (right->min_len(x, k) != INF) {
                ans = min(ans, left->min_len(x, 0) + right->min_len(x, k));
            }
            mem[k] = ans;
        //самая узкая часть, работает за O(k^2), но зато всего 1 раз для каждого состояния
        //если регулярка имеет вид s*
        } else if (operation == '*') {
            //можно ее просто не брать
            mem[0] = 0;
            //если нужна только 1 буква, то по циклу ходить нет смысла
            mem[1] = left->min_len(x, 1);
            //посчитаем все ответы для i < k
            for (int i = 2; i <= k; i++) {
                //если не ходим по циклу
                mem[i] = left->min_len(x, i);
                //пройдя по циклу получаем слово x^j и дописываем его к мин слову с суффиксом x^(i-j)
                for (int j = 1; j < i; j++) {
                    if (mem[j] == j) {
                        mem[i] = min(mem[i], mem[i - j] + j);
                    }
                }
            }
        //если у нас просто буква, то посмотрим какая
        //пустая - можем вернуть только e
        } else if (operation == '1' && k == 0) {
            mem[k] = 0;
        //непустая - если равна нужной, или букв не надо, то длина 1
        } else if ((x == operation && k == 1) || (k == 0)) {
            mem[k] = 1;
        //иначе нет нужного слова
        } else {
            mem[k] = INF;
        }
        return mem[k];
    }
};

int main()
{
    string s = "aab.1+.a*.b.";
    char x = 'b';
    k = 2;
    //cin >> s >> x >> k;
    if (x < 'a' || x > 'c') {
        cout << "WRONG CHAR";
        return 0;
    }
    stack<State*> states;
    State* t1, * t2;
    //пропарсим регулярку стеком
    //заметим, что в результате получится дерево из не более, чем n вершин
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '.' || s[i] == '+') {
            if (states.size() < 2) {
                cout << "WRONG STRING";
                return 0;
            }
            t2 = states.top();
            states.pop();
            t1 = states.top();
            states.pop();
            states.push(new State(t1, t2, s[i]));
        } else if (s[i] == '*') {
            if (states.empty()) {
                cout << "WRONG STRING";
                return 0;
            }
            t1 = states.top();
            states.pop();
            states.push(new State(t1, NULL, s[i]));
        } else if (s[i] == 'a' || s[i] == 'b' || s[i] == 'c' || s[i] == '1'){
            states.push(new State(NULL, NULL, s[i]));
        } else {
            cout << "WRONG STRING";
            return 0;
        }
    }
    if (states.size() != 1) {
        cout << "WRONG STRING";
        return 0;
    }
    //найдем ответ для всего выражения
    //т.к. регулярка представлена в виде дерева, итоговая асимптотика - O(nk^2)
    int ans = states.top()->min_len(x, k);
    if (ans == INF)
        cout << "NO";
    else
        cout << "YES";
    return 0;
}
