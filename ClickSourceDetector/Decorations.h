#ifndef DECORATIONS_H
#define DECORATIONS_H

// a fun class to do goofy things like print colored text (completely unneeded but fun)

#include <string>
#include <Windows.h>
#include "MouseData.h"

namespace Helpers
{
	std::string boolToString(bool boolean);
	void printColoredText(const std::string& text, WORD color);
	void updateConsole(const InputListener::MouseData& mouseData);
}

#endif // DECORATIONS_H
