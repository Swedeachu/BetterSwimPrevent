#include "ClickLog.h"
#include "MouseData.h"
#include "Decorations.h"
#include "Entry.h"

namespace InputListener
{

	// a click log object to the MouseData's log vector.
	// containing the values of if it was a left or right click, the debounce time between the last click, and if it was a blocked click or not
	void MouseData::LogClick(ClickType clickType, long long debounce, bool blocked)
	{
		// Create a ClickLog object with the provided parameters
		ClickLog log(clickType, debounce, blocked);
		// Add the ClickLog object to the vector
		clickLogs.push_back(log);
		// Update the click counts based on the click type and whether it was blocked
		if (clickType == ClickType::LeftClick)
		{
			if (blocked)
			{
				filteredLeftClicks++;
			}
			else
			{
				leftClicks++;
			}
		}
		else if (clickType == ClickType::RightClick)
		{
			if (blocked)
			{
				filteredRightClicks++;
			}
			else
			{
				rightClicks++;
			}
		}
		// update the console
		// Helpers::updateConsole(Entry::md);
	}

} // InputListener