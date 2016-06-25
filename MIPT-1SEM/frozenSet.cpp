#include <bits/stdc++.h>

using std::vector;
using std::hash;
using std::equal_to;
using std::pair;
using std::list;
using std::cout;
using std::make_pair;

const size_t SET_SIZE = 1000;

template <
    class Key,
    class Hash = hash<Key>,
    class Pred = equal_to<Key>
> class FrozenSet {
private:
    vector<list<pair<size_t, Key> > > table;
    size_t keyCount;
    Hash hf;
    Pred eql;

public:
    template <class InputIterator >
    FrozenSet(
        InputIterator first, InputIterator last,
        const Hash & hf = Hash(),
        const Pred & eql = Pred(),
        float loadFactor = 0.72
    ) : hf(hf), eql(eql) {
        keyCount = (last - first);
        size_t size = (size_t) keyCount / loadFactor + 1, h;
        size += (size % 2 == 0);
        for (size_t i = 2; i * i <= size; ++i) {
            if (size % i == 0) {
                size += 2;
                i = 1;
            }
        }

        table.resize(size);
        for (InputIterator i = first; i != last; ++i) {
            if (contains(*i))
                continue;
            h = hf(*i);
            table[h % size].push_back(make_pair(h, *i));
        }
    }

    bool contains(const Key& key) const {
        size_t h = hf(key);
        for (auto& i : table[h % table.size()]) {
            if (i.first == h && i.second == key) {
                return true;
            }
        }
        return false;
    }

    void printInfo() {
        cout << "Size: " << table.size() << '\n';
        cout << "Number of keys: " << keyCount << '\n';
        size_t collisions = 0, max_bucket_size = 0, sqr_sum = 0;
        for (size_t i = 0; i < table.size(); ++i) {
            collisions += table[i].size() * (table[i].size() - 1) / 2;
            sqr_sum += table[i].size() * table[i].size();
            max_bucket_size = (max_bucket_size > table[i].size() ? max_bucket_size : table[i].size());
        }
        cout << "Collisions: " << collisions << '\n';
        cout << "Sum sqr(bucket size): " << sqr_sum << '\n';
        cout << "Max bucket size: " << max_bucket_size << '\n';
    }
};

int main() {
    srand(1337);
    vector<int> a;
    for (size_t i = 0; i < SET_SIZE; i++) {
        a.push_back(rand());
    }
    FrozenSet<int> b(a.begin(), a.end());
    b.printInfo();
    return 0;
}
