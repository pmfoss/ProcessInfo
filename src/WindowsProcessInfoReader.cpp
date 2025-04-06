#include "WindowsProcessInfoReader.h"

#include <psapi.h>
#include <stringapiset.h>

#include "WindowsNtApi.h"

/*constructor / destructor*/
WindowsProcessInfoReader::WindowsProcessInfoReader()
    : WindowsProcessInfoReader(GetCurrentProcessId())
{
}

WindowsProcessInfoReader::WindowsProcessInfoReader(pid_t pProcessID)
    : AbstractProcessInfoReader(pProcessID)
{
    HANDLE lProcess = getProcessHandle();

    if(lProcess)
    {
        calculateCPULoad(lProcess);
        CloseHandle(lProcess);
    }
    else
    {
        throw std::runtime_error(std::format("Process with ID \"{}\" can not be used", pProcessID));
    }

    SYSTEM_INFO lSysInfo;

    GetSystemInfo(&lSysInfo);

    mProcessorCount = lSysInfo.dwNumberOfProcessors;
}

/*public methods*/
bool WindowsProcessInfoReader::readData(ProcessInfo& pData)
{
    HANDLE lProcess = getProcessHandle();
    bool lRetval = false; 
    
    if(lProcess)
    {
        lRetval = AbstractProcessInfoReader::readData(pData);
        lRetval &= readMemoryData(lProcess, pData);
        lRetval &= readCommandLine(lProcess, pData);
        lRetval &= readParentProcessID(lProcess, pData);

        pData.mCPULoad = calculateCPULoad(lProcess);

        CloseHandle(lProcess);
    }

    return lRetval;
}

/*private methods*/
double WindowsProcessInfoReader::calculateCPULoad(HANDLE pProcess)
{
    double lRetval = 0.0;
    FILETIME lFTime, lFSysTime, lFUserTime;
    ULARGE_INTEGER lNowTime, lSysTime, lUserTime;

    GetSystemTimeAsFileTime(&lFTime);
    std::memcpy(&lNowTime, &lFTime, sizeof(FILETIME));

    GetProcessTimes(pProcess, &lFTime, &lFTime, &lFSysTime, &lFUserTime);
    std::memcpy(&lUserTime, &lFUserTime, sizeof(FILETIME));
    std::memcpy(&lSysTime, &lFSysTime, sizeof(FILETIME));

    lRetval = (lSysTime.QuadPart - mLastSysCPUTime.QuadPart) +
              (lUserTime.QuadPart - mLastUserCPUTime.QuadPart);
    lRetval /= (lNowTime.QuadPart - mLastCPUTime.QuadPart);
    lRetval /= mProcessorCount;

    mLastCPUTime = lNowTime;
    mLastSysCPUTime = lSysTime;
    mLastUserCPUTime = lUserTime;

    return lRetval * 100.0;
}

HANDLE WindowsProcessInfoReader::getProcessHandle() const
{
    if(mProcessID == GetCurrentProcessId())
    {
        return GetCurrentProcess();
    }
    
    return OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, mProcessID);
}

bool WindowsProcessInfoReader::readCommandLine(HANDLE pProcess, ProcessInfo& pData)
{
    pData.mCmdLine = "";

    UNICODE_STRING *lCommandLineBuffer;
    ULONG lBufferSize = 0;

    lBufferSize = sizeof(UNICODE_STRING) + MAX_PATH;
    lCommandLineBuffer = static_cast<UNICODE_STRING*>(std::malloc(lBufferSize));

    NTSTATUS lStatus = NtQueryInformationProcess(pProcess, ProcessCommandLineInformation, lCommandLineBuffer, lBufferSize, &lBufferSize);

    if(lStatus != STATUS_INFO_LENGTH_MISMATCH)
    {
        std::free(lCommandLineBuffer);
        lCommandLineBuffer = static_cast<UNICODE_STRING*>(std::malloc(lBufferSize));
        lStatus = NtQueryInformationProcess(pProcess, ProcessCommandLineInformation, lCommandLineBuffer, lBufferSize, &lBufferSize);

        if(lStatus != STATUS_SUCESS)
        {
            pData.mCmdLine = "";
            return false;
        }
    }
    else
    {
        pData.mCmdLine = "";
        return false;
    }

    int lStrLen = WideCharToMultiByte(CP_UTF8, 0, lCommandLineBuffer->Buffer, lCommandLineBuffer->Length, NULL, 0, NULL, NULL) + 1;
    char* lMbString = static_cast<char*>(std::calloc(lStrLen, sizeof(char)));
    WideCharToMultiByte(CP_UTF8, 0, lCommandLineBuffer->Buffer, -1, lMbString, lStrLen, NULL, NULL);

    pData.mCmdLine = lMbString;

    std::free(lMbString);
    std::free(lCommandLineBuffer);

    return true;
}

bool WindowsProcessInfoReader::readMemoryData(HANDLE pProcess, ProcessInfo& pData)
{
    PROCESS_MEMORY_COUNTERS_EX2 lProcMemCounters;

    if(GetProcessMemoryInfo(pProcess, (PROCESS_MEMORY_COUNTERS*)&lProcMemCounters, sizeof(lProcMemCounters)))
    {
        pData.mMemoryResident = lProcMemCounters.WorkingSetSize;
        pData.mMemorySwapped = lProcMemCounters.PagefileUsage;
    }
    else
    {
        return false;
    }
    
    MEMORY_BASIC_INFORMATION lProcMemBaseInfo;
    SIZE_T lProcAddress = 0;
    SIZE_T lProcVirtualSize = 0;

    while(VirtualQueryEx(pProcess, reinterpret_cast<LPCVOID>(lProcAddress), &lProcMemBaseInfo, sizeof(lProcMemBaseInfo)))
    {
        if(lProcMemBaseInfo.State == MEM_COMMIT or lProcMemBaseInfo.State == MEM_RESERVE)
        {
            lProcVirtualSize += lProcMemBaseInfo.RegionSize;
        }
        lProcAddress += lProcMemBaseInfo.RegionSize;
    }

    pData.mMemoryVirtual = lProcVirtualSize;

    return true;
}

bool WindowsProcessInfoReader::readParentProcessID(HANDLE pProcess, ProcessInfo& pData)
{
    PROCESS_BASIC_INFORMATION lPbi;
    ULONG lReturnLength;

    NTSTATUS lStatus = NtQueryInformationProcess(pProcess, ProcessBasicInformation, &lPbi, sizeof(PROCESS_BASIC_INFORMATION), &lReturnLength);
    if(lStatus != STATUS_SUCESS) 
    {
        return false;
    }

    pData.mParentProcessID = lPbi.InheritedFromUniqueProcessId;

    return true;
}
