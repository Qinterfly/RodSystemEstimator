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
using PointerGraph = std::shared_ptr<Graph>;
using EnumData = QPair<QStringList, QList<QIcon>>;

class PropertyTreeWidget : public QTreeWidget
{
public:
    PropertyTreeWidget(QWidget* pParent = nullptr);
    void setSelectedGraph(PointerGraph pGraph);

private:
    void initialize();
    void setEnumTranslations();
    void createHierarchy();
    void updateValues();
    void resetValues();
    QTreeWidgetItem* createDirectionalDataItem(QString const& name);
    QTreeWidgetItem* createSliceDataItem(QString const& name);
    EnumData getEnumData(QMetaObject const& metaObject, std::string const& nameEnumerator);

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
};

}

}

#endif // PROPERTYTREEWIDGET_H
