#include <Windows.h>
#include <iostream>
#include <string>
#include "ClickHandler.h"
#include "MouseData.h"
#include "Decorations.h"

namespace Entry
{
	InputListener::MouseData md;

	// we need a virtual terminal so we can do fancy things such as ASCII in the console
	void EnableVirtualTerminalProcessing()
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut == INVALID_HANDLE_VALUE)
		{
			return;
		}

		DWORD dwMode = 0;
		if (!GetConsoleMode(hOut, &dwMode))
		{
			return;
		}

		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(hOut, dwMode))
		{
			return;
		}
	}

} // Entry

// creates a console window and assert input device listening
int main()
{
	// enable a virtual console environment before we alloc the console
	Entry::EnableVirtualTerminalProcessing();
	// Allocate a console for the application
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);

	// Register a custom window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = InputListener::RawMouseInputReciever; // Window procedure for raw input messages
	wc.lpszClassName = L"RawInputWindow";
	RegisterClassEx(&wc);

	// Create a hidden window to receive raw input messages
	HWND hwnd = CreateWindow(L"RawInputWindow", L"", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

	// Register to receive raw mouse input
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = hwnd;
	RegisterRawInputDevices(&rid, 1, sizeof(rid));

	// Set the low-level mouse hook
	InputListener::hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, InputListener::MouseHookClickReciever, GetModuleHandle(NULL), 0);

	// update console so it isn't blank to begin with
	Helpers::updateConsole(Entry::md);

	// Main message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Remove the hook when the application exits
	UnhookWindowsHookEx(InputListener::hMouseHook);

	// Free the console before the application exits
	FreeConsole();

	return 0;
}
