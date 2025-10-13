#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Index.h"
#include "Search.h"

int main() {
    std::cout << "=== Mini Search Engine CLI ===\n";

    // Add your files here
    std::vector<std::string> files = {
        "testfile1.txt",
        "testfile2.txt"
    };

    globalIndexer.buildIndex(files);
    std::cout << "Index built successfully!\n";

    while (true) {
        std::cout << "\nEnter search query (or 'exit' to quit): ";
        std::string query;
        std::getline(std::cin, query);

        if (query == "exit") break;
        if (query.empty()) continue;

        // Get ranked results
        auto results = searchQuery(query);

        if (results.empty()) {
            std::cout << "No results found.\n";
        } else {
            std::cout << "\nSearch results (ranked):\n";

            for (size_t i = 0; i < results.size(); ++i) {
                const std::string& doc = results[i];
                int totalWords = globalIndexer.getDocLen()[doc];
                std::cout << i + 1 << ". " << doc 
                          << " | Total words: " << totalWords << "\n";

                // Optional: show frequency of each query word in this doc
                std::istringstream iss(query);
                std::string word;
                std::cout << "   Matches: ";
                while (iss >> word) {
                    auto postings = globalIndexer.searchWord(word);
                    int freq = postings.count(doc) ? postings[doc] : 0;
                    std::cout << word << "(" << freq << ") ";
                }
                std::cout << "\n";
            }
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}
