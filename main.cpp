#include "Index.h"
#include <iostream>
#include <vector>
#include <set>

int main() {
    Indexer indexer;

    // List of files to index
    std::vector<std::string> files = {"testfile1.txt"};

    // Build the index
    indexer.buildIndex(files);

    // Print the entire index
    std::cout << "Full Index:\n";
    indexer.printIndex();

    // Test searching for a word
    std::string searchWord;
    std::cout << "\nEnter a word to search: ";
    std::cin >> searchWord;

    std::set<std::string> results = indexer.searchWord(searchWord);
    if (!results.empty()) {
        std::cout << "Found in files: ";
        for (const auto& f : results) {
            std::cout << f << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "Word not found in any file.\n";
    }

    return 0;
}
