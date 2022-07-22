/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the Result class
 */

#include <QFile>
#include "result.h"

using namespace KLP;

Result::Result(QString const& pathFile)
{
    if (read(pathFile))
        buildIndex();
}

//! Get the number of rods associated with the requested frame
int Result::numRods(qint64 iFrame) const
{
    if (iFrame < 0 || iFrame >= mNumRecords - 1)
        return -1;
    IndexData indexData = mIndex[iFrame].data[RecordType::R];
    return indexData.size / mNumBytesRod;
}

//! Get the object associated with the requested frame
FloatFrameObject Result::getFrameObject(qint64 iFrame, RecordType type, float normFactor, qint64 shift) const
{
    FloatFrameObject nullFrameObject;
    // Check if an index of frame is valid
    if (iFrame < 0 || iFrame >= mNumRecords - 1)
        return nullFrameObject;
    IndexData indexData = mIndex[iFrame].data[type];
    // Check if the requested data exists
    if (indexData.position == 0)
        return nullFrameObject;
    // Construct the resulting object
    unsigned char* pBuffer = (unsigned char*) mContent.data();
    float* pData = (float*)&pBuffer[indexData.position + shift];
    return FloatFrameObject(pData, normFactor, indexData.size, indexData.step);
}

//! Retrieve the collection of the frame objects
FrameCollection Result::getFrameCollection(qint64 iFrame) const
{
    FrameCollection collection;
    // Retrieve nondimensional coeffcients
    float normFactorDisplacement  = 1.0f;
    float normFactorForce         = 1.0f;
    float normFactorMoment        = 1.0f;
    FloatFrameObject coefficients = getFrameObject(iFrame, RecordType::ND);
    if (!coefficients.isEmpty())
    {
        normFactorDisplacement = *coefficients[NondimensionalType::Displacement];
        normFactorForce        = *coefficients[NondimensionalType::Force];
        normFactorMoment       = *coefficients[NondimensionalType::Moment];
    }
    // Number of rods
    collection.numRods = numRods(iFrame);
    // Parameter
    collection.parameter = getFrameObject(iFrame, RecordType::Xi);
    // Natural length
    collection.naturalLength            = getFrameObject(iFrame, RecordType::S);
    collection.accumulatedNaturalLength = getFrameObject(iFrame, RecordType::SS);
    // Coordinates
    collection.coordinates[0] = getFrameObject(iFrame, RecordType::X1);
    collection.coordinates[1] = getFrameObject(iFrame, RecordType::X2);
    collection.coordinates[2] = getFrameObject(iFrame, RecordType::X3);
    // State vector
    for (int i = 0; i != kNumDirections; ++i)
    {
        collection.displacements[i] = getFrameObject(iFrame, RecordType::U, normFactorDisplacement, i);
        collection.rotations[i]     = getFrameObject(iFrame, RecordType::U, 1.0f, 3 + i);
        collection.forces[i]        = getFrameObject(iFrame, RecordType::U, normFactorForce, 6 + i);
        collection.moments[i]       = getFrameObject(iFrame, RecordType::U, normFactorMoment, 9 + i);
    }
    return collection;
}

//! Read all the content of the file
bool Result::read(QString const& pathFile)
{
    QFile file(pathFile);
    if (!file.open(QIODeviceBase::ReadOnly))
        return false;
    QDataStream stream(&file);
    mContent = file.readAll();
    file.close();
    return true;
}

//! Construct an object to navigate through records
void Result::buildIndex()
{
    // Reading constants
    const int kStartIndex = 17;
    const int kShiftNumRecords = 10;
    const int kShiftTime = 4;

    // Slice the content data
    unsigned char* pBuffer = (unsigned char*) mContent.data();
    qint64 numBuffer = mContent.size();

    // Count the number of records
    mNumRecords = 0;
    qint64 iStartEntry = kStartIndex;
    qint64 jEndEntry = 0;
    short* pStartEntry;
    short* pRecordType;
    unsigned long* pLengthEntry;
    unsigned short* pHl;
    unsigned long numTime = 0, numTimeOld = 0;
    while (iStartEntry < numBuffer)
    {
        ++mNumRecords;
        pStartEntry = (short*)&pBuffer[iStartEntry];
        pLengthEntry = (unsigned long*)&pBuffer[iStartEntry + 2];
        pHl = (unsigned short*)&pBuffer[iStartEntry + 8];
        jEndEntry = iStartEntry + kShiftNumRecords + (*pHl) + abs(*pStartEntry) * (qint64) * pLengthEntry;
        if (jEndEntry >= numBuffer)
            break;
        if (*pHl >= 2)
        {
            pRecordType = (short*)&pBuffer[iStartEntry + kShiftNumRecords];
            // Number of finished records
            if (*pRecordType == 0)
                numTime = numTime + 1;
            // Old versions of the KLP file
            if (*pRecordType == 1)
                numTimeOld = numTimeOld + 1;
        }
        // Label of the last entry
        if (pBuffer[jEndEntry] == 0)
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
        pStartEntry = (short*)&pBuffer[iStartEntry];
        pLengthEntry = (unsigned long*)&pBuffer[iStartEntry + 2];
        pHl = (unsigned short*)&pBuffer[iStartEntry + 8];
        iStartData = iStartEntry + kShiftNumRecords + *pHl;
        jEndEntry = iStartData + abs(*pStartEntry) * (qint64) * pLengthEntry;
        if (*pHl >= 2)
        {
            pRecordType = (short*)&pBuffer[iStartEntry + kShiftNumRecords];
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
                qint64 size = *pLengthEntry;
                qint64 step = 1;
                switch (iType)
                {
                case U:
                case Ut:
                case Utt:
                case Ul:
                    step = 12;
                    break;
                case RMASS:
                    size /= 12;
                    step = 4;
                    break;
                default:
                    break;
                }
                mIndex[iRecord].data[iType] = {iStartData, size, step};
            }
        }
        iStartEntry = jEndEntry + 1;
    }

    // Retrieve values of time steps
    mTime.resize(numTime);
    float* pValue;
    for (unsigned long i = 0; i != numTime; ++i)
    {
        pValue = (float*)&pBuffer[mIndex[i].recordShift + mIndex[i].relativeDataShift + kShiftTime];
        mTime[i] = *pValue;
    }
}
