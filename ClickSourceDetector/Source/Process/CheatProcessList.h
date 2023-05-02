// CheatProcessList.h
#ifndef CHEAT_PROCESS_LIST_H
#define CHEAT_PROCESS_LIST_H

#include "Helpers/stdafx.h"

#include "Helpers\StringBlockList.h"

namespace Detections
{

	std::string checkIllegalString(const std::string& str);

	extern std::vector<std::string> illegalStrings;

	extern Helpers::StringBlockList stringList;

} // Detections

#endif // CheatProcessList.h
