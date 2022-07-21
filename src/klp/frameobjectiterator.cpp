/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the FrameObjectIterator class
 */

#include "frameobjectiterator.h"

template class KLP::FrameObjectIterator<float>;

using namespace KLP;

template <typename T>
FrameObjectIterator<T>::FrameObjectIterator(pointer pData, qint64 step)
    : mpData(pData), mStep(step)
{

}
