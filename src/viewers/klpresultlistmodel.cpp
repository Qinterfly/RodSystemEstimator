/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the KLPResultListModel class
 */

#include <QFileInfo>
#include <QListView>
#include "klpresultlistmodel.h"
#include "klp/result.h"

using namespace RSE::Models;

KLPResultListModel::KLPResultListModel(Results& results, QObject* pParent)
    : QStandardItemModel(pParent), mResults(results)
{

}

//! Create items linked to results
void KLPResultListModel::updateContent()
{
    clearContent();
    for (auto const& result : mResults)
    {
        QString fileName = QFileInfo(result->pathFile()).baseName();
        QStandardItem* pItem = new QStandardItem();
        pItem->setText(fileName);
        appendRow(pItem);
    }
}

//! Remove all the items created
void KLPResultListModel::clearContent()
{
    removeRows(0, rowCount());
}

//! Remove selected results
void KLPResultListModel::removeSelected()
{
    QListView* pParent = (QListView*)parent();
    QModelIndexList selectedIndices = pParent->selectionModel()->selectedIndexes();
    if (selectedIndices.isEmpty())
        return;
    int numResults = mResults.size();
    std::vector<bool> mask(numResults, true);
    for (auto const& index : selectedIndices)
        mask[index.row()] = false;
    Results newResults;
    for (int i = 0; i != numResults; ++i)
    {
        if (mask[i])
            newResults.push_back(mResults[i]);
    }
    mResults = newResults;
    updateContent();
}
