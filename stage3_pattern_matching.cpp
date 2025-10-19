// g++ -o stage3_pattern_matching stage3_pattern_matching.cpp && ./stage3_pattern_matching

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
using namespace std;

vector<int> lps(const string p);
vector<int> kmp(const string &p, const string &t);
void print_positions(vector<int> &indices, string &word);
void print_content_positions(vector<int> &indices, string &content);

int main() {
    // open the file
    ifstream text("alice_wonderland.txt");
    if (!text) cerr << "error opening file" << endl;

    // convert the file into a single string
    stringstream buffer;
    buffer << text.rdbuf();
    string content = buffer.str();

    // input the word to find
    string word;
    cout << "Input the word to find: ";
    cin >> word;
    cout << endl;


    // get the matches
    auto begin = chrono::high_resolution_clock::now();
    vector<int> indices = kmp(word, content);
    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - begin);

    // output the matches
    // print_positions(indices, word);
    print_content_positions(indices, content);

    cout << "Total number of occurrences: " << indices.size() << endl << endl;

    cout << "Search time: " << elapsed.count() << "ms" << endl;
}

vector<int> lps(const string p) {
    vector<int> v(p.size(), 0);

    int j = 0, i = 1;
    while (i < p.size()) {
        if (p[i] == p[j]) {
            v[i] = j + 1;
            i++;
            j++;
        } else if (j == 0) {
            v[i] = 0;
            i++;
        } else j = v[j-1];
    }

    return v;
}

vector<int> kmp(const string &p, const string &t) {
    vector<int> indices;
    auto v = lps(p);
    int i = 0, j = 0;

    while (i < t.size()) {
        if (t[i] == p[j]) { i++; j++; }
        else if (j == 0) i++;
        else j = v[j - 1];

        if (j == p.size()) {
        indices.push_back(i - j); // add to the coincidences
        j = v[j - 1]; // continue the search
        }
    }

    return indices;
}

void print_positions(vector<int> &indices, string &word) {
    cout << "Positions where " << word << " appear in the text: ";
    for (int i = 0; i < indices.size() - 1; i++) {
        cout << indices[i] << ", ";
    }
    cout << indices[indices.size() - 1] << "." << endl << endl;
}

void print_content_positions(vector<int> &indices, string &content) {
    // print for every word found
    for (int i = 0; i < indices.size(); ++i) {
        cout << "Match " << i + 1 << endl;

        // print 50 characters starting from the first letter of the word found
        cout << "\'";
        for (int j = indices[i]; j < indices[i] + 50; ++j) {
            cout << content[j];
        }
        cout << "\'" << endl << endl;
    }
}