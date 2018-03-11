#include "listdelegatenotes.h"

ListDelegateNotes::ListDelegateNotes(QObject *parent)
{
    Q_UNUSED(parent);
}

void ListDelegateNotes::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{

    QRect r = option.rect;

    if(option.state & QStyle::State_Selected) {
        painter->setRenderHint(QPainter::Antialiasing);
        painter->fillRect(r.left(),r.top(),15,r.height(),QColor::fromRgb(255,255,255));

        //!
        painter->fillRect(r.right()-30,r.top(),15,r.height(),QColor::fromRgb(255,255,255));

        painter->setBrush(QBrush(QColor::fromRgb(255,255,255)));
        QPen penHText(QColor("#ffffff"));
        penHText.setWidth(-1);
            painter->setPen(penHText);
        painter->drawPie(r.right(),r.top(),-30,r.height(),-90*16,180*16);

    } else {
        painter->setRenderHint(QPainter::Antialiasing);
        painter->fillRect(r.left(),r.top(),15,r.height(),QColor::fromRgb(204,102,102));

        //!
        painter->fillRect(r.right()-30,r.top(),15,r.height(),QColor::fromRgb(204,102,102));

        QPen penHText(QColor("#cc6666"));
        penHText.setWidth(-1);
        painter->setPen(penHText);
        painter->setBrush(QBrush(QColor::fromRgb(204,102,102)));
        painter->drawPie(r.right(),r.top(),-30,r.height(),-90*16,180*16);
    }

    QString title = index.data(Qt::DisplayRole).toString();
    r = option.rect.adjusted(0, 0, 0, 0);
    QPen penHText(QColor("#ffffff"));
    painter->setPen(penHText);
    painter->setFont( QFont( "Helvetica Ultra Compressed", 16, QFont::Normal ) );
    painter->drawText(r.left()+25, r.top(), r.width()-40, r.height(), Qt::AlignVCenter|Qt::AlignLeft, title.toUpper(), &r);


}

QSize ListDelegateNotes::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QRect r = option.rect;
    return QSize(r.width(), 33); // very dumb value

}

ListDelegateNotes::~ListDelegateNotes()
{

}
