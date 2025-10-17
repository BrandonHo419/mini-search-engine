# mini-search-engine
A condensed search engine that allows for searching through a collection of text files using a command-line-interface (CLI) for key queries. 
Supports search features like  Term Frequency-Inverse Document Frequency (TF-IDF), prefix searches, basic stemming and more. 

The goal of this project is also aimed to show the use of various data structures such as Trie, Priority_Queue, Hashmaps.
Through this miniature search engine, it recieves a prompt, this prompt then gets scanned across files and gets listed if the document contains the phrase in the title, or in the contents.

To compile:

g++ main.cpp Search.cpp

Additional notes:
However, if additional files are wanted to be added, it would need to be included in the current main.cpp, but this can be expanded to a new system which holds a local database, or outright scans files within permitted directories.

The engine supports a robust query syntax, including explicit Boolean operators (AND, OR, and NOT via -) for combining and filtering terms, as well as Prefix Searching (word*) which utilizes a Trie for fast lookup and suggestions. A key feature is the correct implementation of Phrase Searching ("quick brown fox"), which performs a positional check to ensure terms are adjacent and ordered, a functionality that was recently fixed alongside adding necessary header guards and const correctness to ensure code stability and successful compilation using the C++17 standard.
to use
compile and run
