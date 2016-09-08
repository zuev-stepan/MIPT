#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <random>
#include <string>
#include <cstring>

std::default_random_engine rnd_eng;
const int max_elem = 1e9;

int my_rand(int a, int b) {
    std::uniform_int_distribution<int> rnd_int(a, b);
    return rnd_int(rnd_eng);
}

class Treap {//: virtual public Solver {
private:
    struct Node {
        int key;
        int  prior;
        int sum, inc_len, dec_len;
        size_t count;
        bool rev;
        Node * Left;
        Node * Right;
        Node * Leftmost;
        Node * Rightmost;


        Node(const int &new_key, size_t priority_) : key(new_key), prior(priority_), count(1), Left(NULL), Right(NULL), rev(false), inc_len(1), dec_len(1), sum(new_key), Leftmost(this), Rightmost(this) {
        }

        int Dec_len(Node * t) {
            if (t)
                return t->dec_len;
            else
                return 0;
        }

        int Inc_len(Node * t) {
            if (t)
                return t->inc_len;
            else
                return 0;
        }

        int Count(Node * t) {
            return t ? t->count : 0;
        }



        void update_count(){
            count = 1;
            sum = key;
            Leftmost = Rightmost = this;
            if (Left) {
                Leftmost = Left->Leftmost;
                count += Left->count;
                sum += Left->sum;
            }
            if (Right) {
                Rightmost = Right->Rightmost;
                count += Right->count;
                sum += Right->sum;
            }


            dec_len = Dec_len(Right);
            if ((dec_len == Count(Right)) && (!Right || Right->Leftmost->key <= key)) {
                ++dec_len;
                if (Left && Left->Rightmost && Left->Rightmost->key >= key)
                    dec_len += Dec_len(Left);
            }



            inc_len = Inc_len(Left);
            if ((inc_len == Count(Left)) && (!Left || Left->Rightmost->key <= key)) {
                ++inc_len;
                if (Right && Right->Leftmost && Right->Leftmost->key >= key)
                    inc_len += Inc_len(Right);
            }


        }
        void push_rev(){
            if (rev) {
                rev = false;
                std::swap(Left, Right);
                if (Left)
                    Left->rev ^= true;
                if (Right)
                    Right->rev ^= true;

            }
        }
    };

    Node * head;

    static size_t Count(Node * tmp) {
        if (tmp)
            return tmp->count;
        else
            return 0;
    }


    int Dec_len(Node * tmp) {
        if (tmp)
            return tmp->dec_len;
        else
            return 0;
    }

    int Inc_len(Node * tmp) {
        if (tmp)
            return tmp->inc_len;
        else
            return 0;
    }


    static Node * Merge(Node * L, Node * R) {
        if (L == NULL) {
            if (R)
                R->push_rev();
            return R;
        }
        if (R == NULL) {
            if (L)
                L->push_rev();
            return L;
        }

        R->push_rev();
        L->push_rev();

        if (L->prior > R->prior) {
            L->Right = Merge(L->Right, R);
            L->update_count();
            return  L;
        } else {
            R->Left = Merge(L, R->Left);
            R->update_count();
            return R;
        }
    }

    static void Split(Node * node, size_t pos, Node * &L, Node * &R) {
        if (!node) {
            L = R = NULL;
            return;
        }
        node->push_rev();
        size_t curIndex = Count(node->Left) + 1;
        if (curIndex > pos) {
            Split(node->Left, pos, L, node->Left);
            R = node;
        } else {
            Split(node->Right, pos - curIndex, node->Right, R);
            L = node;
        }
        node->update_count();
    }

     static void SplitForOrdered(Node * node, int k, Node * &L, Node * &R) {
        if (!node) {
            L = R = NULL;
            return;
        }
        node->push_rev();

        if (k >= node->key) {
            SplitForOrdered(node->Left, k, L, node->Left);
            R = node;
        } else {
            SplitForOrdered(node->Right, k, node->Right, R);
            L = node;
        }
        node->update_count();
    }

    void print(Node * node) {
        if (node == NULL)
            return;
        node->push_rev();
        print(node->Left);
        std::cout << node->key << " ";
        print(node->Right);
        return;
    }

    void ShiftLeft_Node(Node * &node, size_t K) {
        if (!node)
            return;
        Node * l;
        Node * r;
        Split(node, node->count - K, l, r);
        node = Merge(r, l);
    }

    static Node * BuildNode(const std::vector<int> &s, int l, int r) {
        if (l > r)
            return NULL;
        if (l == r) {
            return new Node(s[l], my_rand(0, max_elem));
        }
        int m = my_rand(l + 1, r - 1);
        if (r - l == 1)
            m = l;
        return Merge(BuildNode(s, l, m), BuildNode(s, m + 1, r));
    }

    static void free_node(Node * node) {
        if (node == NULL)
            return;
        free_node(node->Left);
        free_node(node->Right);
        delete(node);
    }

    void set(Node * node, int x, int pos)
    {
        if (!node)
            return;
        node->push_rev();
        if(pos == Count(node->Left)) {
            node->key = x;
        } else {
            if (pos <= Count(node->Left))
                set(node->Left, x, pos);
            else
                set(node->Right, x, pos - Count(node->Left) - 1);
        }
        node->update_count();
    }

    void BuildTreap(const std::vector<int> &input) {
        this->head = BuildNode(input, 0, input.size() - 1);
    }
public:
    ~Treap() {
        free_node(head);
        head = NULL;
    }
    Treap(Node *node = NULL) {
        head = node;
    }

    void output() {
        print(head);
    }

    void ShiftLeft(size_t left, size_t right, size_t K) {
        Node * LeftNode;
        Node * MediumNode;
        Node * RightNode;
        Split(head, right, head, RightNode);
        Split(head, left, LeftNode, MediumNode);
        ShiftLeft_Node(MediumNode, K);

        head = Merge(Merge(LeftNode, MediumNode), RightNode);
    }

    void init_array(const std::vector<int> &input) {
        BuildTreap(input);
    }

    void add(size_t pos, int elemX) {
        pos--;
        Node * l;
        Node * r;
        Split(head, pos, l, r);
        Node * m = new Node(elemX, my_rand(0, max_elem));
        head = Merge(Merge(l, m), r);
    }

    void set_el(int x, size_t pos) {
        pos--;
        set(head, x, pos);
    }

    void reverse(size_t left, size_t right){
        Node * LeftNode;
        Node * MediumNode;
        Node * RightNode;
        Split(head, right, head, RightNode);
        Split(head, left, LeftNode, MediumNode);

        MediumNode->rev ^= true;
        head = Merge(Merge(LeftNode, MediumNode), RightNode);
    }

    void NextP(size_t left, size_t right){
        left--;


        Node * LeftNode;
        Node * MediumNode;
        Node * RightNode;
        Split(head, right, head, RightNode);
        Split(head, left, LeftNode, MediumNode);


        if(Dec_len(MediumNode) == Count(MediumNode)) {
            MediumNode->rev ^= true;
            head = Merge(LeftNode, Merge(MediumNode, RightNode));
            return;
        }


        Node * FirstNode;
        Node * SecondNode;
        Split(MediumNode, Count(MediumNode) - Dec_len(MediumNode), MediumNode, FirstNode);
        Node * tmp, * tmp1;
        Split(MediumNode, Count(MediumNode) - 1, MediumNode, tmp);

        SplitForOrdered(FirstNode, tmp->key, FirstNode, SecondNode);

        Split(FirstNode, Count(FirstNode) - 1, FirstNode, tmp1);

        FirstNode = Merge(FirstNode, tmp);
        FirstNode = Merge(FirstNode, SecondNode);

        MediumNode = Merge(MediumNode, tmp1);
        FirstNode->rev ^= true;
        MediumNode = Merge(MediumNode, FirstNode);
        head = Merge(LeftNode, Merge(MediumNode, RightNode));

    }

    int sum(size_t left, size_t right) {
        left--;
        Node * LeftNode;
        Node * MediumNode;
        Node * RightNode;
        Split(head, right, head, RightNode);
        Split(head, left, LeftNode, MediumNode);
        int res = MediumNode->sum;
        head = Merge(LeftNode, Merge(MediumNode, RightNode));

        return res;
    }

    void free_treap() {
        free_node(this->head);
        head = NULL;
    }
};

class Solution {
public:
    virtual std::vector<int> Solve(const std::vector<int> &input){};
};


class TreeSolution:public Solution {
public:
     virtual std::vector<int> Solve(const std::vector<int> &input) {
        Treap * tree = new Treap(NULL);
        std::vector<int> output;
        int j = 0;
        std::vector<int> quick_realization;
        int n = input[j++];

        quick_realization.resize(n);

        for(size_t i = 0; i < n; ++i)
           quick_realization[i] = input[j++];

        Treap * Tree = new Treap(NULL);

        Tree->init_array(quick_realization);

        int m = input[j++];
        for(size_t i = 0; i < m; ++i) {
            int tmp, x, y;
            tmp = input[j++];
            x = input[j++];
            y = input[j++];

            if (tmp == 1) {
                output.push_back(Tree->sum(x, y));
            }
            if (tmp == 2) {
                Tree->set_el(x, y);

            }
            if (tmp == 3) {
                Tree->add(y, x);

            }
            if (tmp == 4) {
                Tree->NextP(x, y);

            }

        }

        Tree->free_treap();
        return output;
    }
};

class VectorSolution:public Solution {
public:
    virtual std::vector<int> Solve(const std::vector<int> &input) {
        std::vector<int> output;
        int j = 0;
        std::vector<int> slow_realization;
        int n = input[j++];

       slow_realization.resize(n);

        for(size_t i = 0; i < n; ++i)
            slow_realization[i] = input[j++];
        int m = input[j++];
        for(size_t i = 0; i < m; ++i) {
            int tmp, x, y;
            tmp = input[j++];
            x = input[j++];
            y = input[j++];

            if (tmp == 1) {
                int s = 0;
                for(size_t k = x - 1; k < y; ++k)
                    s += slow_realization[k];
                output.push_back(s);
            }
            if (tmp == 2)
                slow_realization[y - 1] = x;
            if (tmp == 3) {
                slow_realization.insert(slow_realization.begin() + y - 1, x);

            }
            if (tmp == 4)
                std::next_permutation(&slow_realization[x - 1], &slow_realization[y]);
        }

        return output;

    }

};

std::vector<int> Test(size_t m, int mn, int mx) {
    std::vector<int> input;
    int n = my_rand(mn, mx);
    input.push_back(n);
    for(size_t i = 0; i < n; ++i)
        input.push_back(my_rand(mn, mx));
    input.push_back(m);
    for(size_t i = 0; i < m; ++i) {
        input.push_back(my_rand(1, 4));
        if (input.back() == 1 || input.back() == 4) {
            input.push_back(my_rand(1, n));
            input.push_back(my_rand(input.back(), n));
        }
        else {
            input.push_back(my_rand(mn, mx));
            input.push_back(my_rand(1, n));
            if (input[input.size() - 3] == 3)
                ++n;
        }
    }
    return input;
};

template <class Solver>
std::vector<int> Solve(const std::vector<int> &input, Solver solver) {
    return solver.Solve(input);
}

void printv(std::vector<int> a) {
    for (int i = 0; i < a.size(); i++) {
        std::cout << a[i] << ' ';
    }
    std::cout << '\n';
}

bool tests(size_t n) {
    for(size_t i = 0; i < n; ++i) {
        int l = my_rand(1, 10);
        std::vector<int> input = Test(my_rand(1, 10), l, my_rand(l, 10));
        //std::vector<int> output_vector = Solve(input, VectorSolution());
        std::vector<int> output_tree = Solve(input, TreeSolution());

        if (0) {
            printv(input);
            printv(output_tree);
            //printv(output_vector);
            std::cout << ":(((\n";
            return false;
        }
    }
    return true;
}

int main()
{
    std::cout << tests(1000);
    return 0;
}
