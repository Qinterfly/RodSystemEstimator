/*!
 * \file
 * \author Павел Лакиза
 * \date Июль 2022
 * \brief Объявление класса DoubleSpinBoxItemDelegate
 */

#ifndef DOUBLESPINBOXITEMDELEGATE_H
#define DOUBLESPINBOXITEMDELEGATE_H

#include <QStyledItemDelegate>

namespace RSE::Models
{

//! Class to specify how table values can be edited
class DoubleSpinBoxItemDelegate : public QStyledItemDelegate
{
public:
    DoubleSpinBoxItemDelegate(QObject* parent = nullptr);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* pEditor, const QModelIndex& index) const override;
    void setModelData(QWidget* pEditor, QAbstractItemModel* pModel, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* pEditor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

}

#endif // DOUBLESPINBOXITEMDELEGATE_H
