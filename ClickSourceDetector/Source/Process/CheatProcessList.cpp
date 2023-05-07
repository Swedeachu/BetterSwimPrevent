#include "Helpers\StringBlockList.h"
#include "CheatProcessList.h"

namespace Detections
{

	// high preformance method to check if a string contains a string that is contained within a list (jesus)
	std::string checkIllegalString(const std::string& str)
	{
		std::string illegalString;
		if (stringList.contains(str, illegalString))
		{
			return illegalString;
		}
		return "0"; // clean
	}

	std::vector<std::string> illegalStrings = {
		"nitr0",
		"nitro",
		"speedhack",
		"speed hack",
		"horion",
		"lakeside",
		"ambrosial",
		"helix",
		"ascendancy",
		"cheat engine",
		"xodus",
		"lokiclient",
		"loki client",
		"moon client",
		"cheatengine",
		"zephyr",
		"zephyr client",
		"badman",
		"eternium",
		"packet client",
		"fadeaway",
		"rixcheats",
		"clicker",
		"horionclient",
		"cryptoclient",
		"swiftclient",
		"swift client",
		"auto clicker",
		"nitr0 lite",
		"nitro lite",
		"luconia",
		"strikeinjector",
		"fadedinjector",
		"bominjector",
		"crypto client",
		"bolt client",
		"ambrosial launcher",
		"alphalauncher injector",
		"brush client",
		"b1tch client",
		"eternium client",
		"flare client",
		"fate injector",
		"ghostly client",
		"helixpvp injector",
		"horion injector",
		"horion client",
		"hanzai client",
		"imeu client",
		"lakeside client",
		"moon client",
		"nitr0 client",
		"op autoclicker",
		"ng injector",
		"quadrix client",
		"surge client launcher",
		"ventile client",
		"cnd client",
		"cnd authentication",
		"cnd injector",
		"cndv1",
		"ascendancy client",
		"sp0tify client",
		"jarjar client",
		"saka client",
		"crypt client",
		"openprocess",
		"readprocess",
		"slinky",
		"bearcl1ck3r",
	};

	Helpers::StringBlockList stringList(illegalStrings);

} // Detections
