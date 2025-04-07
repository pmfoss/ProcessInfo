#ifndef PI_ABSTRACT_PROCESS_INFO_READER_H
#define PI_ABSTRACT_PROCESS_INFO_READER_H 1

#include "PIProcessInfo.h"

namespace ProcessInfo
{
    class PIAbstractProcessInfoReader
    {
        public:
            PIAbstractProcessInfoReader() = default;
            constexpr PIAbstractProcessInfoReader(pid_t pProcessID);
            virtual ~PIAbstractProcessInfoReader() {};
    
            constexpr pid_t getProcessID() const; 
    
            virtual bool readData(PIProcessInfo& pData);
    
        protected:
            int32_t mProcessID = -1;
    };
    
    /*constructor*/
    constexpr PIAbstractProcessInfoReader::PIAbstractProcessInfoReader(pid_t pProcessID)
        : mProcessID(pProcessID)
    {
    }
    
    /*public methods*/
    constexpr pid_t PIAbstractProcessInfoReader::getProcessID() const
    {
        return mProcessID;
    }
}

#endif /*PI_ABSTRACT_PROCESS_DATA_READER_H*/
