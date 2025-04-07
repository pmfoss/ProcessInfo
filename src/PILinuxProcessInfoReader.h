#ifndef PI_LINUX_PROCESS_INFO_READER_H
#define PI_LINUX_PROCESS_INFO_READER_H 1

#include <filesystem>
#include <map>
#include <vector>

#include "PIAbstractProcessInfoReader.h"

namespace ProcessInfo
{
    class PILinuxProcessInfoReader : public PIAbstractProcessInfoReader
    {
         public:
             PILinuxProcessInfoReader();
             PILinuxProcessInfoReader(pid_t pProcessID);
             
             bool readData(PIProcessInfo& pData) override;
    
         private:
             double calculateCPULoad();
             clock_t calculateUpTime();
             bool readCommandline(PIProcessInfo& pData);
             bool readPIDStatusFile();
             bool readPIDStatFile();
    
             clock_t mLastCPUTicks = 0;
             clock_t mLastSysCPUTicks = 0;
             clock_t mLastUserCPUTicks = 0;
             std::filesystem::path mProcessProcRoot; 
             std::vector<std::string> mStatFileData;
             std::map<std::string, std::string> mStatusFileData;
    };
	
	using PIProcessInfoReader = PILinuxProcessInfoReader;
}

#endif /*PI_LINUX_PROCESS_INFO_READER*/