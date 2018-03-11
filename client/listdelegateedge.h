#ifndef LISTDELEGATEEDGE_H
#define LISTDELEGATEEDGE_H
#include <QPainter>
#include <QAbstractItemDelegate>


class listdelegateEdge: public QAbstractItemDelegate
{
public:
    listdelegateEdge(QObject *parent=0);
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    virtual ~listdelegateEdge();

};

#endif // LISTDELEGATEEDGE_H
