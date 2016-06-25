#include <bits/stdc++.h>

using std::vector;
using std::cin;
using std::cout;
using std::push_heap;
using std::pop_heap;
using std::copy;
using std::greater;
using std::min;
using std::nth_element;

const bool DEBUG = false;
const size_t MOD = 300, TEST_SIZE = 300, ITERATIONS = 300, TOP_SIZE = 10;

//Add element - O(log(topSize))
//Get (topSize) largest elements - O(topSize)
template <typename T>
class TopElementsNlogK {
private:
    size_t topSize;
    vector<T> topElements;

public:
    TopElementsNlogK(size_t topSize) : topSize(topSize) {}

    void add(const T& t) {
        if (topElements.size() > topSize) {
            pop_heap(topElements.begin(), topElements.end(), greater<T>());
            topElements.pop_back();
        }
        topElements.push_back(t);
        push_heap(topElements.begin(), topElements.end(), greater<T>());
    }

    vector<T> getCurrentTop() const {
        vector<T> top(topElements.size() - 1);
        copy(topElements.begin() + 1, topElements.end(), top.begin());
        return top;
    }
};

//Add element - amortized O(1)
//Get (topSize) largest elements - O(topSize)
template <typename T>
class TopElementsN {
private:
    size_t topSize, lastUpdate;
    vector<T> topElements;

public:
    TopElementsN(size_t topSize) : topSize(topSize), lastUpdate(0) {
        topElements.resize(2 * topSize);
    }

    void add(const T& t) {
        topElements[lastUpdate++] = t;
        if (lastUpdate == 2 * topSize) {
            nth_element(topElements.begin(), topElements.begin() + topSize, topElements.end(), greater<T>());
            lastUpdate = topSize;
        }
    }

    vector<T> getCurrentTop() {
        nth_element(topElements.begin(), topElements.begin() + topSize, topElements.end(), greater<T>());
        vector<T> top(min(topElements.size(), topSize));
        copy(topElements.begin(), topElements.begin() + top.size(), top.begin());
        return top;
    }
};


template <class T, class Solver>
vector<T> solve(const vector<T>& input, size_t topSize) {
    Solver topElements(topSize);
    for (size_t i = 0; i < input.size(); ++i) {
        topElements.add(input[i]);
    }
    return topElements.getCurrentTop();
}

vector<int> generateTest(size_t testSize) {
    vector<int> test;
    for (size_t i = 0; i < testSize; ++i) {
        test.push_back(rand() % MOD);
    }
    return test;
}

bool sortAndCompare(vector<int>& a, vector<int>& b) {
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

void printVector(const vector<int>& a) {
    for (size_t i = 0; i < a.size(); ++i) {
        cout << a[i] << ' ';
    }
    cout << '\n';
}

void stress() {
    for (size_t i = 0; i < ITERATIONS; ++i) {
        vector<int> test = generateTest(TEST_SIZE);
        vector<int> nlogkAnswer = solve<int, TopElementsNlogK<int> >(test, TOP_SIZE);
        vector<int> nAnswer = solve<int, TopElementsN<int> >(test, TOP_SIZE);
        if (!sortAndCompare(nlogkAnswer, nAnswer)) {
            printVector(test);
            printVector(nlogkAnswer);
            printVector(nAnswer);
            return;
        }
    }
}

double getMSTime() {
    return clock() / (CLOCKS_PER_SEC/1000);
}

void speedTest(int n, int k) {
    vector<int> test = generateTest(n);
    double timeN = 0, timeNlogN = 0;
    timeN = getMSTime();
    solve<int, TopElementsN<int> >(test, k);
    timeN = getMSTime() - timeN;
    timeNlogN = getMSTime();
    solve<int, TopElementsNlogK<int> >(test, k);
    timeNlogN = getMSTime() - timeNlogN;
    cout << "O(NlogK) is " << timeNlogN / timeN << " times slower than O(N)\n";
}

void solveIostream() {
    size_t n, k;
    cin >> n >> k;
    vector<int> input;
    input.resize(n);
    for (size_t i = 0; i < n; ++i) {
        cin >> input[i];
    }
    printVector(solve<int, TopElementsN<int> >(input, k));
}


int main() {
    srand(1337);
    //Stress();
    //SpeedTest(1000000, 100);
    solveIostream();
    return 0;
}
