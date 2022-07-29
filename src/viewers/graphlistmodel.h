/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the GraphListModel class
 */

#ifndef GRAPHLISTMODEL_H
#define GRAPHLISTMODEL_H

#include <QStandardItemModel>
#include "aliasviewers.h"

namespace RSE
{

namespace Models
{

class GraphListModel : public QStandardItemModel
{
    Q_OBJECT

public:
    GraphListModel(Viewers::MapGraphs& graphs, QObject* pParent = nullptr);
    void create();
    void updateContent();
    void removeSelected();
    void selectItem(int iSelect = -1);

private:
    void clearContent();
    void renameItem(QStandardItem* pItem);

private:
    Viewers::MapGraphs& mGraphs;
};

}

}

#endif // GRAPHLISTMODEL_H
