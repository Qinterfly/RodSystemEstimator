/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Implementation of the ScalarDataObject class
 */

#include "scalardataobject.h"

using namespace RSE::Core;

quint32 ScalarDataObject::smNumInstances = 0;

//! Construct a scalar data object
ScalarDataObject::ScalarDataObject(QString const& name)
    : AbstractDataObject(kScalar, name)
{
    ++smNumInstances;
}

//! Decrease a number of instances while being destroyed
ScalarDataObject::~ScalarDataObject()
{
    --smNumInstances;
}

//! Insert a new item into ScalarDataObject
DataItemType& ScalarDataObject::addItem(DataValueType key)
{
    auto it = mItems.emplace(key, DataItemType(1, 1));
    return it->second;
}

//! Clone a scalar data object
AbstractDataObject* ScalarDataObject::clone() const
{
    ScalarDataObject* obj = new ScalarDataObject(mName);
    obj->mItems = mItems;
    obj->mID = mID;
    --smNumInstances;
    return obj;
}

//! Import a scalar data object from a file
void ScalarDataObject::import(QTextStream& stream)
{
    mItems.clear();
    quint32 numItems;
    stream >> numItems;
    stream.readLine();
    double key;
    for (quint32 iItem = 0; iItem != numItems; ++iItem)
    {
        stream >> key;
        DataItemType& item = addItem(key);
        stream >> item[0][0];
    }
}
