#ifndef WINDOWS_PROCESS_DATA_READER_H
#define WINDOWS_PROCESS_DATA_READER_H 1

#include <map>
#include <string>

#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>

#include "AbstractProcessDataReader.h"

#pragma comment(lib, "pdh.lib")

class WindowsProcessDataReader : public AbstractProcessDataReader
{
    public:
        WindowsProcessDataReader();
        WindowsProcessDataReader(pid_t pProcessID);
        ~WindowsProcessDataReader() override;
    
        bool updateData(ProcessData& pData) override;

    private:
        bool getMemoryData(HANDLE pProcess, ProcessData& pData);
        HANDLE getProcessHandle() const;
        void initPdhCounters();
        void initPdhQuery();
        HQUERY mPdhQuery;
        std::map<std::string, HCOUNTER> mPdhCounterMap;

};

#endif /*WINDOWS_PROCESS_DATA_READER_H*/
