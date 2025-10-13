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

class PrefixSearcher {
 private:
  TrieNode* root;

  void suggestionsRec(TrieNode* node, string currPrefix,
                      vector<string>& suggestions) {
    if (node->isEndOfWord) suggestions.push_back(currPrefix);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
      if (node->children[i])
        suggestionsRec(node->children[i], currPrefix + char('a' + i),
                       suggestions);
    }
  };

  void collectPostingsRec(TrieNode* node, vector<pair<int, int>>& postings) {
    if (node->isEndOfWord)
      postings.insert(postings.end(), node->postings.begin(),
                      node->postings.end());
    for (int i = 0; i < ALPHABET_SIZE; i++) {
      if (node->children[i]) collectPostingsRec(node->children[i], postings);
    }
  }

  bool isLastNode(TrieNode* node) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
      if (node->children[i]) return false;
      return true;
    }
  }

 public:
  PrefixSearcher() { root = new TrieNode; }
  PrefixSearcher() { /*use a recursive delete*/ }

  void insert(const string& key, const pair<int, int>& posting) {
    TrieNode* pCrawl = root;
    for (char c : key) {
      int index = charToIndex(c);
      if (!pCrawl->children[index]) pCrawl->children[index] = new TrieNode();
      pCrawl = pCrawl->children[index];
    }
    pCrawl->isEndOfWord = true;
    pCrawl->postings.push_back(posting);  // acumulate all the postings
  }

  vector<string> getSuggestions(const string& prefix) {
    TrieNode* pCrawl = root;
    for (char c : prefix) {
      int index = charToIndex(c);
      if (!pCrawl->children[index]) return {};  // no prefix
      pCrawl = pCrawl->children[index];
    }
    vector<string> suggestions;
    if (isLastNode(pCrawl)) {
      if (pCrawl->isEndOfWord) suggestions.push_back(prefix);
      return suggestions;  // or -1 flag as in source
    }
    suggestionsRec(pCrawl, prefix, suggestions);
    return suggestions;
  }

  vector<pair<int, int>> searchPrefix(
      string& prefix) {  // merge postings from matching terms
    // this will be similar but itll collect the merged postings
    TrieNode* pCrawl = root;

    for (char c : prefix) {
      int index = charToIndex(c);
      if (!pCrawl->children[index]) return {};  // No prefix
      pCrawl = pCrawl->children[index];
    }
    vector<pair<int, int>> postings;
    collectPostingsRec(pCrawl, postings);
    // sort and unique after

    return postings;
  }
};