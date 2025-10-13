#include <iostream>
#include <string>
#include <vector>
#include "Index.h"
#include "Search.h"

int main() {
    std::cout << "=== Mini Search Engine CLI ===\n";

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

        auto results = searchQuery(query);

        if (results.empty()) {
            std::cout << "No results found.\n";
        } else {
            std::cout << "Search results:\n";
            for (auto& doc : results)
                std::cout << "- " << doc << "\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}
