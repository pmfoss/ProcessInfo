#include "AbstractProcessInfoReader.h"

/*public methods*/
bool AbstractProcessInfoReader::readData(ProcessInfo& pData)
{
    pData.mProcessID = mProcessID;

    return true;
}