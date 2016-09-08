#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

int p;
char s[400000];
vector<int> end_z, start_z;

int& getZ(int i) {
    if (i < p) {
        return start_z[i];
    }
    return end_z[(i - p) % p];
}

int main()
{
    ios_base::sync_with_stdio(false);
    scanf("%s", s);
    p = strlen(s);
    s[strlen(s)] = '#';
    ++p;
    scanf("%s", s + p);
    start_z.assign(p, 0);
    end_z.assign(p, 0);
    int l = 0, r = 0;
    for (int i = 1; s[i] != 0; ++i) {
        getZ(i) = 0;
        if (i <= r) {
			getZ(i) = min(r - i + 1, getZ(i - l));
		}
		while (s[getZ(i)] == s[i + getZ(i)]) {
            ++getZ(i);
		}
		if (i + getZ(i) - 1 > r) {
			l = i;
            r = i + getZ(i) - 1;
		}
        if (getZ(i) >= p - 1) {
            printf("%d ", i - p);
        }
	}
    return 0;
}
