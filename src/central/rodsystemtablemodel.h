/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the RodSystemTableModel class
 */

#ifndef RODSYSTEMTABLEMODEL_H
#define RODSYSTEMTABLEMODEL_H

#include <QStandardItemModel>

namespace RSE
{

namespace Core
{
class RodSystem;
}

namespace Models
{

//! Table model to set and represent data of a rod system
class RodSystemTableModel : public QStandardItemModel
{
    Q_OBJECT

public:
    RodSystemTableModel(QObject* parent = nullptr);
    ~RodSystemTableModel() = default;
    void setRodSystem(Core::RodSystem* pRodSystem);
    void updateContent();
    void insertAfterSelected();
    void removeSelected();

signals:
    void modified();

private:
    void clearContent();
    void setChangedData(QStandardItem* pItem);

private:
    Core::RodSystem* mpRodSystem = nullptr;
};

}

}



#endif // RODSYSTEMTABLEMODEL_H
