#ifndef INDEXER_H
#define INDEXER_H

#include <string>
#include <unordered_map>
#include <set>
#include <vector>

class Indexer {
private:
    // main structure of the engine: key - word, inner key - doc name, value - frequency 
    std::unordered_map<std::string, std::unordered_map<std::string, int>> invertedIndex;

    // to store length of each doc
    std::unordered_map<std::string, int> docLen;

    // helper: normalize words (lowercase + strip punctuation)
    std::string normalizeWord(const std::string& word) const;

public:
    void addDocument(const std::string& filename);
    void buildIndex(const std::vector<std::string>& files);
    std::unordered_map<std::string, int> searchWord(const std::string& word) const;
    void printIndex() const;

    int getTotalDoc() { return docLen.size();};

    // getters
    std::unordered_map<std::string, std::unordered_map<std::string, int>>& getIndex()  {return invertedIndex;};
    std::unordered_map<std::string, int>& getDocLen() {return docLen;};
}; 

#endif
