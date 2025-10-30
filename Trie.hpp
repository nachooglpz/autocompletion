#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

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

            // Sort the vector by frequency (the .second of the pair)
            // in descending order (highest first).
            std::sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
                return a.second > b.second; // Sorts by int, high-to-low
            });

            return results; // Return the sorted vector
        }
};

// Utils to the project
std::string cleanWord(const std::string& raw_word) {
    std::string cleaned = "";
    for (char c : raw_word) {
        if (std::isalpha(c)) { 
            cleaned += std::tolower(c); 
        }
    }
    return cleaned;
}

void buildTrie(Trie &trie, std::string &textfile) {
    std::ifstream text(textfile);
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
}