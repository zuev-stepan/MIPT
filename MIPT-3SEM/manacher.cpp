#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::min;
using std::cin;
using std::cout;

int main()
{
    string s;
    cin >> s;
    vector<int> d(s.length());
    int ans = 0, ans_pos;
    d.assign(s.length(), 1);
    for (int i = 0, l = 0, r = -1; i < s.length(); ++i) {
        if (i <= r) {
            d[i] = min(r - i, d[l + r - i]) + 1;
        }
        while (i + d[i] < s.length() && i - d[i] >= 0 && s[i + d[i]] == s[i - d[i]]) {
            ++d[i];
        }
        if (i + d[i] - 1 > r)
            l = i - d[i] + 1,  r = i + d[i] - 1;
        if (d[i] * 2 - 1 > ans) {
            ans = d[i] * 2 - 1;
            ans_pos = i;
        }
    }

    d.assign(s.length(), 0);
    for (int i = 0, l = 0, r = -1; i < s.length(); ++i) {
        if (i <= r) {
            d[i] = min(r - i + 1, d[l + r - i + 1]) + 1;
        }
        while (i + d[i] < s.length() && i - d[i] - 1 >= 0 && s[i + d[i]] == s[i - d[i] - 1]) {
            ++d[i];
        }
        if (i + d[i] - 1 > r)
            l = i - d[i],  r = i + d[i] - 1;
        if (d[i] * 2 > ans) {
            ans = d[i] * 2;
            ans_pos = i;
        }
    }
    //cout << ans_pos << ' ' << ans << '\n';
    cout << s.substr(ans_pos - ans / 2, ans);
    return 0;
}
