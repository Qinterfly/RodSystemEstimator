/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of the KLPResultListModel class
 */

#ifndef RESULTLISTMODEL_H
#define RESULTLISTMODEL_H

#include <QStandardItemModel>
#include "klp/aliasklp.h"

namespace RSE
{

namespace Models
{

class ResultListModel : public QStandardItemModel
{
    Q_OBJECT

public:
    ResultListModel(KLP::Results& results, QObject* pParent = nullptr);
    void updateData();
    void updateContent();
    void removeSelected();
    void selectItem(int iSelect = -1);
    QColor resultColor(KLP::PointerResult pResult) const { return mResultColors[pResult.get()]; }

signals:
    void resultsUpdated();

private:
    void clearContent();
    QColor getAvailableColor();
    void specifyConnections();

private:
    KLP::Results& mResults;
    QMap<KLP::Result*, QColor> mResultColors;
};

}

}

#endif // RESULTLISTMODEL_H
