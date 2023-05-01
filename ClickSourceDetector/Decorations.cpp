#include <string>
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include "MouseData.h"

namespace Helpers
{

	// gets a boolean in string form so it can be printed as either "true" or "false" instead of a 1 or a 0
	std::string boolToString(bool boolean)
	{
		return boolean ? "true" : "false";
	}

	// prints colored text to the console window
	void printColoredText(const std::string& text, WORD color)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// Save the current cursor position
		CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
		GetConsoleScreenBufferInfo(hConsole, &bufferInfo);
		COORD originalCursorPosition = bufferInfo.dwCursorPosition;

		// Set the text color
		SetConsoleTextAttribute(hConsole, color);

		// Print the colored text
		std::cout << text;

		// Restore the original cursor position and text color
		SetConsoleCursorPosition(hConsole, originalCursorPosition);
		SetConsoleTextAttribute(hConsole, bufferInfo.wAttributes);
	}

	void updateConsole(const InputListener::MouseData& mouseData)
	{
		// first abuse the os to clear all lines
		std::cout << "\033[2J\033[1;1H";
		// Print the statistics
		std::cout << "Left clicks: " << mouseData.getLeftClicks() << std::endl;
		std::cout << "Right clicks: " << mouseData.getRightClicks() << std::endl;
		std::cout << "Blocked left clicks: " << mouseData.getFilteredLeftClicks() << std::endl;
		std::cout << "Blocked right clicks: " << mouseData.getFilteredRightClicks() << std::endl;
		std::cout << "Total left clicks: " << mouseData.getTotalLeftClicks() << std::endl;
		std::cout << "Total right clicks: " << mouseData.getTotalRightClicks() << std::endl;
	}

} // Helpers