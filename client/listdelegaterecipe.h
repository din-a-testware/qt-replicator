#ifndef LISTDELEGATERECIPE_H
#define LISTDELEGATERECIPE_H
#include <QPainter>
#include <QAbstractItemDelegate>

class listDelegateRecipe: public QAbstractItemDelegate
{
public:
    listDelegateRecipe(QObject *parent = 0);

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    virtual ~listDelegateRecipe();
};

#endif // LISTDELEGATERECIPE_H
