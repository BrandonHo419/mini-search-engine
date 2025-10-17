#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Index.h"

using namespace std;

class ExactSearcher {
private:
  vector<string> tokenize(const string& query) {
    vector<string> tokens;
    istringstream ss(query);
    string token;

    while (ss >> token) {
      transform(token.begin(), token.end(), token.begin(), ::tolower);
      tokens.push_back(token);
    }
    return tokens;
  }

public:
  vector<pair<string, int>> intersectPostings(const vector<pair<string, int>>& a,
                                             const vector<pair<string, int>>& b) {
    vector<pair<string, int>> res;
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
      if (a[i].first == b[j].first) {
        res.emplace_back(a[i].first, a[i].second + b[j].second);
        i++;
        j++;
      } else if (a[i].first < b[j].first) {
        i++;
      } else {
        j++;
      }
    }
    return res;
  }

  vector<pair<string, int>> unionPostings(const vector<pair<string, int>>& a,
                                         const vector<pair<string, int>>& b) {
    vector<pair<string, int>> res;
    unordered_map<string, int> combined;
    for (const auto& p : a) combined[p.first] += p.second;
    for (const auto& p : b) combined[p.first] += p.second;
    for (const auto& [doc, freq] : combined) {
      res.emplace_back(doc, freq);
    }
    sort(res.begin(), res.end());
    return res;
  }

  vector<pair<string, int>> search(const string& query, const Indexer& index,
                                  bool useAnd = true) {
    auto tokens = tokenize(query);
    if (tokens.empty()) return {};

    vector<pair<string, int>> result;
    auto postings = index.searchWord(tokens[0]);
    for (const auto& [doc, data] : postings) {
      result.emplace_back(doc, data.first);
    }
    sort(result.begin(), result.end());

    for (size_t i = 1; i < tokens.size(); ++i) {
      vector<pair<string, int>> next;
      auto nextPostings = index.searchWord(tokens[i]);
      for (const auto& [doc, data] : nextPostings) {
        next.emplace_back(doc, data.first);
      }
      sort(next.begin(), next.end());
      if (useAnd) {
        result = intersectPostings(result, next);
      } else {
        result = unionPostings(result, next);
      }
    }
    return result;
  }

  // **FIXED: PHRASE SEARCH LOGIC**
  vector<pair<string, int>> searchPhrase(const string& query, const Indexer& index) {
    auto tokens = tokenize(query);
    if (tokens.empty()) return {};

    // Get initial postings for the first token
    auto currentPostings = index.searchWord(tokens[0]);
    vector<pair<string, int>> result;
    for (const auto& [doc, data] : currentPostings) {
      result.emplace_back(doc, data.first);
    }
    sort(result.begin(), result.end());

    // Iterate over subsequent tokens (starting from the second)
    for (size_t i = 1; i < tokens.size(); ++i) {
      auto nextPostings = index.searchWord(tokens[i]);
      
      vector<pair<string, int>> phraseResult;
      
      for (const auto& p : result) {
        auto it_next = nextPostings.find(p.first);
        auto it_current = currentPostings.find(p.first); // Postings for the PREVIOUS word (tokens[i-1])

        if (it_next != nextPostings.end() && it_current != currentPostings.end()) {
          const auto& pos_current = it_current->second.second; // Positions of tokens[i-1]
          const auto& pos_next = it_next->second.second;     // Positions of tokens[i]
          

          for (int pos : pos_current) {
            // Check for phrase match: pos of next word must be pos_current + 1
            if (find(pos_next.begin(), pos_next.end(), pos + 1) != pos_next.end()) {
              phraseResult.emplace_back(p.first, p.second);
              break;
            }
          }
        }
      }
      
      // Update result and the currentPostings for the next iteration
      result = phraseResult;
      currentPostings = nextPostings; // CRITICAL: Update to the postings of the word we just matched against
    }
    return result;
  }
};