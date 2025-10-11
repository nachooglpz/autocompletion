// g++ -o preprocessing preprocessing.cpp && ./preprocessing

#include <iostream>
#include <fstream>
#include <cctype>
#include <unordered_set>
using namespace std;

int main() {
    // open input file
    ifstream file("alice_wonderland.txt");
    if (!file) {
        cerr << "error opening input file" << endl;
        return 1;
    }

    // open output file
    ofstream clean("alice_cleaned.txt");
    // ofstream clean("alice_cleaned.csv");
    if (!clean) {
        cerr << "error opening output file" << endl;
        return 2;
    }

    // make the whole output into a single string
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    // check for single instances of words
    string word = "";
    unordered_set<string> unique_words;
    for (unsigned char ch : content) {
        if (!isspace(ch) && isalpha(ch)) {
            word.push_back(tolower(ch));
        } else {
            unique_words.insert(word);
            word = "";
        }
    }

    for (auto &word : unique_words) {
        clean << word << " ";
    }

    file.close();
    clean.close();

    return 0;
}