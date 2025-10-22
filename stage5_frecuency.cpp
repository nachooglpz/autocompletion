#include <ncurses.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <map>

struct TrieNode{
    public:
        std::unordered_map<char, std::unique_ptr<TrieNode>> children;
        int frecuency;
        TrieNode() : frecuency(0){} // nodes are marked as false on creation
};

class Trie{
    private:
        std::unique_ptr<TrieNode> root;

        void dfsPrefixSearch(TrieNode* node, std::string& currentWord, std::vector<std::pair<std::string, int>>& results){
            if(node->frecuency != 0){
                results.push_back({currentWord, node->frecuency});
            }
            for (const auto& pair : node->children){
                currentWord.push_back(pair.first);
                dfsPrefixSearch(pair.second.get(), currentWord, results);
                currentWord.pop_back();
            }
        }
    
    public:
        Trie(){
            root = std::make_unique<TrieNode>();
        }
        void insert(const std::string& word){
            TrieNode* current = root.get();
            for(char c : word){
                if (current->children.find(c) == current->children.end()){ //if not in children
                    current->children[c] = std::make_unique<TrieNode>();
                }
                current = current->children[c].get();
            }
            current->frecuency++;
        }
        int search(const std::string& word){
            TrieNode* current = root.get();
            for(char c : word) {
                auto iterator = current->children.find(c);
                if(iterator ==  current->children.end()){
                    return false;
                }
                current = iterator->second.get();
            }
            return current->frecuency;
        }

        bool startsWith(const std::string& prefix){
            TrieNode* current = root.get();
            for (char c : prefix){
                auto iterator = current->children.find(c);
                if(iterator == current->children.end()){
                    return false;
                }
                current = iterator->second.get();
            }
            return true;
        }


        std::vector<std::pair<std::string, int>> prefixSearch(const std::string _prefix){
            
            // Change results type from map to vector
            std::vector<std::pair<std::string, int>> results;
            TrieNode* current = root.get();
            
            for (char c : _prefix){
                auto iterator = current->children.find(c);
                if(iterator == current->children.end()){
                    return results;
                }
                current = iterator->second.get();
            }

            std::string prefix = _prefix;
            dfsPrefixSearch(current, prefix, results); // This populates the vector

            // --- ADD THIS SORTING STEP ---
            // Sort the vector by frequency (the .second of the pair)
            // in descending order (highest first).
            std::sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
                return a.second > b.second; // Sorts by int, high-to-low
            });

            return results; // Return the sorted vector
        }

};

#include <cctype> // Add this to your includes

std::string cleanWord(const std::string& raw_word) {
    std::string cleaned = "";
    for (char c : raw_word) {
        if (std::isalpha(c)) { 
            cleaned += std::tolower(c); 
        }
    }
    return cleaned;
}
int main() {
    Trie trie;
    // open the file
    std::ifstream text("alice_wonderland.txt");
    if (!text) std::cerr << "error opening file" << std::endl;

    // convert the file into a single string
    std::stringstream buffer;
    buffer << text.rdbuf();
    std::string content = buffer.str();

    int l = 0;
    int r = 0;

    while (r < content.length()) {
        if (content[r] == ' ' || content[r] == '\n' ){
            
            // Get the raw substring
            std::string raw_word = content.substr(l, r-l);
            
            // Clean it
            std::string cleaned_word = cleanWord(raw_word);
            
            // only insert if the cleaned word is not empty
            if (!cleaned_word.empty()) {
                trie.insert(cleaned_word);
            }
            
            l = r + 1;
        }
        r = r + 1;
    }
    
    // Clean the last word too
    if (l < r) {
        std::string cleaned_word = cleanWord(content.substr(l, r-l));
        if (!cleaned_word.empty()) {
            trie.insert(cleaned_word);
        }
    }


    std::string userInput = "";
    size_t l_word = 0; // Use size_t for indices. Start at 0.
    
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
        }/*
        for (int i = 0; i < suggestions.size(); ++i) {
            mvprintw(i + 2, 0, "- %s", suggestions[i].c_str());
        }*/

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
                suggestions = trie.prefixSearch(cleanWord(currentWord));
            }
        }
    }

    // Restore terminal settings
    endwin(); 
    return 0;
}