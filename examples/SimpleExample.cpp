#include <iostream>
#include <cstdlib>
#include <PIProcessInfo.h>
#include <PIProcessInfoReader.h>

#ifdef _WIN32
#include <windows.h>
#define sleep(seconds) Sleep(seconds * 1'000)
#else
#include <unistd.h>
#define sleep(seconds) usleep(seconds * 1'000'000)
#endif

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
    ProcessInfo::PIProcessInfo lData;
    ProcessInfo::PIProcessInfoReader lProcessInfoReader(1041);
    int *lProcData;
    int lCurrentSize = 1'024'000;

    try
    {
       lProcessInfoReader.readData(lData);
    }
    catch(const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return -1;
    }

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

        bool lRes = lProcessInfoReader.readData(lData);
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

        lProcessInfoReader.readData(lData);
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
