#include "Search.h"
#include <sstream>
#include <cmath>
#include <map>
#include <algorithm>

Indexer globalIndexer; 

std::vector<std::string> searchQuery(const std::string& query) {
    std::istringstream iss(query);
    std::string word;

    std::map<std::string, double> docScores;  // filename -> TF-IDF score

    int totalDocs = globalIndexer.getTotalDoc(); 
    
    while (iss >> word) {
        std::string clean = word; 
        auto results = globalIndexer.searchWord(word);
        if (results.empty()) continue;

        int df = results.size();
        double idf = std::log(1.0 * totalDocs / df); // Inverse Document Frequency

        for (auto& [filename, freq] : results) {
            double tf = 1.0 * freq / globalIndexer.getDocLen()[filename]; // Term Frequency
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
