#include <windows.h>
#include <iostream>
#include <regex>
#include <type_traits>
#include <cmath>
#include "Decorations.h"
#include "Entry.h"

namespace Detections
{

	// Valid device names do not contain unicode characters outside the ASCII range
	bool ContainsUnicodeCharacters(const std::string& str)
	{
		// match any character outside the ASCII range
		std::regex nonAsciiPattern(R"([^\x00-\x7F])");
		// return bool for if the string contains any non-ASCII characters
		return std::regex_search(str, nonAsciiPattern);
	}

	// Count the digits of any numeric data type (DWORD, USHORT, int, float, double)
	template <typename T>
	int GetDigits(T value)
	{
		static_assert(std::is_arithmetic<T>::value, "Type must be numeric");
		// Handle negative values for signed types
		if constexpr (std::is_signed<T>::value)
		{
			if (value < 0)
			{
				value = -value;
			}
		}
		// For floating-point types, consider only the whole part
		if constexpr (std::is_floating_point<T>::value)
		{
			value = std::floor(value);
		}
		int count = 0;
		do
		{
			count++;
			value /= 10;
		} while (value > 0);
		return count;
	}

	// Check stats from mouse to make sure they are non negative or huge
	bool IsValidMouseInfo(const RID_DEVICE_INFO& deviceInfo)
	{
		DWORD id = deviceInfo.mouse.dwId;
		int numButtons = deviceInfo.mouse.dwNumberOfButtons;
		DWORD rate = deviceInfo.mouse.dwSampleRate;
		// check none of the values are negative or impossible (such as number of buttons being less than or equal to 0)
		if (id < 0 || numButtons <= 0 || rate < 0)
		{
			return false;
		}
		// check the digits of each value
		int idDigits = GetDigits(id);
		int buttonDigits = GetDigits(numButtons);
		int rateDigits = GetDigits(numButtons);
		// a real mouse would never have this many digits
		if (idDigits >= 5 || buttonDigits >= 3 || rateDigits >= 5)
		{
			return false;
		}
		// if code reaches here then the mouse information is probably not spoofed
		return true;
	}

	// Checks if a mouses HID values are too large to be valid
	bool IsValidHID(const RID_DEVICE_INFO& deviceInfo)
	{
		DWORD dwVendorId = deviceInfo.hid.dwVendorId;
		DWORD dwProductId = deviceInfo.hid.dwProductId;
		DWORD dwVersionNumber = deviceInfo.hid.dwVersionNumber;
		USHORT usUsagePage = deviceInfo.hid.usUsagePage;
		USHORT usUsage = deviceInfo.hid.usUsage;

		// Check if any of the values have 5 or more digits
		if (GetDigits(dwVendorId) >= 5 ||
			GetDigits(dwProductId) >= 5 ||
			GetDigits(dwVersionNumber) >= 5 ||
			GetDigits(usUsagePage) >= 5 ||
			GetDigits(usUsage) >= 5)
		{
			return false;
		}

		return true;
	}

	// Checks the mouse's device info if it is from a spoofed source or not
	void ScanMouseInputData(const RID_DEVICE_INFO& deviceInfo, const std::string& deviceNameString)
	{
		// just for now i update the console here
		Helpers::updateConsole(Entry::md);
		// Check mouse name
		bool realMouseName = !ContainsUnicodeCharacters(deviceNameString);
		std::string realMouseString = Helpers::boolToString(realMouseName);
		std::cout << "Valid Name: " << realMouseString << std::endl;

		// Check mouse info
		bool realMouseInfo = IsValidMouseInfo(deviceInfo);
		std::string realMouseInfoString = Helpers::boolToString(realMouseInfo);
		std::cout << "Valid Info: " << realMouseInfoString << std::endl;

		// Check HID information
		bool realMouseHID = IsValidHID(deviceInfo);
		std::string realMouseHIDString = Helpers::boolToString(realMouseHID);
		std::cout << "HID Info: " << realMouseHIDString << std::endl;
	}

} // Detections
