#include "listdelegaterecipe.h"
#include <QDebug>

listDelegateRecipe::listDelegateRecipe(QObject *parent)
{
Q_UNUSED(parent);
}

void listDelegateRecipe::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{
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


        painter->setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addRoundedRect(QRectF(r.left(),r.top(),r.width(),62),30,30);
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
        path.addRoundedRect(QRectF(r.left(),r.top(),r.width(),62),30,30);
        QPen penBorder(Qt::transparent,0);
        painter->setPen(penBorder);
        painter->fillPath(path,QColor("#ff9900"));
        painter->drawPath(path);
    }

    //GET TITLE, DESCRIPTION AND ICON
    QIcon ic = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole + 1).toString();
    QString track = index.data(Qt::UserRole + 2).toString();

    /*int imageSpace = 10;
    if (!ic.isNull()) {
        //ICON
        r = option.rect.adjusted(5, 10, -10, -10);
        ic.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
        imageSpace = 55;
    }*/
    QPen blackPen(QColor("#000000"));
    painter->setPen(blackPen);

        r = option.rect.adjusted(45, 0, 0, 0);

        QPainterPath pathbRect;
        pathbRect.addRect(r.left()-5,r.top(),90,r.height());
        painter->fillPath(pathbRect,QColor("#000000"));
        painter->drawPath(pathbRect);


        QImage ufp_logo(":/images/UFP-Logo.png");
        QImage ResizedImage =
            ufp_logo.scaled(80, 62, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int posTop = r.top()+((62-ResizedImage.height())/2);
        painter->drawImage(QPoint(r.left(),posTop),ResizedImage);

    //TITLE
    //r = option.rect.adjusted(imageSpace, 0, -10, -30);

        QFont helvetica_Spaced("Helvetica Ultra Compressed",32);
        helvetica_Spaced.setLetterSpacing(QFont::PercentageSpacing,110);

    //r = option.rect.adjusted(0, 0, 0, 0);
    QPen penHText(QColor("#000000"));
    painter->setPen(penHText);
    painter->setFont( helvetica_Spaced);
   QFontMetrics fm(helvetica_Spaced);

   //qDebug() << fm.boundingRect(title.toUpper());
   //qDebug() << r.width()-20;

   /*QString text("some long text without elipsis");
   QFontMetrics metrics(label->font());
   QString elidedText = metrics.elidedText(text, Qt::ElideRight, label->width());
   label->setText(elidedText);
    */

   QString text(title.toUpper());
   int widthfont = r.width()-120;
      QFontMetrics metrics(helvetica_Spaced);
      QString elidedText = metrics.elidedText(text, Qt::ElideRight, widthfont);

    painter->drawText(r.left()+100, r.top(), r.width()-100, r.height(), Qt::AlignVCenter|Qt::AlignLeft|Qt::ElideRight, elidedText, &r);

}

QSize listDelegateRecipe::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(200, 62); // very dumb value
}

listDelegateRecipe::~listDelegateRecipe()
{

}
