#ifndef LISTDELEGATE_MEDIADATABASE_H
#define LISTDELEGATE_MEDIADATABASE_H

#include <QPainter>
#include <QAbstractItemDelegate>


class ListDelegate_MediaDatabase : public QAbstractItemDelegate
{
    public:
        ListDelegate_MediaDatabase(QObject *parent = 0);
        void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
        QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

        virtual ~ListDelegate_MediaDatabase();
};

#endif // LISTDELEGATE_MEDIADATABASE_H
