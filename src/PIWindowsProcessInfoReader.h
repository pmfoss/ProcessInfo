#ifndef PI_WINDOWS_PROCESS_INFO_READER_H
#define PI_WINDOWS_PROCESS_INFO_READER_H 1

#include <format>
#include <map>
#include <stdexcept>
#include <string>

#include <windows.h>

#include "PIAbstractProcessInfoReader.h"

namespace ProcessInfo
{
    class PIWindowsProcessInfoReader : public PIAbstractProcessInfoReader
    {
        public:
            PIWindowsProcessInfoReader();
            PIWindowsProcessInfoReader(int64_t pProcessID);
        
            bool readData(ProcessInfo::PIProcessInfo& pData, ReadMode pMode) override;
    
        private:
            double calculateCPULoad(HANDLE pProcess);
            HANDLE getProcessHandle() const;
            bool readCommandLine(HANDLE pProcess, PIProcessInfo& pData);
            bool readMemoryData(HANDLE pProcess, PIProcessInfo& pData);
            bool readParentProcessID(HANDLE pProcess, PIProcessInfo& pData);
    
            ULARGE_INTEGER mLastCPUTime;
            ULARGE_INTEGER mLastSysCPUTime;
            ULARGE_INTEGER mLastUserCPUTime;
            int mProcessorCount;
    };
	
	using PIProcessInfoReader = PIWindowsProcessInfoReader;
}
#endif /*PI_WINDOWS_PROCESS_INFO_READER_H*/
