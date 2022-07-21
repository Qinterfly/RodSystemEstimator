/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the Records class
 */

#include <QFile>
#include "records.h"

using namespace KLP;

Records::Records(QString const& pathFile)
{
    if (read(pathFile))
        buildIndex();
}

Records::~Records()
{
    delete[] mpBuffer;
}

//! Read all the content of the file
bool Records::read(QString const& pathFile)
{
    // Open the file for reading
    QFile file(pathFile);
    if (!file.open(QIODeviceBase::ReadOnly))
        return false;
    QDataStream stream(&file);

    // Count the length of the buffer
    unsigned char tChar;
    mNumBuffer = 0;
    while (!stream.atEnd())
    {
        stream >> tChar;
        ++mNumBuffer;
    }
    stream.device()->seek(0);

    // Allocate and fill the buffer
    if (mpBuffer)
        delete mpBuffer;
    char* pRawBuffer = new char[mNumBuffer];
    stream.readRawData(pRawBuffer, mNumBuffer);
    mpBuffer = (unsigned char*)pRawBuffer;
    file.close();
    return true;
}

//! Construct an object to navigate through records
void Records::buildIndex()
{
    const int kStartIndex = 17;
    const int kShiftNumRecords = 10;
    const int kShiftTime = 4;

    // Count the number of records
    mNumRecords = 0;
    qint64 iStartEntry = kStartIndex;
    qint64 jEndEntry = 0;
    short* pStartEntry;
    short* pRecordType;
    unsigned long* pLengthEntry;
    unsigned short* pHl;
    unsigned long numTime = 0, numTimeOld = 0;
    while (iStartEntry < mNumBuffer)
    {
        ++mNumRecords;
        pStartEntry = (short*)&mpBuffer[iStartEntry];
        pLengthEntry = (unsigned long*)&mpBuffer[iStartEntry + 2];
        pHl = (unsigned short*)&mpBuffer[iStartEntry + 8];
        jEndEntry = iStartEntry + kShiftNumRecords + (*pHl) + abs(*pStartEntry) * (qint64) * pLengthEntry;
        if (jEndEntry >= mNumBuffer)
            break;
        if (*pHl >= 2)
        {
            pRecordType = (short*)&mpBuffer[iStartEntry + kShiftNumRecords];
            // Number of finished records
            if (*pRecordType == 0)
                numTime = numTime + 1;
            // Old versions of the KLP file
            if (*pRecordType == 1)
                numTimeOld = numTimeOld + 1;
        }
        // Label of the last entry
        if (mpBuffer[jEndEntry] == 0)
            break;
        iStartEntry = jEndEntry + 1;
    }
    if (numTime == 0)
        numTime = numTimeOld;

    // Alocate the mapping structure
    mNumBytesRod = 3;
    mIndex.resize(mNumRecords);

    // Fill in the mapping structure
    qint64 kk = 0;
    qint64 iRecord = 0;
    qint64 iStartData;
    int iType;
    iStartEntry = kStartIndex;
    for (qint64 k = 0; k != mNumRecords; ++k)
    {
        pStartEntry = (short*)&mpBuffer[iStartEntry];
        pLengthEntry = (unsigned long*)&mpBuffer[iStartEntry + 2];
        pHl = (unsigned short*)&mpBuffer[iStartEntry + 8];
        iStartData = iStartEntry + kShiftNumRecords + *pHl;
        jEndEntry = iStartData + abs(*pStartEntry) * (qint64) * pLengthEntry;
        if (*pHl >= 2)
        {
            pRecordType = (short*)&mpBuffer[iStartEntry + kShiftNumRecords];
            iType = *pRecordType;
            // Header
            if (iType == 1)
            {
                mIndex[kk].recordShift = iStartEntry;
                mIndex[kk].relativeDataShift = (unsigned char)(kShiftNumRecords + *pHl);
                if (*pLengthEntry > 4)
                    mNumBytesRod = 4;
                ++kk;
            }
            // Assign the record
            iRecord = kk - 1;
            if (iType > 1 && iType < RecordType::SIZE)
            {
                switch (iType)
                {
                case RMASS:
                    mIndex[iRecord].data[iType] = {iStartData, *pLengthEntry / 12};
                    break;
                default:
                    mIndex[iRecord].data[iType] = {iStartData, *pLengthEntry};
                    break;
                }
            }
        }
        iStartEntry = jEndEntry + 1;
    }

    // Retrieve values of time steps
    mTime.resize(numTime);
    float* pValue;
    for (unsigned long i = 0; i != numTime; ++i)
    {
        pValue = (float*)&mpBuffer[mIndex[i].recordShift + mIndex[i].relativeDataShift + kShiftTime];
        mTime[i] = *pValue;
    }
}
