#include "Search.h"
#include <sstream>
#include <cmath>
#include <map>
#include <algorithm>
#include <utility>

Indexer globalIndexer; 

// Note: This file is largely redundant given the logic in query_processor.hpp, 
// but is included for completeness based on original file list. 
// The main search logic is in processQuery.

std::vector<std::string> searchQuery(const std::string& query) {
    std::istringstream iss(query);
    std::string word;

    std::map<std::string, double> docScores; 

    int totalDocs = globalIndexer.getTotalDoc(); 
    
    while (iss >> word) {
        // This simple function relies on Indexer::searchWord to handle normalization.
        auto results = globalIndexer.searchWord(word);
        if (results.empty()) continue;

        int df = results.size();
        double idf = (df > 0) ? std::log(1.0 * totalDocs / df) : 0.0; 

        for (auto& [filename, data] : results) {
            int freq = data.first;
            double tf = 1.0 * freq / globalIndexer.getDocLen().at(filename);
            double tfidf = tf * idf;
            docScores[filename] += tfidf;
        }
    }

    if (docScores.empty()) return {};

    // sort documents by TF-IDF score descending
    std::vector<std::pair<std::string, double>> scoredDocs(docScores.begin(), docScores.end());
    std::sort(scoredDocs.begin(), scoredDocs.end(),
              [](const auto& a, const auto& b){ return a.second > b.second; });

    std::vector<std::string> finalResults;
    for (auto& [filename, score] : scoredDocs)
        finalResults.push_back(filename);

    return finalResults;
}