#include "FileUtils.h"

#include <tchar.h>
#include <filesystem>
#include <fstream>
#include <Psapi.h>

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Version.lib")

namespace Helpers
{

	// most cheats are not signed in any way, we can know a process is suspicous if it's binary has none of this info
	FileSignerInfo GetFileSignerInfo(const std::wstring& filePath)
	{
		FileSignerInfo info;
		info.productName = L"none";
		info.productVersion = L"none";
		info.copyright = L"none";
		DWORD dummy;
		DWORD versionInfoSize = GetFileVersionInfoSize(filePath.c_str(), &dummy);
		// if there is no file version info we will return nothing
		if (versionInfoSize > 0)
		{
			std::vector<BYTE> buffer(versionInfoSize);
			if (GetFileVersionInfo(filePath.c_str(), 0, versionInfoSize, buffer.data()))
			{
				// parse it out from here
				struct LANGANDCODEPAGE
				{
					WORD wLanguage;
					WORD wCodePage;
				} *lpTranslate;
				UINT cbTranslate;
				// query it in
				VerQueryValue(buffer.data(), TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &cbTranslate);
				TCHAR subBlock[256];
				LPVOID lpBuffer;
				UINT size;
				// read it from here into the info struct
				for (UINT i = 0; i < (cbTranslate / sizeof(LANGANDCODEPAGE)); i++)
				{
					// Product Name
					swprintf_s(subBlock, ARRAYSIZE(subBlock), L"\\StringFileInfo\\%04x%04x\\ProductName", lpTranslate[i].wLanguage, lpTranslate[i].wCodePage);
					if (VerQueryValue(buffer.data(), subBlock, &lpBuffer, &size))
					{
						info.productName = (LPWSTR)lpBuffer;
					}
					// Product Version
					swprintf_s(subBlock, ARRAYSIZE(subBlock), L"\\StringFileInfo\\%04x%04x\\ProductVersion", lpTranslate[i].wLanguage, lpTranslate[i].wCodePage);
					if (VerQueryValue(buffer.data(), subBlock, &lpBuffer, &size))
					{
						info.productVersion = (LPWSTR)lpBuffer;
					}
					// Copyright
					swprintf_s(subBlock, ARRAYSIZE(subBlock), L"\\StringFileInfo\\%04x%04x\\LegalCopyright", lpTranslate[i].wLanguage, lpTranslate[i].wCodePage);
					if (VerQueryValue(buffer.data(), subBlock, &lpBuffer, &size))
					{
						info.copyright = (LPWSTR)lpBuffer;
					}
				}
			}
		}
		return info;
	}

	// Get the file path of a process
	std::wstring GetProcessFilePath(const HANDLE& processHandle)
	{
		WCHAR processName[MAX_PATH] = { 0 };
		if (GetModuleFileNameExW(processHandle, NULL, processName, MAX_PATH) == 0)
		{
			CloseHandle(processHandle);
			return L"Error";
		}
		return processName;
	}

	// "C:\\Program Files\\Dir\\program.exe" would return "program.exe"
	std::string GetFileName(const std::string& path)
	{
		std::filesystem::path filePath(path);
		return filePath.filename().string();
	}

	// returns in bytes
	unsigned long long GetFileSize(const std::wstring& filePath)
	{
		std::ifstream file(filePath, std::ifstream::ate | std::ifstream::binary);
		return file.tellg();
	}

} // Helpers
