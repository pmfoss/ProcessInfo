#ifndef LINUX_PROCESS_DATA_READER_H
#define LINUX_PROCESS_DATA_READER_H 1

#include <filesystem>
#include <map>
#include <vector>

#include "AbstractProcessDataReader.h"

class LinuxProcessDataReader : public AbstractProcessDataReader
{
     public:
         LinuxProcessDataReader();
         LinuxProcessDataReader(pid_t pProcessID);
         
         bool updateData(ProcessData& pData) override;

     private:
         double calculateCPULoad();
         clock_t calculateUpTime();
         bool readCommandline(ProcessData& pData);
         bool readPIDStatusFile();
         bool readPIDStatFile();

         clock_t mLastCPUTicks = 0;
         clock_t mLastSysCPUTicks = 0;
         clock_t mLastUserCPUTicks = 0;
         std::filesystem::path mProcessProcRoot; 
         std::vector<std::string> mStatFileData;
         std::map<std::string, std::string> mStatusFileData;
};

#endif /*LINUX_PROCESS_DATA_READER*/