#include "LinuxProcessInfoReader.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <sys/sysinfo.h>
#include <sys/times.h>
#include <unistd.h>

/*constructor / destructor*/
LinuxProcessInfoReader::LinuxProcessInfoReader()
    : LinuxProcessInfoReader(getpid())
{
}

LinuxProcessInfoReader::LinuxProcessInfoReader(pid_t pProcessID)
    : AbstractProcessInfoReader(pProcessID)
{
    mProcessProcRoot = "/proc/" + std::to_string(mProcessID);

    if(not std::filesystem::exists(mProcessProcRoot))
    {
        throw std::filesystem::filesystem_error("Process directory does not exist", mProcessProcRoot, std::make_error_code(std::errc::no_such_file_or_directory));
    }

    /*52 == fields in /proc/${PID}/stat*/
    mStatFileData.resize(52);
}

/*public methods*/
bool LinuxProcessInfoReader::readData(ProcessInfo& pData)
{
    bool lRetval = readPIDStatusFile();
    
    lRetval &= readPIDStatFile();
    lRetval &= AbstractProcessInfoReader::readData(pData);
    
    if(lRetval)
    {
        pData.mMemoryVirtual = std::stoull(mStatusFileData["VmSize"]) * 1000;
        pData.mMemoryResident = std::stoull(mStatusFileData["VmRSS"]) * 1000;
        pData.mMemorySwapped = std::stoull(mStatusFileData["VmSwap"]) * 1000;
        
        pData.mParentProcessID = std::stoi(mStatusFileData["PPid"]);

        pData.mCPULoad = calculateCPULoad();
    }

    lRetval &= readCommandline(pData);

    return lRetval;
}

/*private methods*/
double LinuxProcessInfoReader::calculateCPULoad()
{
    clock_t lCPUTicks = times(nullptr);

    clock_t lSysCPUTicks = std::stol(mStatFileData[14]); /*stime in /proc/${PID}/stat*/
    clock_t lUserCPUTicks = std::stol(mStatFileData[13]); /*utime in /proc/${PID}/stat*/
    double lRetval = -1.0;

    if(lCPUTicks >= mLastCPUTicks && lSysCPUTicks >= mLastSysCPUTicks && lUserCPUTicks >= mLastUserCPUTicks)
    {
        lRetval = static_cast<double>((lSysCPUTicks - mLastSysCPUTicks) + (lUserCPUTicks - mLastUserCPUTicks));
        lRetval /= static_cast<double>(lCPUTicks - mLastCPUTicks);
        lRetval *= 100.0;
        
        mLastCPUTicks = lCPUTicks;
        mLastSysCPUTicks = lSysCPUTicks;
        mLastUserCPUTicks = lUserCPUTicks;
    }
    
    return lRetval;
}

bool LinuxProcessInfoReader::readCommandline(ProcessInfo& pData)
{
    std::filesystem::path lCmdLineFilePath = mProcessProcRoot / "cmdline";
    std::ifstream lInStream(lCmdLineFilePath);
    
    if(lInStream)
    {
        std::string lLine;
        std::getline(lInStream, lLine);

        pData.mCmdLine = lLine;
   
        lInStream.close();

        return true;
    }

    return false;
}

bool LinuxProcessInfoReader::readPIDStatusFile()
{
    std::filesystem::path lPIDStatusFilePath = mProcessProcRoot / "status";

    auto splitString = [](const std::string& pLine) -> std::pair<std::string, std::string>
    {
         size_t lColonPos = pLine.find(':');

         if(lColonPos != std::string::npos)
         {
            std::string lName = pLine.substr(0, lColonPos);
            std::string lValue = pLine.substr(lColonPos + 1);

            lValue.erase(0, lValue.find_first_not_of(" \t\n\r\f\v"));

            return {lName, lValue};
         }

         return {{},{}}; 
    };

    std::ifstream lInStream(lPIDStatusFilePath);

    if(lInStream)
    {
        while(not lInStream.eof())
        {
            std::string lLine;
            std::getline(lInStream, lLine);

            auto [lName, lValue] = splitString(lLine);
            mStatusFileData[lName] = lValue;
        }

        return true;
    }

    return false;
}

bool LinuxProcessInfoReader::readPIDStatFile()
{
    std::filesystem::path lPIDStatFilePath = mProcessProcRoot / "stat";

    std::ifstream lInStream(lPIDStatFilePath);

    if(lInStream)
    {
        std::string lLine;

        std::getline(lInStream, lLine);

        size_t lSpacePos = 0;
        size_t lNextSpacePos = lLine.find(' ');
        size_t lIdx = 0;

        while(lNextSpacePos != std::string::npos)
        {
            std::string lValue = lLine.substr(lSpacePos, lNextSpacePos - lSpacePos);
            mStatFileData[lIdx] = lValue;

            lSpacePos = lNextSpacePos + 1;
            lNextSpacePos = lLine.find(' ', lSpacePos);
            lIdx++;
        }

        std::string lValue = lLine.substr(lSpacePos);
        mStatFileData[lIdx] = lValue;

        return true;
    }

    return false;
}