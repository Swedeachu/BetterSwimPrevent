#include "Helpers/stdafx.h"

#include <Windows.h>
#include <filesystem>
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

namespace Detections
{
	// we use mutex so the console can't get spammed
	std::mutex coutMutex;

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

	// Get title text of a window
	std::wstring GetWindowTitle(HWND hwnd)
	{
		constexpr int kMaxWindowTitleLength = 1024;
		WCHAR title[kMaxWindowTitleLength];
		const int length = GetWindowText(hwnd, title, kMaxWindowTitleLength);
		return std::wstring(title, length);
	}

	// Scan's a process and its window for suspicous activity
	void ScanProcess(HWND hwnd, DWORD pid, const HANDLE& processHandle)
	{
		// Get the process name 
		std::wstring processName = GetProcessFilePath(processHandle);
		// Get the hash
		std::wstring hash = Hash::MD5Checksum(processName);
		// Get the window title
		std::wstring windowTitle = GetWindowTitle(hwnd);
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
			std::string procString = Helpers::convertWideStringToString(processName);
			std::string procName = GetFileName(procString);
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
		}
		// Close the handle to the process
		CloseHandle(processHandle);
	}

	// Scans all running processes in around 1 second via multi threading (pooling)
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
