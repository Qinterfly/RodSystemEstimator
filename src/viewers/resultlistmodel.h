/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
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

private:
    void clearContent();

private:
    KLP::Results& mResults;
};

}

}

#endif // RESULTLISTMODEL_H
