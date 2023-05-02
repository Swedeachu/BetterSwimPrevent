#include "StringBlockList.h"

namespace Helpers
{

	StringBlockList::StringBlockList(const std::vector<std::string>& strings)
	{
		for (const std::string& str : strings)
		{
			trie.insert(str);
		}
	}

	bool StringBlockList::contains(const std::string& str, std::string& illegalStr) const
	{
		return trie.contains(str, illegalStr);
	}

} // Helpers
