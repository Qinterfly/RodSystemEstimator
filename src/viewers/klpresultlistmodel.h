/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the KLPResultListModel class
 */

#ifndef KLPRESULTLISTMODEL_H
#define KLPRESULTLISTMODEL_H

#include <QStandardItemModel>
#include "appaliases.h"

namespace KLP
{
class Result;
}

namespace RSE
{

namespace Models
{

class KLPResultListModel : public QStandardItemModel
{
    Q_OBJECT

public:
    KLPResultListModel(Results& results, QObject* pParent = nullptr);
    void updateContent();
    void removeSelected();

private:
    void clearContent();

private:
    Results& mResults;
};

}

}

#endif // KLPRESULTLISTMODEL_H
