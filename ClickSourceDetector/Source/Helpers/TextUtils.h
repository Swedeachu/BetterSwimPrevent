// TextUtils.h
#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

// a fun class to do goofy things like print colored text (completely unneeded but fun)

#include "stdafx.h"
#include <Windows.h>
#include "Mouse\MouseData.h"

namespace Helpers
{

	std::string boolToString(bool boolean);

	std::string convertWideStringToString(const std::wstring& wideStr);

	void printColoredText(const std::string& text, WORD color);

	void updateConsole(const InputListener::MouseData& mouseData);

} // Helpers

#endif // TextUtils.h
