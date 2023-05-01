#include <Windows.h>
#include <iostream>
#include <string>
#include "ClickHandler.h"
#include "MouseData.h"
#include "Decorations.h"

namespace Entry
{
	InputListener::MouseData md;
}

// creates a console window and assert input device listening
int main()
{
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
