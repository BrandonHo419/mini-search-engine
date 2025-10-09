#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
constexpr int ALPHABET_SIZE = 26;

int charToIndex(char c) { return c - 'a'; }

struct TrieNode {
  TrieNode* children[ALPHABET_SIZE];
  bool isEndOfWord;
  vector<pair<int, int>> postings;  // store (docID, freq)

  TrieNode() : isEndOfWord(false) {
    fill(children, children + ALPHABET_SIZE, nullptr);
  }

  ~TrieNode() {};
};