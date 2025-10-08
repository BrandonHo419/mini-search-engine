#ifndef INDEXER_H
#define INDEXER_H

#include <string>
#include <map>
#include <set>
#include <vector>

class Indexer {
private:
    // word -> set of documents that contain it
    std::map<std::string, std::set<std::string>> invertedIndex;

    // helper: normalize words (lowercase + strip punctuation)
    std::string normalizeWord(const std::string& word) const;

public:
    void addDocument(const std::string& filename);
    void buildIndex(const std::vector<std::string>& files);
    std::set<std::string> searchWord(const std::string& word) const;
    void printIndex() const;
};

#endif
