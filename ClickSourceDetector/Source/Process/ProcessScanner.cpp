#include "Helpers/stdafx.h"

#include <thread>
#include <mutex>
#include <future>

#include <Psapi.h>
#include <TlHelp32.h>

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Kernel32.lib")

#include "Hash\UtilMD5.h"
#include "CheatProcessList.h"
#include "Helpers\TextUtils.h"
#include "Helpers\FileUtils.h"

namespace Detections
{
	// we use mutex so the console can't get spammed
	std::mutex coutMutex;

	// Get title text of a window
	std::wstring GetWindowTitle(HWND hwnd)
	{
		constexpr int kMaxWindowTitleLength = 1024;
		WCHAR title[kMaxWindowTitleLength];
		const int length = GetWindowText(hwnd, title, kMaxWindowTitleLength);
		return std::wstring(title, length);
	}

	// Checks if the module is loaded from an unusual location
	// returning true means this is a suspicious DLL!
	bool IsIllegallyLoadedDLL(const MODULEENTRY32& moduleEntry)
	{
		std::wstring modulePath(moduleEntry.szExePath);
		// Convert the wide string to lower case
		std::transform(modulePath.begin(), modulePath.end(), modulePath.begin(), ::tolower);
		// Check if the module was not loaded from somewhere normal
		// (modulePath.find() == std::wstring::npos) checks if the specified substring is not found in the modulePath wstring object 
		if (modulePath.find(L"system32") == std::wstring::npos &&
			modulePath.find(L"syswow64") == std::wstring::npos &&
			modulePath.find(L"program files") == std::wstring::npos &&
			modulePath.find(L"program files (x86)") == std::wstring::npos)
		{
			return true; // this is an immediate red flag, all modules should be loaded from system directories and program files directories
		}
		return false;
	}

	// scan a singular module
	void ScanModule(const MODULEENTRY32W& moduleEntry)
	{
		// check if module is blatantly named something illegal
		std::string moduleName = Helpers::convertWideStringToString(moduleEntry.szModule);
		std::string nameResult = checkIllegalString(moduleName);
		if (nameResult != "0")
		{
			std::cout << "Illegal module found: " << moduleName << " | " << nameResult << std::endl;
		}
		// check if DLL was loaded from somewhere illegal
		if (IsIllegallyLoadedDLL(moduleEntry))
		{
			std::string modulePath = Helpers::convertWideStringToString(moduleEntry.szExePath);
			std::cout << "Illegally loaded module found: " << moduleName << " | " << modulePath << std::endl;
		}
		// check if DLL is unsigned 
		Helpers::FileSignerInfo info = Helpers::GetFileSignerInfo(moduleEntry.szExePath);
		if (info.copyright == L"none" || info.productName == L"none" || info.productVersion == L"none")
		{
			// however there are some unsigned dlls we can allow based on where they were loaded from
			std::wstring modulePath(moduleEntry.szExePath);
			std::transform(modulePath.begin(), modulePath.end(), modulePath.begin(), ::tolower);
			bool isInSystem32 = modulePath.find(L"c:\\windows\\system32") != std::wstring::npos;
			bool isInWindowsApps = modulePath.find(L"c:\\program files\\windowsapps") != std::wstring::npos;
			if (!isInSystem32 && !isInWindowsApps)
			{
				std::string modulePathStr = Helpers::convertWideStringToString(moduleEntry.szExePath);
				std::cout << "Illegal unsigned module detected: " << modulePathStr << std::endl;
				//std::wcout << L"   Product Name: " << info.productName << std::endl;
				//std::wcout << L"   Product Version: " << info.productVersion << std::endl;
				//std::wcout << L"   Copyright: " << info.copyright << std::endl;
			}
		}
		else
		{
			// check copy right info for illegal strings
			std::string copyrightResult = checkIllegalString(Helpers::convertWideStringToString(info.copyright));
			if (copyrightResult != "0")
			{
				std::wcout << L"Illegally signed module found: " << moduleEntry.szModule << " | " << info.copyright << std::endl;
			}
			// check product name for illegal strings
			std::string productNameResult = checkIllegalString(Helpers::convertWideStringToString(info.productName));
			if (productNameResult != "0")
			{
				std::wcout << L"Illegally signed module found: " << moduleEntry.szModule << " | " << info.productName << std::endl;
			}
		}
	}

	// checks every loaded dll on the process by calling ScanModule() on each one
	void ScanProcessModules(const DWORD& pid)
	{
		int moduleCount = 0;
		// Take a snapshot of the process's modules
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
		if (snapshot == INVALID_HANDLE_VALUE)
		{
			std::cerr << "Failed to create snapshot for process with PID " << pid << std::endl;
			return;
		}
		// Initialize a MODULEENTRY32 structure
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(MODULEENTRY32);
		// Get the first module in the process
		if (Module32First(snapshot, &moduleEntry))
		{
			do
			{
				moduleCount++;
				ScanModule(moduleEntry);
			}
			// Continue enumerating modules until Module32Next returns FALSE
			while (Module32Next(snapshot, &moduleEntry));
		}
		else
		{
			std::cerr << "Failed to get the first module for process with PID " << pid << std::endl;
		}
		// Close the snapshot handle
		CloseHandle(snapshot);
	}

	// Scan's a process and its window for suspicous activity
	void ScanProcess(const HWND& windowHandle, DWORD pid, const HANDLE& processHandle)
	{
		// Get the process name 
		std::wstring processFilePath = Helpers::GetProcessFilePath(processHandle);
		// Get the hash
		std::wstring hash = Hash::MD5Checksum(processFilePath);
		// Get the window title
		std::wstring windowTitle = GetWindowTitle(windowHandle);
		// Lock the std::wcout to avoid concurrent access by multiple threads
		{
			std::lock_guard<std::mutex> lock(coutMutex);
			// scan the hash value
			std::wstring md5Result = Hash::MatchMD5(hash); // this will be a "0" if it is not a known cheat
			std::string mdrs = Helpers::convertWideStringToString(md5Result);
			if (mdrs != "0")
			{
				std::cout << "Cheat Process hash detected: " << mdrs << std::endl;
			}
			// scan the process name
			std::string procString = Helpers::convertWideStringToString(processFilePath);
			std::string procName = Helpers::GetFileName(procString);
			std::string nameResult = checkIllegalString(procName);
			if (nameResult != "0")
			{
				std::cout << "Illegal Process Name found: " << nameResult << std::endl;
			}
			// scan the window title
			std::string windowResult = checkIllegalString(Helpers::convertWideStringToString(windowTitle));
			if (windowResult != "0")
			{
				std::cout << "Illegal Process Title found: " << windowResult << std::endl;
			}
			// check if process is not signed (TO DO: log all unsigned processes)
			Helpers::FileSignerInfo info = Helpers::GetFileSignerInfo(processFilePath);
			if (info.copyright == L"none" || info.productName == L"none" || info.productVersion == L"none")
			{
				// if a process is found and not signed we should string search it if is under 5 MB
				if (Helpers::GetFileSize(processFilePath) <= 5000000)
				{
					// ScanProcessStrings(processHandle);
				}
			}
			// scan Minecraft's modules
			if (procName == "Minecraft.Windows.exe" || windowTitle == L"Minecraft")
			{
				ScanProcessModules(pid);
			}
		}
		// Close the handle to the process
		CloseHandle(processHandle);
	}

	// Scans all running processes in around 1-2 seconds via multi threading (pooling)
	void CheckRunningProcesses()
	{
		// Start the timer
		auto start = std::chrono::high_resolution_clock::now();
		// Create an unordered_map to store scanned process IDs
		std::unordered_map<DWORD, bool> scannedProcesses;
		// Create a vector to store the futures
		std::vector<std::future<void>> futures;
		// Create a tuple to store the scannedProcesses and futures
		auto data = std::make_tuple(&scannedProcesses, &futures);
		// Enumerate all windows processes
		EnumWindows([](HWND hwnd, LPARAM lParam)
		{
			DWORD pid = -1;
		GetWindowThreadProcessId(hwnd, &pid);
		auto& data = *reinterpret_cast<std::tuple<std::unordered_map<DWORD, bool>*, std::vector<std::future<void>>*>*>(lParam);
		auto& scannedProcesses = *std::get<0>(data);
		auto& futures = *std::get<1>(data);
		// Skip over scanning ourselves, we will however still do special checks to make sure we aren't being tampered with
		if (pid == GetCurrentProcessId())
		{
			// TO DO : implement self security checks
			return TRUE;
		}
		// Check if the process has already been scanned
		if (scannedProcesses.find(pid) == scannedProcesses.end())
		{
			// Mark the process as scanned
			scannedProcesses[pid] = true;
			// Open the process
			HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
			if (processHandle)
			{
				// Add the future to the vector
				futures.push_back(std::async(std::launch::async, ScanProcess, hwnd, pid, processHandle));
			}
		}
		return TRUE;
		}, reinterpret_cast<LPARAM>(&data));
		// Wait for all futures to complete
		for (auto& future : futures)
		{
			future.wait();
		}
		// End the timer and print the total elapsed time
		auto end = std::chrono::high_resolution_clock::now();
		float elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count();
		std::cout << "Time to scan processes: " << std::fixed << std::setprecision(3) << elapsedSeconds << " seconds" << std::endl;
	}

} // Detections
