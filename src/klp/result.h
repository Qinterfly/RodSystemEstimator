/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
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
    QDateTime creationDateTime;
    qint64 numTotalRecords = 0;
    qint64 numTimeRecords = 0;
    uint fileSize = 0;
    uint ID = -1;
};

//! Class to aggregate all the records
class Result
{
public:
    explicit Result(QString const& pathFile);
    ~Result() = default;
    bool isEmpty() const { return mContent.isEmpty(); }
    QVector<double> const& time() const { return mTime; }
    QString const& pathFile() const { return mkPathFile; }
    QString name() const;
    int numRods(qint64 iFrame) const;
    qint64 numTotalRecords() const { return mNumTotalRecords; }
    qint64 numTimeRecords() const { return mTime.size(); }
    ResultInfo info() const;
    FrameCollection getFrameCollection(qint64 iFrame) const;
    void update();

private:
    bool read();
    void buildIndex();
    void setStateFrameData(StateFrame& state, RecordType type, qint64 iFrame, qint64 iStartData, std::vector<float> const& normFactors) const;
    FloatFrameObject getFrameObject(qint64 iFrame, RecordType type, float normFactor = 1.0f, qint64 shift = 0) const;

private:
    //! Path to the KLP file
    QString const mkPathFile;
    //! Content of the file
    QByteArray mContent;
    //! Index of the data buffer
    std::vector<Index> mIndex;
    //! Number of records
    qint64 mNumTotalRecords;
    //! Time array
    QVector<double> mTime;
    //! Number of bytes per rod
    char mNumBytesRod;
};

}

#endif // RESULT_H
