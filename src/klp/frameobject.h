/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the FrameObject class
 */

#ifndef FRAMEOBJECT_H
#define FRAMEOBJECT_H

#include <QDebug>
#include "frameobjectiterator.h"

namespace KLP
{

template <typename T>
class FrameObject
{
public:
    using iterator = FrameObjectIterator<T>;

public:
    FrameObject(T const* pData = nullptr, T normFactor = 1.0, qint64 size = 0, qint64 step = 1);
    ~FrameObject() = default;
    bool isEmpty() const { return !mpData; }
    qint64 size() const { return mSize; }
    iterator begin() const { return iterator(mpData, mNormFactor, mStep); }
    iterator end() const { return begin() + mSize; }
    iterator operator[](int index) const { return begin() + index; }
    template<typename K> friend QDebug operator<<(QDebug stream, FrameObject<K> const& frameObject);

private:
    T const* mpData;
    T mNormFactor;
    qint64 mSize;
    qint64 mStep;
};

template<typename K>
inline QDebug operator<<(QDebug stream, FrameObject<K> const& frameObject)
{
    stream = stream.noquote();
    for (auto it = frameObject.begin(); it != frameObject.end(); ++it)
        stream << QString::number(*it) << Qt::endl;
    return stream;
}

}

#endif // FRAMEOBJECT_H
