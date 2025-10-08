#include "Index.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

// helper to clean up words
std::string Indexer::normalizeWord(const std::string& word) const {
    std::string clean = "";
    for(size_t i = 0; i < word.size(); i++){
        char c = word[i];
        if(!std::ispunct(c)){
            clean += std::tolower(c);
        }
    }

    return clean;
}

void Indexer::addDocument(const std::string& filename) {
    std::ifstream file(filename);
    if(!file){
        std::cout<< "Error with file" << "\n";
        return;
    }


    std::string word;
    int total = 0;
    while (file >> word) {
        std::string clean = normalizeWord(word);
        if (clean.empty()) continue;
        
        invertedIndex[clean][filename]++;
        total++;
    }

    docLen[filename] = total;
    file.close();
}

void Indexer::buildIndex(const std::vector<std::string>& files) {
    for(int i = 0; i < files.size(); i++){
        addDocument(files[i]);
    }
}

std::unordered_map<std::string, int> Indexer::searchWord(const std::string& word) const {
    std::string clean = normalizeWord(word);

    auto it = invertedIndex.find(clean);
    if( it != invertedIndex.end()) return it->second;
    return {};
}

void Indexer::printIndex() const {
   for(auto& [term, postings] :  invertedIndex){
    std::cout<< term<< ": ";
        for(auto& [doc, freq] : postings){
            std::cout<< "(" << doc << freq << ")";
        }

        std::cout << "\n";
   }
}
