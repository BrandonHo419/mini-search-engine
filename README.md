# mini-search-engine
A condensed search engine that allows for searching through a collection of text files using a command-line-interface (CLI) for key queries. 
Supports search features like  Term Frequency-Inverse Document Frequency (TF-IDF), prefix searches, basic stemming and more. 

The goal of this project is also aimed to show the use of various data structures such as Trie, Priority_Queue, Hashmaps.
Through this miniature search engine, it recieves a prompt, this prompt then gets scanned across files and gets listed if the document contains the phrase in the title, or in the contents.

To compile:

g++ main.cpp Search.cpp

Additional notes:
However, if additional files are wanted to be added, it would need to be included in the current main.cpp, but this can be expanded to a new system which holds a local database, or outright scans files within permitted directories.
