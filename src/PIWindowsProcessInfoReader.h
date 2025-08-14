#ifndef PI_WINDOWS_PROCESS_INFO_READER_H
#define PI_WINDOWS_PROCESS_INFO_READER_H 1

#include <format>

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
            bool readCommandLine(HANDLE pProcess, PIProcessInfo& pData) const;
            bool readMemoryData(HANDLE pProcess, PIProcessInfo& pData, ReadMode pMode) const;
            bool readParentProcessID(HANDLE pProcess, PIProcessInfo& pData) const;
    
            ULARGE_INTEGER mLastCPUTime;
            ULARGE_INTEGER mLastSysCPUTime;
            ULARGE_INTEGER mLastUserCPUTime;
            uint32_t mProcessorCount;
    };
	
	using PIProcessInfoReader = PIWindowsProcessInfoReader;
}
#endif /*PI_WINDOWS_PROCESS_INFO_READER_H*/
