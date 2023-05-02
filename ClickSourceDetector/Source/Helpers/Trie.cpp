#include "Trie.h"
#include <algorithm>

namespace Helpers
{

	Trie::~Trie()
	{
		for (auto& entry : children)
		{
			delete entry.second;
		}
	}

	// Insert a string into the trie
	void Trie::insert(const std::string& str)
	{
		Trie* node = this;
		for (char c : str)
		{
			// If the character is not a child of the current node, add a new node for it
			if (node->children.count(c) == 0)
			{
				node->children[c] = new Trie();
			}
			// Move to the child node corresponding to the current character
			node = node->children[c];
		}
		// Set the leaf flag on the final node
		node->isLeaf = true;
	}

	// Check if a string is in the trie, and return the illegal substring if it is
	bool Trie::contains(const std::string& str, std::string& illegalStr) const
	{
		// Convert the string to lower case
		std::string lowerStr = str;
		std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
			[](unsigned char c) { return std::tolower(c); });

		const Trie* node = this;
		std::string curr_str;
		for (char c : lowerStr)
		{
			curr_str += c;
			// If the character is not a child of the current node, the string is not in the trie
			if (node->children.count(c) == 0)
			{
				return false;
			}
			// Move to the child node corresponding to the current character
			node = node->children.at(c);
			// If the current node is a leaf node, the string is in the trie, and the illegal substring is curr_str
			if (node->isLeaf)
			{
				illegalStr = curr_str;
				return true;
			}
		}
		// If the final node is a leaf node, the string is in the trie
		return node->isLeaf;
	}

} // Helpers