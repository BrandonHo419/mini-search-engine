#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class invertedIndex;

class ExactSearcher {
 private:
 public:
  vector<string> tokenize(const string& query) {
    vector<string> tokens;
    istringstream ss(query);
    string token;

    while (ss >> token) {
    }
  }
  vector<pair<int, int>> search(const string& query, const invertedIndex& index,
                                bool useAnd = true) {}
};