#ifndef LISTDELEGATENOTES_H
#define LISTDELEGATENOTES_H
#include <QPainter>
#include <QAbstractItemDelegate>

class ListDelegateNotes: public QAbstractItemDelegate
{
public:
    ListDelegateNotes(QObject *parent=0);
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    virtual ~ListDelegateNotes();
};

#endif // LISTDELEGATENOTES_H
