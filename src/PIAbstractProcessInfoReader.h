#ifndef PI_ABSTRACT_PROCESS_INFO_READER_H
#define PI_ABSTRACT_PROCESS_INFO_READER_H 1

#include "PIProcessInfo.h"

namespace ProcessInfo
{
    class PIAbstractProcessInfoReader
    {
        public:
            struct ReadModeFlags
            {
                static constexpr uint8_t Commandline = 1;
                static constexpr uint8_t CPULoad = 2;
                static constexpr uint8_t ParentPID = 4;
				static constexpr uint8_t VirtualMemory = 8;
				static constexpr uint8_t WorkingMemory = 16;
				static constexpr uint8_t Memory = WorkingMemory | VirtualMemory;
                static constexpr uint8_t All = Commandline | CPULoad | Memory | ParentPID;
            };

            using ReadMode = uint8_t;

            PIAbstractProcessInfoReader() = default;
            constexpr PIAbstractProcessInfoReader(int64_t pProcessID);
            virtual ~PIAbstractProcessInfoReader() = default;
    
            constexpr int64_t getProcessID() const; 

            bool readAllData(PIProcessInfo& pData);
            virtual bool readData(PIProcessInfo& pData, ReadMode pMode);
    
        protected:
            int64_t mProcessID = -1;
    };
    
    /*constructor*/
    constexpr PIAbstractProcessInfoReader::PIAbstractProcessInfoReader(int64_t pProcessID)
        : mProcessID(pProcessID)
    {
    }
    
    /*public methods*/
    constexpr int64_t PIAbstractProcessInfoReader::getProcessID() const
    {
        return mProcessID;
    }
}

#endif /*PI_ABSTRACT_PROCESS_DATA_READER_H*/
