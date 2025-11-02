#include <chrono>
#include <fstream>
#include <iostream>
#include "Trie.hpp"

int main() {
    std::ofstream file("analysis.csv");
    if (!file) {
        std::cerr << "error opening file" << std::endl;
        return 1;
    }

    file << "book,word count,prefix,size of prefix,time (ms),number of results\n";

    std::vector<std::string> books({"alice_wonderland.txt", "jekyll_hyde.txt", 
        "pride_prejudice.txt", "scarlett_letter.txt", "war_peace.txt", "miserables.txt"
    });

    std::vector<std::string> prefixlist({
        "al", "ali", "alic", // -> "alice"
        "an", "and", "ans",  // -> "and", "answer"
        "be", "beg", "bec", "bee", // -> "began", "before", "been", "being"
        "bo", "boo", "bod", // -> "book", "both", "body"
        "ca", "cat", "car", "cal", // -> "cat", "came", "call"
        "cha", "chap", "char", "che", "chi", // -> "chapter", "chair", "child", "cheer"
        "co", "com", "cou", "con", // -> "come", "could", "conversation"
        "da", "dar", "day", // -> "dark", "day"
        "do", "don", "dou", // -> "door", "down", "doubt"
        "dr", "dre", "dra", // -> "dream", "draw"
        "ea", "ear", "eat", // -> "early", "eat"
        "ev", "eve", "ever", "eve", // -> "even", "every"
        "fa", "fal", "fam", // -> "fall", "family"
        "fi", "fin", "fir", // -> "find", "first"
        "fo", "for", "forg", // -> "found", "forget"
        "fr", "fri", "fro", // -> "friend", "from"
        "ga", "gam", "gat", // -> "game", "gate"
        "go", "gon", "goo", // -> "good", "gone"
        "gr", "gro", "gre", // -> "great", "ground"
        "ha", "had", "hap", "hat", "hea", "hea", "hel", // -> "had", "happen", "hat", "head", "heart", "help"
        "he", "her", "hes", // -> "her", "herself", "hesitate"
        "ho", "hou", "how", // -> "house", "how"
        "hum", // -> "humpty"
        "ki", "kin", // -> "kind"
        "la", "lad", "lar", "lat", // -> "large", "late"
        "li", "lit", "lit", // -> "little"
        "lo", "lon", "loo", // -> "long", "look"
        "ma", "man", "mat", "mad", // -> "made", "matter", "mad"
        "me", "men", "mem", // -> "mean", "mention", "memory"
        "mi", "min", "mis", // -> "mind", "miss"
        "mo", "mon", "mom", // -> "moment", "money"
        "ne", "nev", "new", "nic", // -> "never", "next", "nice"
        "no", "not", "now", // -> "nothing", "now"
        "of", "off", "oft", // -> "often", "off"
        "on", "onc", "one", // -> "once", "one"
        "ou", "our", "out", // -> "ourselves", "outside"
        "ov", "ove", "over", // -> "over", "overflow"
        "pa", "par", "parl", // -> "part", "party", "particularly"
        "pe", "per", "perh", // -> "perhaps", "person"
        "pla", "ple", "plu", // -> "place", "please", "plenty"
        "po", "pos", "pow", // -> "poor", "possible", "power"
        "pr", "pre", "pro", "pret", // -> "present", "project", "pretty"
        "pu", "pur", // -> "purple", "purse"
        "qu", "que", "ques", "qui", // -> "queen", "question", "quite"
        "ra", "rab", "rat", "ran", // -> "rabbit", "rather", "ran"
        "re", "rea", "rem", "rep", "res", "ret", // -> "really", "remember", "reply", "rest"
        "ro", "roo", "rou", // -> "room", "round"
        "sa", "sat", "say", "sai", // -> "said", "sail", "same"
        "se", "see", "ser", "sec", // -> "see", "several", "second"
        "sh", "she", "sho", "sha", // -> "she", "shouted", "shadow"
        "si", "sin", "sit", // -> "since", "sitting"
        "so", "som", "sou", // -> "some", "sound", "south"
        "sp", "spe", "spi", // -> "speak", "spoke", "spirit"
        "st", "sta", "sto", "str", // -> "stand", "stop", "strange"
        "su", "sur", // -> "suppose", "surprise"
        "ta", "tak", "tal", "taw", // -> "take", "talk", "taught"
        "te", "tel", "ten", "ter", // -> "tell", "tend", "term"
        "th", "the", "thi", "tho", "thu", // -> "the", "there", "thing", "thought", "thus"
        "to", "tow", "tom", // -> "toward", "tomorrow"
        "tr", "tre", "tri", // -> "tree", "tried"
        "tu", "tur", // -> "turn"
        "un", "und", "unt", // -> "under", "until"
        "up", "upo", // -> "upon", "upset"
        "ve", "ver", // -> "very", "verse"
        "wa", "wan", "was", "wat", // -> "was", "want", "water"
        "we", "wel", "wer", "wh", "whe", "whi", "who", "why", "wil", // -> "well", "when", "white", "while", "who", "why", "will"
        "wo", "won", "wond", // -> "wonder", "wonderful"
        "yo", "you", "your", // -> "you", "your", "yourself"
        "ze", "zy" // -> nonexistent prefixes
    });

    for (auto &book : books) {
        Trie trie;
        int wordcount = buildTrie(trie, book, true);

        for (auto &prefix : prefixlist) {
            std::vector<std::pair<std::string, int>> suggestions;

            auto begin = std::chrono::high_resolution_clock::now();
            suggestions = trie.prefixSearch(cleanWord(prefix));
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

            file << book << "," << wordcount << "," << prefix << "," << prefix.size() << "," << elapsed << "," << suggestions.size() << "\n";
        }
    }
    
    return 0;
}