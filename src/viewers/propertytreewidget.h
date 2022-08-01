/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of the PropertyTreeWidget class
 */

#ifndef PROPERTYTREEWIDGET_H
#define PROPERTYTREEWIDGET_H

#include <QTreeWidget>
#include "aliasklp.h"
#include "aliasviewers.h"

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

using EnumData = QPair<QStringList, QList<QIcon>>;
using KLP::PointerResult;

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
    void createDataSlicerItem();
    void createAxesLabelsItem();
    void specifyConnections();
    void setBlockedSignals(bool flag);
    // Data and visual widgets
    int currentDataIndex(int iData, int iChild);
    QString currentDataText(int iData, int iChild);
    void setTypeWidget(int iData);
    void setColorItem(QColor const& color);
    // Slicer widgets
    void resetSlicerWidgetsData();
    void updateSlicerWidgetsData();
    void setBlockedSlicerWidgetsSignals(bool flag);
    // Assign new properties to the current graph
    void assignGraphData(int iData);
    void assignVisualProperties();
    void assignSlicer();
    void setSlicerIndex(qint64 index);
    void setSlicerValue(float value);
    // Translation of enum keys
    EnumData getEnumData(QMetaObject const& metaObject, std::string const& nameEnumerator);
    void makeTranslationMap();

private:
    // Data items
    PointerGraph mpGraph = nullptr;
    PointerResult mpResult = nullptr;
    QList<QTreeWidgetItem*> mDataItems;
    QTreeWidgetItem* mpDataSlicerItem;
    // Visual items
    QMap<QString, QString> mEnumTranslator;
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
