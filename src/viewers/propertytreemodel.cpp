/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the PropertyTreeModel class
 */

#include "propertytreemodel.h"
#include <QTreeView>

using namespace RSE::Models;

void setEditableItemChildren(QStandardItem* pItem, bool isEditable = false);

PropertyTreeModel::PropertyTreeModel(QObject* pParent)
    : QStandardItemModel(pParent)
{
    setHorizontalHeaderLabels({tr("Свойство"), tr("Значение")});
    createHierarchy();
}

//! Create a hierachy of properties: keys and empty values
void PropertyTreeModel::createHierarchy()
{
    QStandardItem* pRootItem = invisibleRootItem();
    // Data properties: XData, YData, ZData
    appendRow(createDirectionalDataItem(tr("Данные по X")));
    appendRow(createDirectionalDataItem(tr("Данные по Y")));
    appendRow(createDirectionalDataItem(tr("Данные по Z")));
    // Properties for slicing data: XSData, YSData, ZSData
    appendRow(createSliceDataItem(tr("Срез данных по X")));
    appendRow(createSliceDataItem(tr("Срез данных по Y")));
    appendRow(createSliceDataItem(tr("Срез данных по Z")));
    // Line properties
    appendRow(new QStandardItem(tr("Тип линии")));
    appendRow(new QStandardItem(tr("Толщина линии")));
    appendRow(new QStandardItem(tr("Цвет линии")));
    // Scatter properties
    appendRow(new QStandardItem(tr("Тип маркера")));
    appendRow(new QStandardItem(tr("Размер маркера")));
    // Arrangement
    setEditableItemChildren(pRootItem, false);
}

//! Represent values of properties
void PropertyTreeModel::updateValues()
{
    clearValues();
}

//! Clear values of properties
void PropertyTreeModel::clearValues()
{
    removeColumn(1);
}

//! Create a nested hierarchy of directional items
QStandardItem* PropertyTreeModel::createDirectionalDataItem(QString const& name)
{
    QStandardItem* pRootItem = new QStandardItem(name);
    // Category
    QStandardItem* pCategoryItem = new QStandardItem(tr("Категория"));
    // Type
    QStandardItem* pTypeItem = new QStandardItem(tr("Тип"));
    // Direction
    QStandardItem* pDirectionItem = new QStandardItem(tr("Направление"));
    // Arrange
    pRootItem->appendRow(pCategoryItem);
    pRootItem->appendRow(pTypeItem);
    pRootItem->appendRow(pDirectionItem);
    setEditableItemChildren(pRootItem, false);
    return pRootItem;
}

//! Create a nested hierarchy of items to slice data
QStandardItem* PropertyTreeModel::createSliceDataItem(QString const& name)
{
    QStandardItem* pRootItem = new QStandardItem(name);
    // Index
    QStandardItem* pIndexItem = new QStandardItem(tr("Индекс"));
    // Value
    QStandardItem* pValueItem = new QStandardItem(tr("Значение"));
    // Range
    QStandardItem* pRangeItem = new QStandardItem(tr("Диапазон"));
    // Arrange
    pRootItem->appendRow(pIndexItem);
    pRootItem->appendRow(pValueItem);
    pRootItem->appendRow(pRangeItem);
    setEditableItemChildren(pRootItem, false);
    return pRootItem;
}

//! Specify the single graph which properties need to be edited
void PropertyTreeModel::setSelectedGraph(std::shared_ptr<RSE::Viewers::Graph> pGraph)
{
    if (!pGraph)
    {
        clearValues();
        return;
    }
    mpGraph = pGraph;
}

// Helper function to modify editing property of the children belonged to the given item
void setEditableItemChildren(QStandardItem* pItem, bool isEditable)
{
    for (int i = 0; i != pItem->rowCount(); ++i)
        pItem->child(i)->setEditable(isEditable);
}
