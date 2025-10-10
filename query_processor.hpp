#include <algorithm>
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