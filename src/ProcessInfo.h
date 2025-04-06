#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H 1

#include <string>

using pid_t = int;

// To read Memory and CPU data:
// https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

struct ProcessInfo
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

            constexpr void operator=(const T& pData)
            {
                mData = pData;
                mConfigured = true;
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
    Data<pid_t> mProcessID;
    Data<pid_t> mParentProcessID;
};

#endif /*PROCESS_INFO_H*/