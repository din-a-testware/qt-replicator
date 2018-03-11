#include "lcars_sliders.h"
#include <QSlider>
#include <QMouseEvent>
#include <QPainter>
#include <QEvent>
#include <QDebug>
#include <math.h>

LCARS_Sliders::LCARS_Sliders(QWidget *parent) :
QSlider(parent)
{
Q_UNUSED(parent);
}
//! **    OTHER EVENTS       ** !//
void LCARS_Sliders::showEvent(QShowEvent *event){

Q_UNUSED(event);

}
void LCARS_Sliders::mouseReleaseEvent(QMouseEvent * event){
Q_UNUSED(event);
        all_handleBackground = QColor(this->property("all_handleBackground").toString());

        if (resultValue >= this->minimum() && resultValue <=this->maximum()) {
            this->setValue(resultValue);
            emit valueChanged(resultValue);
            emit sliderMoved(resultValue);
        }
    emit sliderReleased();

    this->repaint();


}
void LCARS_Sliders::mousePressEvent(QMouseEvent * e){



    switch (this->orientation()) {
    case Qt::Horizontal:
    {
        if(e->buttons() == Qt::LeftButton) {
             cursorPoint = this->mapFromGlobal(QCursor::pos());
             switch (this->invertedControls()) {
             case 0:
             {
                 p.setX(cursorPoint.x());
                 p.setY(cursorPoint.y());
             }
                 break;
             case 1:
             {
                 p.setX(this->width()-cursorPoint.x());
                 p.setY(cursorPoint.y());
             }
                 break;
             default:
                 break;
             }

             all_handleBackground = all_handleBackgroundPressed;

        }

        int maxVal = this->maximum();

        //! ((LOWER NUMBER)/(BIGGER NUMBER)*100)*(MAXIMUM/100)
        resultValue = (int)((((double)p.x()/(double)this->width())*100)*(double)maxVal/100);

        if (this->hasTracking() == true) {
            if (resultValue >= this->minimum() && resultValue <=this->maximum()) {
                this->setValue(resultValue);
                emit valueChanged(resultValue);
                emit sliderMoved(resultValue);
            }
        } else {

        }
    }
        break;
    case Qt::Vertical: {


        if(e->buttons() == Qt::LeftButton) {
             cursorPoint = this->mapFromGlobal(QCursor::pos());
             switch (this->invertedControls()) {
             case 0:
             {
                 p.setX(cursorPoint.x());
                 p.setY(cursorPoint.y());
             }
                 break;
             case 1:
             {
                 p.setX(cursorPoint.x());
                 p.setY(this->height()-cursorPoint.y());
             }
                 break;
             default:
                 break;
             }
        }

        int maxVal = this->maximum();

        //! ((LOWER NUMBER)/(BIGGER NUMBER)*100)*(MAXIMUM/100)
        resultValue = (int)((((double)p.y()/(double)this->height())*100)*(double)maxVal/100);

        if (this->hasTracking() == true) {
            if (resultValue >= this->minimum() && resultValue <=this->maximum()) {
                this->setValue(resultValue);
                emit valueChanged(resultValue);
                emit sliderMoved(resultValue);
            }
        }
    }
        break;
    default:
        break;
    }


    this->repaint();
}
void LCARS_Sliders::mouseMoveEvent(QMouseEvent *e){

    ////qDebug() << e->x() << e->y();

    switch (this->orientation()) {
    case Qt::Horizontal:
    {
        if(e->buttons() == Qt::LeftButton) {
             //cursorPoint = this->mapFromGlobal(QCursor::pos());
             switch (this->invertedControls()) {
             case 0:
             {
                 /*p.setX(cursorPoint.x());
                 p.setY(cursorPoint.y());*/
                 p.setX(e->x());
                 p.setY(e->y());
             }
                 break;
             case 1:
             {
                 /*p.setX(this->width()-cursorPoint.x());
                 p.setY(cursorPoint.y());*/

                 p.setX(this->width()-e->x());
                 p.setY(e->y());
             }
                 break;
             default:
                 break;
             }


        }

        int maxVal = this->maximum();

        //! ((LOWER NUMBER)/(BIGGER NUMBER)*100)*(MAXIMUM/100)
        resultValue = (int)((((double)p.x()/(double)this->width())*100)*(double)maxVal/100);

        if (this->hasTracking() == true) {
            if (resultValue >= this->minimum() && resultValue <=this->maximum()) {
                this->setValue(resultValue);
                emit valueChanged(resultValue);
                emit sliderMoved(resultValue);
            }
        }
    }
        break;
    case Qt::Vertical: {


        if(e->buttons() == Qt::LeftButton) {
             //cursorPoint = this->mapFromGlobal(QCursor::pos());
             switch (this->invertedControls()) {
             case 0:
             {
                 /*p.setX(cursorPoint.x());
                 p.setY(cursorPoint.y());*/

                 p.setX(e->x());
                 p.setY(e->y());
             }
                 break;
             case 1:
             {

                 /*p.setX(cursorPoint.x());
                 p.setY(this->height()-cursorPoint.y());*/

                 p.setX(e->x());
                 p.setY(this->height()-e->y());
             }
                 break;
             default:
                 break;
             }
        }

        int maxVal = this->maximum();

        //! ((LOWER NUMBER)/(BIGGER NUMBER)*100)*(MAXIMUM/100)
        resultValue = (int)((((double)p.y()/(double)this->height())*100)*(double)maxVal/100);

        if (this->hasTracking() == true) {
            if (resultValue >= this->minimum() && resultValue <=this->maximum()) {
                this->setValue(resultValue);
                emit valueChanged(resultValue);
                emit sliderMoved(resultValue);
            }
        }
    }
        break;
    default:
        break;
    }


    this->repaint();


}
//! **    END OTHER EVENTS   ** !//
void LCARS_Sliders::paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);
    //! NORMAL COLORS
         zero_barBackgroundTop = QColor(this->property("zero_barBackgroundTop").toString());
         zero_barBackgroundBottom = QColor(this->property("zero_barBackgroundBottom").toString());
         all_handleBackground = QColor(this->property("all_handleBackground").toString());
         all_handleBackgroundPressed = QColor(this->property("all_handleBackgroundPressed").toString());
         all_handleBorderColor = QColor(this->property("all_handleBorderColor").toString());
         all_bgGradient_Start = QColor(this->property("all_bgGradient_Start").toString());
         all_bgGradient_End = QColor(this->property("all_bgGradient_End").toString());
         all_frontGradient_Start = QColor(this->property("all_frontGradient_Start").toString());
         all_frontGradient_End = QColor(this->property("all_frontGradient_End").toString());
         all_dividerColor = QColor(this->property("all_dividerColor").toString());
         two_barBackground = QColor(this->property("two_barBackground").toString());

        //! RED ALERT COLORS INACTIVE
         red_zero_barBackgroundTop = QColor(this->property("red_zero_barBackgroundTop").toString());
         red_zero_barBackgroundBottom = QColor(this->property("red_zero_barBackgroundBottom").toString());
         red_all_handleBackground = QColor(this->property("red_all_handleBackground").toString());
         red_all_handleBackgroundPressed = QColor(this->property("red_all_handleBackgroundPressed").toString());
         red_all_handleBorderColor = QColor(this->property("red_all_handleBorderColor").toString());
         red_all_bgGradient_Start = QColor(this->property("red_all_bgGradient_Start").toString());
         red_all_bgGradient_End = QColor(this->property("red_all_bgGradient_End").toString());
         red_all_frontGradient_Start = QColor(this->property("red_all_frontGradient_Start").toString());
         red_all_frontGradient_End = QColor(this->property("red_all_frontGradient_End").toString());
         red_all_dividerColor = QColor(this->property("red_all_dividerColor").toString());
         red_two_barBackground = QColor(this->property("red_two_barBackground").toString());

        //! RED ALERT COLORS ACTIVE
         red_active_zero_barBackgroundTop = QColor(this->property("red_active_zero_barBackgroundTop").toString());
         red_active_zero_barBackgroundBottom = QColor(this->property("red_active_zero_barBackgroundBottom").toString());
         red_active_all_handleBackground = QColor(this->property("red_active_all_handleBackground").toString());
         red_active_all_handleBackgroundPressed = QColor(this->property("red_active_all_handleBackgroundPressed").toString());
         red_active_all_handleBorderColor = QColor(this->property("red_active_all_handleBorderColor").toString());
         red_active_all_bgGradient_Start = QColor(this->property("red_active_all_bgGradient_Start").toString());
         red_active_all_bgGradient_End = QColor(this->property("red_active_all_bgGradient_End").toString());
         red_active_all_frontGradient_Start = QColor(this->property("red_active_all_frontGradient_Start").toString());
         red_active_all_frontGradient_End = QColor(this->property("red_active_all_frontGradient_End").toString());
         red_active_all_dividerColor = QColor(this->property("red_active_all_dividerColor").toString());
         red_active_two_barBackground = QColor(this->property("red_active_two_barBackground").toString());

        //! SIZES
         all_handleSize = QSize(this->property("all_handleSize").toSize());
         all_handleBorderSize = QSize(this->property("all_handleBorderSize").toSize());
         all_roundedEdge = QSize(this->property("all_roundedEdge").toSize());
         zero_barTopLine = QSize(this->property("zero_barTopLine").toSize());
         zero_barBottomLine = QSize(this->property("zero_barBottomLine").toSize());
         two_size_roundedHandle_pie = QSize(this->property("two_size_roundedHandle_pie").toSize());
         two_size_roundedHandle_rect = QSize(this->property("two_size_roundedHandle_rect").toSize());
         two_size_bar = QSize(this->property("two_size_bar").toSize());


        //! NUMBERS

         all_widthDash = this->property("all_widthDash").toInt();
         all_distanceDash = this->property("all_distanceDash").toInt();
         one_pieHeight = this->property("one_pieHeight").toInt();
         two_roundHandle_Bar_width = this->property("two_roundHandle_Bar_width").toInt();
         two_border_roundedHandleWidth = this->property("two_border_roundedHandleWidth").toInt();
         slider_Type = this->property("slider_Type").toInt();
         redAlertState = this->property("redAlertState").toInt();
         roundBorder = this->property("roundBorder").toInt();
         custom_distance =this->property("custom_distance").toInt();
         extra_distance_top = this->property("extra_distance_top").toInt();
         extra_distance_bottom = this->property("extra_distance_bottom").toInt();



    resultValue=this->value();
    QPainter painter(this);

    switch (redAlertState) {
    case 0:
    {

    }
        break;
    case 1:
    {
        zero_barBackgroundTop = red_zero_barBackgroundTop;
        zero_barBackgroundBottom = red_zero_barBackgroundBottom;
        all_handleBackground = red_all_handleBackground;
        all_handleBackgroundPressed = red_all_handleBackgroundPressed;
        all_handleBorderColor = red_all_handleBorderColor;
        all_bgGradient_Start = red_all_bgGradient_Start;
        all_bgGradient_End = red_all_bgGradient_End;
        all_frontGradient_Start = red_all_frontGradient_Start;
        all_frontGradient_End = red_all_frontGradient_End;
        all_dividerColor = red_all_dividerColor;
        two_barBackground = red_two_barBackground;
    }
        break;

    case 2:
    {
        zero_barBackgroundTop = red_active_zero_barBackgroundTop;
        zero_barBackgroundBottom = red_active_zero_barBackgroundBottom;
        all_handleBackground = red_active_all_handleBackground;
        all_handleBackgroundPressed = red_active_all_handleBackgroundPressed;
        all_handleBorderColor = red_active_all_handleBorderColor;
        all_bgGradient_Start = red_active_all_bgGradient_Start;
        all_bgGradient_End = red_active_all_bgGradient_End;
        all_frontGradient_Start = red_active_all_frontGradient_Start;
        all_frontGradient_End = red_active_all_frontGradient_End;
        all_dividerColor = red_active_all_dividerColor;
        two_barBackground = red_active_two_barBackground;
    }
        break;
    default:
        zero_barBackgroundTop = QColor(this->property("zero_barBackgroundTop").toString());
        zero_barBackgroundBottom = QColor(this->property("zero_barBackgroundBottom").toString());
        all_handleBackground = QColor(this->property("all_handleBackground").toString());
        all_handleBackgroundPressed = QColor(this->property("all_handleBackgroundPressed").toString());
        all_handleBorderColor = QColor(this->property("all_handleBorderColor").toString());
        all_bgGradient_Start = QColor(this->property("all_bgGradient_Start").toString());
        all_bgGradient_End = QColor(this->property("all_bgGradient_End").toString());
        all_frontGradient_Start = QColor(this->property("all_frontGradient_Start").toString());
        all_frontGradient_End = QColor(this->property("all_frontGradient_End").toString());
        all_dividerColor = QColor(this->property("all_dividerColor").toString());
        two_barBackground = QColor(this->property("two_barBackground").toString());
        break;
    }


    switch (slider_Type) {

    //! SLIDER TYPE ZERO
    case 0:
    {

        //! [ ## SLIDER ORIENTATION ## ]
        switch (orientation()) {

        //! [ --- HORIZONTAL --- ]
        case Qt::Horizontal:
        {

            //! [ INVERTED APPEAREANCE AND INVERTED CONTROLS HORIZONTAL]

            //! NORMAL APPEAREANCE & NORMAL CONTROLS
            if (this->invertedAppearance()==false && this->invertedControls()==false) {
                Type_0_horizontalBar_Normal(&painter);
                Type_0_horizontalHandle_Normal(&painter);
            }

            else

            //! NORMAL APPEAREANCE & INVERTED CONTROLS
            if (this->invertedAppearance()==false && this->invertedControls()==true) {
                Type_0_horizontalBar_Normal(&painter);
                Type_0_horizontalHandle_Normal(&painter);
            }

            else

            //! INVERTED APPEAREANCE & NORMAL CONTROLS
            if (this->invertedAppearance()==true && this->invertedControls()==false) {
                Type_0_horizontalBar_Normal(&painter);
                Type_0_horizontalHandle_Normal(&painter);
            }

            else

            //! INVERTED APPEAREANCE & INVERTED CONTROLS
            if (this->invertedAppearance()==true && this->invertedControls()==true) {
                Type_0_horizontalBar_Normal(&painter);
                Type_0_horizontalHandle_Normal(&painter);
            }


        }
            break;
        //! [ --- /HORIZONTAL --- ]

        //! [ --- VERTICAL --- ]
        case Qt::Vertical:
        {

            //! [ INVERTED APPEAREANCE AND INVERTED CONTROLS VERTICAL]

            //! NORMAL APPEAREANCE & NORMAL CONTROLS
            if (this->invertedAppearance()==false && this->invertedControls()==false) {
                Type_0_verticalBar_Normal(&painter);
                Type_0_verticalHandle_Normal(&painter);
            }

            else

            //! NORMAL APPEAREANCE & INVERTED CONTROLS
            if (this->invertedAppearance()==false && this->invertedControls()==true) {
                Type_0_verticalBar_Normal(&painter);
                Type_0_verticalHandle_Normal(&painter);
            }

            else

            //! INVERTED APPEAREANCE & NORMAL CONTROLS
            if (this->invertedAppearance()==true && this->invertedControls()==false) {
                Type_0_verticalBar_Normal(&painter);
                Type_0_verticalHandle_Normal(&painter);
            }

            else

            //! INVERTED APPEAREANCE & INVERTED CONTROLS
            if (this->invertedAppearance()==true && this->invertedControls()==true) {
                Type_0_verticalBar_Normal(&painter);
                Type_0_verticalHandle_Normal(&painter);
            }

        }
            break;
        //! [ --- /VERTICAL --- ]

        default:
            break;
        }
        //! [ ## /SLIDER ORIENTATION ## ]

    }
        break;
    //! /SLIDER TYPE ZERO


    //! SLIDER TYPE ONE
    case 1:
    {

        //! [ ## SLIDER ORIENTATION ## ]
        switch (orientation()) {

        //! [ --- HORIZONTAL --- ]
        case Qt::Horizontal:
        {

            //! [ INVERTED APPEAREANCE AND INVERTED CONTROLS HORIZONTAL]

            //! NORMAL APPEAREANCE & NORMAL CONTROLS
            if (this->invertedAppearance()==false && this->invertedControls()==false) {

                Type_1_horizontalBar_Normal(&painter);
                Type_1_horizontalHandle_Normal(&painter);
            }

            else

            //! NORMAL APPEAREANCE & INVERTED CONTROLS
            if (this->invertedAppearance()==false && this->invertedControls()==true) {

                Type_1_horizontalBar_Normal(&painter);
                Type_1_horizontalHandle_Normal(&painter);
            }

            else

            //! INVERTED APPEAREANCE & NORMAL CONTROLS
            if (this->invertedAppearance()==true && this->invertedControls()==false) {

                painter.translate(this->rect().topRight());
                painter.scale(-1.0,1.0);
                Type_1_horizontalBar_Normal(&painter);
                Type_1_horizontalHandle_Normal(&painter);
            }

            else

            //! INVERTED APPEAREANCE & INVERTED CONTROLS
            if (this->invertedAppearance()==true && this->invertedControls()==true) {

                /*p.setX(this->width()-cursorPoint.x());
                p.setY(cursorPoint.y());
                */
                ////qDebug() << "this";
                painter.translate(this->rect().topRight());
                painter.scale(-1.0,1.0);


                Type_1_horizontalBar_Normal(&painter);
                Type_1_horizontalHandle_Normal(&painter);

            }


        }
            break;
        //! [ --- /HORIZONTAL --- ]

        //! [ --- VERTICAL --- ]
        case Qt::Vertical:
        {

            //! [ INVERTED APPEAREANCE AND INVERTED CONTROLS VERTICAL]

            //! NORMAL APPEAREANCE & NORMAL CONTROLS
            if (this->invertedAppearance()==false && this->invertedControls()==false) {
                painter.translate(this->rect().bottomLeft());
                painter.scale(1.0,-1.0);

                Type_1_verticalBar_Normal(&painter);
                Type_1_verticalHandle_Normal(&painter);
            }

            else

            //! NORMAL APPEAREANCE & INVERTED CONTROLS
            if (this->invertedAppearance()==false && this->invertedControls()==true) {
                painter.translate(this->rect().bottomLeft());
                painter.scale(1.0,-1.0);


                Type_1_verticalBar_Normal(&painter);
                Type_1_verticalHandle_Normal(&painter);
            }

            else

            //! INVERTED APPEAREANCE & NORMAL CONTROLS
            if (this->invertedAppearance()==true && this->invertedControls()==false) {


                Type_1_verticalBar_Normal(&painter);
                Type_1_verticalHandle_Normal(&painter);
                //painter.restore();

            }

            else

            //! INVERTED APPEAREANCE & INVERTED CONTROLS
            if (this->invertedAppearance()==true && this->invertedControls()==true) {
                Type_1_verticalBar_Normal(&painter);
                Type_1_verticalHandle_Normal(&painter);
            }

        }
            break;
        //! [ --- /VERTICAL --- ]

        default:
            break;
        }
        //! [ ## /SLIDER ORIENTATION ## ]

    }
        break;
    //! /SLIDER TYPE ONE

    //! SLIDER TYPE TWO
    case 2:
    {
        switch (orientation()) {

        //! [ --- HORIZONTAL --- ]
        case Qt::Horizontal:
        {

            //! [ INVERTED APPEAREANCE AND INVERTED CONTROLS HORIZONTAL]

            //! NORMAL APPEAREANCE & NORMAL CONTROLS
            if (this->invertedAppearance()==false && this->invertedControls()==false) {
                Type_2_horizontalBar_Normal(&painter);
                Type_2_horizontalHandle_Normal(&painter);
            }

            else

            //! NORMAL APPEAREANCE & INVERTED CONTROLS
            if (this->invertedAppearance()==false && this->invertedControls()==true) {
                Type_2_horizontalBar_Normal(&painter);
                Type_2_horizontalHandle_Normal(&painter);
            }

            else

            //! INVERTED APPEAREANCE & NORMAL CONTROLS
            if (this->invertedAppearance()==true && this->invertedControls()==false) {
                Type_2_horizontalBar_Normal(&painter);
                Type_2_horizontalHandle_Normal(&painter);
            }

            else

            //! INVERTED APPEAREANCE & INVERTED CONTROLS
            if (this->invertedAppearance()==true && this->invertedControls()==true) {
                Type_2_horizontalBar_Normal(&painter);
                Type_2_horizontalHandle_Normal(&painter);
            }


        }
            break;
        //! [ --- /HORIZONTAL --- ]

        //! [ --- VERTICAL --- ]
        case Qt::Vertical:
        {

            //! [ INVERTED APPEAREANCE AND INVERTED CONTROLS VERTICAL]

            //! NORMAL APPEAREANCE & NORMAL CONTROLS
            if (this->invertedAppearance()==false && this->invertedControls()==false) {
                Type_2_verticalBar_Normal(&painter);
                Type_2_verticalHandle_Normal(&painter);
            }

            else

            //! NORMAL APPEAREANCE & INVERTED CONTROLS
            if (this->invertedAppearance()==false && this->invertedControls()==true) {
                painter.translate(this->rect().topRight());
                painter.scale(-1.0,1.0);

                Type_2_verticalBar_Normal(&painter);
                Type_2_verticalHandle_Normal(&painter);
            }

            else

            //! INVERTED APPEAREANCE & NORMAL CONTROLS
            if (this->invertedAppearance()==true && this->invertedControls()==false) {
                painter.translate(this->rect().topRight());
                painter.scale(-1.0,1.0);
                Type_2_verticalBar_Normal(&painter);
                Type_2_verticalHandle_Normal(&painter);
            }

            else

            //! INVERTED APPEAREANCE & INVERTED CONTROLS
            if (this->invertedAppearance()==true && this->invertedControls()==true) {
                painter.translate(this->rect().topRight());
                painter.scale(-1.0,1.0);

                Type_2_verticalBar_Normal(&painter);
                Type_2_verticalHandle_Normal(&painter);
            }

        }
            break;
        //! [ --- /VERTICAL --- ]

        default:
            break;
        }
        //! [ ## /SLIDER ORIENTATION ## ]
    }
        break;
    //! /SLIDER TYPE TWO

    //! SLIDER TYPE THREE
    case 3:
        {

            //! [ ## SLIDER ORIENTATION ## ]
            switch (orientation()) {

            //! [ --- HORIZONTAL --- ]
            case Qt::Horizontal:
            {

                //! [ INVERTED APPEAREANCE AND INVERTED CONTROLS HORIZONTAL]

                //! NORMAL APPEAREANCE & NORMAL CONTROLS
                if (this->invertedAppearance()==false && this->invertedControls()==false) {

                    Type_3_horizontalBar_Normal(&painter);
                    Type_3_horizontalHandle_Normal(&painter);
                }

                else

                //! NORMAL APPEAREANCE & INVERTED CONTROLS
                if (this->invertedAppearance()==false && this->invertedControls()==true) {

                    Type_3_horizontalBar_Normal(&painter);
                    Type_3_horizontalHandle_Normal(&painter);
                }

                else

                //! INVERTED APPEAREANCE & NORMAL CONTROLS
                if (this->invertedAppearance()==true && this->invertedControls()==false) {

                    painter.translate(this->rect().topRight());
                    painter.scale(-1.0,1.0);
                    Type_3_horizontalBar_Normal(&painter);
                    Type_3_horizontalHandle_Normal(&painter);
                }

                else

                //! INVERTED APPEAREANCE & INVERTED CONTROLS
                if (this->invertedAppearance()==true && this->invertedControls()==true) {

                    /*p.setX(this->width()-cursorPoint.x());
                    p.setY(cursorPoint.y());
                    */
                    ////qDebug() << "this";
                    painter.translate(this->rect().topRight());
                    painter.scale(-1.0,1.0);


                    Type_3_horizontalBar_Normal(&painter);
                    Type_3_horizontalHandle_Normal(&painter);

                }


            }
                break;
            //! [ --- /HORIZONTAL --- ]

            //! [ --- VERTICAL --- ]
            case Qt::Vertical:
            {

                //! [ INVERTED APPEAREANCE AND INVERTED CONTROLS VERTICAL]

                //! NORMAL APPEAREANCE & NORMAL CONTROLS
                if (this->invertedAppearance()==false && this->invertedControls()==false) {
                    painter.translate(this->rect().bottomLeft());
                    painter.scale(1.0,-1.0);

                    Type_3_verticalBar_Normal(&painter);
                    Type_3_verticalHandle_Normal(&painter);
                }

                else

                //! NORMAL APPEAREANCE & INVERTED CONTROLS
                if (this->invertedAppearance()==false && this->invertedControls()==true) {
                    painter.translate(this->rect().bottomLeft());
                    painter.scale(1.0,-1.0);


                    Type_3_verticalBar_Normal(&painter);
                    Type_3_verticalHandle_Normal(&painter);
                }

                else

                //! INVERTED APPEAREANCE & NORMAL CONTROLS
                if (this->invertedAppearance()==true && this->invertedControls()==false) {


                    Type_3_verticalBar_Normal(&painter);
                    Type_3_verticalHandle_Normal(&painter);
                    //painter.restore();

                }

                else

                //! INVERTED APPEAREANCE & INVERTED CONTROLS
                if (this->invertedAppearance()==true && this->invertedControls()==true) {
                    Type_3_verticalBar_Normal(&painter);
                    Type_3_verticalHandle_Normal(&painter);
                }

            }
                break;
            //! [ --- /VERTICAL --- ]

            default:
                break;
            }
            //! [ ## /SLIDER ORIENTATION ## ]

        }
        break;
    //! /SLIDER TYPE THREE
    default:
        break;
    }
}

//! **    END PAINT EVENT    ** !//









//! **    PAINTING FUNCTIONS  ** !//

/*! [ --- SLIDER TYPE ZERO --- ] */

//! THE BAR BACKGROUNDS

    //! HORIZONTAL

        //! NORMAL APPEARANCE
        void LCARS_Sliders::Type_0_horizontalBar_Normal(QPainter *painter){

            int heightTop = zero_barTopLine.height();
            int heightBottom = zero_barBottomLine.height();

            int y1 = (this->height()/2-heightTop)/2;
            int y2 = ((this->height()/2-heightBottom)/2)+this->height()/2;
            painter->fillRect(0,y1+1,this->width(),heightTop,zero_barBackgroundTop);
            painter->fillRect(0,y2+1,this->width(),heightBottom,zero_barBackgroundBottom);

        }

    //! VERTICAL

        //! NORMAL APPEARANCE
        void LCARS_Sliders::Type_0_verticalBar_Normal(QPainter *painter){
        Q_UNUSED(painter);
        }

//! THE HANDLES

    //! HORIZONTAL

        //! NORMAL APPEARANCE
        void LCARS_Sliders::Type_0_horizontalHandle_Normal(QPainter *painter){

            int handleWidth = all_handleSize.width();//20;
            int handleBorder = all_handleBorderSize.width(); //5;

            int handleY=0;


            int handleX = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->width()/100);

            if (handleX+handleWidth>=this->width()) {
                handleX=this->width()-(handleWidth);
            }

            painter->fillRect(handleX,handleY,handleWidth,this->height(),all_handleBackground);
            painter->fillRect(handleX-handleBorder,handleY,handleBorder,this->height(),all_handleBorderColor);
            painter->fillRect(handleX+handleWidth,handleY,handleBorder,this->height(),all_handleBorderColor);

        }

    //! VERTICAL

        //! NORMAL APPEARANCE
        void LCARS_Sliders::Type_0_verticalHandle_Normal(QPainter *painter){
        Q_UNUSED(painter);
        }

/*! [ ------------------------- ] */




        /*! [ --- TYPE ONE--- ] */

        //! THE BAR BACKGROUNDS

            //! HORIZONTAL

                //! NORMAL APPEARANCE
                void LCARS_Sliders::Type_1_horizontalBar_Normal(QPainter *painter){

                    //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK
                    int borderDistance;
                    switch (this->invertedAppearance()) {
                    case 0:
                    {
                        borderDistance=0;
                    }
                        break;

                    case 1:
                    {
                        borderDistance=-1;
                    }
                        break;
                    default:
                        break;
                    }

                    //! SWITCH BETWEEN A ROUND BORDER OR NO ROUND BORDER
                    switch (roundBorder) {
                    case 0:
                    {
                        QLinearGradient BackgroundGradient(rect().topLeft(), rect().bottomRight());
                        BackgroundGradient.setColorAt(0, all_bgGradient_Start); //QColor("#e66e1e")
                        BackgroundGradient.setColorAt(1, all_bgGradient_End); //QColor("#ba001e")
                        QBrush GradientBrush = QBrush(BackgroundGradient);
                        painter->fillRect(this->rect().topLeft().x(),this->rect().topLeft().y(),this->rect().width(),this->rect().height(),GradientBrush);
                    }
                        break;
                    case 1:
                    {


                        QLinearGradient BackgroundGradient(rect().topLeft(), rect().bottomRight());
                        BackgroundGradient.setColorAt(0, all_bgGradient_Start); //QColor("#e66e1e")
                        BackgroundGradient.setColorAt(1, all_bgGradient_End); //QColor("#ba001e")
                        QBrush GradientBrush = QBrush(BackgroundGradient);
                        painter->setRenderHint(QPainter::Antialiasing,true);
                        painter->setPen(QPen(BackgroundGradient,-1));
                        painter->fillRect(borderDistance,0,this->width()-all_roundedEdge.width(),this->height(),GradientBrush);
                        painter->fillRect(this->width()+borderDistance-all_roundedEdge.width(),all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                        painter->setBrush(GradientBrush);

                        painter->drawPie(this->width()+borderDistance-all_roundedEdge.width()*2,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);

                        painter->drawPie(this->width()+borderDistance-all_roundedEdge.width()*2,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);
                    }
                    case 2:
                    {
                        QLinearGradient BackgroundGradient(rect().topLeft(), rect().bottomRight());
                        BackgroundGradient.setColorAt(0, all_bgGradient_Start); //QColor("#e66e1e")
                        BackgroundGradient.setColorAt(1, all_bgGradient_End); //QColor("#ba001e")
                        QBrush GradientBrush = QBrush(BackgroundGradient);
                        painter->setRenderHint(QPainter::Antialiasing,true);
                        painter->setPen(QPen(BackgroundGradient,-1));

                        painter->fillRect(all_roundedEdge.width()+borderDistance,0,this->width()-all_roundedEdge.width()*2,this->height(),GradientBrush);

                        //! LEFT PIE:
                        painter->setBrush(GradientBrush);
                        painter->drawPie(this->width()+borderDistance-all_roundedEdge.width()*2,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);
                        painter->drawPie(this->width()+borderDistance-all_roundedEdge.width()*2,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);
                        painter->fillRect(this->width()+borderDistance-all_roundedEdge.width(),all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                        //! RIGHT PIE: bottom 8/4 4/4 top
                        painter->setBrush(GradientBrush);
                        painter->drawPie(borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                        painter->drawPie(borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                        painter->fillRect(borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);

                    }
                        break;
                    default:
                        break;
                    }


                }

            //! VERTICAL

                //! NORMAL APPEARANCE
                void LCARS_Sliders::Type_1_verticalBar_Normal(QPainter *painter){
                    //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK


                    int borderDistance;
                    switch (this->invertedAppearance()) {
                    case 0:
                    {
                        borderDistance=-1;
                    }
                        break;

                    case 1:
                    {
                        borderDistance=0;
                    }
                        break;
                    default:
                        break;
                    }

                    //! SWITCH BETWEEN A ROUND BORDER OR NO ROUND BORDER
                    switch (roundBorder) {
                    case 0:
                    {
                        QLinearGradient BackgroundGradient(rect().topLeft(), rect().bottomRight());
                        BackgroundGradient.setColorAt(0, all_bgGradient_Start); //QColor("#e66e1e")
                        BackgroundGradient.setColorAt(1, all_bgGradient_End); //QColor("#ba001e")
                        QBrush GradientBrush = QBrush(BackgroundGradient);
                        painter->fillRect(0,0,this->width(),this->height(),GradientBrush);
                    }
                        break;
                    case 1:
                    {


                        QLinearGradient BackgroundGradient(rect().topLeft(), rect().bottomRight());
                        BackgroundGradient.setColorAt(0, all_bgGradient_Start); //QColor("#e66e1e")
                        BackgroundGradient.setColorAt(1, all_bgGradient_End); //QColor("#ba001e")
                        QBrush GradientBrush = QBrush(BackgroundGradient);
                        painter->setRenderHint(QPainter::Antialiasing,true);
                        painter->setPen(QPen(BackgroundGradient,-1));
                        painter->setBrush(GradientBrush);

                        //! ROUND BORDER WITH FILLING RECTANGLE
                        painter->drawPie(0,this->height()+borderDistance-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                        painter->drawPie(this->width()-all_roundedEdge.width()*2,this->height()+borderDistance-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);
                        painter->fillRect(all_roundedEdge.width(),this->height()+borderDistance-all_roundedEdge.height(),this->width()-all_roundedEdge.width()*2,all_roundedEdge.height()*2,GradientBrush);
                        painter->fillRect(0,0+borderDistance,this->width(),this->height()-all_roundedEdge.height(),GradientBrush);

                    }
                        break;
                    case 2:
                    {
                        QLinearGradient BackgroundGradient(rect().topLeft(), rect().bottomRight());
                        BackgroundGradient.setColorAt(0, all_bgGradient_Start); //QColor("#e66e1e")
                        BackgroundGradient.setColorAt(1, all_bgGradient_End); //QColor("#ba001e")
                        QBrush GradientBrush = QBrush(BackgroundGradient);
                        painter->setRenderHint(QPainter::Antialiasing,true);
                        painter->setPen(QPen(BackgroundGradient,-1));
                        painter->setBrush(GradientBrush);

                        //! ROUND BORDER WITH FILLING RECTANGLE
                        painter->drawPie(0,this->height()+borderDistance-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                        painter->drawPie(this->width()-all_roundedEdge.width()*2,this->height()+borderDistance-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);
                        painter->fillRect(all_roundedEdge.width(),this->height()+borderDistance-all_roundedEdge.height(),this->width()-all_roundedEdge.width()*2,all_roundedEdge.height()*2,GradientBrush);

                        painter->drawPie(0,borderDistance,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                        painter->drawPie(this->width()-all_roundedEdge.width()*2,
                                         borderDistance,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);
                        painter->fillRect(all_roundedEdge.width(),borderDistance,
                                          this->width()-all_roundedEdge.width()*2,all_roundedEdge.height(),GradientBrush);
                        painter->fillRect(0,borderDistance+all_roundedEdge.height(),this->width(),this->height()-all_roundedEdge.height()*2,GradientBrush);

                    }
                        break;
                    default:
                        break;
                    }

                }

        //! THE HANDLES

            //! HORIZONTAL

                //! NORMAL APPEARANCE
                void LCARS_Sliders::Type_1_horizontalHandle_Normal(QPainter *painter){


                    //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK DUNNO WHY THIS IS NEEDED
                    int borderDistance;
                    Q_UNUSED(borderDistance);
                    int pieDistance;
                    switch (this->invertedAppearance()) {
                    case 0:
                    {
                        borderDistance=-1;
                        pieDistance=0;
                    }
                        break;

                    case 1:
                    {
                        borderDistance=-1;
                        pieDistance=1;
                    }
                        break;
                    default:
                        break;
                    }

                    //! GET POSITION OF THE HANDLE
                    int handleX = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->width()/100);
                    Q_UNUSED(handleX);

                    all_distanceDash = all_widthDash+custom_distance;

                    QLinearGradient FrontGradient(rect().topLeft(), rect().bottomRight());
                    FrontGradient.setColorAt(0, all_frontGradient_Start); //QColor("#e66e1e")
                    FrontGradient.setColorAt(1, all_frontGradient_End); //QColor("#ba001e")
                    QBrush GradientBrush = QBrush(FrontGradient);
                    painter->setRenderHint(QPainter::Antialiasing,true);
                    painter->setPen(QPen(FrontGradient,-1));
                    painter->setBrush(GradientBrush);
                    //! SWITCH BETWEEN A ROUND BORDER OR NO ROUND BORDER
                    switch (roundBorder) {
                    case 0:
                    {


                        int handleX = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->width()/100);

                        QLinearGradient ForegroundGradient(rect().topLeft(), rect().bottomRight());
                        ForegroundGradient.setColorAt(0, all_frontGradient_Start); //QColor("#e66e1e")
                        ForegroundGradient.setColorAt(1, all_frontGradient_End); //QColor("#ba001e")
                        QBrush GradientBrush = QBrush(ForegroundGradient);
                        painter->setRenderHint(QPainter::Antialiasing,true);
                        painter->setPen(QPen(ForegroundGradient,-1));

                        //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK
                        int borderDistance;
                        switch (this->invertedAppearance()) {
                        case 0:
                        {
                            borderDistance=0;
                        }
                            break;

                        case 1:
                        {
                            borderDistance=-1;
                        }
                            break;
                        default:
                            break;
                        }

                        //! SWITCH BETWEEN A ROUND BORDER OR NO ROUND BORDER
                        all_distanceDash = all_widthDash+custom_distance;
                        if (handleX==0) {

                        } else if (handleX<this->width()-
                                                all_roundedEdge.width()*2-
                                                custom_distance-
                                                all_widthDash*2+
                                                borderDistance-
                                                extra_distance_top+
                                                all_distanceDash*2) {
                            for (int i=extra_distance_bottom+borderDistance;i<=handleX;i++) {
                                painter->fillRect(i,0,custom_distance+all_widthDash,this->height(),GradientBrush);
                                i=i+custom_distance+all_widthDash*2-1;
                            }

                        } else if (handleX>=this->width()-
                                   all_roundedEdge.width()*2-
                                   custom_distance-
                                   all_widthDash*2+
                                   borderDistance-
                                   extra_distance_top+
                                   all_distanceDash*2) {
                            painter->fillRect(0+borderDistance,0,this->width(),this->height(),GradientBrush);
                        }

                        //! PAINT DIVIDERS OVER EVERYTHING

                        for (int i=extra_distance_bottom+borderDistance-all_widthDash;i<=this->width()-extra_distance_top;i++) {
                            painter->fillRect(i,0,5,this->height(),all_dividerColor);
                            i=i+custom_distance+all_widthDash*2-1;
                        }
                    }

                        break;

                    case 1:
                    {




                        //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK DUNNO WHY THIS IS NEEDED
                        int borderDistance;
                        int pieDistance;
                        switch (this->invertedAppearance()) {
                        case 0:
                        {
                            borderDistance=-1;
                            pieDistance=0;
                        }
                            break;

                        case 1:
                        {
                            borderDistance=-1;
                            pieDistance=1;
                        }
                            break;
                        default:
                            break;
                        }

                        //! GET POSITION OF THE HANDLE
                        int handleX = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->width()/100);

                        all_distanceDash = all_widthDash+custom_distance;

                        QLinearGradient FrontGradient(rect().topLeft(), rect().bottomRight());
                        FrontGradient.setColorAt(0, all_frontGradient_Start);
                        FrontGradient.setColorAt(1, all_frontGradient_End);
                        QBrush GradientBrush = QBrush(FrontGradient);
                        painter->setRenderHint(QPainter::Antialiasing,true);
                        painter->setPen(QPen(FrontGradient,-1));
                        painter->setBrush(GradientBrush);


                        if (handleX == 0) {

                        } else if (handleX<=this->width()-
                                                all_roundedEdge.width()*2-
                                                custom_distance-
                                                all_widthDash*2+
                                                borderDistance-
                                                extra_distance_top+
                                                all_distanceDash*2) {

                            for (int i=extra_distance_bottom+borderDistance;i<=handleX;i++) {


                                painter->fillRect(i,0,custom_distance+all_widthDash,this->height(),GradientBrush);
                                i=i+custom_distance+all_widthDash*2-1;

                            }

                        } else {
                            //! ROUND BORDER WITH FILLING RECTANGLE
                            //! TOP PIE:
                            painter->drawPie(this->width()-all_roundedEdge.width()*2-pieDistance,
                                             0,
                                             all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);

                            //! BOTTOM PIE:
                            painter->drawPie(this->width()-all_roundedEdge.width()*2-pieDistance,
                                             this->height()-all_roundedEdge.height()*2,
                                             all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);

                            painter->fillRect(this->width(),all_roundedEdge.height(),-all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);

                            //! THE REST:
                            painter->fillRect(0+borderDistance,0,this->width()-all_roundedEdge.width()+1,this->height(),GradientBrush);

                        }


                        //! PAINT DIVIDERS OVER EVERYTHING
                        //! int i=borderDistance+pieDistance-all_widthDash/2+all_roundedEdge.height()*2

                        for (int i=all_distanceDash+extra_distance_bottom+borderDistance;
                                 i<=this->width()-all_roundedEdge.width()-extra_distance_top-custom_distance;
                                 i++) {


                            painter->fillRect(i,0,all_widthDash,this->height(),all_dividerColor);

                            i=i+custom_distance+all_widthDash*2-1;

                        }








                    }
                        break;

                    case 2:
                    {
                        //! GET POSITION OF THE HANDLE
                        int handleX = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->width()/100);

                        QLinearGradient ForegroundGradient(rect().topLeft(), rect().bottomRight());
                        ForegroundGradient.setColorAt(0, all_frontGradient_Start); //QColor("#e66e1e")
                        ForegroundGradient.setColorAt(1, all_frontGradient_End); //QColor("#ba001e")
                        QBrush GradientBrush = QBrush(ForegroundGradient);
                        painter->setRenderHint(QPainter::Antialiasing,true);
                        painter->setPen(QPen(ForegroundGradient,-1));

                        //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK
                        int borderDistance;
                        switch (this->invertedAppearance()) {
                        case 0:
                        {
                            borderDistance=0;
                            pieDistance=1;
                        }
                            break;

                        case 1:
                        {
                            borderDistance=-1;
                            pieDistance=0;
                        }
                            break;
                        default:
                            break;
                        }

                        //! SWITCH BETWEEN A ROUND BORDER OR NO ROUND BORDER

                        /*

                        painter->fillRect(all_roundedEdge.width()+borderDistance,0,this->width()-all_roundedEdge.width()*2,this->height(),GradientBrush);

                        //! LEFT PIE:
                        painter->setBrush(GradientBrush);
                        painter->drawPie(this->width()+borderDistance-all_roundedEdge.width()*2,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);
                        painter->drawPie(this->width()+borderDistance-all_roundedEdge.width()*2,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);
                        painter->fillRect(this->width()+borderDistance-all_roundedEdge.width(),all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                        //! RIGHT PIE: bottom 8/4 4/4 top
                        painter->setBrush(GradientBrush);
                        painter->drawPie(borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                        painter->drawPie(borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                        painter->fillRect(borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);

                        */


                        all_distanceDash = all_widthDash+custom_distance;

                        //! PAINT THE BAR
                        //! ANFANGSWERTE
                        if (p.x()==0) {

                            if (handleX >= this->width()-all_roundedEdge.width()) {
                                painter->fillRect(0+borderDistance+all_roundedEdge.width(),0,this->width()-all_roundedEdge.width()*2,this->height(),GradientBrush);
                                painter->fillRect(this->width()-all_roundedEdge.width()+borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                                painter->setBrush(GradientBrush);
                                painter->drawPie(this->width()-all_roundedEdge.width()*2+borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);
                                painter->drawPie(this->width()-all_roundedEdge.width()*2+borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);

                                painter->drawPie(borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                                painter->drawPie(borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                                painter->fillRect(borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                            } else {

                                for (int i=all_roundedEdge.width()+extra_distance_bottom+borderDistance;i<=handleX-all_roundedEdge.width()-all_widthDash;i++) {
                                    painter->fillRect(i+borderDistance,0,all_distanceDash,this->height(),GradientBrush);
                                    i=i+all_distanceDash-all_widthDash+borderDistance;
                                }
                                painter->fillRect(all_roundedEdge.width()+extra_distance_bottom+borderDistance+borderDistance,0,-((all_roundedEdge.width()+extra_distance_bottom+borderDistance)-all_roundedEdge.width()),this->height(),GradientBrush);
                                painter->drawPie(borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                                painter->drawPie(borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                                painter->fillRect(borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                            }
                        }


                        //! BEI BEWEGUNG VOLL
                        else if (handleX>this->width()-all_roundedEdge.width()*2) {

                            painter->fillRect(0+borderDistance+all_roundedEdge.width(),0,this->width()-all_roundedEdge.width()*2,this->height(),GradientBrush);
                            painter->fillRect(this->width()-all_roundedEdge.width()+borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                            painter->setBrush(GradientBrush);
                            painter->drawPie(this->width()-all_roundedEdge.width()*2+borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);
                            painter->drawPie(this->width()-all_roundedEdge.width()*2+borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);

                            painter->drawPie(borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                            painter->drawPie(borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                            painter->fillRect(borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                        }

                        else if (handleX<=0) {

                        }
                        //! IRGENDWAS ZWISCHENDRIN
                        else /*if (p.x()>=handleX && p.x()<=this->width()-all_roundedEdge.width()-all_distanceDash)*/ {
                             ////qDebug() << p.x() << handleX << this->width()-all_roundedEdge.width()-all_distanceDash;
                            /*for (int i=0-all_distanceDash-all_widthDash/2+borderDistance;i<=handleX;i++) {
                                painter->fillRect(i-1,0,all_distanceDash,this->height(),GradientBrush);
                                i=i+all_distanceDash+all_widthDash/2+borderDistance;
                            }*/


                                for (int i=all_roundedEdge.width()+extra_distance_bottom+borderDistance+all_widthDash;i<=handleX;i++) {
                                    painter->fillRect(i+borderDistance,0,all_distanceDash+pieDistance,this->height(),GradientBrush);
                                    i=i+all_distanceDash+all_widthDash+borderDistance;
                                }
                                //painter->fillRect(all_roundedEdge.width()+borderDistance,0,all_distanceDash+all_widthDash-all_roundedEdge.width(),this->height(),GradientBrush);
                                painter->fillRect(all_roundedEdge.width()+extra_distance_bottom+borderDistance+borderDistance,0,-((all_roundedEdge.width()+extra_distance_bottom+borderDistance)-all_roundedEdge.width()),this->height(),GradientBrush);
                                painter->drawPie(borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                                painter->drawPie(borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                                painter->fillRect(borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);


                        }
                        //! PAINT DIVIDERS OVER EVERYTHING
                        for (int i=all_roundedEdge.width()+extra_distance_bottom+borderDistance;i<=this->width()-all_widthDash-all_roundedEdge.width()-extra_distance_top;i++) {
                            painter->fillRect(i+borderDistance,0,all_widthDash,this->height(),all_dividerColor);
                            i=i+all_distanceDash+all_widthDash+borderDistance;
                        }
                    }
                        break;
                    //! TESTING CASE
                    case 3:
                    {
                        painter->fillRect(0,0,this->width(),this->height(),QColor("#cc6666"));

                        int interval = tickInterval();
                           if (interval == 0)
                           {
                               interval = pageStep();
                           }
                            painter->setRenderHint(QPainter::Antialiasing,true);
                           if (tickPosition() != NoTicks)
                           {

                               QSize handle = QSize(0,this->height());
                               int tickwidth = 5;
                               Q_UNUSED(tickwidth);
                               for (int i = minimum(); i <= maximum(); i += interval)
                               {

                                   int x = round((double)((double)((double)(i - this->minimum()) / (double)(this->maximum() - this->minimum())) * (double)(this->width() - handle.width()) + (double)(handle.width() / 2.0))) - 1;
                                   int h = this->height();
                                   painter->setPen(QPen(QColor("#000000"),5));
                                   if (tickPosition() == TicksBothSides || tickPosition() == TicksAbove)
                                   {
                                       int y = this->rect().top();
                                       //painter->drawLine(x, y, x, y + h);
                                       painter->drawRect(x,y,0,this->height());
                                   }
                                   if (tickPosition() == TicksBothSides || tickPosition() == TicksBelow)
                                   {
                                       int y = this->rect().bottom();
                                       painter->drawLine(x, y, x, y - h);
                                   }

                               }
                           }
                    }
                        break;
                    default:
                        break;
                    }
                }

            //! VERTICAL

                //! NORMAL APPEARANCE
                void LCARS_Sliders::Type_1_verticalHandle_Normal(QPainter *painter){

                    //! SWITCH BETWEEN A ROUND BORDER OR NO ROUND BORDER
                    switch (roundBorder) {
                    case 0:
                    {
                        //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK DUNNO WHY THIS IS NEEDED
                        int borderDistance;
                        int pieDistance;
                        Q_UNUSED(pieDistance);
                        switch (this->invertedAppearance()) {
                        case 0:
                        {
                            borderDistance=-1;
                            pieDistance=0;
                        }
                            break;

                        case 1:
                        {
                            borderDistance=-1;
                            pieDistance=1;
                        }
                            break;
                        default:
                            break;
                        }

                        //! GET POSITION OF THE HANDLE
                        int handleY = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->height()/100);

                        QLinearGradient ForegroundGradient(rect().topLeft(), rect().bottomRight());
                        ForegroundGradient.setColorAt(0, all_frontGradient_Start); //QColor("#e66e1e")
                        ForegroundGradient.setColorAt(1, all_frontGradient_End); //QColor("#ba001e")
                        QBrush GradientBrush = QBrush(ForegroundGradient);

                        all_distanceDash = (all_widthDash+custom_distance)/1;

                        for (int i=0-all_distanceDash-all_widthDash/2;i<=handleY;i++) {
                            painter->fillRect(0,i+borderDistance*2,this->width(),all_distanceDash+borderDistance,GradientBrush);
                            i=i+all_distanceDash+all_widthDash/2;
                        }


                        //! PAINT DIVIDERS OVER EVERYTHING
                        for (int i=0-all_widthDash+borderDistance;i<=this->height()-all_widthDash;i++) {
                            painter->fillRect(0,i,this->width(),all_widthDash,all_dividerColor);
                            i=i+all_distanceDash+all_widthDash/2;
                        }
                    }
                        break;
                    case 1:
                    {

                        //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK DUNNO WHY THIS IS NEEDED
                        int borderDistance;
                        int pieDistance;
                        switch (this->invertedAppearance()) {
                        case 0:
                        {
                            borderDistance=-1;
                            pieDistance=0;
                        }
                            break;

                        case 1:
                        {
                            borderDistance=-1;
                            pieDistance=1;
                        }
                            break;
                        default:
                            break;
                        }

                        //! GET POSITION OF THE HANDLE
                        int handleY = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->height()/100);

                        all_distanceDash = all_widthDash+custom_distance;

                        QLinearGradient FrontGradient(rect().topLeft(), rect().bottomRight());
                        FrontGradient.setColorAt(0, all_frontGradient_Start); //QColor("#e66e1e")
                        FrontGradient.setColorAt(1, all_frontGradient_End); //QColor("#ba001e")
                        QBrush GradientBrush = QBrush(FrontGradient);
                        painter->setRenderHint(QPainter::Antialiasing,true);
                        painter->setPen(QPen(FrontGradient,-1));
                        painter->setBrush(GradientBrush);

                        if (handleY == 0) {

                        } else if (handleY<=this->height()-
                                                all_roundedEdge.height()*2-
                                                custom_distance-
                                                all_widthDash*2+
                                                borderDistance-
                                                extra_distance_top+
                                                all_distanceDash*2) {

                            for (int i=extra_distance_bottom+borderDistance;i<=handleY;i++) {


                                painter->fillRect(0,i,this->width(),custom_distance+all_widthDash,GradientBrush);
                                i=i+custom_distance+all_widthDash*2-1;

                            }

                        } else {
                            //! ROUND BORDER WITH FILLING RECTANGLE
                            //! TOP PIE:
                            painter->drawPie(0,
                                             this->height()+borderDistance-all_roundedEdge.height()*2+pieDistance,
                                             all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);

                            painter->drawPie(this->width()-all_roundedEdge.width()*2,
                                             this->height()+borderDistance-all_roundedEdge.height()*2+pieDistance,
                                             all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);

                            painter->fillRect(all_roundedEdge.width(),this->height()+borderDistance-all_roundedEdge.height(),this->width()-all_roundedEdge.width()*2,all_roundedEdge.height()*2,GradientBrush);

                            //! THE REST:
                            painter->fillRect(0,0+borderDistance+pieDistance,this->width(),this->height()-all_roundedEdge.height(),GradientBrush);

                        }

                        //! PAINT DIVIDERS OVER EVERYTHING
                        //! int i=borderDistance+pieDistance-all_widthDash/2+all_roundedEdge.height()*2

                        for (int i=all_distanceDash+extra_distance_bottom+borderDistance

                                    ;i<=this->height()-all_roundedEdge.height()-extra_distance_bottom-custom_distance;i++) {


                            painter->fillRect(0,i,this->width(),all_widthDash,all_dividerColor);

                            i=i+custom_distance+all_widthDash*2-1;

                        }


                    }
                        break;
                    case 2:
                    {
                        //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK DUNNO WHY THIS IS NEEDED
                        int borderDistance;
                        int pieDistance;
                        switch (this->invertedAppearance()) {
                        case 0:
                        {
                            borderDistance=-1;
                            pieDistance=0;
                        }
                            break;

                        case 1:
                        {
                            borderDistance=-1;
                            pieDistance=1;
                        }
                            break;
                        default:
                            break;
                        }

                        //! GET POSITION OF THE HANDLE
                        int handleY = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->height()/100);

                        all_distanceDash = all_widthDash+custom_distance;

                        QLinearGradient FrontGradient(rect().topLeft(), rect().bottomRight());
                        FrontGradient.setColorAt(0, all_frontGradient_Start); //QColor("#e66e1e")
                        FrontGradient.setColorAt(1, all_frontGradient_End); //QColor("#ba001e")
                        QBrush GradientBrush = QBrush(FrontGradient);
                        painter->setRenderHint(QPainter::Antialiasing,true);
                        painter->setPen(QPen(FrontGradient,-1));
                        painter->setBrush(GradientBrush);

                        if (handleY<=this->height()-all_roundedEdge.height()*2-custom_distance-extra_distance_bottom) {
                        //if (handleY<=this->height()-all_roundedEdge.height()*2-all_distanceDash+all_widthDash*2-extra_distance) {

                            for (int i=all_roundedEdge.height()+
                                        custom_distance-
                                        all_roundedEdge.height()+
                                        all_widthDash*2+
                                        extra_distance_top

                                        ;i<=handleY;i++) {


                                painter->fillRect(0,i,this->width(),custom_distance+all_widthDash,GradientBrush);
                                i=i+custom_distance+all_widthDash*2-1;

                            }

                            //! BOTTOM PIE:
                            if (handleY>0) {
                            painter->drawPie(0,borderDistance+pieDistance,
                                             all_roundedEdge.width()*2+pieDistance,
                                             all_roundedEdge.height()*2,
                                             360*4,360*4);

                            painter->drawPie(this->width()-all_roundedEdge.width()*2,
                                             borderDistance+pieDistance,
                                             all_roundedEdge.width()*2,
                                             all_roundedEdge.height()*2,
                                             360*16,360*4);

                            painter->fillRect(all_roundedEdge.width(),borderDistance+pieDistance,
                                              this->width()-all_roundedEdge.width()*2,
                                              all_roundedEdge.height(),
                                              GradientBrush);

                            painter->fillRect(0,
                                              all_roundedEdge.height()+borderDistance+pieDistance,
                                              this->width(),
                                              custom_distance-all_roundedEdge.height()+all_widthDash-borderDistance-pieDistance+extra_distance_top,
                                              GradientBrush);


                            }
                        } else {
                            //! ROUND BORDER WITH FILLING RECTANGLE
                            //! TOP PIE:
                            painter->drawPie(0,
                                             this->height()+borderDistance-all_roundedEdge.height()*2+pieDistance,
                                             all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);

                            painter->drawPie(this->width()-all_roundedEdge.width()*2,
                                             this->height()+borderDistance-all_roundedEdge.height()*2+pieDistance,
                                             all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);

                            painter->fillRect(all_roundedEdge.width(),this->height()+borderDistance-all_roundedEdge.height(),this->width()-all_roundedEdge.width()*2,all_roundedEdge.height()*2,GradientBrush);

                            //! BOTTOM PIE:
                            painter->drawPie(0,borderDistance+pieDistance,
                                             all_roundedEdge.width()*2,
                                             all_roundedEdge.height()*2,
                                             360*4,360*4);

                            painter->drawPie(this->width()-all_roundedEdge.width()*2,
                                             borderDistance+pieDistance,
                                             all_roundedEdge.width()*2,
                                             all_roundedEdge.height()*2,
                                             360*16,360*4);

                            painter->fillRect(all_roundedEdge.width(),borderDistance+pieDistance,
                                              this->width()-all_roundedEdge.width()*2,all_roundedEdge.height(),GradientBrush);

                            //! THE REST:
                            painter->fillRect(0,borderDistance+all_roundedEdge.height()+pieDistance,this->width(),this->height()-all_roundedEdge.height()*2,GradientBrush);

                        }

                        //! PAINT DIVIDERS OVER EVERYTHING
                        //! int i=borderDistance+pieDistance-all_widthDash/2+all_roundedEdge.height()*2
                        for (int i=-all_distanceDash-all_widthDash/2+borderDistance+pieDistance+1+all_distanceDash/2+all_widthDash/2+all_roundedEdge.height()*2/*0-all_widthDash+borderDistance*/;i<=this->height()-all_widthDash-all_roundedEdge.height()*2+all_distanceDash/2;i++) {
                            //painter->fillRect(0,i,this->width(),all_widthDash,all_dividerColor);
                            i=i+all_distanceDash+all_widthDash/2;
                        }

                        for (int i=all_roundedEdge.height()+
                                    custom_distance-
                                    all_roundedEdge.height()+
                                    all_widthDash+
                                    extra_distance_top

                                    ;i<=this->height()-all_roundedEdge.height()-extra_distance_bottom-custom_distance;i++) {


                            painter->fillRect(0,i,this->width(),all_widthDash,all_dividerColor);

                            i=i+custom_distance+all_widthDash*2-1;

                        }



                    }
                        break;
                    default:
                        break;
                    }
                }

        /*! [ ------------------------- ] */





                /*! [ --- SLIDER TYPE TWO --- ] */

                //! THE BAR BACKGROUNDS

                    //! HORIZONTAL

                        //! NORMAL APPEARANCE
                        void LCARS_Sliders::Type_2_horizontalBar_Normal(QPainter *painter){

                            int heightTop = zero_barTopLine.height();
                            int heightBottom = zero_barBottomLine.height();

                            int y1 = (this->height()/2-heightTop)/2;
                            int y2 = ((this->height()/2-heightBottom)/2)+this->height()/2;
                            painter->fillRect(0,y1+1,this->width(),heightTop,zero_barBackgroundTop);
                            painter->fillRect(0,y2+1,this->width(),heightBottom,zero_barBackgroundBottom);

                        }

                    //! VERTICAL

                        //! NORMAL APPEARANCE
                        void LCARS_Sliders::Type_2_verticalBar_Normal(QPainter *painter){
                            painter->fillRect(0,0,two_size_bar.width(),this->height(),two_barBackground);
                        }

                //! THE HANDLES

                    //! HORIZONTAL

                        //! NORMAL APPEARANCE
                        void LCARS_Sliders::Type_2_horizontalHandle_Normal(QPainter *painter){

                            int handleWidth = all_handleSize.width();//20;
                            int handleBorder = all_handleBorderSize.width(); //5;

                            int handleY=0;

                            int handleX = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->width()/100);

                            if (handleX+handleWidth>=this->width()) {
                                handleX=this->width()-(handleWidth);
                            }

                            painter->fillRect(handleX,handleY,handleWidth,this->height(),all_handleBackground);
                            painter->fillRect(handleX-handleBorder,handleY,handleBorder,this->height(),all_handleBorderColor);
                            painter->fillRect(handleX+handleWidth,handleY,handleBorder,this->height(),all_handleBorderColor);

                        }

                    //! VERTICAL

                        //! NORMAL APPEARANCE
                        void LCARS_Sliders::Type_2_verticalHandle_Normal(QPainter *painter){


                            int handleWidth = two_size_roundedHandle_rect.width();//20;
                            int handleHeight = two_size_roundedHandle_rect.height();
                            int handleBorder = all_handleBorderSize.width(); //5;
                            int pieHeight = two_size_roundedHandle_pie.height();
                            int pieWidth = two_size_roundedHandle_pie.width();
                            Q_UNUSED(handleBorder);

                            int handleX = 0;
                            int handleY = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->height()/100);


                            if (handleY+all_handleSize.height()+all_handleBorderSize.width()>this->height()) {
                                handleY=this->height()-(all_handleSize.height()-all_handleBorderSize.width());
                            }

                            painter->setRenderHint(QPainter::Antialiasing,true);
                            painter->setPen(QPen(all_handleBorderColor,all_handleBorderSize.width()*2));
                            painter->setBrush(QBrush(all_handleBackground));

                            // BACKGROUND PIES
                            painter->drawPie(handleWidth-pieWidth,
                                             handleY,
                                             pieWidth*2,
                                             pieHeight*2,
                                             360*16,360*4);

                            painter->drawPie(handleWidth-pieWidth,
                                             handleY+handleHeight-pieHeight*2,//handleY+pieHeight-all_handleBorderSize.width()*2,
                                             pieWidth*2,
                                             pieHeight*2,
                                             360*12,360*4);

                            // BACKGROUND FILLER RECT
                            painter->drawRect(handleWidth+pieWidth,
                                              handleY+pieHeight,
                                              -pieWidth,
                                              handleHeight-pieHeight*2);

                            // BACKGROUND HANDLE RECT
                            painter->drawRect(handleX,handleY,handleWidth,handleHeight);



                            // FRONT PIES
                            painter->setPen(QPen(all_handleBorderColor,-1));
                            painter->drawPie(handleWidth-pieWidth,
                                             handleY,
                                             pieWidth*2,
                                             pieHeight*2,
                                             360*16,360*4);

                            painter->drawPie(handleWidth-pieWidth,
                                             handleY+handleHeight-pieHeight*2,//handleY+pieHeight-all_handleBorderSize.width()*2,
                                             pieWidth*2,
                                             pieHeight*2,
                                             360*12,360*4);

                            // FRONT FILLER RECT
                            painter->fillRect(handleWidth+pieWidth,
                                              handleY+pieHeight,
                                              -pieWidth,
                                              handleHeight-pieHeight*2,
                                              all_handleBackground);

                            // FRONT HANDLE RECT
                            painter->fillRect(handleX,handleY,handleWidth,handleHeight,all_handleBackground);

                        }

                /*! [ ------------------------- ] */




                        /*! [ --- TYPE THREE--- ] */

                        //! THE BAR BACKGROUNDS

                            //! HORIZONTAL

                                //! NORMAL APPEARANCE
                                void LCARS_Sliders::Type_3_horizontalBar_Normal(QPainter *painter){

                                    //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK
                                    int borderDistance;
                                    switch (this->invertedAppearance()) {
                                    case 0:
                                    {
                                        borderDistance=0;
                                    }
                                        break;

                                    case 1:
                                    {
                                        borderDistance=-1;
                                    }
                                        break;
                                    default:
                                        break;
                                    }

                                    //! SWITCH BETWEEN A ROUND BORDER OR NO ROUND BORDER
                                    switch (roundBorder) {
                                    case 0:
                                    {
                                        QLinearGradient BackgroundGradient(rect().topLeft(), rect().bottomRight());
                                        BackgroundGradient.setColorAt(0, all_bgGradient_Start); //QColor("#e66e1e")
                                        BackgroundGradient.setColorAt(1, all_bgGradient_End); //QColor("#ba001e")
                                        QBrush GradientBrush = QBrush(BackgroundGradient);
                                        painter->fillRect(this->rect().topLeft().x(),this->rect().topLeft().y(),this->rect().width(),this->rect().height(),GradientBrush);
                                    }
                                        break;
                                    case 1:
                                    {


                                        QLinearGradient BackgroundGradient(rect().topLeft(), rect().bottomRight());
                                        BackgroundGradient.setColorAt(0, all_bgGradient_Start); //QColor("#e66e1e")
                                        BackgroundGradient.setColorAt(1, all_bgGradient_End); //QColor("#ba001e")
                                        QBrush GradientBrush = QBrush(BackgroundGradient);
                                        painter->setRenderHint(QPainter::Antialiasing,true);
                                        painter->setPen(QPen(BackgroundGradient,-1));
                                        painter->fillRect(borderDistance,0,this->width()-all_roundedEdge.width(),this->height(),GradientBrush);
                                        painter->fillRect(this->width()+borderDistance-all_roundedEdge.width(),all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                                        painter->setBrush(GradientBrush);

                                        painter->drawPie(this->width()+borderDistance-all_roundedEdge.width()*2,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);

                                        painter->drawPie(this->width()+borderDistance-all_roundedEdge.width()*2,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);
                                    }
                                    case 2:
                                    {
                                        QLinearGradient BackgroundGradient(rect().topLeft(), rect().bottomRight());
                                        BackgroundGradient.setColorAt(0, all_bgGradient_Start); //QColor("#e66e1e")
                                        BackgroundGradient.setColorAt(1, all_bgGradient_End); //QColor("#ba001e")
                                        QBrush GradientBrush = QBrush(BackgroundGradient);
                                        painter->setRenderHint(QPainter::Antialiasing,true);
                                        painter->setPen(QPen(BackgroundGradient,-1));

                                        painter->fillRect(all_roundedEdge.width()+borderDistance,0,this->width()-all_roundedEdge.width()*2,this->height(),GradientBrush);

                                        //! LEFT PIE:
                                        painter->setBrush(GradientBrush);
                                        painter->drawPie(this->width()+borderDistance-all_roundedEdge.width()*2,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);
                                        painter->drawPie(this->width()+borderDistance-all_roundedEdge.width()*2,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);
                                        painter->fillRect(this->width()+borderDistance-all_roundedEdge.width(),all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                                        //! RIGHT PIE: bottom 8/4 4/4 top
                                        painter->setBrush(GradientBrush);
                                        painter->drawPie(borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                                        painter->drawPie(borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                                        painter->fillRect(borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);

                                    }
                                        break;
                                    default:
                                        break;
                                    }


                                }

                            //! VERTICAL

                                //! NORMAL APPEARANCE
                                void LCARS_Sliders::Type_3_verticalBar_Normal(QPainter *painter){
                                    //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK


                                    int borderDistance;
                                    switch (this->invertedAppearance()) {
                                    case 0:
                                    {
                                        borderDistance=-1;
                                    }
                                        break;

                                    case 1:
                                    {
                                        borderDistance=0;
                                    }
                                        break;
                                    default:
                                        break;
                                    }

                                    //! SWITCH BETWEEN A ROUND BORDER OR NO ROUND BORDER
                                    switch (roundBorder) {
                                    case 0:
                                    {
                                        QLinearGradient BackgroundGradient(rect().topLeft(), rect().bottomRight());
                                        BackgroundGradient.setColorAt(0, all_bgGradient_Start); //QColor("#e66e1e")
                                        BackgroundGradient.setColorAt(1, all_bgGradient_End); //QColor("#ba001e")
                                        QBrush GradientBrush = QBrush(BackgroundGradient);
                                        painter->fillRect(0,0,this->width(),this->height(),GradientBrush);
                                    }
                                        break;
                                    case 1:
                                    {


                                        QLinearGradient BackgroundGradient(rect().topLeft(), rect().bottomRight());
                                        BackgroundGradient.setColorAt(0, all_bgGradient_Start); //QColor("#e66e1e")
                                        BackgroundGradient.setColorAt(1, all_bgGradient_End); //QColor("#ba001e")
                                        QBrush GradientBrush = QBrush(BackgroundGradient);
                                        painter->setRenderHint(QPainter::Antialiasing,true);
                                        painter->setPen(QPen(BackgroundGradient,-1));
                                        painter->setBrush(GradientBrush);

                                        //! ROUND BORDER WITH FILLING RECTANGLE
                                        painter->drawPie(0,this->height()+borderDistance-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                                        painter->drawPie(this->width()-all_roundedEdge.width()*2,this->height()+borderDistance-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);
                                        painter->fillRect(all_roundedEdge.width(),this->height()+borderDistance-all_roundedEdge.height(),this->width()-all_roundedEdge.width()*2,all_roundedEdge.height()*2,GradientBrush);
                                        painter->fillRect(0,0+borderDistance,this->width(),this->height()-all_roundedEdge.height(),GradientBrush);

                                    }
                                        break;
                                    case 2:
                                    {
                                        QLinearGradient BackgroundGradient(rect().topLeft(), rect().bottomRight());
                                        BackgroundGradient.setColorAt(0, all_bgGradient_Start); //QColor("#e66e1e")
                                        BackgroundGradient.setColorAt(1, all_bgGradient_End); //QColor("#ba001e")
                                        QBrush GradientBrush = QBrush(BackgroundGradient);
                                        painter->setRenderHint(QPainter::Antialiasing,true);
                                        painter->setPen(QPen(BackgroundGradient,-1));
                                        painter->setBrush(GradientBrush);

                                        //! ROUND BORDER WITH FILLING RECTANGLE
                                        painter->drawPie(0,this->height()+borderDistance-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                                        painter->drawPie(this->width()-all_roundedEdge.width()*2,this->height()+borderDistance-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);
                                        painter->fillRect(all_roundedEdge.width(),this->height()+borderDistance-all_roundedEdge.height(),this->width()-all_roundedEdge.width()*2,all_roundedEdge.height()*2,GradientBrush);

                                        painter->drawPie(0,borderDistance,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                                        painter->drawPie(this->width()-all_roundedEdge.width()*2,
                                                         borderDistance,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);
                                        painter->fillRect(all_roundedEdge.width(),borderDistance,
                                                          this->width()-all_roundedEdge.width()*2,all_roundedEdge.height(),GradientBrush);
                                        painter->fillRect(0,borderDistance+all_roundedEdge.height(),this->width(),this->height()-all_roundedEdge.height()*2,GradientBrush);

                                    }
                                        break;
                                    default:
                                        break;
                                    }

                                }

                        //! THE HANDLES

                            //! HORIZONTAL

                                //! NORMAL APPEARANCE
                                void LCARS_Sliders::Type_3_horizontalHandle_Normal(QPainter *painter){


                                    //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK DUNNO WHY THIS IS NEEDED
                                    int borderDistance;
                                    Q_UNUSED(borderDistance);
                                    int pieDistance;
                                    switch (this->invertedAppearance()) {
                                    case 0:
                                    {
                                        borderDistance=-1;
                                        pieDistance=0;
                                    }
                                        break;

                                    case 1:
                                    {
                                        borderDistance=-1;
                                        pieDistance=1;
                                    }
                                        break;
                                    default:
                                        break;
                                    }

                                    //! GET POSITION OF THE HANDLE
                                    int handleX = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->width()/100);
                                    Q_UNUSED(handleX);
                                    all_distanceDash = all_widthDash+custom_distance;

                                    QLinearGradient FrontGradient(rect().topLeft(), rect().bottomRight());
                                    FrontGradient.setColorAt(0, all_frontGradient_Start); //QColor("#e66e1e")
                                    FrontGradient.setColorAt(1, all_frontGradient_End); //QColor("#ba001e")
                                    QBrush GradientBrush = QBrush(FrontGradient);
                                    painter->setRenderHint(QPainter::Antialiasing,true);
                                    painter->setPen(QPen(FrontGradient,-1));
                                    painter->setBrush(GradientBrush);
                                    //! SWITCH BETWEEN A ROUND BORDER OR NO ROUND BORDER
                                    switch (roundBorder) {
                                    case 0:
                                    {


                                        int handleX = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->width()/100);

                                        QLinearGradient ForegroundGradient(rect().topLeft(), rect().bottomRight());
                                        ForegroundGradient.setColorAt(0, all_frontGradient_Start); //QColor("#e66e1e")
                                        ForegroundGradient.setColorAt(1, all_frontGradient_End); //QColor("#ba001e")
                                        QBrush GradientBrush = QBrush(ForegroundGradient);
                                        painter->setRenderHint(QPainter::Antialiasing,true);
                                        painter->setPen(QPen(ForegroundGradient,-1));

                                        //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK
                                        int borderDistance;
                                        switch (this->invertedAppearance()) {
                                        case 0:
                                        {
                                            borderDistance=0;
                                        }
                                            break;

                                        case 1:
                                        {
                                            borderDistance=-1;
                                        }
                                            break;
                                        default:
                                            break;
                                        }

                                        //! SWITCH BETWEEN A ROUND BORDER OR NO ROUND BORDER
                                        all_distanceDash = all_widthDash+custom_distance;
                                        if (handleX==0) {

                                        } else if (handleX<this->width()-
                                                                all_roundedEdge.width()*2-
                                                                custom_distance-
                                                                all_widthDash*2+
                                                                borderDistance-
                                                                extra_distance_top+
                                                                all_distanceDash*2) {
                                            for (int i=extra_distance_bottom+borderDistance;i<=handleX;i++) {
                                                painter->fillRect(i,0,custom_distance+all_widthDash,this->height(),GradientBrush);
                                                i=i+custom_distance+all_widthDash*2-1;
                                            }

                                        } else if (handleX>=this->width()-
                                                   all_roundedEdge.width()*2-
                                                   custom_distance-
                                                   all_widthDash*2+
                                                   borderDistance-
                                                   extra_distance_top+
                                                   all_distanceDash*2) {
                                            painter->fillRect(0+borderDistance,0,this->width(),this->height(),GradientBrush);
                                        }

                                        //! PAINT DIVIDERS OVER EVERYTHING

                                        double element=all_distanceDash+all_widthDash;
                                        ////qDebug() << all_distanceDash << all_widthDash;
                                        double anzahlObjekte;
                                        anzahlObjekte = (this->width()-all_widthDash*2)/(element);
                                        ////qDebug() << anzahlObjekte;
                                        double wasistrest=this->width()-((element)*anzahlObjekte-all_widthDash);
                                        ////qDebug() << wasistrest;

                                        double element2=element+(wasistrest/(anzahlObjekte-1));
                                        ////qDebug() << round(element2) << anzahlObjekte/wasistrest;
                                        ////qDebug() << (wasistrest/anzahlObjekte) << element2;
                                        for (double i=-all_widthDash;i<=this->width()-all_widthDash;i++) {
                                            painter->fillRect(i,0,all_widthDash,this->height(),all_dividerColor);
                                            i=i+element2+all_widthDash;
                                        }


                                        for (int i=extra_distance_bottom+borderDistance-all_widthDash;i<=this->width()-extra_distance_top;i++) {
                                            //painter->fillRect(i,0,5,this->height(),all_dividerColor);
                                            i=i+custom_distance+all_widthDash*2-1;
                                        }
                                    }

                                        break;

                                    case 1:
                                    {




                                        //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK DUNNO WHY THIS IS NEEDED
                                        int borderDistance;
                                        int pieDistance;
                                        switch (this->invertedAppearance()) {
                                        case 0:
                                        {
                                            borderDistance=-1;
                                            pieDistance=0;
                                        }
                                            break;

                                        case 1:
                                        {
                                            borderDistance=-1;
                                            pieDistance=1;
                                        }
                                            break;
                                        default:
                                            break;
                                        }

                                        //! GET POSITION OF THE HANDLE
                                        int handleX = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->width()/100);

                                        all_distanceDash = all_widthDash+custom_distance;

                                        QLinearGradient FrontGradient(rect().topLeft(), rect().bottomRight());
                                        FrontGradient.setColorAt(0, all_frontGradient_Start);
                                        FrontGradient.setColorAt(1, all_frontGradient_End);
                                        QBrush GradientBrush = QBrush(FrontGradient);
                                        painter->setRenderHint(QPainter::Antialiasing,true);
                                        painter->setPen(QPen(FrontGradient,-1));
                                        painter->setBrush(GradientBrush);


                                        if (handleX == 0) {

                                        } else if (handleX<=this->width()-
                                                                all_roundedEdge.width()*2-
                                                                custom_distance-
                                                                all_widthDash*2+
                                                                borderDistance-
                                                                extra_distance_top+
                                                                all_distanceDash*2) {

                                            for (int i=extra_distance_bottom+borderDistance;i<=handleX;i++) {


                                                painter->fillRect(i,0,custom_distance+all_widthDash,this->height(),GradientBrush);
                                                i=i+custom_distance+all_widthDash*2-1;

                                            }

                                        } else {
                                            //! ROUND BORDER WITH FILLING RECTANGLE
                                            //! TOP PIE:
                                            painter->drawPie(this->width()-all_roundedEdge.width()*2-pieDistance,
                                                             0,
                                                             all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);

                                            //! BOTTOM PIE:
                                            painter->drawPie(this->width()-all_roundedEdge.width()*2-pieDistance,
                                                             this->height()-all_roundedEdge.height()*2,
                                                             all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);

                                            painter->fillRect(this->width(),all_roundedEdge.height(),-all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);

                                            //! THE REST:
                                            painter->fillRect(0+borderDistance,0,this->width()-all_roundedEdge.width()+1,this->height(),GradientBrush);

                                        }


                                        //! PAINT DIVIDERS OVER EVERYTHING
                                        //! int i=borderDistance+pieDistance-all_widthDash/2+all_roundedEdge.height()*2

                                        for (int i=all_distanceDash+extra_distance_bottom+borderDistance;
                                                 i<=this->width()-all_roundedEdge.width()-extra_distance_top-custom_distance;
                                                 i++) {


                                            painter->fillRect(i,0,all_widthDash,this->height(),all_dividerColor);

                                            i=i+custom_distance+all_widthDash*2-1;

                                        }








                                    }
                                        break;

                                    case 2:
                                    {
                                        //! GET POSITION OF THE HANDLE
                                        int handleX = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->width()/100);

                                        QLinearGradient ForegroundGradient(rect().topLeft(), rect().bottomRight());
                                        ForegroundGradient.setColorAt(0, all_frontGradient_Start); //QColor("#e66e1e")
                                        ForegroundGradient.setColorAt(1, all_frontGradient_End); //QColor("#ba001e")
                                        QBrush GradientBrush = QBrush(ForegroundGradient);
                                        painter->setRenderHint(QPainter::Antialiasing,true);
                                        painter->setPen(QPen(ForegroundGradient,-1));

                                        //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK
                                        int borderDistance;
                                        switch (this->invertedAppearance()) {
                                        case 0:
                                        {
                                            borderDistance=0;
                                            pieDistance=1;
                                        }
                                            break;

                                        case 1:
                                        {
                                            borderDistance=-1;
                                            pieDistance=0;
                                        }
                                            break;
                                        default:
                                            break;
                                        }

                                        //! SWITCH BETWEEN A ROUND BORDER OR NO ROUND BORDER

                                        /*

                                        painter->fillRect(all_roundedEdge.width()+borderDistance,0,this->width()-all_roundedEdge.width()*2,this->height(),GradientBrush);

                                        //! LEFT PIE:
                                        painter->setBrush(GradientBrush);
                                        painter->drawPie(this->width()+borderDistance-all_roundedEdge.width()*2,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);
                                        painter->drawPie(this->width()+borderDistance-all_roundedEdge.width()*2,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);
                                        painter->fillRect(this->width()+borderDistance-all_roundedEdge.width(),all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                                        //! RIGHT PIE: bottom 8/4 4/4 top
                                        painter->setBrush(GradientBrush);
                                        painter->drawPie(borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                                        painter->drawPie(borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                                        painter->fillRect(borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);

                                        */


                                        all_distanceDash = all_widthDash+custom_distance;

                                        //! PAINT THE BAR
                                        //! ANFANGSWERTE
                                        if (p.x()==0) {

                                            if (handleX >= this->width()-all_roundedEdge.width()) {
                                                painter->fillRect(0+borderDistance+all_roundedEdge.width(),0,this->width()-all_roundedEdge.width()*2,this->height(),GradientBrush);
                                                painter->fillRect(this->width()-all_roundedEdge.width()+borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                                                painter->setBrush(GradientBrush);
                                                painter->drawPie(this->width()-all_roundedEdge.width()*2+borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);
                                                painter->drawPie(this->width()-all_roundedEdge.width()*2+borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);

                                                painter->drawPie(borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                                                painter->drawPie(borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                                                painter->fillRect(borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                                            } else {

                                                for (int i=all_roundedEdge.width()+extra_distance_bottom+borderDistance;i<=handleX-all_roundedEdge.width()-all_widthDash;i++) {
                                                    painter->fillRect(i+borderDistance,0,all_distanceDash,this->height(),GradientBrush);
                                                    i=i+all_distanceDash-all_widthDash+borderDistance;
                                                }
                                                painter->fillRect(all_roundedEdge.width()+extra_distance_bottom+borderDistance+borderDistance,0,-((all_roundedEdge.width()+extra_distance_bottom+borderDistance)-all_roundedEdge.width()),this->height(),GradientBrush);
                                                painter->drawPie(borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                                                painter->drawPie(borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                                                painter->fillRect(borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                                            }
                                        }


                                        //! BEI BEWEGUNG VOLL
                                        else if (handleX>this->width()-all_roundedEdge.width()*2) {

                                            painter->fillRect(0+borderDistance+all_roundedEdge.width(),0,this->width()-all_roundedEdge.width()*2,this->height(),GradientBrush);
                                            painter->fillRect(this->width()-all_roundedEdge.width()+borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                                            painter->setBrush(GradientBrush);
                                            painter->drawPie(this->width()-all_roundedEdge.width()*2+borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*16,360*4);
                                            painter->drawPie(this->width()-all_roundedEdge.width()*2+borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);

                                            painter->drawPie(borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                                            painter->drawPie(borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                                            painter->fillRect(borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);
                                        }

                                        else if (handleX<=0) {

                                        }
                                        //! IRGENDWAS ZWISCHENDRIN
                                        else /*if (p.x()>=handleX && p.x()<=this->width()-all_roundedEdge.width()-all_distanceDash)*/ {
                                             //qDebug() << p.x() << handleX << this->width()-all_roundedEdge.width()-all_distanceDash;
                                            /*for (int i=0-all_distanceDash-all_widthDash/2+borderDistance;i<=handleX;i++) {
                                                painter->fillRect(i-1,0,all_distanceDash,this->height(),GradientBrush);
                                                i=i+all_distanceDash+all_widthDash/2+borderDistance;
                                            }*/


                                                for (int i=all_roundedEdge.width()+extra_distance_bottom+borderDistance+all_widthDash;i<=handleX;i++) {
                                                    painter->fillRect(i+borderDistance,0,all_distanceDash+pieDistance,this->height(),GradientBrush);
                                                    i=i+all_distanceDash+all_widthDash+borderDistance;
                                                }
                                                //painter->fillRect(all_roundedEdge.width()+borderDistance,0,all_distanceDash+all_widthDash-all_roundedEdge.width(),this->height(),GradientBrush);
                                                painter->fillRect(all_roundedEdge.width()+extra_distance_bottom+borderDistance+borderDistance,0,-((all_roundedEdge.width()+extra_distance_bottom+borderDistance)-all_roundedEdge.width()),this->height(),GradientBrush);
                                                painter->drawPie(borderDistance,0,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*4,360*4);
                                                painter->drawPie(borderDistance,this->height()-all_roundedEdge.height()*2,all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);
                                                painter->fillRect(borderDistance,all_roundedEdge.height(),all_roundedEdge.width(),this->height()-all_roundedEdge.height()*2,GradientBrush);


                                        }
                                        //! PAINT DIVIDERS OVER EVERYTHING
                                        for (int i=all_roundedEdge.width()+extra_distance_bottom+borderDistance;i<=this->width()-all_widthDash-all_roundedEdge.width()-extra_distance_top;i++) {
                                            painter->fillRect(i+borderDistance,0,all_widthDash,this->height(),all_dividerColor);
                                            i=i+all_distanceDash+all_widthDash+borderDistance;
                                        }
                                    }
                                        break;
                                    //! TESTING CASE
                                    case 3:
                                    {
                                        painter->fillRect(0,0,this->width(),this->height(),QColor("#cc6666"));

                                        int interval = tickInterval();
                                           if (interval == 0)
                                           {
                                               interval = pageStep();
                                           }
                                            painter->setRenderHint(QPainter::Antialiasing,true);
                                           if (tickPosition() != NoTicks)
                                           {

                                               QSize handle = QSize(0,this->height());
                                               int tickwidth = 5;
                                               Q_UNUSED(tickwidth);
                                               for (int i = minimum(); i <= maximum(); i += interval)
                                               {

                                                   int x = round((double)((double)((double)(i - this->minimum()) / (double)(this->maximum() - this->minimum())) * (double)(this->width() - handle.width()) + (double)(handle.width() / 2.0))) - 1;
                                                   int h = this->height();
                                                   painter->setPen(QPen(QColor("#000000"),5));
                                                   if (tickPosition() == TicksBothSides || tickPosition() == TicksAbove)
                                                   {
                                                       int y = this->rect().top();
                                                       //painter->drawLine(x, y, x, y + h);
                                                       painter->drawRect(x,y,0,this->height());
                                                   }
                                                   if (tickPosition() == TicksBothSides || tickPosition() == TicksBelow)
                                                   {
                                                       int y = this->rect().bottom();
                                                       painter->drawLine(x, y, x, y - h);
                                                   }

                                               }
                                           }
                                    }
                                        break;
                                    default:
                                        break;
                                    }
                                }

                            //! VERTICAL

                                //! NORMAL APPEARANCE
                                void LCARS_Sliders::Type_3_verticalHandle_Normal(QPainter *painter){

                                    //! SWITCH BETWEEN A ROUND BORDER OR NO ROUND BORDER
                                    switch (roundBorder) {
                                    case 0:
                                    {
                                        //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK DUNNO WHY THIS IS NEEDED
                                        int borderDistance;
                                        int pieDistance;
                                        Q_UNUSED(pieDistance);
                                        switch (this->invertedAppearance()) {
                                        case 0:
                                        {
                                            borderDistance=-1;
                                            pieDistance=0;
                                        }
                                            break;

                                        case 1:
                                        {
                                            borderDistance=-1;
                                            pieDistance=1;
                                        }
                                            break;
                                        default:
                                            break;
                                        }

                                        //! GET POSITION OF THE HANDLE
                                        int handleY = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->height()/100);

                                        QLinearGradient ForegroundGradient(rect().topLeft(), rect().bottomRight());
                                        ForegroundGradient.setColorAt(0, all_frontGradient_Start); //QColor("#e66e1e")
                                        ForegroundGradient.setColorAt(1, all_frontGradient_End); //QColor("#ba001e")
                                        QBrush GradientBrush = QBrush(ForegroundGradient);

                                        all_distanceDash = (all_widthDash+custom_distance)/1;

                                        for (int i=0-all_distanceDash-all_widthDash/2;i<=handleY;i++) {
                                            painter->fillRect(0,i+borderDistance*2,this->width(),all_distanceDash+borderDistance,GradientBrush);
                                            i=i+all_distanceDash+all_widthDash/2;
                                        }


                                        //! PAINT DIVIDERS OVER EVERYTHING
                                        for (int i=0-all_widthDash+borderDistance;i<=this->height()-all_widthDash;i++) {
                                            painter->fillRect(0,i,this->width(),all_widthDash,all_dividerColor);
                                            i=i+all_distanceDash+all_widthDash/2;
                                        }
                                    }
                                        break;
                                    case 1:
                                    {

                                        //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK DUNNO WHY THIS IS NEEDED
                                        int borderDistance;
                                        int pieDistance;
                                        switch (this->invertedAppearance()) {
                                        case 0:
                                        {
                                            borderDistance=-1;
                                            pieDistance=0;
                                        }
                                            break;

                                        case 1:
                                        {
                                            borderDistance=-1;
                                            pieDistance=1;
                                        }
                                            break;
                                        default:
                                            break;
                                        }

                                        //! GET POSITION OF THE HANDLE
                                        int handleY = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->height()/100);

                                        all_distanceDash = all_widthDash+custom_distance;

                                        QLinearGradient FrontGradient(rect().topLeft(), rect().bottomRight());
                                        FrontGradient.setColorAt(0, all_frontGradient_Start); //QColor("#e66e1e")
                                        FrontGradient.setColorAt(1, all_frontGradient_End); //QColor("#ba001e")
                                        QBrush GradientBrush = QBrush(FrontGradient);
                                        painter->setRenderHint(QPainter::Antialiasing,true);
                                        painter->setPen(QPen(FrontGradient,-1));
                                        painter->setBrush(GradientBrush);

                                        if (handleY == 0) {

                                        } else if (handleY<=this->height()-
                                                                all_roundedEdge.height()*2-
                                                                custom_distance-
                                                                all_widthDash*2+
                                                                borderDistance-
                                                                extra_distance_top+
                                                                all_distanceDash*2) {

                                            for (int i=extra_distance_bottom+borderDistance;i<=handleY;i++) {


                                                painter->fillRect(0,i,this->width(),custom_distance+all_widthDash,GradientBrush);
                                                i=i+custom_distance+all_widthDash*2-1;

                                            }

                                        } else {
                                            //! ROUND BORDER WITH FILLING RECTANGLE
                                            //! TOP PIE:
                                            painter->drawPie(0,
                                                             this->height()+borderDistance-all_roundedEdge.height()*2+pieDistance,
                                                             all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);

                                            painter->drawPie(this->width()-all_roundedEdge.width()*2,
                                                             this->height()+borderDistance-all_roundedEdge.height()*2+pieDistance,
                                                             all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);

                                            painter->fillRect(all_roundedEdge.width(),this->height()+borderDistance-all_roundedEdge.height(),this->width()-all_roundedEdge.width()*2,all_roundedEdge.height()*2,GradientBrush);

                                            //! THE REST:
                                            painter->fillRect(0,0+borderDistance+pieDistance,this->width(),this->height()-all_roundedEdge.height(),GradientBrush);

                                        }

                                        //! PAINT DIVIDERS OVER EVERYTHING
                                        //! int i=borderDistance+pieDistance-all_widthDash/2+all_roundedEdge.height()*2

                                        for (int i=all_distanceDash+extra_distance_bottom+borderDistance

                                                    ;i<=this->height()-all_roundedEdge.height()-extra_distance_bottom-custom_distance;i++) {


                                            painter->fillRect(0,i,this->width(),all_widthDash,all_dividerColor);

                                            i=i+custom_distance+all_widthDash*2-1;

                                        }


                                    }
                                        break;
                                    case 2:
                                    {
                                        //! IF INVERTED APPEREANCE ITS ALL MOVED ONE PIXEL BACK DUNNO WHY THIS IS NEEDED
                                        int borderDistance;
                                        int pieDistance;
                                        switch (this->invertedAppearance()) {
                                        case 0:
                                        {
                                            borderDistance=-1;
                                            pieDistance=0;
                                        }
                                            break;

                                        case 1:
                                        {
                                            borderDistance=-1;
                                            pieDistance=1;
                                        }
                                            break;
                                        default:
                                            break;
                                        }

                                        //! GET POSITION OF THE HANDLE
                                        int handleY = ((int)((double)resultValue/(double)this->maximum()*100)*(double)this->height()/100);

                                        all_distanceDash = all_widthDash+custom_distance;

                                        QLinearGradient FrontGradient(rect().topLeft(), rect().bottomRight());
                                        FrontGradient.setColorAt(0, all_frontGradient_Start); //QColor("#e66e1e")
                                        FrontGradient.setColorAt(1, all_frontGradient_End); //QColor("#ba001e")
                                        QBrush GradientBrush = QBrush(FrontGradient);
                                        painter->setRenderHint(QPainter::Antialiasing,true);
                                        painter->setPen(QPen(FrontGradient,-1));
                                        painter->setBrush(GradientBrush);

                                        if (handleY<=this->height()-all_roundedEdge.height()*2-custom_distance-extra_distance_bottom) {
                                        //if (handleY<=this->height()-all_roundedEdge.height()*2-all_distanceDash+all_widthDash*2-extra_distance) {

                                            for (int i=all_roundedEdge.height()+
                                                        custom_distance-
                                                        all_roundedEdge.height()+
                                                        all_widthDash*2+
                                                        extra_distance_top

                                                        ;i<=handleY;i++) {


                                                painter->fillRect(0,i,this->width(),custom_distance+all_widthDash,GradientBrush);
                                                i=i+custom_distance+all_widthDash*2-1;

                                            }

                                            //! BOTTOM PIE:
                                            if (handleY>0) {
                                            painter->drawPie(0,borderDistance+pieDistance,
                                                             all_roundedEdge.width()*2+pieDistance,
                                                             all_roundedEdge.height()*2,
                                                             360*4,360*4);

                                            painter->drawPie(this->width()-all_roundedEdge.width()*2,
                                                             borderDistance+pieDistance,
                                                             all_roundedEdge.width()*2,
                                                             all_roundedEdge.height()*2,
                                                             360*16,360*4);

                                            painter->fillRect(all_roundedEdge.width(),borderDistance+pieDistance,
                                                              this->width()-all_roundedEdge.width()*2,
                                                              all_roundedEdge.height(),
                                                              GradientBrush);

                                            painter->fillRect(0,
                                                              all_roundedEdge.height()+borderDistance+pieDistance,
                                                              this->width(),
                                                              custom_distance-all_roundedEdge.height()+all_widthDash-borderDistance-pieDistance+extra_distance_top,
                                                              GradientBrush);


                                            }
                                        } else {
                                            //! ROUND BORDER WITH FILLING RECTANGLE
                                            //! TOP PIE:
                                            painter->drawPie(0,
                                                             this->height()+borderDistance-all_roundedEdge.height()*2+pieDistance,
                                                             all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*8,360*4);

                                            painter->drawPie(this->width()-all_roundedEdge.width()*2,
                                                             this->height()+borderDistance-all_roundedEdge.height()*2+pieDistance,
                                                             all_roundedEdge.width()*2,all_roundedEdge.height()*2,360*12,360*4);

                                            painter->fillRect(all_roundedEdge.width(),this->height()+borderDistance-all_roundedEdge.height(),this->width()-all_roundedEdge.width()*2,all_roundedEdge.height()*2,GradientBrush);

                                            //! BOTTOM PIE:
                                            painter->drawPie(0,borderDistance+pieDistance,
                                                             all_roundedEdge.width()*2,
                                                             all_roundedEdge.height()*2,
                                                             360*4,360*4);

                                            painter->drawPie(this->width()-all_roundedEdge.width()*2,
                                                             borderDistance+pieDistance,
                                                             all_roundedEdge.width()*2,
                                                             all_roundedEdge.height()*2,
                                                             360*16,360*4);

                                            painter->fillRect(all_roundedEdge.width(),borderDistance+pieDistance,
                                                              this->width()-all_roundedEdge.width()*2,all_roundedEdge.height(),GradientBrush);

                                            //! THE REST:
                                            painter->fillRect(0,borderDistance+all_roundedEdge.height()+pieDistance,this->width(),this->height()-all_roundedEdge.height()*2,GradientBrush);

                                        }

                                        //! PAINT DIVIDERS OVER EVERYTHING
                                        //! int i=borderDistance+pieDistance-all_widthDash/2+all_roundedEdge.height()*2
                                        for (int i=-all_distanceDash-all_widthDash/2+borderDistance+pieDistance+1+all_distanceDash/2+all_widthDash/2+all_roundedEdge.height()*2/*0-all_widthDash+borderDistance*/;i<=this->height()-all_widthDash-all_roundedEdge.height()*2+all_distanceDash/2;i++) {
                                            //painter->fillRect(0,i,this->width(),all_widthDash,all_dividerColor);
                                            i=i+all_distanceDash+all_widthDash/2;
                                        }

                                        for (int i=all_roundedEdge.height()+
                                                    custom_distance-
                                                    all_roundedEdge.height()+
                                                    all_widthDash+
                                                    extra_distance_top

                                                    ;i<=this->height()-all_roundedEdge.height()-extra_distance_bottom-custom_distance;i++) {


                                            painter->fillRect(0,i,this->width(),all_widthDash,all_dividerColor);

                                            i=i+custom_distance+all_widthDash*2-1;

                                        }



                                    }
                                        break;
                                    default:
                                        break;
                                    }
                                }

                        /*! [ ------------------------- ] */


        /*! [ --- PROTOTYPE ZERO --- ] */

        /*//! THE BAR BACKGROUNDS

            //! HORIZONTAL

                //! NORMAL APPEARANCE
                void LCARS_Sliders::Type_0_horizontalBar_Normal(QPainter *painter){

                }
                void LCARS_Sliders::Type_0_horizontalBar_Normal_InvertedControls(QPainter *painter){

                }

                //! INVERTED APPEARANCE
                void LCARS_Sliders::Type_0_horizontalBar_InvertedAppearance(QPainter *painter){

                }
                void LCARS_Sliders::Type_0_horizontalBar_InvertedAppearance_InvertedControls(QPainter *painter){

                }

            //! VERTICAL

                //! NORMAL APPEARANCE
                void LCARS_Sliders::Type_0_verticalBar_Normal(QPainter *painter){

                }
                void LCARS_Sliders::Type_0_verticalBar_Normal_InvertedControls(QPainter *painter){

                }

                //! INVERTED APPEARANCE
                void LCARS_Sliders::Type_0_verticalBar_InvertedAppearance(QPainter *painter){

                }
                void LCARS_Sliders::Type_0_verticalBar_InvertedAppearance_InvertedControls(QPainter *painter){

                }

        //! THE HANDLES

            //! HORIZONTAL

                //! NORMAL APPEARANCE
                void LCARS_Sliders::Type_0_horizontalHandle_Normal(QPainter *painter){

                }
                void LCARS_Sliders::Type_0_horizontalHandle_Normal_InvertedControls(QPainter *painter){

                }

                //! INVERTED APPEARANCE
                void LCARS_Sliders::Type_0_horizontalHandle_InvertedAppearance(QPainter *painter){

                }
                void LCARS_Sliders::Type_0_horizontalHandle_InvertedAppearance_InvertedControls(QPainter *painter){

                }

            //! VERTICAL

                //! NORMAL APPEARANCE
                void LCARS_Sliders::Type_0_verticalHandle_Normal(QPainter *painter){

                }
                void LCARS_Sliders::Type_0_verticalHandle_Normal_InvertedControls(QPainter *painter){

                }

                //! INVERTED APPEARANCE
                void LCARS_Sliders::Type_0_verticalHandle_InvertedAppearance(QPainter *painter){

                }
                void LCARS_Sliders::Type_0_verticalHandle_InvertedAppearance_InvertedControls(QPainter *painter){

                }
        */
        /*! [ ------------------------- ] */

//! **    END PAINTING FUNCTS ** !//
