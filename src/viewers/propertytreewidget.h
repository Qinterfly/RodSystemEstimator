/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the PropertyTreeWidget class
 */

#ifndef PROPERTYTREEWIDGET_H
#define PROPERTYTREEWIDGET_H

#include <QTreeWidget>

namespace RSE
{

namespace Viewers
{

class Graph;
class AbstractGraphData;
using PointerGraph = std::shared_ptr<Graph>;
using EnumData = QPair<QStringList, QList<QIcon>>;

class PropertyTreeWidget : public QTreeWidget
{
public:
    PropertyTreeWidget(QWidget* pParent = nullptr);
    void setSelectedGraph(PointerGraph pGraph);

private:
    void initialize();
    void createHierarchy();
    void updateValues();
    QTreeWidgetItem* createDirectionalDataItem(QString const& name);
    QTreeWidgetItem* createSliceDataItem(QString const& name);
    void createAxesLabelsItem();
    void setTypeValue(AbstractGraphData* pBaseData, QTreeWidgetItem* pItem);
    void setColorItem(QColor const& color);
    EnumData getEnumData(QMetaObject const& metaObject, std::string const& nameEnumerator);
    void setEnumTranslations();

private:
    PointerGraph mpGraph = nullptr;
    QMap<QString, QString> mEnumTranslator;
    QList<QTreeWidgetItem*> mDataItems;
    QList<QTreeWidgetItem*> mSliceDataItems;
    QTreeWidgetItem* mpLineStyleItem;
    QTreeWidgetItem* mpLineWidthItem;
    QTreeWidgetItem* mpColorItem;
    QTreeWidgetItem* mpScatterStyleItem;
    QTreeWidgetItem* mpScatterSizeItem;
    QTreeWidgetItem* mpAxesLabelsItem;
};

}

}

#endif // PROPERTYTREEWIDGET_H
