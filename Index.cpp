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
    while (file >> word) {
        std::string clean = normalizeWord(word);
        if (!clean.empty())
            invertedIndex[clean].insert(filename);
    }

    file.close();
}

void Indexer::buildIndex(const std::vector<std::string>& files) {
    for(int i = 0; i < files.size(); i++){
        addDocument(files[i]);
    }
}

std::set<std::string> Indexer::searchWord(const std::string& word) const {
    std::string clean = normalizeWord(word);

    if(invertedIndex.find(clean) != invertedIndex.end()){
        return invertedIndex.at(clean);
    }else{
        return std::set<std::string>();
    }
}

void Indexer::printIndex() const {
   for(auto it = invertedIndex.begin(); it != invertedIndex.end(); it++){
    std::cout<< it->first << " : ";
    for(auto docIt = it->second.begin(); docIt != it->second.end(); docIt++){
        std::cout<< *docIt << " ";
    }

    std::cout<< "\n";
   }
}
