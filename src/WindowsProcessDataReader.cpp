#include "WindowsProcessDataReader.h"

#include <psapi.h>
#include <iostream>

#include "PdhException.h"

/*constructor / destructor*/
WindowsProcessDataReader::WindowsProcessDataReader()
    : WindowsProcessDataReader(GetCurrentProcessId())
{
}

WindowsProcessDataReader::WindowsProcessDataReader(pid_t pProcessID)
    : AbstractProcessDataReader(pProcessID)
{
    initPdhCounters();
    initPdhQuery();
}

WindowsProcessDataReader::~WindowsProcessDataReader()
{
    PdhCloseQuery(mPdhQuery);
}

/*public methods*/
bool WindowsProcessDataReader::updateData(ProcessData& pData)
{
    HANDLE lProcess = getProcessHandle();
    bool lRetval = false; 
    
    if(lProcess != NULL)
    {
        lRetval = getMemoryData(lProcess, pData);
    }

    PDH_STATUS lStatus = PdhCollectQueryData(mPdhQuery);
    
    if(lStatus != ERROR_SUCCESS)
    {
        std::cout << "query failed\n";
        return false;
    }

    return lRetval;
}

/*private methods*/
bool WindowsProcessDataReader::getMemoryData(HANDLE pProcess, ProcessData& pData)
{
    PROCESS_MEMORY_COUNTERS_EX2 lProcMemCounters;

    if(GetProcessMemoryInfo(pProcess, (PROCESS_MEMORY_COUNTERS*)&lProcMemCounters, sizeof(lProcMemCounters)))
    {
        pData.mMemoryResident = lProcMemCounters.WorkingSetSize;
        pData.mMemorySwapped = lProcMemCounters.PagefileUsage;
    }
    
    MEMORY_BASIC_INFORMATION lProcMemBaseInfo;
    
    if(VirtualQueryEx(pProcess, nullptr, &lProcMemBaseInfo, sizeof(lProcMemBaseInfo)))
    {
        pData.mMemoryVirtual = lProcMemBaseInfo.RegionSize;
        return true;
    }

    return false;
}

HANDLE WindowsProcessDataReader::getProcessHandle() const
{
    if(mProcessID == GetCurrentProcessId())
    {
        return GetCurrentProcess();
    }
    
    return OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, mProcessID);
}

void WindowsProcessDataReader::initPdhCounters()
{
    mPdhCounterMap.emplace("Virtual Bytes", HCOUNTER());
    mPdhCounterMap.emplace("Working Set", HCOUNTER());
    mPdhCounterMap.emplace("Page File Bytes", HCOUNTER());
    mPdhCounterMap.emplace("% Processor Time", HCOUNTER());
}

void WindowsProcessDataReader::initPdhQuery()
{
    std::string lCounterBasePath = "\\Process(" + std::to_string(mProcessID) + ")\\";

    PDH_STATUS lStatus = PdhOpenQuery(nullptr, 0, &mPdhQuery);
    
    if(lStatus != ERROR_SUCCESS)
    {
        throw PdhException(lStatus);
    }

    for(auto [lName, lCounter] : mPdhCounterMap)
    {
        std::string lCounterPath = lCounterBasePath + lName;

        lStatus = PdhAddEnglishCounter(mPdhQuery, lCounterPath.c_str(), 0, &lCounter);

        if(lStatus != ERROR_SUCCESS)
        {
            throw PdhException(lStatus);
        }
    }
}
