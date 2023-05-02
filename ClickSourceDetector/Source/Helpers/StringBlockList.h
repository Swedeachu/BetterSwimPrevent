// StringBlockList.h
#ifndef STRING_BLOCK_LIST_H
#define STRING_BLOCK_LIST_H

#include "Helpers/stdafx.h"
#include "Trie.h"

namespace Helpers
{

	class StringBlockList
	{

	public:
		
		// constructor
		StringBlockList(const std::vector<std::string>& strings);

		// check if contains
		bool contains(const std::string& str, std::string& illegalStr) const;

	private:
		Trie trie;

	};

} // Helpers

#endif // StringBlockList.h