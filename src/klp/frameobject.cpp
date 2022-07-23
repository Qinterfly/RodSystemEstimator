/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the FrameObject class
 */

#include "frameobject.h"

template class KLP::FrameObject<float>;

using namespace KLP;

template <typename T>
FrameObject<T>::FrameObject(T const* pData, T normFactor, qint64 size, qint64 step)
    : mpData(pData), mNormFactor(normFactor), mSize(size), mStep(step)
{

}

