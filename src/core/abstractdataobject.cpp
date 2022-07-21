/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Implementation of the AbstractDataObject class
 */

#include "abstractdataobject.h"
#include "constants.h"

using namespace RSE::Core;

DataIDType AbstractDataObject::smMaxObjectID = 0;

//! Base constructor
AbstractDataObject::AbstractDataObject(ObjectType type, QString const& name)
    : mkType(type)
    , mName(name)
{
    mID = ++smMaxObjectID;
}

AbstractDataObject::~AbstractDataObject()
{

}

//! Modify an existing key
bool AbstractDataObject::changeItemKey(DataKeyType oldKey, DataKeyType newKey, DataHolder* items)
{
    if (!items)
        items = &mItems;
    // If the table does not contain the old key or the new one is already presented
    auto iterOldKey = items->find(oldKey);
    auto iterNewKey = items->find(newKey);
    if (iterOldKey == items->end() || iterNewKey != items->end())
        return false;
    DataItemType obj = iterOldKey->second;
    items->erase(oldKey);
    items->emplace(newKey, std::move(obj));
    return true;
}

//! Remove the entity paired to the specified key
void AbstractDataObject::removeItem(DataKeyType key)
{
    mItems.erase(key);
}

//! Set an array value with the specified indices
bool AbstractDataObject::setArrayValue(DataKeyType key, DataValueType newValue, IndexType iRow, IndexType iColumn)
{
    auto iterKey = mItems.find(key);
    if (iterKey == mItems.end())
        return false;
    DataItemType& array = iterKey->second;
    array[iRow][iColumn] = newValue;
    return true;
}

//! Retrieve a value from an array
DataValueType AbstractDataObject::arrayValue(DataKeyType key, IndexType iRow, IndexType iColumn)
{
    auto iterKey = mItems.find(key);
    DataItemType& array = iterKey->second;
    return array[iRow][iColumn];
}

//! Retrieve all the keys
std::vector<DataKeyType> AbstractDataObject::keys() const
{
    std::vector<DataKeyType> keys;
    for (auto const& item : mItems)
        keys.push_back(item.first);
    return keys;
}

//! Serialize an abstract data object
void AbstractDataObject::serialize(QDataStream& stream) const
{
    stream << (quint32)mkType;
    stream << mName;
    stream << (DataIDType)mID;
    stream << (quint32)mItems.size();
    for (auto& item : mItems)
    {
        stream << item.first;
        stream << item.second;
    }
}

/*!
 * \brief Partly deserialize an abstract data object
 *
 * It is assumed that a type and name have already been assigned.
 * So, only an identifier and items need to be set.
 */
void AbstractDataObject::deserialize(QDataStream& stream)
{
    mItems.clear();
    quint32 numItems;
    DataKeyType key;
    stream >> mID;
    stream >> numItems;
    for (quint32 i = 0; i != numItems; ++i)
    {
        DataItemType dataItem;
        stream >> key;
        stream >> dataItem;
        mItems.emplace(key, dataItem);
    }
}

//! Write an abstract data object to a file
void AbstractDataObject::write(QTextStream& stream) const
{
    stream << mItems.size() << 1;
    stream << Qt::endl;
    for (auto& item : mItems)
        stream << QString::number(item.first, 'g', RSE::Constants::kWritingPrecision) << item.second;
}
