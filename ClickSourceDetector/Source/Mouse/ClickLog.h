#ifndef CLICK_LOG_H
#define CLICK_LOG_H

namespace InputListener
{
	enum class ClickType
	{
		LeftClick,
		RightClick
	};

	class ClickLog
	{
	public:

		// Constructor is all we really need for this, as this class is just a compact data holder for our ClickLogs vector in ClickData
		ClickLog(ClickType clickType, long long debounce, bool blocked)
			: clickType(clickType), debounce(debounce), blocked(blocked)
		{}

		// Member variables
		ClickType clickType; // left or right
		long long debounce; // time between the last click registered (50 ms for example)
		bool blocked; // if it was blocked or not (under 50 ms debounce time)
	};
}

#endif // CLICK_LOG_H
