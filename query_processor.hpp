#include <algorithm>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "ExactSearcher.hpp"
#include "PrefixSearcher.hpp"
using namespace std;

struct Result {
  int docID;
  double score;
};
bool operator<(const Result& a, const Result& b) {
  return a.score < b.score;
};  // max-heap

vector<pair<int, double>> processQuery(string& query, Indexer& index,
                                       PrefixSearcher& prefixSearcher,
                                       int topK) {
  bool isPrefix = (query.back() == '*');
  string cleanQuery = isPrefix ? query.substr(0, query.size() - 1) : query;
  vector<pair<int, int>> candidates;
  if (isPrefix) {
    candidates = prefixSearcher.searchPrefix(cleanQuery);
  } else {
    ExactSearcher exact;
    candidates = exact.search(cleanQuery, index);  // Default AND
  }
  unordered_map<int, double> scores;
  int N = index.getTotalDoc();  // Assume method
  for (auto& p : candidates) {
    int docID = p.first, freq = p.second;
    double tf = freq / static_cast<double>(index.getTotalDoc());
    double idf = std::log(
        N / static_cast<double>(index.getDocFreq(  // ts pmo :wilted_rose:
                query)));                          // Approx for multi-term
    scores[docID] += tf * idf;                     // Accumulate for multi-term
  }

  // Rank with priority_queue (max-heap for top-K)
  priority_queue<Result> heap;
  for (auto& s : scores) heap.push({s.first, s.second});
  std::vector<std::pair<int, double>> ranked;
  while (!heap.empty() && ranked.size() < topK) {
    auto top = heap.top();
    heap.pop();
    ranked.emplace_back(top.docID, top.score);
  }

  return ranked;  // Output to CLI with doc names/snippets
}
