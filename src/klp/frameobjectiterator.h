/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the FrameObjectIterator class
 */

#ifndef FRAMEOBJECTITERATOR_H
#define FRAMEOBJECTITERATOR_H

#include <QtGlobal>

namespace KLP
{

//! Class to iterate through data of a record
template <typename T>
class FrameObjectIterator
{
public:
    using iterator          = FrameObjectIterator<T>;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T const*;
    using reference         = T const&;

public:
    FrameObjectIterator(pointer pData = nullptr, qint64 step = 1);
    ~FrameObjectIterator() {};
    // Access
    reference operator*() const { return *mpData; }
    pointer operator->() const { return mpData; }
    // Increment
    iterator& operator++() { mpData += mStep; return *this; }
    // Comparison
    friend bool operator== (iterator const& first, iterator const& second) { return first.mpData == second.mpData; };
    friend bool operator!= (iterator const& first, iterator const& second) { return !(first == second); };

private:
    pointer mpData;
    qint64 const mStep;
};

}

#endif // FRAMEOBJECTITERATOR_H
