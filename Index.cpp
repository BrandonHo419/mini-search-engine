#include "Index.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

std::string Indexer::normalizeWord(const std::string& word) const {
    std::string clean = "";
    for (size_t i = 0; i < word.size(); i++) {
        char c = word[i];
        // Only strip basic punctuation, keep letters
        if (!std::ispunct(c) || c == '-' || c == '\'') { 
            clean += std::tolower(c);
        }
    }
    return clean;
}

void Indexer::addDocument(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Error with file: " << filename << "\n";
        return;
    }

    std::string word, content;
    int total = 0, position = 0;
    while (file >> word) {
        std::string clean = normalizeWord(word);
        if (clean.empty()) continue;
        invertedIndex[clean][filename].first++;
        invertedIndex[clean][filename].second.push_back(position++);
        total++;
        content += word + " ";
    }
    docLen[filename] = total;
    docContent[filename] = content;
    file.close();
}

void Indexer::buildIndex(const std::vector<std::string>& files) {
    for (const auto& file : files) {
        addDocument(file);
    }
}

std::unordered_map<std::string, std::pair<int, std::vector<int>>> Indexer::searchWord(const std::string& word) const {
    std::string clean = normalizeWord(word);
    auto it = invertedIndex.find(clean);
    if (it != invertedIndex.end()) return it->second;
    return {};
}

int Indexer::getDocFreq(const std::string& word) const {
    std::string clean = normalizeWord(word);
    auto it = invertedIndex.find(clean);
    if (it != invertedIndex.end()) return it->second.size();
    return 0;
}

std::string Indexer::getSnippet(const std::string& filename, const std::string& query, int window) const {
    auto it = docContent.find(filename);
    if (it == docContent.end()) return "";
    std::vector<std::string> words;
    std::istringstream iss(it->second);
    std::string word;
    while (iss >> word) words.push_back(word);

    // --- Prepare Query Terms ---
    std::vector<std::string> queryTerms;
    std::istringstream qiss(query);
    while (qiss >> word) {
        std::string cleanWord = word;
        // Strip phrase quotes and prefix wildcard before normalization for snippet
        if (cleanWord.front() == '"' && cleanWord.back() == '"') cleanWord = cleanWord.substr(1, cleanWord.size() - 2);
        if (cleanWord.back() == '*') cleanWord.pop_back();

        // Tokenize and normalize
        std::istringstream twiss(cleanWord);
        std::string token;
        while (twiss >> token) {
            std::string normalized = normalizeWord(token);
            if (!normalized.empty()) {
                 queryTerms.push_back(normalized);
            }
        }
    }
    // Remove duplicates
    std::sort(queryTerms.begin(), queryTerms.end());
    queryTerms.erase(std::unique(queryTerms.begin(), queryTerms.end()), queryTerms.end());
    // --- End Query Term Preparation ---


    std::string snippet;
    std::vector<bool> included_positions(words.size(), false);
    bool found_match = false;

    for (size_t i = 0; i < words.size(); ++i) {
        std::string current_normalized_word = normalizeWord(words[i]);

        for (const auto& term : queryTerms) {
            if (current_normalized_word == term) {
                found_match = true;
                int start = std::max(0, static_cast<int>(i) - window);
                int end = std::min(static_cast<int>(words.size()), static_cast<int>(i) + window + 1);

                // Check if this segment is already part of a snippet
                bool already_included = false;
                for(int j = start; j < end; ++j) {
                    if (included_positions[j]) {
                        already_included = true;
                        break;
                    }
                }
                
                if (!already_included) {
                    for (int j = start; j < end; ++j) {
                        snippet += words[j] + " ";
                        included_positions[j] = true;
                    }
                    snippet += "... ";
                }
                break; // Move to the next word in the document
            }
        }
    }
    
    // If no match, return the start of the document
    return found_match ? snippet : (it->second.substr(0, std::min((size_t)50, it->second.size())) + "...");
}

void Indexer::printIndex() const {
    for (auto& [term, postings] : invertedIndex) {
        std::cout << term << ": ";
        for (auto& [doc, data] : postings) {
            std::cout << "(" << doc << ", " << data.first << ")";
        }
        std::cout << "\n";
    }
}