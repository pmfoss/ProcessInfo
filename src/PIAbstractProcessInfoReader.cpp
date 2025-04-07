#include "PIAbstractProcessInfoReader.h"

using namespace ProcessInfo;

/*public methods*/
bool PIAbstractProcessInfoReader::readData(PIProcessInfo& pData)
{
    pData.mProcessID = mProcessID;

    return true;
}