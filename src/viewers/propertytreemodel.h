/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the PropertyTreeModel class
 */

#ifndef PROPERTYTREEMODEL_H
#define PROPERTYTREEMODEL_H

#include <QStandardItemModel>

namespace RSE
{

namespace Viewers
{
class Graph;
}

namespace Models
{

using PointerGraph = std::shared_ptr<RSE::Viewers::Graph>;

class PropertyTreeModel : public QStandardItemModel
{
public:
    PropertyTreeModel(QObject* pParent = nullptr);
    void setSelectedGraph(PointerGraph pGraph);

private:
    void createHierarchy();
    void updateValues();
    void clearValues();
    QStandardItem* createDirectionalDataItem(QString const& name);
    QStandardItem* createSliceDataItem(QString const& name);

private:
    PointerGraph mpGraph = nullptr;
};

}

}

#endif // PROPERTYTREEMODEL_H
