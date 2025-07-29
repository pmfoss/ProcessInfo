#include "PIAbstractProcessInfoReader.h"

using namespace ProcessInfo;

/*public methods*/
bool PIAbstractProcessInfoReader::readAllData(PIProcessInfo& pData)
{
    return readData(pData, ReadModeFlags::All);
}

bool PIAbstractProcessInfoReader::readData(PIProcessInfo& pData, ReadMode pMode)
{
    pData.mProcessID = mProcessID;

    return true;
}