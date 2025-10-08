#include <iostream>
#include <string>
#include <vector>
#include "search.h" 

void displayResults(const std::vector<int>& results) {
    if (results.empty()) {
        std::cout << "No results found.\n";
        return;
    }

    std::cout << "Search results:\n";
    for (int docID : results) {
        std::cout << "- " << getDocumentTitle(docID) << " (ID: " << docID << ")\n";
    }
}

int main() {
    std::cout << "=== Mini Search Engine CLI ===\n";

    while (true) {
        std::cout << "\nEnter search query (or 'exit' to quit): ";
        std::string query;
        std::getline(std::cin, query);

        if (query == "exit") break;
        if (query.empty()) continue;

        std::vector<int> results = searchQuery(query);
        
        displayResults(results);
    }

    std::cout << "Goodbye!\n";
    return 0;
}
