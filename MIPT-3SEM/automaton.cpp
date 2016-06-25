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
    //������� ��������� ������� ����� ����������� �����, ����������� ��� ����� ���������, ������� ����� ������� x^k
    int min_len(char x, int k) {
        //���� ��� ������� - ������ ������� �� O(1)
        if (mem[k] >= 0)
            return mem[k];
        //���� ��������� ����� ��� s1+s2, �� ������� ����������� ����� �� ���������� ��� s1 � s2
        if (operation == '+') {
            mem[k] = min(left->min_len(x, k), right->min_len(x, k));
        //�� O(k), �� �� ���������� �� k ���
        //���� ��������� ����� ��� s1.s2, �� ��������� ������� �� ���� x ��� ���� s2 � ��������� ������� �� s1
        } else if (operation == '.') {
            int ans = INF;
            for (int i = 0; i < k; i++) {
                //s2 ������� ������ �� x
                if (right->min_len(x, i) == i) {
                    ans = min(ans, left->min_len(x, k - i) + i);
                }
            }
            //��� ������� ��� ����� �� s2
            if (right->min_len(x, k) != INF) {
                ans = min(ans, left->min_len(x, 0) + right->min_len(x, k));
            }
            mem[k] = ans;
        //����� ����� �����, �������� �� O(k^2), �� ���� ����� 1 ��� ��� ������� ���������
        //���� ��������� ����� ��� s*
        } else if (operation == '*') {
            //����� �� ������ �� �����
            mem[0] = 0;
            //���� ����� ������ 1 �����, �� �� ����� ������ ��� ������
            mem[1] = left->min_len(x, 1);
            //��������� ��� ������ ��� i < k
            for (int i = 2; i <= k; i++) {
                //���� �� ����� �� �����
                mem[i] = left->min_len(x, i);
                //������ �� ����� �������� ����� x^j � ���������� ��� � ��� ����� � ��������� x^(i-j)
                for (int j = 1; j < i; j++) {
                    if (mem[j] == j) {
                        mem[i] = min(mem[i], mem[i - j] + j);
                    }
                }
            }
        //���� � ��� ������ �����, �� ��������� �����
        //������ - ����� ������� ������ e
        } else if (operation == '1' && k == 0) {
            mem[k] = 0;
        //�������� - ���� ����� ������, ��� ���� �� ����, �� ����� 1
        } else if ((x == operation && k == 1) || (k == 0)) {
            mem[k] = 1;
        //����� ��� ������� �����
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
    //��������� ��������� ������
    //�������, ��� � ���������� ��������� ������ �� �� �����, ��� n ������
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
    //������ ����� ��� ����� ���������
    //�.�. ��������� ������������ � ���� ������, �������� ����������� - O(nk^2)
    int ans = states.top()->min_len(x, k);
    if (ans == INF)
        cout << "NO";
    else
        cout << "YES";
    return 0;
}
