/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the RodSystemTableModel class
 */

#include <QTableView>
#include "rodsystemtablemodel.h"
#include "core/rodsystem.h"

using namespace RSE::Models;
using namespace RSE::Core;

RodSystemTableModel::RodSystemTableModel(QObject* pParent)
    : QStandardItemModel(pParent)
{
    setHorizontalHeaderLabels({tr("Длина пролета, м"), tr("Длина провода, м"), tr("Масса провода, кг")});
    connect(this, &QStandardItemModel::itemChanged, this, &RodSystemTableModel::setChangedData);
}

//! Acquire the pointer to a rod system
void RodSystemTableModel::setRodSystem(RodSystem* pRodSystem)
{
    mpRodSystem = pRodSystem;
    if (!mpRodSystem)
    {
        clearContent();
        return;
    }
    updateContent();
}

//! Represent all data of a rod system
void RodSystemTableModel::updateContent()
{
    clearContent();
    if (!mpRodSystem)
        return;
    QStandardItem* rootItem = invisibleRootItem();
    int numRods = mpRodSystem->numRods();
    std::vector<double> const& distances = mpRodSystem->distances();
    Spans spans = mpRodSystem->computeSpans();
    for (int i = 0; i != numRods; ++i)
    {
        double distance = distances[i];
        double length   = spans.L[i];
        double mass     = mpRodSystem->massPerLength() * length;
        // Distance between supports
        QStandardItem* pDistanceItem = new QStandardItem();
        pDistanceItem->setText(QString::number(distance));
        pDistanceItem->setData(distances[i], Qt::UserRole);
        pDistanceItem->setTextAlignment(Qt::AlignCenter);
        // Calculated length of a cable
        QStandardItem* pLengthItem = new QStandardItem();
        pLengthItem->setText(QString::number(length, 'g', 8));
        pLengthItem->setTextAlignment(Qt::AlignCenter);
        pLengthItem->setEditable(false);
        // Mass of cable
        QStandardItem* pMassItem = new QStandardItem();
        pMassItem->setText(QString::number(mass));
        pMassItem->setTextAlignment(Qt::AlignCenter);
        pMassItem->setEditable(false);
        // Resulted row
        rootItem->appendRow({pDistanceItem, pLengthItem, pMassItem});
    }
}

//! Remove all the objects created
void RodSystemTableModel::clearContent()
{
    removeRows(0, rowCount());
}

//! Set the changed distances between supports
void RodSystemTableModel::setChangedData(QStandardItem* pItem)
{
    if (pItem->index().column() != 0)
        return;
    int iRow = pItem->index().row();
    QStandardItem* pRootItem = invisibleRootItem();
    QStandardItem* pChild = pRootItem->child(iRow, 0);
    std::vector<double> distances = mpRodSystem->distances();
    distances[iRow] = pChild->data(Qt::UserRole).toDouble();
    mpRodSystem->setDistances(distances);
    emit modified();
}

//! Insert fresh rows after selected ones
void RodSystemTableModel::insertAfterSelected()
{
    const double kDefaultInsertValue = 1.0;
    const int kMaxNumRods = 10;

    // Check if the actual number of rods is higher than the maximal limit
    if (mpRodSystem->numRods() >= kMaxNumRods)
        return;
    // Obtain the selection
    QTableView* pParent = (QTableView*) parent();
    QItemSelectionModel* pSelectionModel = pParent->selectionModel();
    QModelIndexList itemIndices = pSelectionModel->selectedIndexes();
    // Insert after the first selected item
    std::vector<double> distances = mpRodSystem->distances();
    if (!itemIndices.empty() && !distances.empty())
    {
        int iRod = itemIndices[0].row();
        std::vector<double>::iterator it = distances.begin() + iRod;
        distances.insert(it, distances[iRod]);
    }
    else if (!distances.empty())
    {
        distances.push_back(distances.back());
    }
    else
    {
        distances.push_back(kDefaultInsertValue);
    }
    mpRodSystem->setDistances(distances);
    emit modified();
}

//! Remove the selected rows
void RodSystemTableModel::removeSelected()
{
    // Acquire the selection
    QTableView* pParent = (QTableView*) parent();
    QItemSelectionModel* pSelectionModel = pParent->selectionModel();
    QModelIndexList itemIndices = pSelectionModel->selectedIndexes();
    // Set the removing mask
    int numRods = mpRodSystem->numRods();
    std::vector<bool> mask(numRods);
    std::fill(mask.begin(), mask.end(), true);
    int numSelected = itemIndices.size();
    for (int i = 0; i != numSelected; ++i)
    {
        int iRow = itemIndices[i].row();
        mask[iRow] = false;
    }
    std::vector<double> distances = mpRodSystem->distances();
    // Copy the resulted distances
    std::vector<double> resultDistances;
    for (int i = 0; i != numRods; ++i)
    {
        if (mask[i])
            resultDistances.push_back(distances[i]);
    }
    // Discard the result which deletes all the rods
    if (resultDistances.empty())
        return;
    mpRodSystem->setDistances(resultDistances);
    emit modified();
}
