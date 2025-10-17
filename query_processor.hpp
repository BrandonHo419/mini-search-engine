#include <algorithm>
#include <queue>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cmath>

#include "ExactSearcher.hpp"
#include "PrefixSearcher.hpp"
#include "Index.h"

using namespace std;

struct Result {
    string docID;
    double score;
};
bool operator<(const Result& a, const Result& b) {
    return a.score < b.score;
};

vector<pair<string, double>> processQuery(string& query, Indexer& index,
                                         const PrefixSearcher& prefixSearcher, int topK) {
    bool isPrefix = (query.back() == '*' && !(query.front() == '"' && query.back() == '"'));
    bool isPhrase = (query.front() == '"' && query.back() == '"');
    string cleanQuery = query;
    if (isPrefix) cleanQuery = query.substr(0, query.size() - 1);
    if (isPhrase) cleanQuery = query.substr(1, query.size() - 2);

    vector<string> tokens; // Full query tokens including operators
    vector<string> operators;
    vector<string> terms; // Terms only
    istringstream iss(query); // Use original query to correctly parse operators
    string token;
    while (iss >> token) {
        if (token == "AND" || token == "OR" || token == "-") {
            operators.push_back(token);
        } else {
            // The terms stored here are the unnormalized search words
            terms.push_back(token);
        }
    }

    vector<pair<string, int>> candidates;
    ExactSearcher exact;

    // --- Special Query Handling (Prefix, Phrase) ---
    if (isPrefix) {
        string prefix_term = query.substr(0, query.size() - 1);
        candidates = prefixSearcher.searchPrefix(prefix_term);
        // Note: For prefix/phrase, only the first term (the full query string) is used for IDF calculation later.
    } else if (isPhrase) {
        // The searchPhrase function expects the clean, unquoted phrase text
        candidates = exact.searchPhrase(cleanQuery, index);
    } 
    // --- Boolean/Standard Query Handling ---
    else {
        if (terms.empty()) return {};
        
        // Start with the first term's postings
        candidates = exact.search(terms[0], index, false); // useAnd=false for single word, just gets postings

        // Apply Boolean Logic
        for (size_t i = 1; i < terms.size(); ++i) {
            auto next = exact.search(terms[i], index, false);
            // Default to "AND" if no operator is provided between terms (implicit AND)
            string op = (i - 1 < operators.size()) ? operators[i - 1] : "AND"; 
            
            if (op == "AND") {
                candidates = exact.intersectPostings(candidates, next);
            } else if (op == "OR") {
                candidates = exact.unionPostings(candidates, next);
            } else if (op == "-") {
                vector<pair<string, int>> diff;
                
                // Keep documents in 'candidates' that are NOT in 'next'
                unordered_map<string, bool> next_docs;
                for (const auto& q : next) next_docs[q.first] = true;

                for (const auto& p : candidates) {
                    if (next_docs.find(p.first) == next_docs.end()) {
                        diff.push_back(p);
                    }
                }
                candidates = diff;
            }
        }
    }

    // --- Scoring (TF-IDF) ---
    unordered_map<string, double> scores;
    int N = index.getTotalDoc();
    
    // Determine the word for IDF calculation
    string idf_term;
    if (isPrefix || isPhrase) {
        // For prefix/phrase, use the single term itself (which will be normalized inside getDocFreq)
        idf_term = cleanQuery;
    } else if (!terms.empty()) {
        // For boolean/standard, just use the first term (simplification for multi-term IDF)
        idf_term = terms[0];
    } else {
        return {}; // Should be caught by terms.empty() check, but for safety
    }
    
    // Calculate a single, simplified IDF for the entire query
    // This is a simplification; a more robust system would calculate IDF per term
    // and sum the scores.
    int df = index.getDocFreq(idf_term);
    double idf = (df > 0) ? log(N / (df)) : 0.0;
    
    // Calculate total score for each candidate document
    for (auto& p : candidates) {
        string docID = p.first;
        int total_freq_in_doc = p.second; 
        
        // TF-IDF simplified scoring (using combined frequency)
        double tf = total_freq_in_doc / static_cast<double>(index.getDocLen().at(docID));
        scores[docID] += tf * idf;
    }

    // --- Ranking ---
    priority_queue<Result> heap;
    for (auto& s : scores) heap.push({s.first, s.second});
    vector<pair<string, double>> ranked;
    while (!heap.empty() && ranked.size() < static_cast<size_t>(topK)) {
        auto top = heap.top();
        heap.pop();
        ranked.emplace_back(top.docID, top.score);
    }

    return ranked;
}