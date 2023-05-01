// RawInputDataProcessor.h
#ifndef RAW_INPUT_DATA_PROCESSOR_H
#define RAW_INPUT_DATA_PROCESSOR_H

#include <windows.h>

namespace Detections
{
	void ScanMouseInputData(const RID_DEVICE_INFO& deviceInfo, const std::string& deviceNameString);
}

#endif // RawInputDataProcessor.h
