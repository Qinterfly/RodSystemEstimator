/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the PropertyTreeWidget class
 */

#ifndef PROPERTYTREEWIDGET_H
#define PROPERTYTREEWIDGET_H

#include <QTreeWidget>
#include "abstractgraphdata.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
QT_END_NAMESPACE

namespace KLP
{
class Result;
}

namespace RSE
{

namespace Viewers
{

class Graph;
using PointerGraph = std::shared_ptr<Graph>;
using PointerResult = std::shared_ptr<KLP::Result>;
using EnumData = QPair<QStringList, QList<QIcon>>;

class PropertyTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    PropertyTreeWidget(QWidget* pParent = nullptr);
    void setSelectedGraph(PointerGraph pGraph);
    void setSelectedResult(PointerResult pResult);

signals:
    void graphChanged();

private:
    void initialize();
    void updateValues();
    // Construct widgets
    void createHierarchy();
    QTreeWidgetItem* createDirectionalDataItem(QString const& name);
    QTreeWidgetItem* createSliceDataItem(QString const& name);
    void createAxesLabelsItem();
    void specifyConnections();
    // Set values of properties
    int currentDataIndex(int iData, int iChild);
    void setTypeValue(int iData);
    void setColorItem(QColor const& color);
    void setBlockedSignals(bool flag);
    void setSliceWidgets(int iData);
    // Assign new properties to the current graph
    void assignGraphData(int iData);
    void assignVisualProperties();
    void assignSliceIndex(int value, int iData);
    void assignSliceCheckedState(int iData);
    // Translation of enum keys
    EnumData getEnumData(QMetaObject const& metaObject, std::string const& nameEnumerator);
    void makeTranslationMap();

private:
    PointerGraph mpGraph = nullptr;
    PointerResult mpResult = nullptr;
    QMap<QString, QString> mEnumTranslator;
    QList<QTreeWidgetItem*> mDataItems;
    QList<QTreeWidgetItem*> mSliceDataItems;
    QComboBox* mpLineStyleWidget;
    QSpinBox* mpLineWidthWidget;
    QTreeWidgetItem* mpColorItem;
    QComboBox* mpScatterShapeWidget;
    QDoubleSpinBox* mpScatterSizeWidget;
    QTreeWidgetItem* mpAxesLabelsItem;
};

}

}

#endif // PROPERTYTREEWIDGET_H
