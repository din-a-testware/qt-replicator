#ifndef LISTDELEGATELANGUAGES_H
#define LISTDELEGATELANGUAGES_H

#include <QObject>
#include <QPainter>
#include <QAbstractItemDelegate>

class ListDelegateLanguages: public QAbstractItemDelegate
{
public:
    ListDelegateLanguages(QObject *parent=0);

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    virtual ~ListDelegateLanguages();
};

#endif // LISTDELEGATELANGUAGES_H
