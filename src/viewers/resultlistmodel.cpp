/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Definition of the KLPResultListModel class
 */

#include <QFileInfo>
#include <QListView>
#include <QColorDialog>
#include "apputilities.h"
#include "resultlistmodel.h"
#include "klp/result.h"

using namespace RSE::Models;

ResultListModel::ResultListModel(KLP::Results& results, QObject* pParent)
    : QStandardItemModel(pParent), mResults(results)
{
    specifyConnections();
    updateContent();
}

//! Update results from files
void ResultListModel::updateData()
{
    for (auto& result : mResults)
        result->update();
    emit resultsUpdated();
}

//! Create items linked to results
void ResultListModel::updateContent()
{
    clearContent();
    for (auto const& result : mResults)
    {
        QString fileName = QFileInfo(result->pathFile()).baseName();
        QStandardItem* pItem = new QStandardItem();
        KLP::Result* pResult = result.get();
        QColor color = mResultColors.contains(pResult) ? mResultColors[pResult] : getAvailableColor();
        pItem->setText(fileName);
        pItem->setData(color, Qt::DecorationRole);
        pItem->setData((qintptr)pResult, Qt::UserRole);
        mResultColors[pResult] = color;
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
        KLP::PointerResult pResult = mResults[i];
        if (mask[i])
            newResults.push_back(pResult);
        else
            mResultColors.remove(pResult.get());
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
    QListView* pView = (QListView*)parent();
    pView->setCurrentIndex(index(iSelect, 0));
}

//! Retireve the next available color from the predefined set of colors
QColor ResultListModel::getAvailableColor()
{
    static int siColor = 0;
    static QStringList const skStandardColorNames = Utilities::App::standardColorNames();
    if (siColor == skStandardColorNames.size())
        siColor = 0;
    return QColor(skStandardColorNames[siColor++]);
}

//! Enable widgets to communicate
void ResultListModel::specifyConnections()
{
    QListView* pView = (QListView*)parent();
    // Create a color dialog to modify the color of a project
    connect(pView, &QListView::doubleClicked, this, [this, pView](const QModelIndex & index)
    {
        // Acuqire the addres of the current result
        int iItem = index.row();
        qintptr* pIHolderResult = (qintptr*)item(iItem)->data(Qt::UserRole).data();
        KLP::Result* pResult = (KLP::Result*)(*pIHolderResult);
        QColorDialog* pDialog = new QColorDialog(pView);
        // Connect it with the color dialog
        connect(pDialog, &QColorDialog::colorSelected, this, [this, pResult, iItem](QColor const & color)
        {
            mResultColors[pResult] = color;
            updateContent();
            selectItem(iItem);
        });
        pDialog->show();
    });
}
