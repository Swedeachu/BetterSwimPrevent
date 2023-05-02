// Trie.h
#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <string>
#include <vector>

namespace Helpers
{

	class Trie
	{
	public:

		// constructor
		Trie() : isLeaf(false) {}

		// deconstructor
		~Trie();

		// inserting
		void insert(const std::string& str);

		// contain checking
		bool contains(const std::string& str, std::string& illegalStr) const;

	private:
		bool isLeaf;
		std::string str;
		std::unordered_map<char, Trie*> children;
	};

} // Helpers

#endif // Trie.h