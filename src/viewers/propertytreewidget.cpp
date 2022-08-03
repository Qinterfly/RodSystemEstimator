/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Definition of the PropertyTreeWidget class
 */

#include <QMetaEnum>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QHeaderView>
#include "propertytreewidget.h"
#include "klp/result.h"
#include "graph.h"
#include "abstractgraphdata.h"
#include "spacetimegraphdata.h"
#include "kinematicsgraphdata.h"
#include "energygraphdata.h"
#include "estimationgraphdata.h"

using namespace RSE::Viewers;

PropertyTreeWidget::PropertyTreeWidget(QWidget* pParent)
    : QTreeWidget(pParent)
{
    initialize();
    makeTranslationMap();
    createHierarchy();
    expandAll();
}

//! Intialize the widget and specify service data
void PropertyTreeWidget::initialize()
{
    setColumnCount(2);
    setHeaderLabels({tr("Свойство"), tr("Значение")});
    setSelectionMode(QAbstractItemView::SingleSelection);
    header()->setSectionResizeMode(QHeaderView::Stretch);
    header()->setDefaultAlignment(Qt::AlignHCenter);
}

//! Represent values of properties
void PropertyTreeWidget::updateValues()
{
    if (!mpGraph)
        return;
    setBlockedSignals(true);
    // Data properties
    int numData = mDataItems.size();
    AbstractGraphData** data = mpGraph->data();
    QComboBox* pComboBox;
    bool isDirectionalData;
    int index;
    for (int i = 0; i != numData; ++i)
    {
        isDirectionalData = data[i] != nullptr;
        // Category
        pComboBox = (QComboBox*)itemWidget(mDataItems[i]->child(0), 1);
        pComboBox->clear();
        pComboBox->addItems(getEnumData(AbstractGraphData::staticMetaObject, "Category").first);
        index = isDirectionalData ? data[i]->category() : -1;
        pComboBox->setCurrentIndex(index);
        // Type
        pComboBox = (QComboBox*)itemWidget(mDataItems[i]->child(1), 1);
        pComboBox->clear();
        if (isDirectionalData)
        {
            setTypeWidget(i);
            pComboBox->setCurrentIndex(data[i]->type());
        }
        // Direction
        pComboBox = (QComboBox*)itemWidget(mDataItems[i]->child(2), 1);
        pComboBox->clear();
        pComboBox->addItems(getEnumData(AbstractGraphData::staticMetaObject, "Direction").first);
        index = isDirectionalData ? data[i]->direction() : -1;
        pComboBox->setCurrentIndex(index);
    }
    // Data slicer
    mpDataSlicerItem->setCheckState(0, mpGraph->isDataSlicer() ? Qt::Checked : Qt::Unchecked);
    resetSlicerWidgetsData();
    updateSlicerWidgetsData();
    // Line style
    mpLineStyleWidget->clear();
    mpLineStyleWidget->addItems(getEnumData(QCPGraph::staticMetaObject, "LineStyle").first);
    mpLineStyleWidget->setCurrentIndex(mpGraph->lineStyle());
    // Line width & color
    mpLineWidthWidget->setValue(mpGraph->lineWidth());
    setColorItem(mpGraph->color());
    // Scatter shape
    mpScatterShapeWidget->clear();
    EnumData scatterData = getEnumData(QCPScatterStyle::staticMetaObject, "ScatterShape");
    int numScatterData = scatterData.first.size();
    for (int i = 0; i != numScatterData; ++i)
    {
        QString const& name = scatterData.first[i];
        QIcon const& icon = scatterData.second[i];
        if (!icon.isNull())
            mpScatterShapeWidget->addItem(icon, name);
        else
            mpScatterShapeWidget->addItem(name);
    }
    mpScatterShapeWidget->setCurrentIndex(mpGraph->scatterShape());
    // Scatter size
    mpScatterSizeWidget->setValue(mpGraph->scatterSize());
    // Title
    mpTitleWidget->setText(mpGraph->title());
    // Axes labels
    QLineEdit* pEdit;
    QStringList const& axesLabels = mpGraph->axesLabels();
    for (int i = 0; i != numData; ++i)
    {
        pEdit = (QLineEdit*)itemWidget(mpAxesLabelsItem->child(i), 1);
        pEdit->setText(axesLabels[i]);
    }
    setBlockedSignals(false);
}

//! Create a hierachy of properties: keys and empty values
void PropertyTreeWidget::createHierarchy()
{
    int const kMaxLineWidth = 50;
    double const kMaxScatterSize = 100;
    QTreeWidgetItem* pRoot = invisibleRootItem();
    // Properties of data
    mDataItems.push_back(createDirectionalDataItem(tr("Данные по X")));
    mDataItems.push_back(createDirectionalDataItem(tr("Данные по Y")));
    mDataItems.push_back(createDirectionalDataItem(tr("Данные по Z")));
    pRoot->addChildren(mDataItems);
    // Data slicer
    createDataSlicerItem();
    pRoot->addChild(mpDataSlicerItem);
    // Line properties
    QTreeWidgetItem* pLineStyleItem = new QTreeWidgetItem({tr("Тип линии")});
    QTreeWidgetItem* pLineWidthItem = new QTreeWidgetItem({tr("Толщина линии")});
    mpColorItem                     = new QTreeWidgetItem({tr("Цвет линии"), tr("")});
    pRoot->addChildren({pLineStyleItem, pLineWidthItem, mpColorItem});
    mpLineStyleWidget = new QComboBox();
    mpLineWidthWidget = new QSpinBox();
    mpLineWidthWidget->setRange(1, kMaxLineWidth);
    mpLineWidthWidget->setValue(1);
    setItemWidget(pLineStyleItem, 1, mpLineStyleWidget);
    setItemWidget(pLineWidthItem, 1, mpLineWidthWidget);
    // Scatter properties
    QTreeWidgetItem* pScatterShapeItem = new QTreeWidgetItem({tr("Тип маркера")});
    QTreeWidgetItem* pScatterSizeItem = new QTreeWidgetItem({tr("Размер маркера")});
    pRoot->addChildren({pScatterShapeItem, pScatterSizeItem});
    mpScatterShapeWidget = new QComboBox();
    mpScatterSizeWidget = new QDoubleSpinBox();
    mpScatterSizeWidget->setRange(0, kMaxScatterSize);
    mpScatterSizeWidget->setValue(5);
    setItemWidget(pScatterShapeItem, 1, mpScatterShapeWidget);
    setItemWidget(pScatterSizeItem, 1, mpScatterSizeWidget);
    // Title
    QTreeWidgetItem* pTitleItem = new QTreeWidgetItem({tr("Название")});
    pRoot->addChild(pTitleItem);
    mpTitleWidget = new QLineEdit();
    setItemWidget(pTitleItem, 1, mpTitleWidget);
    // Axes
    createAxesLabelsItem();
    pRoot->addChild(mpAxesLabelsItem);
    // Enable widgets to communicate
    specifyConnections();
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
void PropertyTreeWidget::createDataSlicerItem()
{
    // Create keys
    mpDataSlicerItem = new QTreeWidgetItem({tr("Срез данных")});
    mpDataSlicerItem->setCheckState(0, Qt::Unchecked);
    QTreeWidgetItem* pTypeItem  = new QTreeWidgetItem({tr("Тип")});
    QTreeWidgetItem* pIndexItem = new QTreeWidgetItem({tr("Индекс")});
    QTreeWidgetItem* pValueItem = new QTreeWidgetItem({tr("Значение")});
    QTreeWidgetItem* pRangeItem = new QTreeWidgetItem({tr("Диапазон")});
    mpDataSlicerItem->addChildren({pTypeItem, pIndexItem, pValueItem, pRangeItem});
    // Create widgets to deal with values
    QDoubleSpinBox* pValueWidget = new QDoubleSpinBox();
    pValueWidget->setDecimals(3);
    pValueWidget->setStepType(QDoubleSpinBox::AdaptiveDecimalStepType);
    QSlider* pRangeWidget = new QSlider();
    pRangeWidget->setOrientation(Qt::Horizontal);
    setItemWidget(pTypeItem, 1, new QComboBox());
    setItemWidget(pIndexItem, 1, new QSpinBox());
    setItemWidget(pValueItem, 1, pValueWidget);
    setItemWidget(pRangeItem, 1, pRangeWidget);
}

//! Create an item to specify labels for axes
void PropertyTreeWidget::createAxesLabelsItem()
{
    mpAxesLabelsItem = new QTreeWidgetItem({tr("Подписи осей")});
    mpAxesLabelsItem->addChild(new QTreeWidgetItem({tr("X")}));
    mpAxesLabelsItem->addChild(new QTreeWidgetItem({tr("Y")}));
    mpAxesLabelsItem->addChild(new QTreeWidgetItem({tr("Z")}));
    int numChildren = mpAxesLabelsItem->childCount();
    for (int i = 0; i != numChildren; ++i)
        setItemWidget(mpAxesLabelsItem->child(i), 1, new QLineEdit());
}

//! Enable widgets to communicate
void PropertyTreeWidget::specifyConnections()
{
    // Data properties
    int numData = mDataItems.size();
    auto funSlicer = [this]() { resetSlicerWidgetsData(); assignSlicer(); updateSlicerWidgetsData(); };
    for (int i = 0; i != numData; ++i)
    {
        // Category
        QComboBox* pComboBox = (QComboBox*)itemWidget(mDataItems[i]->child(0), 1);
        connect(pComboBox, &QComboBox::currentIndexChanged, this, [ this, i, funSlicer]()
        {
            if (mpGraph)
                mpGraph->eraseData(i);
            setTypeWidget(i);
            funSlicer();
        });
        // Type & direction
        auto funAssignGraphData = [ this, i, funSlicer ]() { assignGraphData(i); funSlicer(); updateValues(); };
        for (int j = 1; j != numData; ++j)
        {
            pComboBox = (QComboBox*)itemWidget(mDataItems[i]->child(j), 1);
            connect(pComboBox, &QComboBox::currentIndexChanged, this, funAssignGraphData);
        }
    }
    // State of the data slicer
    connect(this, &QTreeWidget::itemChanged, this, [this, funSlicer](QTreeWidgetItem * pItem, int column)
    {
        if (mpDataSlicerItem == pItem && column == 0)
        {
            bool isFullSet = mpResult && mpGraph;
            blockSignals(true);
            if (pItem->checkState(0) == Qt::Checked && !isFullSet)
                pItem->setCheckState(0, Qt::Unchecked);
            blockSignals(false);
            funSlicer();
        }
    });
    // Type of the data slicer
    QComboBox* pTypeSlicerWidget = (QComboBox*)itemWidget(mpDataSlicerItem->child(0), 1);
    connect(pTypeSlicerWidget, &QComboBox::currentIndexChanged, this, [this]() { assignSlicer(); updateSlicerWidgetsData(); });
    // Indices of the data slicer
    auto funSlicerIndex = [this](qint64 index) { setSlicerIndex(index); updateSlicerWidgetsData(); };
    QSpinBox* pIndexSlicerWidget = (QSpinBox*)itemWidget(mpDataSlicerItem->child(1), 1);
    QSlider* pRangeSlicerWidget = (QSlider*)itemWidget(mpDataSlicerItem->child(3), 1);
    connect(pIndexSlicerWidget, &QSpinBox::valueChanged, this, funSlicerIndex);
    connect(pRangeSlicerWidget, &QSlider::valueChanged, this, funSlicerIndex);
    // Value of the data slicer
    QDoubleSpinBox* pValueSlicerWidget = (QDoubleSpinBox*)itemWidget(mpDataSlicerItem->child(2), 1);
    connect(pValueSlicerWidget, &QDoubleSpinBox::editingFinished, this, [this, pValueSlicerWidget]() { setSlicerValue(pValueSlicerWidget->value()); updateSlicerWidgetsData(); });
    // Line properties
    connect(mpLineStyleWidget, &QComboBox::currentIndexChanged, this, &PropertyTreeWidget::assignVisualProperties);
    connect(mpLineWidthWidget, &QSpinBox::valueChanged, this, &PropertyTreeWidget::assignVisualProperties);
    connect(this, &PropertyTreeWidget::itemDoubleClicked, this, [this](QTreeWidgetItem * pItem, int column)
    {
        if (pItem == mpColorItem && column == 1)
        {
            QColorDialog* pDialog = new QColorDialog(this);
            connect(pDialog, &QColorDialog::colorSelected, this, [this](QColor const & color) { setColorItem(color); assignVisualProperties(); });
            pDialog->show();
        }
    });
    // Scatter properties
    connect(mpScatterShapeWidget, &QComboBox::currentIndexChanged, this, &PropertyTreeWidget::assignVisualProperties);
    connect(mpScatterSizeWidget, &QDoubleSpinBox::valueChanged, this, &PropertyTreeWidget::assignVisualProperties);
    // Title
    connect(mpTitleWidget, &QLineEdit::textChanged, this, &PropertyTreeWidget::assignVisualProperties);
    // Axes labels
    for (int i = 0; i != numData; ++i)
    {
        QLineEdit* pEdit = (QLineEdit*)itemWidget(mpAxesLabelsItem->child(i), 1);
        connect(pEdit, &QLineEdit::editingFinished, this, &PropertyTreeWidget::assignVisualProperties);
    }
}

//! Specify the single graph which properties need to be edited
void PropertyTreeWidget::setSelectedGraph(PointerGraph pGraph)
{
    if (!pGraph)
    {
        mpGraph.reset();
        hide();
        return;
    }
    mpGraph = pGraph;
    updateValues();
    show();
    emit graphChanged();
}

//! Specify the single result to control slicing of data
void PropertyTreeWidget::setSelectedResult(PointerResult pResult)
{
    if (!pResult)
    {
        mpResult.reset();
        updateValues();
        emit graphChanged();
        return;
    }
    if (mpResult != pResult && mpGraph && mpGraph->isDataSlicer())
        mpGraph->dataSlicer().setResult(pResult);
    mpResult = pResult;
    updateValues();
    emit graphChanged();
}

//! Get current data index
int PropertyTreeWidget::currentDataIndex(int iData, int iChild)
{
    QComboBox* pComboBox = (QComboBox*)itemWidget(mDataItems[iData]->child(iChild), 1);
    return pComboBox->currentIndex();
}

//! Get current data text
QString PropertyTreeWidget::currentDataText(int iData, int iChild)
{
    QComboBox* pComboBox = (QComboBox*)itemWidget(mDataItems[iData]->child(iChild), 1);
    return pComboBox->currentText();
}

//! Represent the type of the given graph data
void PropertyTreeWidget::setTypeWidget(int iData)
{
    // Retrieve the current category
    AbstractGraphData::Category category = (AbstractGraphData::Category)currentDataIndex(iData, 0);
    // Get the widget to select types
    QComboBox* pComboBox = (QComboBox*)itemWidget(mDataItems[iData]->child(1), 1);
    {
        QSignalBlocker blocker(pComboBox);
        pComboBox->clear();
        switch (category)
        {
        case AbstractGraphData::cSpaceTime:
            pComboBox->addItems(getEnumData(SpaceTimeGraphData::staticMetaObject, "SpaceTimeType").first);
            break;
        case AbstractGraphData::cKinematics:
            pComboBox->addItems(getEnumData(KinematicsGraphData::staticMetaObject, "KinematicsType").first);
            break;
        case AbstractGraphData::cForce:
            // TODO
            break;
        case AbstractGraphData::cEnergy:
            pComboBox->addItems(getEnumData(EnergyGraphData::staticMetaObject, "EnergyType").first);
            break;
        case AbstractGraphData::cModal:
            // TODO
            break;
        case AbstractGraphData::cEstimation:
            pComboBox->addItems(getEnumData(EstimationGraphData::staticMetaObject, "EstimationType").first);
            break;
        }
        pComboBox->setCurrentIndex(-1);
    }
}

//! Set the color of the graph
void PropertyTreeWidget::setColorItem(QColor const& color)
{
    mpColorItem->setData(1, Qt::DecorationRole, color);
    mpColorItem->setData(1, Qt::DisplayRole, color.name());
}

//! Set the blocked state of all widgets
void PropertyTreeWidget::setBlockedSignals(bool flag)
{
    QTreeWidgetItemIterator iter(this);
    while (*iter)
    {
        QWidget* pWidget = itemWidget(*iter, 1);
        if (pWidget)
            pWidget->blockSignals(flag);
        ++iter;
    }
}

//! Clear data of widgets associated with the data slicer
void PropertyTreeWidget::resetSlicerWidgetsData()
{
    setBlockedSlicerWidgetsSignals(true);
    // Widgets
    QComboBox* pTypeWidget = (QComboBox*)itemWidget(mpDataSlicerItem->child(0), 1);
    QSpinBox* pIndexWidget = (QSpinBox*)itemWidget(mpDataSlicerItem->child(1), 1);
    QDoubleSpinBox* pValueWidget = (QDoubleSpinBox*)itemWidget(mpDataSlicerItem->child(2), 1);
    QSlider* pRangeWidget = (QSlider*)itemWidget(mpDataSlicerItem->child(3), 1);
    // Enabled state
    bool isEnabled = mpDataSlicerItem->checkState(0) == Qt::Checked;
    int numWidgets = mpDataSlicerItem->childCount();
    for (int i = 0; i != numWidgets; ++i)
        itemWidget(mpDataSlicerItem->child(i), 1)->setEnabled(isEnabled);
    // Types
    pTypeWidget->clear();
    if (isEnabled)
    {
        auto const& indicesData = mpGraph->indicesUniqueData();
        for (auto i : indicesData)
        {
            // Only space and time data can be chosen for slicing
            if (mpGraph->data()[i]->category() == AbstractGraphData::cSpaceTime)
            {
                QComboBox* pComboBox = (QComboBox*)itemWidget(mDataItems[i]->child(1), 1);
                pTypeWidget->addItem(pComboBox->currentText(), i);
            }
        }
    }
    pTypeWidget->setCurrentIndex(-1);
    // Limits
    pIndexWidget->setRange(0, 0);
    pValueWidget->setRange(0, 0);
    pRangeWidget->setRange(0, 0);
    // Values
    pIndexWidget->setValue(0);
    pValueWidget->setValue(0.0);
    pRangeWidget->setValue(0);
    setBlockedSlicerWidgetsSignals(false);
}

//! Specify the data of the widget associated with the slicer
void PropertyTreeWidget::updateSlicerWidgetsData()
{
    if (!mpGraph->isDataSlicer())
        return;
    setBlockedSlicerWidgetsSignals(true);
    // Widgets
    QComboBox* pTypeWidget = (QComboBox*)itemWidget(mpDataSlicerItem->child(0), 1);
    QSpinBox* pIndexWidget = (QSpinBox*)itemWidget(mpDataSlicerItem->child(1), 1);
    QDoubleSpinBox* pValueWidget = (QDoubleSpinBox*)itemWidget(mpDataSlicerItem->child(2), 1);
    QSlider* pRangeWidget = (QSlider*)itemWidget(mpDataSlicerItem->child(3), 1);
    // Specify indices and values as well as their ranges
    GraphDataSlicer const& dataSlicer = mpGraph->dataSlicer();
    auto [minSliceIndex, maxSliceIndex] = dataSlicer.limitsIndices();
    // Ranges
    pIndexWidget->setRange(minSliceIndex, maxSliceIndex);
    pValueWidget->setRange(dataSlicer.limitsValues().first, dataSlicer.limitsValues().second);
    pRangeWidget->setRange(minSliceIndex, maxSliceIndex);
    // Values
    qint64 typeIndex = -1;
    qint64 sliceIndex = dataSlicer.index();
    float sliceValue = dataSlicer.value();
    for (int i = 0; i != pTypeWidget->count(); ++i)
    {
        if (pTypeWidget->itemData(i).toInt() == dataSlicer.type())
        {
            typeIndex = i;
            break;
        }
    }
    pTypeWidget->setCurrentIndex(typeIndex);
    pIndexWidget->setValue(sliceIndex);
    pValueWidget->setValue(sliceValue);
    pRangeWidget->setValue(sliceIndex);
    setBlockedSlicerWidgetsSignals(false);
}

//! Set the enabled state of items to slice data
void PropertyTreeWidget::setBlockedSlicerWidgetsSignals(bool flag)
{
    int numChildren = mpDataSlicerItem->childCount();
    for (int i = 0; i != numChildren; ++i)
        itemWidget(mpDataSlicerItem->child(i), 1)->blockSignals(flag);
}

//! Assign new graph data
void PropertyTreeWidget::assignGraphData(int iData)
{
    if (!mpGraph)
        return;
    int iCategory = currentDataIndex(iData, 0);
    int iType = currentDataIndex(iData, 1);
    int iDirection = currentDataIndex(iData, 2);
    if (iCategory >= 0 && iType >= 0)
    {
        if (iDirection < 0)
            iDirection = AbstractGraphData::dFull;
        AbstractGraphData* pData = nullptr;
        auto direction = (AbstractGraphData::Direction)iDirection;
        switch (iCategory)
        {
        case AbstractGraphData::cSpaceTime:
            pData = new SpaceTimeGraphData((SpaceTimeGraphData::SpaceTimeType)iType, direction);
            break;
        case AbstractGraphData::cKinematics:
            pData = new KinematicsGraphData((KinematicsGraphData::KinematicsType)iType, direction);
            break;
        case AbstractGraphData::cForce:
            // TODO
            break;
        case AbstractGraphData::cEnergy:
            pData = new EnergyGraphData((EnergyGraphData::EnergyType)iType, direction);
            break;
        case AbstractGraphData::cModal:
            // TODO
            break;
        case AbstractGraphData::cEstimation:
            pData = new EstimationGraphData((EstimationGraphData::EstimationType)iType, direction);
            break;
        }
        mpGraph->setData(pData, iData);
        mpGraph->setAxisLabel(currentDataText(iData, 1), iData);
        emit graphChanged();
    }
}

//! Assign visual properties of current graph
void PropertyTreeWidget::assignVisualProperties()
{
    if (!mpGraph)
        return;
    // Line properties
    mpGraph->setLineStyle((QCPGraph::LineStyle)mpLineStyleWidget->currentIndex());
    mpGraph->setLineWidth(mpLineWidthWidget->value());
    mpGraph->setColor(mpColorItem->data(1, Qt::DisplayRole).toString());
    // Scatter properties
    mpGraph->setScatterShape((QCPScatterStyle::ScatterShape)mpScatterShapeWidget->currentIndex());
    mpGraph->setScatterSize(mpScatterSizeWidget->value());
    // Title
    mpGraph->setTitle(mpTitleWidget->text());
    // Axes labels
    int numData = mpAxesLabelsItem->childCount();
    for (int i = 0; i != numData; ++i)
    {
        QLineEdit* pEdit = (QLineEdit*)itemWidget(mpAxesLabelsItem->child(i), 1);
        mpGraph->setAxisLabel(pEdit->text(), i);
    }
    emit graphChanged();
}

//! Make a new instance of the data slicer or delete the current one
void PropertyTreeWidget::assignSlicer()
{
    bool isEnabled = mpDataSlicerItem->checkState(0) == Qt::Checked;
    bool isSlicer = mpGraph->isDataSlicer();
    // Remove data slicer if it is no longer needed
    if (!isEnabled && isSlicer)
    {
        mpGraph->removeDataSlicer();
        emit graphChanged();
        return;
    }
    // Try to create a new data slicer
    QComboBox* pTypeWidget = (QComboBox*)itemWidget(mpDataSlicerItem->child(0), 1);
    int currentIndex = pTypeWidget->currentIndex();
    if (currentIndex < 0)
        return;
    int iType = pTypeWidget->itemData(currentIndex, Qt::UserRole).toInt();
    mpGraph->createDataSlicer((GraphDataSlicer::SliceType)iType, mpResult);
    emit graphChanged();
}

//! Specify the leading index for slicing
void PropertyTreeWidget::setSlicerIndex(qint64 index)
{
    if (!mpGraph->isDataSlicer())
        return;
    mpGraph->dataSlicer().setIndex(index);
    emit graphChanged();
}

//! Specfiy the leading value for slicing
void PropertyTreeWidget::setSlicerValue(float value)
{
    if (!mpGraph->isDataSlicer())
        return;
    mpGraph->dataSlicer().setClosestValue(value);
    emit graphChanged();
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
void PropertyTreeWidget::makeTranslationMap()
{
    // Category enum
    mEnumTranslator["cSpaceTime"]  = tr("Пространство-время");
    mEnumTranslator["cKinematics"] = tr("Кинематическая");
    mEnumTranslator["cForce"]      = tr("Силовая");
    mEnumTranslator["cEnergy"]     = tr("Энергетическая");
    mEnumTranslator["cModal"]      = tr("Модальная");
    mEnumTranslator["cEstimation"] = tr("Оценочная");
    // Spacetime enum
    mEnumTranslator["stTime"]                     = tr("Время");
    mEnumTranslator["stParameter"]                = tr("Параметр");
    mEnumTranslator["stNaturalLength"]            = tr("Естественная длина");
    mEnumTranslator["stAccumulatedNaturalLength"] = tr("Суммарная естественная длина");
    mEnumTranslator["stCoordinate"]               = tr("Координата");
    // Kinematics enum
    mEnumTranslator["kStrain"]              = tr("Продольная деформация");
    mEnumTranslator["kDisplacement"]        = tr("Перемещение");
    mEnumTranslator["kRotation"]            = tr("Вектор поворота");
    mEnumTranslator["kSpeed"]               = tr("Линейная скорость");
    mEnumTranslator["kAngularSpeed"]        = tr("Угловая скорость");
    mEnumTranslator["kAcceleration"]        = tr("Линейное ускорение");
    mEnumTranslator["kAngularAcceleration"] = tr("Угловое ускорение");
    // Energy enum
    mEnumTranslator["enKinetic"]   = tr("Кинетическая энергия");
    mEnumTranslator["enPotential"] = tr("Потенциальная энергия");
    mEnumTranslator["enFull"]      = tr("Полная энергия");
    // Estimation enum
    mEnumTranslator["esDisplacement"] = tr("Невязка перемещения");
    mEnumTranslator["esRotation"]     = tr("Невязка вектора поворота");
    mEnumTranslator["esForce"]        = tr("Невязка усилий");
    mEnumTranslator["esMoment"]       = tr("Невязка моментов");
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
