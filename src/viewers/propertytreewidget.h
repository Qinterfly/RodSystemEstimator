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

namespace RSE
{

namespace Viewers
{

class Graph;
using PointerGraph = std::shared_ptr<Graph>;
using EnumData = QPair<QStringList, QList<QIcon>>;

class PropertyTreeWidget : public QTreeWidget
{
public:
    PropertyTreeWidget(QWidget* pParent = nullptr);
    void setSelectedGraph(PointerGraph pGraph);

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
    // Assign new properties to the current graph
    void assignGraphData(int iData);
    void assignVisualProperties();
    // Translation of enum keys
    EnumData getEnumData(QMetaObject const& metaObject, std::string const& nameEnumerator);
    void makeTranslationMap();

private:
    PointerGraph mpGraph = nullptr;
    QMap<QString, QString> mEnumTranslator;
    QList<QTreeWidgetItem*> mDataItems;
    QList<QTreeWidgetItem*> mSliceDataItems;
    QComboBox* mpLineStyleWidget;
    QSpinBox* mpLineWidthWidget;
    QTreeWidgetItem* mpColorItem;
    QComboBox* mpScatterStyleWidget;
    QDoubleSpinBox* mpScatterSizeWidget;
    QTreeWidgetItem* mpAxesLabelsItem;
};

}

}

#endif // PROPERTYTREEWIDGET_H
