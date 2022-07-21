/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the Records class
 */

#ifndef RECORDS_H
#define RECORDS_H

#include <QString>
#include "recordtype.h"

namespace KLP
{

struct Index;

//! Class to aggregate all the records
class Records
{
public:
    Records(QString const& pathFile);
    ~Records();

private:
    bool read(QString const& pathFile);
    void buildIndex();

private:
    //! Data buffer
    unsigned char* mpBuffer = nullptr;
    //! Length of the data buffer
    qint64 mNumBuffer;
    //! Index of the data buffer
    std::vector<Index> mIndex;
    //! Number of records
    qint64 mNumRecords;
    //! Time array
    std::vector<float> mTime;
    //! Number of bytes per rod
    char mNumBytesRod;
};

//! Structure to navigate through records
struct Index
{
    //! Base constructor
    Index() { data.resize(RecordType::SIZE); }
    //! Data arranged in pairs: [shift, length]
    std::vector<std::pair<qint64, qint64>> data;
    //! Shift of the main record
    quint64 recordShift = 0;
    //! Relative shift of data
    quint64 relativeDataShift = 0;
};

}

#endif // RECORDS_H
