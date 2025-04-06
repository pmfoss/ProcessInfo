#ifndef ABSTRACT_PROCESS_INFO_READER_H
#define ABSTRACT_PROCESS_INFO_READER_H 1

#include "ProcessInfo.h"

class AbstractProcessInfoReader
{
    public:
        AbstractProcessInfoReader() = default;
        constexpr AbstractProcessInfoReader(pid_t pProcessID);
        virtual ~AbstractProcessInfoReader() {};

        constexpr pid_t getProcessID() const; 

        virtual bool readData(ProcessInfo& pData);

    protected:
        int32_t mProcessID = -1;
};

/*constructor*/
constexpr AbstractProcessInfoReader::AbstractProcessInfoReader(pid_t pProcessID)
    : mProcessID(pProcessID)
{
}

/*public methods*/
constexpr pid_t AbstractProcessInfoReader::getProcessID() const
{
    return mProcessID;
}


#endif /*ABSTRACT_PROCESS_DATA_READER_H*/
