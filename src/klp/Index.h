/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Specification of a structure to index records
 */

#ifndef INDEX_H
#define INDEX_H

#include <QtGlobal>
#include "recordtype.h"
#include <vector>

namespace KLP
{

//! Data of each record
struct IndexData
{
    //! Position of a record in the buffer
    qint64 position = 0;
    //! Size of a record
    qint64 size = 0;
    //! Step for iterating inside a record
    qint64 step = 1;
};

//! Structure to navigate through records
struct Index
{
    //! Base constructor
    Index() { data.resize(RecordType::SIZE); }
    //! Data
    std::vector<IndexData> data;
    //! Shift of the main record
    quint64 recordShift = 0;
    //! Relative shift of data
    quint64 relativeDataShift = 0;
};

}

#endif // INDEX_H
