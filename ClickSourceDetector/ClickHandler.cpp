#include <Windows.h>
#include <locale>
#include <codecvt>
#include <chrono>
#include <iostream>
#include "Decorations.h"
#include "Entry.h"
#include "RawInputDataProcessor.h"
#include "ClickHandler.h"

namespace InputListener
{

	// Logs and determines if a click should be blocked or not
	bool handleButtonClick(std::chrono::steady_clock::time_point& lastClickTime, InputListener::ClickType clickType, const std::chrono::steady_clock::time_point& now)
	{
		// Get time since last clicked in milliseconds
		auto debounce = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClickTime).count();
		// Block clicks under 50 ms
		bool blocked = debounce < 50;
		// Log the click
		Entry::md.LogClick(clickType, debounce, blocked);
		// Update last clicked time
		lastClickTime = now;
		// return if we need to block the click or not
		return blocked;
	}

	HHOOK hMouseHook;

	// Recieves click messages and blocks them if they are double clicks
	LRESULT CALLBACK MouseHookClickReciever(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == HC_ACTION)
		{
			// these time points work like pointers for when clicks were last registered
			static std::chrono::steady_clock::time_point lastLeftClickTime;
			static std::chrono::steady_clock::time_point lastRightClickTime;
			// Typical debounce prevention logic from here
			auto now = std::chrono::steady_clock::now();
			bool blocked = false;
			if (wParam == WM_LBUTTONDOWN)
			{
				blocked = handleButtonClick(lastLeftClickTime, InputListener::ClickType::LeftClick, now);
			}
			else if (wParam == WM_RBUTTONDOWN)
			{
				blocked = handleButtonClick(lastRightClickTime, InputListener::ClickType::RightClick, now);
			}
			// this prevents the input from going through further
			if (blocked)
			{
				return 1;
			}
		}
		return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
	}

	// Recieves raw device input for real time forensics such as determining if an input was sent through hardware or spoofed with cheats
	LRESULT CALLBACK RawMouseInputReciever(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_INPUT:
			{
				// set up input data storing
				UINT dwSize;
				GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
				LPBYTE lpb = new BYTE[dwSize];
				if (lpb == NULL)
				{
					return 0;
				}
				// now actually get the input data from the windows message
				GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
				RAWINPUT* raw = (RAWINPUT*)lpb;
				// if input came from a mouse
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					// only care about left and right clicks
					if (raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_1_DOWN || raw->data.mouse.usButtonFlags == RI_MOUSE_BUTTON_2_DOWN)
					{
						// Get the device handle
						HANDLE deviceHandle = raw->header.hDevice;
						// Use the device handle to get more information about the device
						RID_DEVICE_INFO deviceInfo;
						deviceInfo.cbSize = sizeof(RID_DEVICE_INFO);
						UINT size = sizeof(RID_DEVICE_INFO);
						GetRawInputDeviceInfo(deviceHandle, RIDI_DEVICEINFO, &deviceInfo, &size); // device info has all kinds of useful things to look into

						// Get device name
						TCHAR deviceName[256];
						UINT deviceNameSize = sizeof(deviceName);
						GetRawInputDeviceInfo(deviceHandle, RIDI_DEVICENAME, deviceName, &deviceNameSize);
						std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
						std::string deviceNameString = converter.to_bytes(deviceName);

						/* don't have a reason to get preparsed data, leaving as a comment though in case we ever do
						// Determine the size of the pre-parsed data
						UINT preParsedDataSize = 0;
						GetRawInputDeviceInfo(deviceHandle, RIDI_PREPARSEDDATA, NULL, &preParsedDataSize);
						// Allocate a buffer to hold the pre-parsed data
						BYTE* preParsedData = new BYTE[preParsedDataSize];
						// Retrieve the pre-parsed data
						GetRawInputDeviceInfo(deviceHandle, RIDI_PREPARSEDDATA, preParsedData, &preParsedDataSize);
						// Free the allocated buffer after
						delete[] preParsedData;
						*/

						// pass the data off to be scanned by our detections
						Detections::ScanMouseInputData(deviceInfo, deviceNameString);
					}
				}
				delete[] lpb; // remember to free
				break;
			}
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
		}
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

} // InputListener