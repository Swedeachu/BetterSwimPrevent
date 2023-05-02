#ifndef MOUSE_DATA_H
#define MOUSE_DATA_H

#include <vector>
#include <string>
#include "Mouse/ClickLog.h"

namespace InputListener
{
	class MouseData
	{
	public:

		// Constructor
		MouseData() : leftClicks(0), rightClicks(0), filteredLeftClicks(0), filteredRightClicks(0) {}

		// Methods
		void LogClick(ClickType clickType, long long duration, bool blocked);

		// getter methods for some very basic statistics
		int getLeftClicks() const { return leftClicks; }
		int getRightClicks() const { return rightClicks; }
		int getFilteredLeftClicks() const { return filteredLeftClicks; }
		int getFilteredRightClicks() const { return filteredRightClicks; }
		int getTotalLeftClicks() const { return leftClicks + filteredLeftClicks; }
		int getTotalRightClicks() const { return rightClicks + filteredRightClicks; }

	private:
		int leftClicks;
		int rightClicks;
		int filteredLeftClicks;
		int filteredRightClicks;
		std::vector<InputListener::ClickLog> clickLogs;

	};
}

#endif // MOUSE_DATA_H
