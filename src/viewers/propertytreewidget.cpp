/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the PropertyTreeWidget class
 */

#include <QMetaEnum>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QHeaderView>
#include "propertytreewidget.h"
#include "abstractgraphdata.h"

using namespace RSE::Viewers;

PropertyTreeWidget::PropertyTreeWidget(QWidget* pParent)
    : QTreeWidget(pParent)
{
    initialize();
    createHierarchy();
}

//! Intialize the widget and specify service data
void PropertyTreeWidget::initialize()
{
    setColumnCount(2);
    setHeaderLabels({tr("Свойство"), tr("Значение")});
    setSelectionMode(QAbstractItemView::SingleSelection);
    header()->setSectionResizeMode(QHeaderView::Stretch);
    header()->setDefaultAlignment(Qt::AlignHCenter);
    // Category enum
    mEnumTranslator["cSpaceTime"]  = tr("Пространство-время");
    mEnumTranslator["cKinematics"] = tr("Кинематическая");
    mEnumTranslator["cForce"]      = tr("Силовая");
    mEnumTranslator["cEnergy"]     = tr("Энергетическая");
    mEnumTranslator["cModal"]      = tr("Модальная");
    mEnumTranslator["cEstimation"] = tr("Оценочная");
    // Direction enum
    mEnumTranslator["dFirst"]  = tr("Первое");
    mEnumTranslator["dSecond"] = tr("Второе");
    mEnumTranslator["dThird"]  = tr("Третье");
    mEnumTranslator["dFull"]   = tr("Полное");
}

//! Create a hierachy of properties: keys and empty values
void PropertyTreeWidget::createHierarchy()
{
    int const kMaxLineWidth = 10;
    double const kMaxScatterSize = 20;
    QTreeWidgetItem* pRoot = invisibleRootItem();
    // Properties of data
    mDataItems.push_back(createDirectionalDataItem(tr("Данные по X")));
    mDataItems.push_back(createDirectionalDataItem(tr("Данные по Y")));
    mDataItems.push_back(createDirectionalDataItem(tr("Данные по Z")));
    pRoot->addChildren(mDataItems);
    // Properties for slicing data: XSData, YSData, ZSData
    mSliceDataItems.push_back(createSliceDataItem(tr("Срез данных по X")));
    mSliceDataItems.push_back(createSliceDataItem(tr("Срез данных по Y")));
    mSliceDataItems.push_back(createSliceDataItem(tr("Срез данных по Z")));
    pRoot->addChildren(mSliceDataItems);
    // Line properties
    mpLineStyleItem = new QTreeWidgetItem({tr("Тип линии")});
    mpLineWidthItem = new QTreeWidgetItem({tr("Толщина линии")});
    mpColorItem     = new QTreeWidgetItem({tr("Цвет линии")});
    pRoot->addChildren({mpLineStyleItem, mpLineWidthItem, mpColorItem});
    QSpinBox* pLineWidthSpinBox = new QSpinBox();
    pLineWidthSpinBox->setRange(0, kMaxLineWidth);
    setItemWidget(mpLineStyleItem, 1, new QComboBox());
    setItemWidget(mpLineWidthItem, 1, pLineWidthSpinBox);
    setItemWidget(mpColorItem, 1, new QComboBox());
    // Scatter properties
    mpScatterStyleItem = new QTreeWidgetItem({tr("Тип маркера")});
    mpScatterSizeItem = new QTreeWidgetItem({tr("Размер маркера")});
    pRoot->addChildren({mpScatterStyleItem, mpScatterSizeItem});
    QDoubleSpinBox* pScatterSizeSpinBox = new QDoubleSpinBox();
    pScatterSizeSpinBox->setRange(0, kMaxScatterSize);
    setItemWidget(mpScatterStyleItem, 1, new QComboBox());
    setItemWidget(mpScatterSizeItem, 1, pScatterSizeSpinBox);
}

//! Represent values of properties
void PropertyTreeWidget::updateValues()
{
    // TODO
}

//! Clear values of properties
void PropertyTreeWidget::resetValues()
{
    // TODO
}

//! Create a nested hierarchy of directional items
QTreeWidgetItem* PropertyTreeWidget::createDirectionalDataItem(QString const& name)
{
    // Create keys
    QTreeWidgetItem* pRootItem = new QTreeWidgetItem({name});
    QTreeWidgetItem* pCategoryItem = new QTreeWidgetItem({tr("Категория")});
    QTreeWidgetItem* pTypeItem = new QTreeWidgetItem({tr("Тип")});
    QTreeWidgetItem* pDirectionItem = new QTreeWidgetItem({tr("Направление")});
    pRootItem->addChild(pCategoryItem);
    pRootItem->addChild(pTypeItem);
    pRootItem->addChild(pDirectionItem);
    // Create widgets to deal with values
    QComboBox* pCategoryComboBox = new QComboBox();
    QComboBox* pDirectionComboBox = new QComboBox();
    setItemWidget(pCategoryItem, 1, pCategoryComboBox);
    setItemWidget(pTypeItem, 1, new QComboBox);
    setItemWidget(pDirectionItem, 1, pDirectionComboBox);
    return pRootItem;
}

//! Create a nested hierarchy of items to slice data
QTreeWidgetItem* PropertyTreeWidget::createSliceDataItem(QString const& name)
{
    // Create keys
    QTreeWidgetItem* pRootItem = new QTreeWidgetItem({name});
    pRootItem->setCheckState(0, Qt::Unchecked);
    QTreeWidgetItem* pIndexItem = new QTreeWidgetItem({tr("Индекс")});
    QTreeWidgetItem* pValueItem = new QTreeWidgetItem({tr("Значение")});
    QTreeWidgetItem* pRangeItem = new QTreeWidgetItem({tr("Диапазон")});
    pRootItem->addChild(pIndexItem);
    pRootItem->addChild(pValueItem);
    pRootItem->addChild(pRangeItem);
    // Create widgets to deal with values
    QSlider* pSlider = new QSlider();
    pSlider->setOrientation(Qt::Horizontal);
    setItemWidget(pIndexItem, 1, new QSpinBox());
    setItemWidget(pValueItem, 1, new QDoubleSpinBox());
    setItemWidget(pRangeItem, 1, pSlider);
    return pRootItem;
}

//! Specify the single graph which properties need to be edited
void PropertyTreeWidget::setSelectedGraph(PointerGraph pGraph)
{
    if (!pGraph)
    {
        resetValues();
        return;
    }
    mpGraph = pGraph;
}

//! Retrieve translated keys from a meta object
QStringList PropertyTreeWidget::getEnumKeys(QMetaObject const& metaObject, std::string const& nameEnumerator)
{
    QStringList translatedKeys;
    int iEnum = metaObject.indexOfEnumerator(nameEnumerator.c_str());
    QMetaEnum metaEnum = metaObject.enumerator(iEnum);
    int numKeys = metaEnum.keyCount();
    translatedKeys.resize(numKeys);
    for (int i = 0; i != numKeys; ++i)
    {
        QString key = metaEnum.key(i);
        if (mEnumTranslator.contains(key))
            key = mEnumTranslator[key];
        translatedKeys[i] = key;
    }
    return translatedKeys;
}
