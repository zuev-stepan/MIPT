#include <random>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;
const int CONTEXT = 10;
const double d = 0.85;
const double EPS = 1e-9;
typedef vector<vector<int> > Graph;

map<string, set<string> > dictionary;
vector<string> words;

Graph initFromDict(const map<string, set<string> >& dictionary) {
	int i = 0;
	map<string, int> tWords;
	for (auto it = dictionary.begin(); it != dictionary.end(); ++it) {
		words.push_back(it->first);
		tWords[it->first]=i;
		i++;
	}
	Graph graph(i);
	for (auto it = dictionary.begin(); it != dictionary.end(); ++it) {
		for (auto subIt = it->second.begin(); subIt != it->second.end(); ++subIt) {
			graph[tWords[*subIt]].push_back(tWords[it->first]);
			graph[tWords[it->first]].push_back(tWords[*subIt]);
		}
	}
	return graph;
}

void printGraph(const Graph& graph) {
	for (int i = 0; i < graph.size(); i++) {
		cout << i << ": ";
		for (int j = 0; j < graph[i].size(); j++) {
			cout << graph[i][j] << ' ';
		}
		cout << '\n';
	}
}

Graph randGraph(int size) {
	Graph graph(size);
	for (int i = 0; i < graph.size(); i++) {
		for (int j = 0; j < graph.size(); j++) {
			if ((rand() % 2) && (i != j)) {
				graph[i].push_back(j);
			}
		}
	}
	return graph;
}

vector<double> getPageRank(const Graph& graph) {
    vector<double> pageRank(graph.size());
	for (int i = 0; i < graph.size(); i++) {
		pageRank[i] = 1.0 / graph.size();
	}
	vector<double> tempPageRank(graph.size());
	int iter = 0;
	double error = 1;
	for(iter = 0; error > EPS; iter++) {
		error = 0;
		for (int i = 0; i < graph.size(); i++) {
			tempPageRank[i] = (1 - d) / graph.size();
		}
		for (int i = 0; i < graph.size(); i++) {
			for (int j = 0; j < graph[i].size(); j++) {
				if (i != j) {
					tempPageRank[graph[i][j]] += d * pageRank[i] / graph[i].size();
				}
			}
		}
		for (int i = 0; i < graph.size(); i++) {
			error = max(error, 2 * abs(tempPageRank[i] - pageRank[i]) / (pageRank[i] + tempPageRank[i]));
			pageRank[i] = tempPageRank[i];
		}
		//cout << "   " << iter << ": " << error << '\n';
	}
	return pageRank;
}

string goodWord(string& word) {
    string tmp = "";
    for (int i = 0; i < word.size(); i++) {
		if (('a' <= word[i]) && (word[i] <= 'z')) {
			tmp += word[i];
		}
		if (('A' <= word[i]) && (word[i] <= 'Z')) {
			tmp += word[i] - 'A' + 'a';
		}
		//cout << tmp << '\n';
    }
    return tmp;
}

void fillDict() {
	vector<string> tWords;
	string newWord;
	while(!cin.eof()) {
		cin >> newWord;
		newWord = goodWord(newWord);
		if(newWord.size() == 0)
            continue;
		tWords.push_back(newWord);
		for (int i = max(0, ((int) tWords.size() - CONTEXT)); i < tWords.size() - 1; i++) {
			dictionary[newWord].insert(tWords[i]);
			dictionary[tWords[i]].insert(newWord);
		}

	}
}

void prove() {
    freopen("out.txt", "wt", stdout);
    Graph graph = randGraph(10);
    printf("10: \n"); getPageRank(graph);
    graph = randGraph(100);
    printf("100: \n"); getPageRank(graph);
    graph = randGraph(1000);
    printf("1000: \n"); getPageRank(graph);
    graph = randGraph(10000);
    printf("10000: \n"); getPageRank(graph);
}

int main()
{
	if (1) {
        prove();
        return 0;
	}
	freopen("input.txt", "rt", stdin);
	fillDict();
	Graph graph = initFromDict(dictionary);
	vector<double> pageRank = getPageRank(graph);
	vector<pair<double,string > > result;
	for (int i = 0; i < words.size(); i++) {
		result.push_back(make_pair(pageRank[i], words[i]));
	}
	sort(result.rbegin(), result.rend());
	for (int i = 0; i < min((int)result.size(), 100); i++) {
		cout << result[i].second << '\n';
	}
	return 0;
}

