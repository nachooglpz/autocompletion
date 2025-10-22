#include <ncurses.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <unordered_set>

struct TrieNode{
    public:
        std::unordered_map<char, std::unique_ptr<TrieNode>> children;
        bool endOfWord;
        TrieNode() : endOfWord(false){} // nodes are marked as false on creation
};

class Trie{
    private:
        std::unique_ptr<TrieNode> root;

        void dfsPrefixSearch(TrieNode* node, std::string& currentWord, std::vector<std::string>& results){
            if(node->endOfWord){
                results.push_back(currentWord);
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
            current->endOfWord = true;
        }
        bool search(const std::string& word){
            TrieNode* current = root.get();
            for(char c : word) {
                auto iterator = current->children.find(c);
                if(iterator ==  current->children.end()){
                    return false;
                }
                current = iterator->second.get();
            }
            return current->endOfWord;
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


        std::vector<std::string> prefixSearch(const std::string _prefix){
            std::vector<std::string> results;
            TrieNode* current = root.get();
            for (char c : _prefix){
                auto iterator = current->children.find(c);
                if(iterator == current->children.end()){
                    return results;
                }
                current = iterator->second.get();
            }

            std::string prefix = _prefix;
            dfsPrefixSearch(current, prefix, results);
            return results;
        }

};


int main() {
    Trie trie;
    trie.insert("spell");
    trie.insert("spells");
    trie.insert("spellbound");
    trie.insert("spin");
    trie.insert("spit");
    trie.insert("apple");

    std::string userInput = "";
    std::vector<std::string> suggestions;

    initscr();     // Start ncurses mode
    cbreak();      // Disable line buffering
    noecho();      // Don't echo characters automatically
    keypad(stdscr, TRUE); // Enable arrow keys, etc.

    while (true) {
        // Draw Screen
        clear();
        mvprintw(0, 0, "Enter text: %s", userInput.c_str());
        
        // Draw all suggestions
        for (int i = 0; i < suggestions.size() && i < 10; ++i) {
            mvprintw(i + 2, 0, "- %s", suggestions[i].c_str());
        }

        // Manually move the cursor back to the end of the user's input.
        // 12 is the length of "Enter text: "
        move(0, 12 + userInput.length());

        refresh(); 

        // Get Input 
        int ch = getch(); // waits here for a single key press

        if (ch == 27) { // 27 is the Escape key
            break;
        } 
        else if (ch == KEY_BACKSPACE || ch == 127) { // Handle backspace
            if (!userInput.empty()) {
                userInput.pop_back();
            }
        } 
        else if (ch >= 'a' && ch <= 'z') {
            userInput.push_back((char)ch);
        }

        if (!userInput.empty()) {
            suggestions = trie.prefixSearch(userInput);
        } else {
            suggestions.clear();
        }
    }

    // Restore terminal settings
    endwin(); 
    return 0;
}