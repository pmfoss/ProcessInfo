#include <iostream>
#include <cstdlib>
#include <ProcessData.h>
#include <ProcessDataReader.h>

#ifdef _WIN32
#include <windows.h>
#define sleep(seconds) Sleep(seconds * 1'000)
#else
#include <unistd.h>
#define sleep(seconds) usleep(seconds * 1'000'000)
#endif

//https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

void fillData(int* pData, int pSize)
{
    for(int lIdx = 0; lIdx < pSize; ++lIdx)
    {
        pData[lIdx] = lIdx;
    }
}

int Sum(int *pData, int pSize)
{
    int lRetval = 0;

    for(int lIdx = 0; lIdx < pSize; ++lIdx)
    {
        lRetval += pData[lIdx];
    }

    return lRetval;
}

int main()
{
    ProcessData lData;
    ProcessDataReader lProcessDataReader;
    int *lProcData;
    int lCurrentSize = 1'024'000;

    lProcessDataReader.updateData(lData);

    lProcData = static_cast<int *>(malloc(lCurrentSize * sizeof(int)));
    fillData(lProcData, lCurrentSize);

    std::cout << "pid: " << lData.mProcessID() << "\n";
    std::cout << "ppid: " << lData.mParentProcessID() << "\n";
    std::cout << "cmdline: " << lData.mCmdLine() << "\n";
    std::cout << "memory:\n";
    std::cout << "   total: " << lData.mMemoryVirtual() << "\n";
    std::cout << "   resident: " << lData.mMemoryResident() << "\n";
    std::cout << "   swapped: " << lData.mMemorySwapped() << "\n";
    std::cout << "cpu load: " << lData.mCPULoad() << "%\n";

    for(int i = 0; i < 20; i++)
    {
        sleep(1);
        lCurrentSize += 1'024'000;
        lProcData = static_cast<int *>(realloc(lProcData, lCurrentSize * sizeof(int)));
        fillData(lProcData, lCurrentSize);

        bool lRes = lProcessDataReader.updateData(lData);
        std::cout << "size (" << i << "): " << lCurrentSize << " " << lRes << "\n";
        std::cout << "memory:\n";
        std::cout << "   total: " << lData.mMemoryVirtual() << "\n";
        std::cout << "   resident: " << lData.mMemoryResident() << "\n";
        std::cout << "   swapped: " << lData.mMemorySwapped() << "\n";
        std::cout << "cpu load: " << lData.mCPULoad() << "%\n\n\n";
    }
    
    for(int i = 0; i < 5; i++)
    {
        sleep(1);
        lCurrentSize -= 1'024'000;
        lProcData = static_cast<int *>(realloc(lProcData, lCurrentSize * sizeof(int)));
        fillData(lProcData, lCurrentSize);

        lProcessDataReader.updateData(lData);
        std::cout << "size (" << i << "): " << lCurrentSize << "\n";
        std::cout << "memory:\n";
        std::cout << "   total: " << lData.mMemoryVirtual() << "\n";
        std::cout << "   resident: " << lData.mMemoryResident() << "\n";
        std::cout << "   swapped: " << lData.mMemorySwapped() << "\n";
        std::cout << "cpu load: " << lData.mCPULoad() << "%\n\n\n";
    }

    std::cout << "sum of data: " << Sum(lProcData, lCurrentSize) << "\n";

    return 0;
}
