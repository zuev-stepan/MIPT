#include <bits/stdc++.h>

using namespace std;

int p;
int r = -1;
vector<char> s, t;
vector<int> z, z0;

char& getChar(int i) {
    char c;
    if (i < t.size()) {
        return t[i];
    }
    i -= t.size();
    while (i > r) {
        scanf("%c", &s[(r + 1) % p]);
        r++;
    }
    return s[i % p];
}

int& getZ(int i) {
    if (i < t.size()) {
        return z0[i];
    }
    i -= t.size();
    return z[i % p];
}

int main()
{
    //freopen("input.txt", "rt", stdin);
    ios_base::sync_with_stdio(false);
    char c = 0;
    while (c != '\n') {
        scanf("%c", &c);
        t.push_back(c);
    }
    t.back() = '@';
    z.assign(t.size() * 2, 0);
    z0.assign(t.size(), 0);
    s.assign(t.size() * 2, '@');
    p = t.size() * 2;
    for (int i = 1, l = 0, r = 0; getChar(i) != '\n'; ++i) {
        getZ(i) = 0;
        if (i <= r) {
			getZ(i) = min(r - i + 1, getZ(i - l));
		}
		while (getChar(getZ(i)) == getChar(i + getZ(i))) {
            getZ(i)++;
		}
		if (i + getZ(i) - 1 > r) {
			l = i,  r = i + getZ(i) - 1;
		}
        if (getZ(i) >= t.size() - 1) {
            printf("%d ", i - t.size());
        }
	}
    return 0;
}
