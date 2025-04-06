#include "AbstractProcessDataReader.h"

/*public methods*/
bool AbstractProcessDataReader::updateData(ProcessData& pData)
{
    pData.mProcessID = mProcessID;

    return true;
}