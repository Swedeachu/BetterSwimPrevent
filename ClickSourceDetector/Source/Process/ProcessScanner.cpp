#include "Helpers/stdafx.h"

#include <Windows.h>
#include <thread>
#include <mutex>

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

	// Function to scan a single process
	void ScanProcess(const HANDLE& processHandle)
	{
		// Get the process name and md5 hash
		std::wstring processName = GetProcessFilePath(processHandle);
		std::wstring hash = Hash::MD5Checksum(processName);
		// Lock the std::wcout to avoid concurrent access by multiple threads
		{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::wstring md5Result = Hash::MatchMD5(hash); // this will be a "0" if it is not a known cheat
			std::string mdrs = Helpers::convertWideStringToString(md5Result);
			if (mdrs != "0")
			{
				std::cout << "Cheat Process hash detected: " << mdrs << std::endl;
			}
			std::wcout << processName << std::endl;
			std::string nameResult = checkIllegalString(Helpers::convertWideStringToString(processName));
			if (nameResult != "0")
			{
				std::cout << "Illegal Process Name found: " << nameResult << std::endl;
			}
		}
		// Close the handle to the process
		CloseHandle(processHandle);
	}

	// this can blast through and scan all running main processes in around 3-4 seconds
	void CheckRunningProcesses()
	{
		// Start the timer
		auto start = std::chrono::high_resolution_clock::now();
		// Create a snapshot of all running processes
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot == INVALID_HANDLE_VALUE)
		{
			std::cerr << "Error: Unable to create process snapshot." << std::endl;
			return;
		}
		PROCESSENTRY32W processEntry;
		processEntry.dwSize = sizeof(PROCESSENTRY32W);
		// Get the number of available hardware threads
		unsigned int numThreads = std::thread::hardware_concurrency();
		// Iterate through all running processes
		std::vector<std::thread> threads;
		std::unordered_set<std::wstring> scannedProcesses;
		if (Process32FirstW(snapshot, &processEntry))
		{
			do
			{
				// Open the process
				HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processEntry.th32ProcessID);
				if (processHandle)
				{
					// Get the process name
					std::wstring processName = GetProcessFilePath(processHandle);
					// Check if the process has already been scanned
					if (scannedProcesses.find(processName) == scannedProcesses.end())
					{
						// Add the process name to the set of scanned processes
						scannedProcesses.insert(processName);
						// Launch a new thread to process this handle
						threads.emplace_back(ScanProcess, processHandle);
						// If the maximum number of threads is reached, wait for all threads to finish
						if (threads.size() >= numThreads)
						{
							for (std::thread& t : threads)
							{
								t.join();
							}
							threads.clear();
						}
					}
					else
					{
						// Close the handle since it's not going to be processed
						CloseHandle(processHandle);
					}
				}
			} while (Process32NextW(snapshot, &processEntry));
		}
		// Wait for any remaining threads to finish
		for (std::thread& t : threads)
		{
			t.join();
		}
		// Close the process snapshot handle
		CloseHandle(snapshot);
		// End the timer and print the total elapsed time
		auto end = std::chrono::high_resolution_clock::now();
		float elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count();
		std::cout << "Time to scan processes: " << std::fixed << std::setprecision(3) << elapsedSeconds << " seconds" << std::endl;
	}

} // Detections
