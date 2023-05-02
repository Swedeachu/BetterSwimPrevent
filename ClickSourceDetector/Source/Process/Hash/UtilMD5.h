// UtilMD5.h
#ifndef UTIL_MD5_H
#define UTIL_MD5_H

#include "Helpers/stdafx.h"

namespace Hash
{

	std::wstring MD5Checksum(const std::wstring& wideFilePath);

	std::wstring MatchMD5(const std::wstring& hash);

	extern std::unordered_map<std::wstring, std::wstring> mappedHashes;

}

#endif // UtilMD5.h
