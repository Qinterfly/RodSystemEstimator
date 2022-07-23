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
    using self_type         = FrameObjectIterator<T>;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T const*;
    using reference         = T const&;

public:
    FrameObjectIterator(pointer pData, T normFactor, qint64 step);
    // Access
    value_type operator*() { return *mpData * mNormFactor; }
    // Operators
    self_type& operator++() { mpData += mStep; return *this; }
    self_type operator++(int) { self_type temp = *this; ++(*this); return temp; }
    self_type operator+(const difference_type& movement) { auto pOldData = mpData; mpData += movement * mStep; self_type temp = *this; mpData = pOldData; return temp; }
    difference_type operator-(const FrameObjectIterator& another) const { return mpData - another.mpData; }
    // Comparison
    friend bool operator== (self_type const& first, self_type const& second) { return first.mpData == second.mpData; };
    friend bool operator!= (self_type const& first, self_type const& second) { return !(first == second); };

private:
    pointer mpData;
    T mNormFactor;
    qint64 const mStep;
};

}

#endif // FRAMEOBJECTITERATOR_H
