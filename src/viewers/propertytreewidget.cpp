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
#include "spacetimegraphdata.h"
#include "kinematicsgraphdata.h"
#include "graph.h"
#include "klp/result.h"

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
            setTypeValue(i);
            pComboBox->setCurrentIndex(data[i]->type());
        }
        // Direction
        pComboBox = (QComboBox*)itemWidget(mDataItems[i]->child(2), 1);
        pComboBox->clear();
        pComboBox->addItems(getEnumData(AbstractGraphData::staticMetaObject, "Direction").first);
        index = isDirectionalData ? data[i]->direction() : -1;
        pComboBox->setCurrentIndex(index);
        // Slice state
        Qt::CheckState sliceState = isDirectionalData && mpGraph->isSliced(i) ? Qt::Checked : Qt::Unchecked;
        mSliceDataItems[i]->setCheckState(0, sliceState);
        // Slice index
        QSpinBox* pSpinBox = (QSpinBox*)itemWidget(mSliceDataItems[i]->child(0), 1);
        QSlider* pSlider = (QSlider*)itemWidget(mSliceDataItems[i]->child(1), 1);
        pSpinBox->setValue(0);
        pSlider->setValue(0);
        // Slice limits
        if (isDirectionalData)
            setSliceWidgets(i);
        setEnabledSliceWidgets(i);
    }
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
    QTreeWidgetItem* pLineStyleItem = new QTreeWidgetItem({tr("Тип линии")});
    QTreeWidgetItem* pLineWidthItem = new QTreeWidgetItem({tr("Толщина линии")});
    mpColorItem                     = new QTreeWidgetItem({tr("Цвет линии"), tr("")});
    pRoot->addChildren({pLineStyleItem, pLineWidthItem, mpColorItem});
    mpLineStyleWidget = new QComboBox();
    mpLineWidthWidget = new QSpinBox();
    mpLineWidthWidget->setRange(0, kMaxLineWidth);
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
QTreeWidgetItem* PropertyTreeWidget::createSliceDataItem(QString const& name)
{
    // Create keys
    QTreeWidgetItem* pRootItem = new QTreeWidgetItem({name});
    pRootItem->setCheckState(0, Qt::Unchecked);
    QTreeWidgetItem* pIndexItem = new QTreeWidgetItem({tr("Индекс")});
    QTreeWidgetItem* pRangeItem = new QTreeWidgetItem({tr("Диапазон")});
    pRootItem->addChildren({pIndexItem, pRangeItem});
    // Create widgets to deal with values
    QSpinBox* pSpinBox = new QSpinBox();
    pSpinBox->setMinimum(0);
    QSlider* pSlider = new QSlider();
    pSlider->setOrientation(Qt::Horizontal);
    setItemWidget(pIndexItem, 1, pSpinBox);
    setItemWidget(pRangeItem, 1, pSlider);
    return pRootItem;
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
    for (int i = 0; i != numData; ++i)
    {
        // Category
        QComboBox* pComboBox = (QComboBox*)itemWidget(mDataItems[i]->child(0), 1);
        connect(pComboBox, &QComboBox::currentIndexChanged, this, [ this, i ]()
        {
            if (mpGraph)
                mpGraph->eraseData(i);
            setTypeValue(i);
        });
        // Type & direction
        auto funAssignGraphData = [ this, i ]() { assignGraphData(i); setSliceWidgets(i); };
        for (int j = 1; j != numData; ++j)
        {
            pComboBox = (QComboBox*)itemWidget(mDataItems[i]->child(j), 1);
            connect(pComboBox, &QComboBox::currentIndexChanged, this, funAssignGraphData);
        }
        // Slicing state
        connect(this, &QTreeWidget::itemChanged, this, [this, i](QTreeWidgetItem * pItem, int column)
        {
            if (mSliceDataItems[i] == pItem && column == 0)
            {
                bool isData = mpResult && mpGraph && mpGraph->data()[i];
                blockSignals(true);
                if (pItem->checkState(0) == Qt::Checked && !isData)
                    pItem->setCheckState(0, Qt::Unchecked);
                blockSignals(false);
                setEnabledSliceWidgets(i);
                assignSliceCheckedState(i);
            }
        });
        // Slice index
        auto funSliceIndex = [this,  i](int value) { assignSliceIndex(value, i); setSliceWidgets(i); };
        QSpinBox* pSpinBox = (QSpinBox*)itemWidget(mSliceDataItems[i]->child(0), 1);
        QSlider* pSlider = (QSlider*)itemWidget(mSliceDataItems[i]->child(1), 1);
        connect(pSpinBox, &QSpinBox::valueChanged, this, funSliceIndex);
        connect(pSlider, &QSlider::valueChanged, this, funSliceIndex);
    }
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
        return;
    }
    mpResult = pResult;
    updateValues();
}

//! Get current data index
int PropertyTreeWidget::currentDataIndex(int iData, int iChild)
{
    QComboBox* pComboBox = (QComboBox*)itemWidget(mDataItems[iData]->child(iChild), 1);
    return pComboBox->currentIndex();
}

//! Represent the type of the given graph data
void PropertyTreeWidget::setTypeValue(int iData)
{
    // Retrieve the current category
    AbstractGraphData::Category category = (AbstractGraphData::Category)currentDataIndex(iData, 0);
    // Get the widget to select types
    QComboBox* pComboBox = (QComboBox*)itemWidget(mDataItems[iData]->child(1), 1);
    pComboBox->blockSignals(true);
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
        // TODO
        break;
    case AbstractGraphData::cModal:
        // TODO
        break;
    case AbstractGraphData::cEstimation:
        // TODO
        break;
    }
    pComboBox->setCurrentIndex(-1);
    pComboBox->blockSignals(false);
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

//! Specify values and limites of widgets for slicing
void PropertyTreeWidget::setSliceWidgets(int iData)
{
    if (!mpGraph || !mpResult)
        return;
    const int kBaseFrame = 0;
    // Obtain the boundary values for slicing
    AbstractGraphData* pData = mpGraph->data()[iData];
    if (!pData)
        return;
    int maxIndex;
    bool isTime = pData->category() == AbstractGraphData::cSpaceTime && pData->type() == SpaceTimeGraphData::stTime;
    if (isTime)
    {
        maxIndex = mpResult->numTimeRecords() - 1;
    }
    else
    {
        auto const& collection = mpResult->getFrameCollection(kBaseFrame);
        auto const& dataset = pData->getDataset(collection);
        maxIndex = dataset.size() - 1;
    }
    // Retrieve widgets for slicing
    QSpinBox* pIndexSpinBox = (QSpinBox*)itemWidget(mSliceDataItems[iData]->child(0), 1);
    QSlider* pIndexSlider = (QSlider*)itemWidget(mSliceDataItems[iData]->child(1), 1);
    {
        QSignalBlocker blockerIndexSpinBox(pIndexSpinBox);
        QSignalBlocker blockerIndexSlider(pIndexSlider);
        // Specify limits
        pIndexSpinBox->setRange(0, maxIndex);
        pIndexSlider->setRange(0, maxIndex);
        // Set current indices
        if (mpGraph->isSliced(iData))
        {
            int iSlice = mpGraph->sliceIndex(iData);
            pIndexSpinBox->setValue(iSlice);
            pIndexSlider->setValue(iSlice);
        }
    }
}

//! Set enabled state of widgets to slice data
void PropertyTreeWidget::setEnabledSliceWidgets(int iData)
{
    bool isChecked = mSliceDataItems[iData]->checkState(0) == Qt::Checked;
    QSpinBox* pIndexSpinBox = (QSpinBox*)itemWidget(mSliceDataItems[iData]->child(0), 1);
    QSlider* pIndexSlider = (QSlider*)itemWidget(mSliceDataItems[iData]->child(1), 1);
    pIndexSpinBox->setEnabled(isChecked);
    pIndexSlider->setEnabled(isChecked);
}

//! Assign new graph data
void PropertyTreeWidget::assignGraphData(int iData)
{
    if (!mpGraph)
        return;
    int iCategory = currentDataIndex(iData, 0);
    int iType = currentDataIndex(iData, 1);
    int iDirection = currentDataIndex(iData, 2);
    if (iCategory >= 0 && iType >= 0 && iDirection >= 0)
    {
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
            // TODO
            break;
        case AbstractGraphData::cModal:
            // TODO
            break;
        case AbstractGraphData::cEstimation:
            // TODO
            break;
        }
        mpGraph->setData(pData, iData);
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
    // Axes labels
    int numData = mpAxesLabelsItem->childCount();
    QStringList axesLabels(numData);
    for (int i = 0; i != numData; ++i)
    {
        QLineEdit* pEdit = (QLineEdit*)itemWidget(mpAxesLabelsItem->child(i), 1);
        axesLabels[i] = pEdit->text();
    }
    mpGraph->setAxesLabels(axesLabels);
    emit graphChanged();
}

//! Assign the index for slicing along specified data direction
void PropertyTreeWidget::assignSliceIndex(int value, int iData)
{
    if (!mpGraph || !mpGraph->isSliced(iData))
        return;
    mpGraph->setSliceIndex(value, iData);
    emit graphChanged();
}

//! Assign whether the graph data needs to be sliced
void PropertyTreeWidget::assignSliceCheckedState(int iData)
{
    bool isChecked = mSliceDataItems[iData]->checkState(0) == Qt::Checked;
    if (mpGraph && mpGraph->data()[iData])
    {
        if (mpGraph->isSliced(iData) != isChecked)
        {
            int sliceIndex = isChecked ? 0 : -1;
            mpGraph->setSliceIndex(sliceIndex, iData);
            emit graphChanged();
        }
    }
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
    mEnumTranslator["stCoordiante"]               = tr("Координата");
    // Kinematics enum
    mEnumTranslator["kStrain"]              = tr("Продольная деформация");
    mEnumTranslator["kDisplacement"]        = tr("Перемещение");
    mEnumTranslator["kRotation"]            = tr("Проекция вектора поворота");
    mEnumTranslator["kSpeed"]               = tr("Линейная скорость");
    mEnumTranslator["kAngularSpeed"]        = tr("Угловая скорость");
    mEnumTranslator["kAcceleration"]        = tr("Линейное ускорение");
    mEnumTranslator["kAngularAcceleration"] = tr("Угловое ускорение");
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
