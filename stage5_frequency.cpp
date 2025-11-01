#include <ncurses.h>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <map>
#include <cctype>
#include <chrono>
#include "Trie.hpp"

int main() {
    Trie trie;
    std::string textfile = "alice_wonderland.txt";

    buildTrie(trie, textfile);

    std::string userInput = "";
    size_t l_word = 0; // Use size_t for indices. Start at 0.
    long long elapsed = 0;
    
    std::vector<std::pair<std::string, int>> suggestions;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    int i;
    while (true) {
        // 1. Draw Screen
        clear();
        mvprintw(0, 0, "Enter text: %s", userInput.c_str());
        
        i = 0;
        for (const auto& suggestion : suggestions){
            mvprintw(i + 2, 0, "- %s : %i times", suggestion.first.c_str(), suggestion.second);
            i++;
            if (i >= 10) break; // Good idea to add a limit
        }

        mvprintw(13, 0, "Search time: %lld ms", elapsed);

        // Move cursor to the end of the input
        move(0, 12 + userInput.length());
        refresh(); 

        // 2. Get Input 
        int ch = getch();

        if (ch == 27) { // Escape key
            break;
        } 
        else if (ch == KEY_BACKSPACE || ch == 127) { // Handle backspace
            if (!userInput.empty()) {
                
                bool deletedSpace = (userInput.back() == ' ');
                userInput.pop_back();

                if (deletedSpace) {
                    size_t lastSpacePos = userInput.find_last_of(' ');
                    
                    if (lastSpacePos == std::string::npos) {
                        l_word = 0;
                    } else {
                        l_word = lastSpacePos + 1;
                    }
                }
            }
        } 
        else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) { // Allow upper and lower
            userInput.push_back((char)ch);
        } 
        else if (ch == ' '){
            userInput.push_back((char)ch);
            l_word = userInput.length(); 
        }

        // Process Input (run autocomplete)
        if (userInput.empty()) {
            suggestions.clear();
            l_word = 0; // Reset l_word when all text is deleted
        } else {
            // Get the current word from l_word to the end
            std::string currentWord = userInput.substr(l_word);

            if (cleanWord(currentWord).empty()) {
                // This happens right after a space is typed
                suggestions.clear();
            } else {
                // Pass the current word to the autocomplete
                auto begin = std::chrono::high_resolution_clock::now();
                suggestions = trie.prefixSearch(cleanWord(currentWord));
                auto end = std::chrono::high_resolution_clock::now();
                elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
            }
        }
    }

    // Restore terminal settings
    endwin(); 
    return 0;
}