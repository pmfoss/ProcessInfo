#ifndef ABSTRACT_PROCESS_DATA_READER_H
#define ABSTRACT_PROCESS_DATA_READER_H 1

#include "ProcessData.h"

class AbstractProcessDataReader
{
    public:
        AbstractProcessDataReader() = default;
        constexpr AbstractProcessDataReader(pid_t pProcessID);
        virtual ~AbstractProcessDataReader() {};

        constexpr pid_t getProcessID() const; 

        virtual bool updateData(ProcessData& pData);

    protected:
        int32_t mProcessID = -1;
};

/*constructor*/
constexpr AbstractProcessDataReader::AbstractProcessDataReader(pid_t pProcessID)
    : mProcessID(pProcessID)
{
}

/*public methods*/
constexpr pid_t AbstractProcessDataReader::getProcessID() const
{
    return mProcessID;
}


#endif /*ABSTRACT_PROCESS_DATA_READER_H*/
