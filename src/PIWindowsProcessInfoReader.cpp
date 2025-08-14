#include "PIWindowsProcessInfoReader.h"

#include <psapi.h>
#include <stringapiset.h>

#include <fstream>
#include "WindowsNtApi.h"

using namespace ProcessInfo;

/*constructor / destructor*/
PIWindowsProcessInfoReader::PIWindowsProcessInfoReader()
    : PIWindowsProcessInfoReader(GetCurrentProcessId())
{
}

PIWindowsProcessInfoReader::PIWindowsProcessInfoReader(int64_t pProcessID)
    : PIAbstractProcessInfoReader(pProcessID)
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
bool PIWindowsProcessInfoReader::readData(PIProcessInfo& pData, ReadMode pMode)
{
    HANDLE lProcess = getProcessHandle();
    bool lRetVal = false; 
    
    if(lProcess)
    {
        lRetVal = PIAbstractProcessInfoReader::readData(pData, pMode);

        if((pMode & ReadModeFlags::Commandline) == ReadModeFlags::Commandline)
        {
            lRetVal &= readCommandLine(lProcess, pData);
        }

        if((pMode & ReadModeFlags::CPULoad) == ReadModeFlags::CPULoad)
        {
            pData.mCPULoad = calculateCPULoad(lProcess);
        }

        if((pMode & ReadModeFlags::Memory) != 0)
        {
            lRetVal &= readMemoryData(lProcess, pData, pMode);
        }

        if((pMode & ReadModeFlags::ParentPID) == ReadModeFlags::ParentPID)
        {
            lRetVal &= readParentProcessID(lProcess, pData);
        }

        CloseHandle(lProcess);
    }

    return lRetVal;
}

/*private methods*/
double PIWindowsProcessInfoReader::calculateCPULoad(HANDLE pProcess)
{
    FILETIME lFTime, lFSysTime, lFUserTime;
    ULARGE_INTEGER lNowTime, lSysTime, lUserTime;

    GetSystemTimeAsFileTime(&lFTime);
    std::memcpy(&lNowTime, &lFTime, sizeof(FILETIME));

    GetProcessTimes(pProcess, &lFTime, &lFTime, &lFSysTime, &lFUserTime);
    std::memcpy(&lUserTime, &lFUserTime, sizeof(FILETIME));
    std::memcpy(&lSysTime, &lFSysTime, sizeof(FILETIME));

    double lRetVal = static_cast<double>(lSysTime.QuadPart - mLastSysCPUTime.QuadPart +
              (lUserTime.QuadPart - mLastUserCPUTime.QuadPart));
    lRetVal /= static_cast<double>(lNowTime.QuadPart - mLastCPUTime.QuadPart);
    lRetVal /= mProcessorCount;

    mLastCPUTime = lNowTime;
    mLastSysCPUTime = lSysTime;
    mLastUserCPUTime = lUserTime;

    return lRetVal * 100.0;
}

HANDLE PIWindowsProcessInfoReader::getProcessHandle() const
{
    if(mProcessID == GetCurrentProcessId())
    {
        return GetCurrentProcess();
    }
    
    return OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, mProcessID);
}

bool PIWindowsProcessInfoReader::readCommandLine(HANDLE pProcess, PIProcessInfo& pData) const
{
    pData.mCmdLine = "";

    ULONG lBufferSize = 0;

    lBufferSize = sizeof(UNICODE_STRING) + MAX_PATH;
    UNICODE_STRING* lCommandLineBuffer = static_cast<UNICODE_STRING*>(std::malloc(lBufferSize));

    NTSTATUS lStatus = NtQueryInformationProcess(pProcess, ProcessCommandLineInformation, lCommandLineBuffer, lBufferSize, &lBufferSize);

    if(lStatus != STATUS_INFO_LENGTH_MISMATCH)
    {
        std::free(lCommandLineBuffer);
        lCommandLineBuffer = static_cast<UNICODE_STRING*>(std::malloc(lBufferSize));
        lStatus = NtQueryInformationProcess(pProcess, ProcessCommandLineInformation, lCommandLineBuffer, lBufferSize, &lBufferSize);

        if(lStatus != STATUS_SUCESS)
        {
			std::free(lCommandLineBuffer);
            pData.mCmdLine = "";
            return false;
        }
    }
    else
    {
		std::free(lCommandLineBuffer);
        pData.mCmdLine = "";
        return false;
    }

    int lStrLen = WideCharToMultiByte(CP_UTF8, 0, lCommandLineBuffer->Buffer, lCommandLineBuffer->Length, nullptr, 0, nullptr, nullptr) + 1;
    char* lMbString = static_cast<char*>(std::calloc(lStrLen, sizeof(char)));
    WideCharToMultiByte(CP_UTF8, 0, lCommandLineBuffer->Buffer, -1, lMbString, lStrLen, nullptr, nullptr);

    pData.mCmdLine = lMbString;

    std::free(lMbString);
    std::free(lCommandLineBuffer);

    return true;
}

bool PIWindowsProcessInfoReader::readMemoryData(HANDLE pProcess, PIProcessInfo& pData, ReadMode pMode) const
{
	bool lRetVal = true;
	
    if((pMode & ReadModeFlags::WorkingMemory) == ReadModeFlags::WorkingMemory)
	{	
        PROCESS_MEMORY_COUNTERS_EX2 lProcMemCounters;
        if(GetProcessMemoryInfo(pProcess, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&lProcMemCounters), sizeof(lProcMemCounters)))
        {
            pData.mMemoryResident = lProcMemCounters.WorkingSetSize;
            pData.mMemorySwapped = lProcMemCounters.PagefileUsage;
        }
        else
        {
            lRetVal = false;
        }
	}
    
	if((pMode & ReadModeFlags::VirtualMemory) == ReadModeFlags::VirtualMemory)
	{
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
	}

    return lRetVal;
}

bool PIWindowsProcessInfoReader::readParentProcessID(HANDLE pProcess, PIProcessInfo& pData) const
{
    PROCESS_BASIC_INFORMATION lPbi;
    ULONG lReturnLength;

    NTSTATUS lStatus = NtQueryInformationProcess(pProcess, ProcessBasicInformation, &lPbi, sizeof(PROCESS_BASIC_INFORMATION), &lReturnLength);
    if(lStatus != STATUS_SUCESS) 
    {
        return false;
    }

    pData.mParentProcessID = static_cast<pid_t>(lPbi.InheritedFromUniqueProcessId);

    return true;
}
