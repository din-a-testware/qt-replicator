#include "listdelegatelanguages.h"
#include <QPainter>
#include <QIcon>
#include <QPixmap>
#include <QDebug>
#include "extra_functions.h"

ListDelegateLanguages::ListDelegateLanguages(QObject *parent)
{Q_UNUSED(parent);

}

void ListDelegateLanguages::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{

    QRect r = option.rect;

    painter->setRenderHint(QPainter::Antialiasing);

    QImage baseImage(":/images/flags/flag_"+index.data(Qt::UserRole).toString()+".png");
    QImage overlayImage(":/images/UFP_LAYER.png");

    double overlayImageWidth = overlayImage.width();
    double overlayImageHeight = overlayImage.height();
    int actualoverlayImageWidth = int((overlayImageWidth/overlayImageHeight)*r.height());

    //QImage SideImage = extra_functions::createImageWithOverlay(baseImage, overlayImage,228,180);
    QImage SideImage = extra_functions::createImageWithOverlay(baseImage, overlayImage,
                                                               1,
                                                               actualoverlayImageWidth,r.height(),
                                                               130,130,
                                                               {},{},
                                                               {},16,
                                                               {},{},
                                                               true,false);


    QImage baseImageScaled = QImage(SideImage.scaled(actualoverlayImageWidth,r.height(),Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    painter->drawImage(r.left(),r.top(),baseImageScaled);


    if(option.state & QStyle::State_Selected) {
        painter->setRenderHint(QPainter::Antialiasing);

        //painter->fillRect(r.right(),r.top(),-10,r.height(),QColor::fromRgb(255,255,255));
        painter->setPen(QPen(QColor::fromRgb(255,255,255),-1));
        painter->setBrush(QBrush(QColor::fromRgb(255,255,255)));
        painter->drawPie(r.right()-30,r.top(),30,r.height(),-90*16,180*16);
        painter->drawRect(r.right()-r.height()/2-10,r.top(),10,r.height());

    } else {
        painter->setRenderHint(QPainter::Antialiasing);

        //painter->fillRect(r.right(),r.top(),-10,r.height(),QColor::fromRgb(204,102,102));
        painter->setPen(QPen(QColor::fromRgb(204,102,102),-1));
        painter->setBrush(QBrush(QColor::fromRgb(204,102,102)));
        painter->drawPie(r.right()-30,r.top(),30,r.height(),-90*16,180*16);
        painter->drawRect(r.right()-r.height()/2-10,r.top(),10,r.height());
    }


    /*switch (option.state) {
    case QStyle::State_Selected:

        //painter->pen().setWidth(0);
        painter->setPen(QPen(QColor("#ffffff")));
        painter->setBrush(QBrush(QColor::fromRgb(255,255,255)));
        painter->drawPie(r.right()-30,r.top(),30,r.height(),-90*16,180*16);
        painter->drawRect(r.right()-r.height()/2-10,r.top(),10,r.height());

        break;

    default:
        painter->setPen(QPen(QColor("#cc6666")));
        painter->setBrush(QBrush(QColor::fromRgb(255,255,255)));
        painter->drawPie(r.right()-30,r.top(),30,r.height(),-90*16,180*16);
        painter->drawRect(r.right()-r.height()/2-10,r.top(),10,r.height());
        break;
    }*/



    /*QPixmap image = QPixmap(":/images/UFP_LAYER.png");

    double imgw = image.width();
    double imgh = image.height();
    int actualImgW = int((imgw/imgh)*r.height());
    painter->drawPixmap(r.left(),r.top(),actualImgW,r.height(),image);*/

    QString title = index.data(Qt::DisplayRole).toString();
    r = option.rect.adjusted(0, 0, 0, 0);
    QPen penNText(QColor("#ffffff"));
    painter->setPen(penNText);
    painter->setFont( QFont( "Helvetica Ultra Compressed", 22, QFont::Normal ) );
    painter->drawText(actualoverlayImageWidth+5, r.top(), r.width()-40, r.height(), Qt::AlignVCenter|Qt::AlignLeft, title.toUpper(), &r);

}

QSize ListDelegateLanguages::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QRect r = option.rect;
    return QSize(r.width(), 30); // very dumb value
}

ListDelegateLanguages::~ListDelegateLanguages() {

}
