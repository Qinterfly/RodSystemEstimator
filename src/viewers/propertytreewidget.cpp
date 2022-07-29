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
#include "graph.h"

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
    setEnumTranslations();
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
    for (auto item : mDataItems)
        expandItem(item);
    // Properties for slicing data: XSData, YSData, ZSData
    mSliceDataItems.push_back(createSliceDataItem(tr("Срез данных по X")));
    mSliceDataItems.push_back(createSliceDataItem(tr("Срез данных по Y")));
    mSliceDataItems.push_back(createSliceDataItem(tr("Срез данных по Z")));
    pRoot->addChildren(mSliceDataItems);
    // Line properties
    mpLineStyleItem = new QTreeWidgetItem({tr("Тип линии")});
    mpLineWidthItem = new QTreeWidgetItem({tr("Толщина линии")});
    mpColorItem     = new QTreeWidgetItem({tr("Цвет линии"), tr("")});
    pRoot->addChildren({mpLineStyleItem, mpLineWidthItem, mpColorItem});
    QSpinBox* pLineWidthSpinBox = new QSpinBox();
    pLineWidthSpinBox->setRange(0, kMaxLineWidth);
    pLineWidthSpinBox->setValue(1);
    setItemWidget(mpLineStyleItem, 1, new QComboBox());
    setItemWidget(mpLineWidthItem, 1, pLineWidthSpinBox);
    // Scatter properties
    mpScatterStyleItem = new QTreeWidgetItem({tr("Тип маркера")});
    mpScatterSizeItem = new QTreeWidgetItem({tr("Размер маркера")});
    pRoot->addChildren({mpScatterStyleItem, mpScatterSizeItem});
    QDoubleSpinBox* pScatterSizeSpinBox = new QDoubleSpinBox();
    pScatterSizeSpinBox->setRange(0, kMaxScatterSize);
    pScatterSizeSpinBox->setValue(5);
    setItemWidget(mpScatterStyleItem, 1, new QComboBox());
    setItemWidget(mpScatterSizeItem, 1, pScatterSizeSpinBox);
    // Specify default properties
    resetValues();
}

//! Represent values of properties
void PropertyTreeWidget::updateValues()
{
    resetValues();
    // TODO
}

//! Clear values of properties
void PropertyTreeWidget::resetValues()
{
    QColor const kDefaultColor = Qt::blue;
    // Data properties
    int numDirections = mDataItems.size();
    QComboBox* pComboBox;
    for (int i = 0; i != numDirections; ++i)
    {
        // Category
        pComboBox = (QComboBox*)itemWidget(mDataItems[i]->child(0), 1);
        pComboBox->clear();
        pComboBox->addItems(getEnumData(AbstractGraphData::staticMetaObject, "Category").first);
        pComboBox->setCurrentIndex(-1);
        // Type
        pComboBox = (QComboBox*)itemWidget(mDataItems[i]->child(1), 1);
        pComboBox->clear();
        // Direction
        pComboBox = (QComboBox*)itemWidget(mDataItems[i]->child(2), 1);
        pComboBox->clear();
        pComboBox->addItems(getEnumData(AbstractGraphData::staticMetaObject, "Direction").first);
        pComboBox->setCurrentIndex(-1);
    }
    // Line properties
    pComboBox = (QComboBox*)itemWidget(mpLineStyleItem, 1);
    pComboBox->clear();
    pComboBox->addItems(getEnumData(QCPGraph::staticMetaObject, "LineStyle").first);
    pComboBox->setCurrentIndex(-1);
    mpColorItem->setData(1, Qt::DecorationRole, kDefaultColor);
    mpColorItem->setData(1, Qt::DisplayRole, kDefaultColor.name());
    // Scatter properties
    pComboBox = (QComboBox*)itemWidget(mpScatterStyleItem, 1);
    pComboBox->clear();
    EnumData scatterData = getEnumData(QCPScatterStyle::staticMetaObject, "ScatterShape");
    int numScatterData = scatterData.first.size();
    for (int i = 0; i != numScatterData; ++i)
    {
        QString const& name = scatterData.first[i];
        QIcon const& icon = scatterData.second[i];
        if (!icon.isNull())
            pComboBox->addItem(icon, name);
        else
            pComboBox->addItem(name);
    }
    pComboBox->setCurrentIndex(-1);
}

//! Create a nested hierarchy of directional items
QTreeWidgetItem* PropertyTreeWidget::createDirectionalDataItem(QString const& name)
{
    // Create keys
    QTreeWidgetItem* pRootItem = new QTreeWidgetItem({name});
    QTreeWidgetItem* pCategoryItem = new QTreeWidgetItem({tr("Категория")});
    QTreeWidgetItem* pTypeItem = new QTreeWidgetItem({tr("Тип")});
    QTreeWidgetItem* pDirectionItem = new QTreeWidgetItem({tr("Направление")});
    pRootItem->addChildren({pCategoryItem, pTypeItem, pDirectionItem});
    // Create widgets to deal with values
    setItemWidget(pCategoryItem, 1, new QComboBox());
    setItemWidget(pTypeItem, 1, new QComboBox);
    setItemWidget(pDirectionItem, 1, new QComboBox());
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
    pRootItem->addChildren({pIndexItem, pValueItem, pRangeItem});
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
        mpGraph.reset();
        return;
    }
    mpGraph = pGraph;
    updateValues();
}

//! Retrieve translated keys and icons from a meta object
EnumData PropertyTreeWidget::getEnumData(QMetaObject const& metaObject, std::string const& nameEnumerator)
{
    // Retrieve the enum object
    int iEnum = metaObject.indexOfEnumerator(nameEnumerator.c_str());
    QMetaEnum metaEnum = metaObject.enumerator(iEnum);
    int numKeys = metaEnum.keyCount();
    // Allocate the resulting object
    EnumData enumData;
    QStringList& translatedKeys = enumData.first;
    QList<QIcon>& icons = enumData.second;
    translatedKeys.resize(numKeys);
    icons.resize(numKeys);
    // Iterate through all the enum keys
    for (int i = 0; i != numKeys; ++i)
    {
        QString key = metaEnum.key(i);
        QString pathIcon = QString(":/icons/%1.png").arg(key);
        if (QFile::exists(pathIcon))
            icons[i].addFile(pathIcon);
        if (mEnumTranslator.contains(key))
            key = mEnumTranslator[key];
        translatedKeys[i] = key;
    }
    return enumData;
}

//! Specify translations for enum options
void PropertyTreeWidget::setEnumTranslations()
{
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
    // Line style enum
    mEnumTranslator["lsNone"]       = tr("Без линии");
    mEnumTranslator["lsLine"]       = tr("Сплошная");
    mEnumTranslator["lsStepLeft"]   = tr("Левые ступеньки");
    mEnumTranslator["lsStepRight"]  = tr("Правые ступеньки");
    mEnumTranslator["lsStepCenter"] = tr("Средние ступеньки");
    mEnumTranslator["lsImpulse"]    = tr("Импульсная");
    // Scatter style enum
    mEnumTranslator["ssNone"]             = tr("Без маркера");
    mEnumTranslator["ssDot"]              = tr("Точка");
    mEnumTranslator["ssCross"]            = tr("Крест");
    mEnumTranslator["ssPlus"]             = tr("Плюс");
    mEnumTranslator["ssCircle"]           = tr("Круг");
    mEnumTranslator["ssDisc"]             = tr("Диск");
    mEnumTranslator["ssSquare"]           = tr("Квадрат");
    mEnumTranslator["ssDiamond"]          = tr("Ромб");
    mEnumTranslator["ssStar"]             = tr("Звезда");
    mEnumTranslator["ssTriangle"]         = tr("Треугольник");
    mEnumTranslator["ssTriangleInverted"] = tr("Перевернутый треугольник");
    mEnumTranslator["ssCrossSquare"]      = tr("Крест в квадрате");
    mEnumTranslator["ssPlusSquare"]       = tr("Плюс в квадрате");
    mEnumTranslator["ssCrossCircle"]      = tr("Крест в круге");
    mEnumTranslator["ssPlusCircle"]       = tr("Плюс в круге");
    mEnumTranslator["ssPeace"]            = tr("Мир");
    mEnumTranslator["ssPixmap"]           = tr("Писксельная карта");
    mEnumTranslator["ssCustom"]           = tr("Пользовательский");
}
