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
      transform(token.begin(), token.end(), token.begin(),
                ::tolower);  // change all to lower
      tokens.push_back(token);
    }
    return tokens;
  }

  // using the two-pointer strat for intersects
  vector<pair<int, int>> intersectPostings(const vector<pair<int, int>>& a,
                                           const vector<pair<int, int>>& b) {
    vector<pair<int, int>> res;

    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
      if (a[i].first == b[j].first) {  // if equal
        res.emplace_back(a[i].first,
                         a[i].second + b[j].second);  // calculate freq
        i++;
        j++;
      } else if (a[i].first < b[j].first)  //
        i++;
      else
        j++;
    }
    return res;
  }

 public:
  vector<pair<int, int>> search(const string& query, const invertedIndex& index,
                                bool useAnd = true) {
    auto tokens = tokenize(query);
    if (tokens.empty()) return {};  // return an empty

    /*auto result = index.getPostings(tokens[0]);  // Start with first term's
    postings for (size_t i = 1; i < tokens.size(); ++i) { auto next =
    index.getPostings(tokens[i]); if (useAnd) { result =
    intersectPostings(result, next); } else { result = unionPostings(result,
    next);
        }
    }
    return result;*/
  }
};