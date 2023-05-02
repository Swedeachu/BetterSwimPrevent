#ifndef CLICK_HANDLER_H
#define CLICK_HANDLER_H

#include <Windows.h>

namespace InputListener
{
	extern HHOOK hMouseHook;
	LRESULT CALLBACK MouseHookClickReciever(int nCode, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK RawMouseInputReciever(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
}

#endif // CLICK_HANDLER_H
