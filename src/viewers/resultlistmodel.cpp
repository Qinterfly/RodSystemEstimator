/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the KLPResultListModel class
 */

#include <QFileInfo>
#include <QListView>
#include "resultlistmodel.h"
#include "klp/result.h"

using namespace RSE::Models;

ResultListModel::ResultListModel(KLP::Results& results, QObject* pParent)
    : QStandardItemModel(pParent), mResults(results)
{
    updateContent();
}

//! Update results from files
void ResultListModel::updateData()
{
    for (auto& result : mResults)
        result->update();
}

//! Create items linked to results
void ResultListModel::updateContent()
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
void ResultListModel::clearContent()
{
    removeRows(0, rowCount());
}

//! Remove selected results
void ResultListModel::removeSelected()
{
    QListView* pParent = (QListView*)parent();
    QModelIndexList selectedIndices = pParent->selectionModel()->selectedIndexes();
    if (selectedIndices.isEmpty())
        return;
    int numResults = mResults.size();
    std::vector<bool> mask(numResults, true);
    for (auto const& index : selectedIndices)
        mask[index.row()] = false;
    KLP::Results newResults;
    for (int i = 0; i != numResults; ++i)
    {
        if (mask[i])
            newResults.push_back(mResults[i]);
    }
    mResults = newResults;
    updateContent();
    selectItem();
}

//! Select an item by index
void ResultListModel::selectItem(int iSelect)
{
    int numRows = rowCount();
    if (iSelect < 0)
        iSelect = numRows - 1;
    if (iSelect >= numRows)
        return;
    QListView* pParent = (QListView*)parent();
    pParent->setCurrentIndex(index(iSelect, 0));
}
