/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the GraphListModel class
 */

#include <QListView>
#include "graphlistmodel.h"
#include "graph.h"

using namespace RSE::Viewers;
using namespace RSE::Models;

GraphListModel::GraphListModel(MapGraphs& graphs, QObject* pParent)
    : QStandardItemModel(pParent), mGraphs(graphs)
{
    connect(this, &QStandardItemModel::itemChanged, this, &GraphListModel::renameItem);
    updateContent();
}

//! Create items linked to graphs
void GraphListModel::updateContent()
{
    clearContent();
    for (auto const& iter : mGraphs)
    {
        QStandardItem* pItem = new QStandardItem(iter.second->name());
        pItem->setData(iter.first, Qt::UserRole);
        appendRow(pItem);
    }
}

//! Remove all the items created
void GraphListModel::clearContent()
{
    removeRows(0, rowCount());
}

//! Create a new default graph
void GraphListModel::create()
{
    QString name = tr("График %1").arg(Graph::maxGraphID() + 1);
    std::shared_ptr<Graph> pGraph = std::make_shared<Graph>(name);
    mGraphs.insert({pGraph->id(), pGraph});
    updateContent();
}

//! Remove selected graphs
void GraphListModel::removeSelected()
{
    QListView* pParent = (QListView*) parent();
    QModelIndexList const& indices = pParent->selectionModel()->selectedIndexes();
    if (indices.isEmpty())
        return;
    for (auto const& index : indices)
    {
        qint64 id = index.data(Qt::UserRole).toInt();
        mGraphs.erase(id);
    }
    updateContent();
}

//! Rename a graph after editing
void GraphListModel::renameItem(QStandardItem* pItem)
{
    qint64 id = pItem->data(Qt::UserRole).toInt();
    QString newName = pItem->data(Qt::DisplayRole).toString();
    mGraphs[id]->setName(newName);
}

