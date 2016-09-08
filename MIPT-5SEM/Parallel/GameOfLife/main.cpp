#include <iostream>
#include <vector>
#include <fstream>
#include <thread>

using namespace std;

const int g_threadCount = 4;

vector<vector<bool>> g_current, g_tmp;
int g_height, g_width;


void process(int l, int r)
{
    for (int i = l; i < r; ++i)
    {
        for (int j = 0; j < g_width; ++j)
        {
            int neighbors = 0;
            for (int i1 = i - 1; i1 <= i + 1; ++i1)
            {
                for (int j1 = j - 1; j1 <= j + 1; ++j1)
                {
                    neighbors += g_current[(i1 + g_height) % g_height][(j1 + g_width) % g_width];
                }
            }
            neighbors -= g_current[i][j];
            g_tmp[i][j] = (neighbors == 3) || (neighbors == 2 && g_current[i][j]);
        }
    }
}

void step()
{
    int partSize = g_height / g_threadCount + (g_height % g_threadCount != 0);
    vector<thread> threads(g_threadCount);
    for (int i = 0; i < g_threadCount; ++i)
    {
        threads[i] = thread(process, i * partSize, min((i + 1) * partSize, g_height));
    }
    for (int i = 0; i < g_threadCount; ++i)
    {
        threads[i].join();
    }
    swap(g_current, g_tmp);
}

void read(istream& in)
{
    in >> g_width >> g_height;
    g_current.resize(g_height);
    g_tmp.resize(g_height);
    for (int i = 0; i < g_height; ++i)
    {
        g_current[i].resize(g_width);
        g_tmp[i].resize(g_width);
    }
    char c;
    for (int i = 0; i < g_height; ++i)
    {
        for (int j = 0; j < g_width; ++j)
        {
            in >> c;
            g_current[i][j] = (c == '@');
        }
    }
}

void print(ostream& out)
{
    out << g_width << ' ' << g_height << '\n';
    for (int i = 0; i < g_height; ++i)
    {
        for (int j = 0; j < g_width; ++j)
        {
            out << (g_current[i][j] ? '@' : '.');
        }
        out << '\n';
    }
}

int main()
{
    ifstream in("in.txt", fstream::in);
    ofstream out("out.txt", fstream::out);
    read(in);
    for (int i = 0; i < 1000; ++i)
    {
        step();
        print(out);
    }
    return 0;
}