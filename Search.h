#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include <vector>
#include "Index.h"

extern Indexer globalIndexer;
std::vector<std::string> searchQuery(const std::string& query);

#endif
