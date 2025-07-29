#ifndef PI_PROCESS_INFO_H
#define PI_PROCESS_INFO_H 1

#include <string>

using pid_t = int;

// To read Memory and CPU data:
// https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

namespace ProcessInfo
{
    struct PIProcessInfo
    {
        template<typename T>
        class Data
        {
            public:
                Data() = default;
    
                constexpr bool isConfigured() const
                {
                    return mConfigured;
                }
    
                constexpr Data& operator=(const T& pData)
                {
                    mData = pData;
                    mConfigured = true;

                    return *this;
                }
    
                constexpr T operator()() const
                {
                    return mData;
                }
    
            private:
                T mData;
                bool mConfigured = false;           
        };
    
        Data<std::string> mCmdLine;
        Data<double> mCPULoad;
        Data<size_t> mMemoryResident;
        Data<size_t> mMemorySwapped;
        Data<size_t> mMemoryVirtual;
        Data<int64_t> mProcessID;
        Data<int64_t> mParentProcessID;
    };
}

#endif /*PI_PROCESS_INFO_H*/