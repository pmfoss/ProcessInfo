#ifndef LINUX_PROCESS_INFO_READER_H
#define LINUX_PROCESS_INFO_READER_H 1

#include <filesystem>
#include <map>
#include <vector>

#include "AbstractProcessInfoReader.h"

class LinuxProcessInfoReader : public AbstractProcessInfoReader
{
     public:
         LinuxProcessInfoReader();
         LinuxProcessInfoReader(pid_t pProcessID);
         
         bool readData(ProcessInfo& pData) override;

     private:
         double calculateCPULoad();
         clock_t calculateUpTime();
         bool readCommandline(ProcessInfo& pData);
         bool readPIDStatusFile();
         bool readPIDStatFile();

         clock_t mLastCPUTicks = 0;
         clock_t mLastSysCPUTicks = 0;
         clock_t mLastUserCPUTicks = 0;
         std::filesystem::path mProcessProcRoot; 
         std::vector<std::string> mStatFileData;
         std::map<std::string, std::string> mStatusFileData;
};

#endif /*LINUX_PROCESS_INFO_READER*/