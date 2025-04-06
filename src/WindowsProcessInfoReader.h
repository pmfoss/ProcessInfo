#ifndef WINDOWS_PROCESS_INFO_READER_H
#define WINDOWS_PROCESS_INFO_READER_H 1

#include <format>
#include <map>
#include <stdexcept>
#include <string>

#include <windows.h>

#include "AbstractProcessInfoReader.h"

class WindowsProcessInfoReader : public AbstractProcessInfoReader
{
    public:
        WindowsProcessInfoReader();
        WindowsProcessInfoReader(pid_t pProcessID);
    
        bool readData(ProcessInfo& pData) override;

    private:
        double calculateCPULoad(HANDLE pProcess);
        HANDLE getProcessHandle() const;
        bool readCommandLine(HANDLE pProcess, ProcessInfo& pData);
        bool readMemoryData(HANDLE pProcess, ProcessInfo& pData);
        bool readParentProcessID(HANDLE pProcess, ProcessInfo& pData);

        ULARGE_INTEGER mLastCPUTime;
        ULARGE_INTEGER mLastSysCPUTime;
        ULARGE_INTEGER mLastUserCPUTime;
        int mProcessorCount;
};

#endif /*WINDOWS_PROCESS_INFO_READER_H*/
