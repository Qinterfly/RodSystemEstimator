/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the Result class
 */

#ifndef RESULT_H
#define RESULT_H

#include <QString>
#include <QDateTime>
#include "index.h"
#include "framecollection.h"

namespace KLP
{

struct ResultInfo
{
    //! Creation date and time
    QDateTime creationDateTime;
    //! Total number of records
    qint64 numTotalRecords;
    //! Number of time records
    qint64 numTimeRecords;
    //! File size, Kb
    uint fileSize;
    //! Unique identifier
    uint identifier;
};

//! Class to aggregate all the records
class Result
{
public:
    explicit Result(QString const& pathFile);
    ~Result() = default;
    bool isEmpty() const { return mContent.isEmpty(); }
    std::vector<float> const& time() const { return mTime; }
    QString const& pathFile() const { return mkPathFile; }
    int numRods(qint64 iFrame) const;
    ResultInfo info() const;
    FloatFrameObject getFrameObject(qint64 iFrame, RecordType type, float normFactor = 1.0f, qint64 shift = 0) const;
    FrameCollection getFrameCollection(qint64 iFrame) const;
    void update();

private:
    bool read();
    void buildIndex();
    void setStateFrameData(StateFrame& state, RecordType type, qint64 iFrame, qint64 iStartData, std::vector<float> const& normFactors) const;

private:
    //! Path to the KLP file
    QString const mkPathFile;
    //! Content of the file
    QByteArray mContent;
    //! Index of the data buffer
    std::vector<Index> mIndex;
    //! Number of records
    qint64 mNumRecords;
    //! Time array
    std::vector<float> mTime;
    //! Number of bytes per rod
    char mNumBytesRod;
};

}

#endif // RESULT_H
