#include "lcars_button.h"
#include <QPainter>
//#include <QMouseEvent>
#include <QDebug>
#include <QPushButton>
#include <QStyleOptionButton>

LCARS_Button::LCARS_Button(QWidget *parent) :
QPushButton(parent) {

    //typedef void (QWidget::*WidgetUpdateSlot)();
    switch (this->property("RedAlertState").toInt()) {
    case 0:
        repaint();
        break;
    case 1:
        repaint();
        break;
    case 2:
        repaint();
        break;

    default:
        break;
    }
}

Qt::Alignment LCARS_Button::getAlignment(int alignnumber) {

    switch (alignnumber) {
    case 0:
        return Qt::AlignLeft;
        break;
    case 1:
        return Qt::AlignRight;
        break;
    case 2:
        return Qt::AlignHCenter;
        break;
    case 3:
        return Qt::AlignJustify;
        break;
    case 4:
        return Qt::AlignTop;
        break;
    case 5:
        return Qt::AlignBottom;
        break;
    case 6:
        return Qt::AlignVCenter;
        break;
    case 7:
        return Qt::AlignBaseline;
        break;
    default:
        return Qt::AlignLeft;
        break;
    }

}

void LCARS_Button::paintEvent(QPaintEvent*) {


    QStyleOptionButton option;
        option.initFrom(this);
        option.state = isDown() ? QStyle::State_Sunken : QStyle::State_Raised;

    QPainter painter(this);
    painter.setBackgroundMode(Qt::TransparentMode);

    painter.setRenderHint(QPainter::Antialiasing,true);

    painter.setBrush(QBrush(QColor(this->property("Center_Color").toString())));
    painter.setPen(QPen(QColor(this->property("Center_Color").toString()),-1));


    //! [ -- SIZES -- ]

    //! { RECTANGLES }

    //! ( LEFT )

    QSize Size_TopLeft(this->property("Size_TopLeft").toSize().width()*2,
                       this->property("Size_TopLeft").toSize().height()*2);//Size_TopLeft(15*2,15*2);

    QSize Size_BottomLeft(this->property("Size_BottomLeft").toSize().width()*2,
                          this->property("Size_BottomLeft").toSize().height()*2);

    QRect rect_topLeftEdge(0,
                           0,
                           Size_TopLeft.width(),
                           Size_TopLeft.height());
    QRect rect_bottomLeftEdge(0,
                              this->height()-Size_BottomLeft.height(),
                              Size_BottomLeft.width(),
                              Size_BottomLeft.height());
    QRect leftRect(getRect(rect_topLeftEdge,rect_bottomLeftEdge,Qt::LeftEdge).at(0));

    int extendLeft = this->property("Size_extendEdgeLeft").toInt();//20;

    //! ( RIGHT )

    QSize Size_TopRight(this->property("Size_TopRight").toSize().width()*2,
                        this->property("Size_TopRight").toSize().height()*2);
    QSize Size_BottomRight(this->property("Size_BottomRight").toSize().width()*2,
                           this->property("Size_BottomRight").toSize().height()*2);

    QRect rect_topRightEdge(this->width()-Size_TopRight.width(),
                            0,
                            Size_TopRight.width(),
                            Size_TopRight.height());

    QRect rect_bottomRightEdge(this->width()-Size_BottomRight.width(),
                               this->height()-Size_BottomRight.height(),
                               Size_BottomRight.width(),
                               Size_BottomRight.height());
    QRect rightRect(getRect(rect_topRightEdge,rect_bottomRightEdge,Qt::RightEdge).at(0));

    int extendRight = this->property("Size_extendEdgeRight").toInt();//0;
    //! ( CENTER )

    QPoint leftCenter(extendLeft+getRect(rect_topLeftEdge,rect_bottomLeftEdge,Qt::LeftEdge).at(1).x()+getRect(rect_topLeftEdge,rect_bottomLeftEdge,Qt::LeftEdge).at(1).width(),0);
    QPoint rightCenter(getRect(rect_topRightEdge,rect_bottomRightEdge,Qt::RightEdge).at(1).x()+getRect(rect_topRightEdge,rect_bottomRightEdge,Qt::RightEdge).at(1).width()-extendRight,this->height());
    QRect centerRect(leftCenter,rightCenter);

    //! { DIVIDERS }

    int Divider_Left_Position = this->property("Divider_Left_Position").toInt();//0;
    int Divider_Left_Width = this->property("Divider_Left_Width").toInt();//40;

    int Divider_Right_Position = this->property("Divider_Right_Position").toInt();//0;
    int Divider_Right_Width = this->property("Divider_Right_Width").toInt();//0;

    QRect Divider_Left(centerRect.x()+Divider_Left_Position,0,Divider_Left_Width,this->height());
    QRect Divider_Right(centerRect.x()+centerRect.width()-Divider_Right_Position,0,-1*Divider_Right_Width,this->height());

    //! [ ----------- ]


    //! [ -- COLORS -- ]

    QColor Center_Color;
    QColor Edge_Left_Color;
    QColor Edge_Right_Color;
    QColor Divider_Left_Color;
    QColor Divider_Right_Color;
    QColor Left_Text_Color;
    QColor Center_Text_Color;
    QColor Right_Text_Color;

    int RedAlertState = this->property("RedAlertState").toInt();
    switch (RedAlertState) {
    case 0:
        //! { NORMAL }
        Center_Color = this->property("Center_Color").toString();
        Edge_Left_Color = this->property("Edge_Left_Color").toString();
        Edge_Right_Color = this->property("Edge_Right_Color").toString();
        Divider_Left_Color = this->property("Divider_Left_Color").toString();
        Divider_Right_Color = this->property("Divider_Right_Color").toString();
        Left_Text_Color = this->property("Left_Text_Color").toString();
        Center_Text_Color = this->property("Center_Text_Color").toString();
        Right_Text_Color = this->property("Right_Text_Color").toString();
        break;
    case 1:
        //! { RED }
        Center_Color = this->property("Red_Center_Color").toString();
        Edge_Left_Color = this->property("Red_Edge_Left_Color").toString();
        Edge_Right_Color = this->property("Red_Edge_Right_Color").toString();
        Divider_Left_Color = this->property("Red_Divider_Left_Color").toString();
        Divider_Right_Color = this->property("Red_Divider_Right_Color").toString();
        Left_Text_Color = this->property("Red_Left_Text_Color").toString();
        Center_Text_Color = this->property("Red_Center_Text_Color").toString();
        Right_Text_Color = this->property("Red_Right_Text_Color").toString();
        break;
    case 2:
        //! { WHITE }
        Center_Color = this->property("Active_Center_Color").toString();
        Edge_Left_Color = this->property("Active_Edge_Left_Color").toString();
        Edge_Right_Color = this->property("Active_Edge_Right_Color").toString();
        Divider_Left_Color = this->property("Active_Divider_Left_Color").toString();
        Divider_Right_Color = this->property("Active_Divider_Right_Color").toString();
        Left_Text_Color = this->property("Active_Left_Text_Color").toString();
        Center_Text_Color = this->property("Active_Center_Text_Color").toString();
        Right_Text_Color = this->property("Active_Right_Text_Color").toString();
        break;
    default:

        //! { NORMAL }
         Center_Color = this->property("Center_Color").toString();
         Edge_Left_Color = this->property("Edge_Left_Color").toString();
         Edge_Right_Color = this->property("Edge_Right_Color").toString();
         Divider_Left_Color = this->property("Divider_Left_Color").toString();
         Divider_Right_Color = this->property("Divider_Right_Color").toString();
         Left_Text_Color = this->property("Left_Text_Color").toString();
         Center_Text_Color = this->property("Center_Text_Color").toString();
         Right_Text_Color = this->property("Right_Text_Color").toString();

        break;
    }

    if (isDown()) {

        //! { PRESSED }
         Center_Color = this->property("Pressed_Center_Color").toString();
         Edge_Left_Color = this->property("Pressed_Edge_Left_Color").toString();
         Edge_Right_Color = this->property("Pressed_Edge_Right_Color").toString();
         Divider_Left_Color = this->property("Pressed_Divider_Left_Color").toString();
         Divider_Right_Color = this->property("Pressed_Divider_Right_Color").toString();
         Left_Text_Color = this->property("Pressed_Left_Text_Color").toString();
         Center_Text_Color = this->property("Pressed_Center_Text_Color").toString();
         Right_Text_Color = this->property("Pressed_Right_Text_Color").toString();
    }


    //! [ ------------ ]



    //! [ -- FONTS -- ]

    QFont Left_Text_Font(this->property("Left_Text_Font").toString()/*"Helvetica Ultra Compressed", 30, QFont::Normal */);
    QFont Center_Text_Font(this->font()/*"Helvetica Ultra Compressed", 20, QFont::Normal */);
    QFont Right_Text_Font(this->property("Right_Text_Font").toString()/*"Helvetica Ultra Compressed", 30, QFont::Normal */);

    int Text_Center_Distance_Right = this->property("Text_Center_Distance_Right").toInt(); //15;
    int Text_Center_Distance_Left = this->property("Text_Center_Distance_Left").toInt(); //0;


    int positioningCenter = this->property("positioningCenter").toInt();;


    switch (positioningCenter) {
    case Qt::AbsoluteSize:
        break;

    case Qt::RelativeSize:
        Text_Center_Distance_Left+=leftCenter.x()+Divider_Left_Width;
        break;
    default:
        break;
    }


    //QFont LeftTextFont(this->property("fontLeftText").toString());


    //! [ ----------- ]



    //! [ ==== LET'S DRAW EVERYTHING! ==== ]

    //! { ---- LEFT EDGE ---- }
    painter.setBrush(QBrush(Edge_Left_Color));

    //! (  TOP LEFT EDGE )
    painter.drawPie(rect_topLeftEdge,360*4,360*4);

    //! ( LEFT RECT )
    painter.fillRect(leftRect,Edge_Left_Color);

    //! ( BOTTOM LEFT EDGE )
    painter.drawPie(rect_bottomLeftEdge,360*8,360*4);

    //! ( EDGE FILLER RECTANGLE )
    painter.fillRect(getRect(rect_topLeftEdge,rect_bottomLeftEdge,Qt::LeftEdge).at(1),
                     Edge_Left_Color);

    //! ( EXTEND THE EDGE WITH THE GIVEN SIZE )
    painter.fillRect(centerRect.x()-extendLeft,0,extendLeft,this->height(),Edge_Left_Color);

    //! [ ---------------- ]



    //! [ ---- RIGHT EDGE ---- ]
    painter.setBrush(QBrush(Edge_Right_Color));

    //! ( TOP RIGHT EDGE )
    painter.drawPie(rect_topRightEdge,360*16,360*4);

    //! ( RIGHT RECT )
    painter.fillRect(rightRect,Edge_Right_Color);

    //! ( BOTTOM LEFT EDGE )
    painter.drawPie(rect_bottomRightEdge,360*12,360*4);

    //! ( EDGE FILLER RECTANGLE )
    painter.fillRect(getRect(rect_topRightEdge,rect_bottomRightEdge,Qt::RightEdge).at(1),Edge_Right_Color);

    //! ( EXTEND THE EDGE WITH THE GIVEN SIZE )
    painter.fillRect(centerRect.width()+centerRect.x(),0,extendRight,this->height(),Edge_Right_Color);

    //! [ ---------------- ]



    //! [ CENTER RECTANGLE ]
    painter.fillRect(centerRect,Center_Color);



    //! [ LEFT DIVIDER ]
    painter.eraseRect(Divider_Left);
    painter.fillRect(Divider_Left,Divider_Left_Color);

    //! [ RIGHT DIVIDER ]
    painter.eraseRect(Divider_Right);
    painter.fillRect(Divider_Right,Divider_Right_Color);


    int Center_Text_VAlign =getAlignment(this->property("Center_Text_VAlign").toInt());
    int Center_Text_HAlign = getAlignment(this->property("Center_Text_HAlign").toInt());

    int Left_Text_VAlign = getAlignment(this->property("Left_Text_VAlign").toInt());
    int Left_Text_HAlign = getAlignment(this->property("Left_Text_HAlign").toInt());
    //! [ -- TEXT -- ]

    //! [ CENTER TEXT ]
    painter.setFont(Center_Text_Font);
    painter.setPen(QPen(Center_Text_Color));
    painter.drawText(Text_Center_Distance_Left,
                     0,
                     this->width()-Text_Center_Distance_Right-Text_Center_Distance_Left,
                     this->height(),
                     Center_Text_VAlign|Center_Text_HAlign,
                     this->text());

    //! [ LEFT TEXT ]
    painter.setPen(QPen(Left_Text_Color));
    painter.setFont(Left_Text_Font);
    painter.drawText(centerRect.x(),0,Divider_Left_Width,this->height(),Left_Text_VAlign|Left_Text_HAlign,this->property("Text_Left").toString());

    //! [ RIGHT TEXT ]
    /*painter.setPen(QPen(Left_Text_Color));
    painter.setFont(Left_Text_Font);
    painter.drawText(centerRect.x(),0,Divider_Left_Width,this->height(),Qt::AlignVCenter|Qt::AlignHCenter,"52");*/

}

QVector<QRect> LCARS_Button::getRect(QRect rect_topLeftEdge, QRect rect_bottomLeftEdge, int edge) {

    QRect leftRect;
    QPoint topLeftPoint;
    QPoint bottomRightPoint;
    QVector<QRect> *returnVector = new QVector<QRect>;

    switch (edge) {
    case Qt::LeftEdge:
    {
        switch (getBigger(rect_topLeftEdge.width(),rect_bottomLeftEdge.width())) {
        case -1:

        {


           topLeftPoint= QPoint(rect_bottomLeftEdge.x(),rect_topLeftEdge.height()/2);
           bottomRightPoint = QPoint(rect_bottomLeftEdge.width()/2-1,(rect_bottomLeftEdge.y()+rect_bottomLeftEdge.height()/2)-1);
          // qDebug() << "brp-1" << rect_bottomLeftEdge.y()+rect_bottomLeftEdge.height()/2;

           leftRect = QRect(topLeftPoint,bottomRightPoint);
           returnVector->append(leftRect);

           QPoint bottomLeftPoint(rect_topLeftEdge.width()/2,0);
           //QPoint nochwas(30-1,15);
           QPoint nochwas(rect_bottomLeftEdge.width()/2-1,rect_topLeftEdge.height()/2-1);
           QRect fillRect(bottomLeftPoint,nochwas);
           returnVector->append(fillRect);
       }




            break;
        case 0:
         {


            topLeftPoint= QPoint(rect_topLeftEdge.x(),rect_topLeftEdge.height()/2);
            bottomRightPoint = QPoint(rect_topLeftEdge.width()/2-1,(rect_bottomLeftEdge.y()+rect_bottomLeftEdge.height()/2)-1);
            //qDebug() << "brp0" << rect_bottomLeftEdge.y()+rect_bottomLeftEdge.height()/2;
            leftRect = QRect(topLeftPoint,bottomRightPoint);
            returnVector->append(leftRect);

            QRect fillRect(rect_topLeftEdge.width()/2,0,0,0);
            returnVector->append(fillRect);
        }
            break;
        case 1:
        {
            topLeftPoint= QPoint(rect_topLeftEdge.x(),rect_topLeftEdge.height()/2);
            bottomRightPoint = QPoint(rect_topLeftEdge.width()/2-1,(rect_bottomLeftEdge.y()+rect_bottomLeftEdge.height()/2)-1);
            //qDebug() << "brp1" << rect_bottomLeftEdge.y()+rect_bottomLeftEdge.height()/2;
            leftRect = QRect(topLeftPoint,bottomRightPoint);
                   //qDebug() << "topLeft, x: " << topLeftPoint.x() << " y:"<< topLeftPoint.y();
                   //qDebug() << "bottomLeft, x: " << bottomRightPoint.x() << " y:"<< bottomRightPoint.y();

            returnVector->append(leftRect);

                QPoint bottomLeftPoint(rect_bottomLeftEdge.width()/2-1,this->height());
                QRect fillRect(bottomLeftPoint,bottomRightPoint);
            returnVector->append(fillRect);
         }
            break;
        default:
            break;
        }
    }

        break;

    case Qt::RightEdge:

    {

        switch (getBigger(rect_topLeftEdge.width(),rect_bottomLeftEdge.width())) {
        case -1:
        {
            topLeftPoint= QPoint(rect_bottomLeftEdge.x()+rect_bottomLeftEdge.width()/2,rect_topLeftEdge.height()/2);
            bottomRightPoint = QPoint(this->width(),(rect_bottomLeftEdge.y()+rect_bottomLeftEdge.height()/2)-1);
            //qDebug() << "brp1" << rect_bottomLeftEdge.y()+rect_bottomLeftEdge.height()/2;
            leftRect = QRect(topLeftPoint,bottomRightPoint);
            returnVector->append(leftRect);


            QPoint bottomLeftPoint(rect_topLeftEdge.x()+rect_topLeftEdge.width()/2,0);
            QPoint botright2(this->width()-rect_bottomLeftEdge.width()/2-1,rect_topLeftEdge.height()/2-1);
            QRect fillRect(bottomLeftPoint,botright2);
            returnVector->append(fillRect);

        }


            break;
        case 0:
         {


            topLeftPoint= QPoint(rect_topLeftEdge.x()+rect_topLeftEdge.width()/2,rect_topLeftEdge.height()/2);
            bottomRightPoint = QPoint(this->width(),(rect_bottomLeftEdge.y()+rect_bottomLeftEdge.height()/2)-1);
            //qDebug() << "brp1" << rect_bottomLeftEdge.y()+rect_bottomLeftEdge.height()/2;
            leftRect = QRect(topLeftPoint,bottomRightPoint);
            returnVector->append(leftRect);



            QRect fillRect(topLeftPoint.x(),0,0,0);
            returnVector->append(fillRect);
        }
            break;
        case 1:
        {
            topLeftPoint= QPoint(rect_topLeftEdge.x()+rect_topLeftEdge.width()/2,rect_topLeftEdge.height()/2);
            bottomRightPoint = QPoint(this->width(),(rect_bottomLeftEdge.y()+rect_bottomLeftEdge.height()/2)-1);
            //qDebug() << "brp1" << rect_bottomLeftEdge.y()+rect_bottomLeftEdge.height()/2;
            leftRect = QRect(topLeftPoint,bottomRightPoint);
            returnVector->append(leftRect);

            QPoint bottomLeftPoint(rect_topLeftEdge.x()+rect_topLeftEdge.width()/2,this->height());
            QPoint botright2(this->width()-rect_bottomLeftEdge.width()/2,this->height()-rect_bottomLeftEdge.height()/2-1);
            QRect fillRect(bottomLeftPoint,botright2);
            returnVector->append(fillRect);

        }
            break;
        default:
            break;
        }
    }


        break;
    default:
        break;
    }


return *returnVector;

}

int LCARS_Button::getBigger(int x, int y) {
    int checkval = x-y;

    //qDebug() << ((!!checkval) | (checkval >> (8 * sizeof (int) -1)));
    return ((!!checkval) | (checkval >> (8 * sizeof (int) -1)));
}

int LCARS_Button::getFontSize(QFont font, QString text,int alignment) {

    QFontMetrics fm(font);

    int pixelsWide = fm.width(text);
    int pixelsHigh = fm.height();

    switch (alignment) {
    case Qt::Horizontal:
        return pixelsWide;
        break;

    case Qt::Vertical:
        return pixelsHigh;
        break;
    default:
        return pixelsWide;
        break;
    }

}
