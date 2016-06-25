#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <map>

using namespace std;

class ICharArray {
public:
    virtual int getValue(char)=0;
    virtual void setValue(char, int)=0;
    virtual void clear()=0;
};

template<char minch, char maxch>
class CSimpleCharArray : public ICharArray {
private:
    int a[maxch - minch + 1];
public:
    int getValue(char c) {
        if (c > maxch || c < minch)
            return -1;
        return a[c - minch];
    }
    void setValue(char c, int i) {
        if (c > maxch || c < minch)
            return;
        a[c - minch] = i;
    }
    void clear() {
        fill(a, a + maxch - minch + 1, -1);
    }
};

class CMapCharArray : public ICharArray {
private:
    map<char, int> a;
public:
    int getValue(char c) {
        auto t = a.find(c);
        if (t == a.end())
            return -1;
        return t->second;
    }
    void setValue(char c, int i) {
        a[c] = i;
    }
    void clear() {
        a.clear();
    }
};

template<typename CCharArray>
class CSuffixTree {
private:
    struct Node {
        int pos, length, depth;
        int link, parent;
        CCharArray childs;
        Node(){}
        ~Node(){
            childs.clear();
        }
        inline void init(int p, int l) {
            link = parent = -1;
            pos = p;
            length = l;
            depth = 0;
            childs.clear();
        }
        void setChild(char c, int child) {
            childs.setValue(c, child);
        }
        int getChild(char c) {
            return childs.getValue(c);
        }
    };

    vector<Node> nodes;
    int maxNode;
    string str;
    int root, head, tail, fake;

    int newNode(int pos, int len) {
        maxNode++;
        if (maxNode >= nodes.size()) {
            nodes.push_back(Node());
        }
        nodes[maxNode].init(pos, len);
        return maxNode;
    }

    void setChild(char c, int parent, int child) {
        nodes[parent].setChild(c, child);
        nodes[child].parent = parent;
        nodes[child].depth = nodes[parent].depth + nodes[child].length;
    }

    inline int split(int parent, int child, int len) {
        int x = newNode(nodes[child].pos, len);
        setChild(str[nodes[child].pos], parent, x);
        nodes[child].pos += len;
        nodes[child].length -= len;
        setChild(str[nodes[child].pos], x, child);
        return x;
    }

    int fastScan(int start, int pos, int len) {
        if (len == 0) {
            return start;
        }
        int next = nodes[start].getChild(str[pos]);
        if (nodes[next].length > len) {
            return split(start, next, len);
        } else if (nodes[next].length == len) {
            return next;
        } else {
            return fastScan(next, pos + nodes[next].length, len - nodes[next].length);
        }
    }

    int slowScan(int start, int pos, int len) {
        if (len == 0) {
            return start;
        }
        int next = nodes[start].getChild(str[pos]);
        if (next == -1) {
            int x = newNode(pos, len);
            setChild(str[pos], start, x);
            return start;
        }
        for (int i = 1; i < nodes[next].length; i++) {
            if (len == i || str[nodes[next].pos + i] != str[pos + i]) {
                int x = split(start, next, i);
                int y = newNode(pos + i, len - i);
                setChild(str[pos + i], x, y);
                return x;
            }
        }
        return slowScan(next, pos + nodes[next].length, len - nodes[next].length);
    }

    void init() {
        char maxch = -1;
        for (int i = 0; i < str.length(); i++) {
            maxch = max(maxch, str[i]);
        }
        str.push_back(maxch + 1);
        maxNode = -1;
        root = newNode(0, 1);
        head = root;
        tail = newNode(0, str.length());
        nodes[tail].depth = str.length();
        setChild(str[0], root, tail);
        fake = newNode(0, 0);
        nodes[fake].depth = -1;
        for (int i = 0; i < str.length(); i++) {
            setChild(str[i], fake, root);
        }
        nodes[root].link = fake;

        for(int i = 0; i < str.length(); ++i) {
            if (head == root) {
                head = slowScan(root, i + nodes[head].depth + 1, str.length() - i - 1 - nodes[head].depth);
                continue;
            }
            int u = nodes[head].parent;
            int w = fastScan(nodes[u].link, nodes[head].pos, nodes[head].length);
            nodes[head].link = w;
            head = slowScan(w, i + nodes[head].depth, str.length() - i - nodes[head].depth);
        }
    }

    int getDifferentSubstringsCount(int t) {
        int ans = 0;
        for (int i = 0; i < 27; i++) {
            int child = nodes[t].getChild(i + 'a');
            if (child == -1)
                continue;
            ans += getDifferentSubstringsCount(child) + (nodes[child].length);
        }
        return ans;
    }
public:
    CSuffixTree(string s) : str(s) {
        init();
    }
    ~CSuffixTree() {
        nodes.clear();
        str.clear();
    }

    int getDifferentSubstringsCount() {
        return getDifferentSubstringsCount(root);
    }
    void rebuild(string s) {
        str = s;
        init();
    }
};

//timus id 6588591
int main()
{
    ios_base::sync_with_stdio(false);
    string s;
    int k, l;
    cin >> k >> s;
    s += s;
    CSuffixTree<CSimpleCharArray<'a', '{'> > tree("");
    for (int i = 0; i * 2 < s.length(); i++) {
        tree.rebuild(s.substr(i, k));
        cout << tree.getDifferentSubstringsCount() - k - 1 << ' ';
    }
    /*string s;
    cin >> s;
    CSuffixTree<CSimpleCharArray<'a', '{'> > tree(s);
    cout << tree.getDifferentSubstringsCount() - s.length() - 1;*/
    //tree.print(NULL);
    return 0;
}
