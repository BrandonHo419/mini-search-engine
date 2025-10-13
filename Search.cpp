#include "Search.h"
#include <sstream>
#include <map>
#include <algorithm>

Indexer globalIndexer;  // single instance for the whole program

std::vector<std::string> searchQuery(const std::string& query) {
    std::istringstream iss(query);
    std::string word;

    std::map<std::string, int> docScores;  // filename -> score

    while (iss >> word) {
        auto results = globalIndexer.searchWord(word); // returns unordered_map<string,int>
        for (auto& [filename, freq] : results) {
            docScores[filename] += freq; // simple sum scoring
        }
    }

    if (docScores.empty()) return {};

    // sort by score descending
    std::vector<std::pair<std::string,int>> scoredDocs(docScores.begin(), docScores.end());
    std::sort(scoredDocs.begin(), scoredDocs.end(),
              [](const auto& a, const auto& b){ return a.second > b.second; });

    std::vector<std::string> finalResults;
    for (auto& [filename, score] : scoredDocs)
        finalResults.push_back(filename);

    return finalResults;
}
