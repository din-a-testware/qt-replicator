#include "listdelegate_mediadatabase.h"

ListDelegate_MediaDatabase::ListDelegate_MediaDatabase(QObject *parent)
{
        Q_UNUSED(parent);
}

void ListDelegate_MediaDatabase::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    QRect r = option.rect;

    //Color: #C4C4C4
    QPen linePen(QColor::fromRgb(211,211,211), 1, Qt::SolidLine);

    //Color: #005A83
    QPen lineMarkedPen(QColor::fromRgb(0,90,131), 1, Qt::SolidLine);

    //Color: #333
    QPen fontPen(QColor::fromRgb(51,51,51), 1, Qt::SolidLine);

    //Color: #fff
    QPen fontMarkedPen(Qt::white, 1, Qt::SolidLine);

    if(option.state & QStyle::State_Selected){
        /*QLinearGradient gradientSelected(r.left(),r.top(),r.left(),r.height()+r.top());
        gradientSelected.setColorAt(0.0, QColor::fromRgb(119,213,247));
        gradientSelected.setColorAt(0.9, QColor::fromRgb(27,134,183));
        gradientSelected.setColorAt(1.0, QColor::fromRgb(0,120,174));
        painter->setBrush(gradientSelected);
        painter->drawRect(r);

        //BORDER
        painter->setPen(lineMarkedPen);
        painter->drawLine(r.topLeft(),r.topRight());
        painter->drawLine(r.topRight(),r.bottomRight());
        painter->drawLine(r.bottomLeft(),r.bottomRight());
        painter->drawLine(r.topLeft(),r.bottomLeft());

        painter->setPen(fontMarkedPen);*/

        painter->setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addRoundedRect(QRectF(r.left(),r.top(),r.width(),r.height()),17,17);
        QPen penBorder(Qt::transparent,0);
        painter->setPen(penBorder);
        painter->fillPath(path,Qt::red);
        painter->drawPath(path);

    } else {
        /*//BACKGROUND
                    //ALTERNATING COLORS
        painter->setBrush( (index.row() % 2) ? Qt::white : QColor(20,20,252) );
        painter->drawRect(r);

        //BORDER
        painter->setPen(linePen);
        painter->drawLine(r.topLeft(),r.topRight());
        painter->drawLine(r.topRight(),r.bottomRight());
        painter->drawLine(r.bottomLeft(),r.bottomRight());
        painter->drawLine(r.topLeft(),r.bottomLeft());

        painter->setPen(fontPen);*/
        painter->setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addRoundedRect(QRectF(r.left(),r.top(),r.width(),r.height()),15,15);
        QPen penBorder(Qt::transparent,0);
        painter->setPen(penBorder);
        painter->fillPath(path,QColor("#ff9900"));
        painter->drawPath(path);
    }

    //GET TITLE, DESCRIPTION AND ICON
    QIcon ic = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QString title = index.data(Qt::DisplayRole).toString().toUpper();
    QString description = index.data(Qt::UserRole -1).toString();
    QString track /*= index.data(Qt::UserRole-1).toString()*/;

    /*int imageSpace = 10;
    if (!ic.isNull()) {
        //ICON
        r = option.rect.adjusted(5, 10, -10, -10);
        ic.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
        imageSpace = 55;
    }*/
    QPen blackPen(QColor("#000000"));
    painter->setPen(blackPen);

        r = option.rect.adjusted(20, 0, 70, 0);

        QPainterPath pathbRect;
        pathbRect.addRect(r.left(),r.top(),70,r.height());
        painter->fillPath(pathbRect,QColor("#000000"));
        painter->drawPath(pathbRect);
    //TITLE
    //r = option.rect.adjusted(imageSpace, 0, -10, -30);

    r = option.rect.adjusted(0, 0, 0, 0);
    QPen penHText(QColor("#00000"));
    painter->setPen(penHText);
    painter->setFont( QFont( "Helvetica Ultra Compressed", 20, QFont::Normal ) );
    painter->drawText(r.left()+100, r.top(), r.width(), r.height(), Qt::AlignVCenter|Qt::AlignLeft, title, &r);

    //DESCRIPTION
    QPen penW1Text(QColor("#ffffff"));
    painter->setPen(penW1Text);
    r = option.rect.adjusted(25, 0, -20, 0);
    painter->setFont( QFont( "Helvetica Ultra Compressed", 35, QFont::Normal ) );
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignVCenter|Qt::AlignLeft, description, &r);

    //ID3 TAG
    if (track != "") {

    /*QPen blackRect(QColor("#000000"));
    painter->setPen(blackRect);
    */

    painter->setPen(blackPen);

        r = option.rect.adjusted(20, 0, -75, 0);

        QPainterPath pathbRect;
        pathbRect.addRect(r.right(),r.top(),r.width(),r.height());
        painter->fillPath(pathbRect,QColor("#000000"));
        painter->drawPath(pathbRect);



    QPen penWText(QColor("#ffffff"));
    painter->setPen(penWText);
    r = option.rect.adjusted(0, 0, 0, 0);




    painter->setFont( QFont( "Helvetica Ultra Compressed", 35, QFont::Normal ) );
    painter->drawText(-r.left(), r.top(), r.width()+10, r.height(), Qt::AlignVCenter|Qt::AlignRight, track, &r);

    }
}

QSize ListDelegate_MediaDatabase::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(200, 35); // very dumb value
}

ListDelegate_MediaDatabase::~ListDelegate_MediaDatabase()
{

}
