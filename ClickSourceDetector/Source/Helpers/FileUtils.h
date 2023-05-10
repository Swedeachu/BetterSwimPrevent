// FileUtils.h
#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "stdafx.h"

namespace Helpers
{

	struct FileSignerInfo
	{
		std::wstring productName;
		std::wstring productVersion;
		std::wstring copyright;
	};

	FileSignerInfo GetFileSignerInfo(const std::wstring& filePath);

	std::wstring GetProcessFilePath(const HANDLE& processHandle);

	std::string GetFileName(const std::string& path);

	unsigned long long GetFileSize(const std::wstring& filePath);

} // Helpers

#endif // FileUtils.h
