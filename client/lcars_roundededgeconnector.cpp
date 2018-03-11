/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "lcars_roundededgeconnector.h"

#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

LCARS_RoundedEdgeConnector::LCARS_RoundedEdgeConnector(QWidget *parent) :
QWidget(parent)
{
    //typedef void (QWidget::*WidgetUpdateSlot)();

}

void LCARS_RoundedEdgeConnector::paintEvent(QPaintEvent *) {

    QPainter painter(this);


        painter.setRenderHint(QPainter::Antialiasing,true);
        painter.setPen(QPen(QColor(Qt::transparent),-1));

        int RedAlertState = this->property("RedAlertState").toInt();
        switch (RedAlertState) {
        case 0:
            painter.setBrush(QBrush( QColor(this->property("bgColor").toString() /*"#ff9900"*/) ));
            break;
        case 1:
            painter.setBrush(QBrush( QColor(this->property("RedAlertBgColor").toString() /*"#ff9900"*/) ));
            break;

        case 2:
            painter.setBrush(QBrush( QColor(this->property("Active_RedAlertBgColor").toString() /*"#ff9900"*/) ));
            break;
        default:
            break;
        }


        int bottomCurveHeight = this->property("bottomCurveHeight").toInt();//20;
        qreal bottomCurveLeftWidth = 2*this->property("bottomCurveLeftWidth").toDouble();//20;
        qreal bottomCurveRightWidth = 2*this->property("bottomCurveRightWidth").toDouble();//20;

        int rightCurveWidth = this->property("rightCurveWidth").toInt();//8;
        qreal rightCurveTopHeight = 2*this->property("rightCurveTopHeight").toDouble();//7;
        qreal rightCurveBottomHeight = 2*this->property("rightCurveBottomHeight").toDouble();//7;

        int orientation = this->property("orientation").toInt();
        switch (orientation) {
        case 0:
        {
            QPainterPath Curves;

            int outerCircleWidth =  this->property("outerCurveWidth").toInt(); //16;
            int outerCircleHeight = this->property("outerCurveHeight").toInt();//16;

            int outerCircleX = 0;
            int outerCircleY = 0;

            int innerCircleX = this->property("innerCurveX").toInt();//15;
            int innerCircleY = this->property("innerCurveY").toInt();//10;

            int innerCircleWidth = this->property("innerCurveWidth").toInt();//10;
            int innerCircleHeight = this->property("innerCurveHeight").toInt();//10;

            Curves.moveTo(outerCircleWidth/2,outerCircleHeight/2);
            Curves.arcTo(outerCircleX,outerCircleY,outerCircleWidth,outerCircleHeight,-90.0*2,-90.0);

            Curves.setFillRule(Qt::WindingFill);
            Curves.addRegion(QRect(outerCircleX,outerCircleHeight/2,this->width(),this->height()-outerCircleHeight/2-bottomCurveHeight));

            Curves.addRegion(QRect(outerCircleWidth/2,outerCircleY,this->width()-outerCircleWidth/2-rightCurveWidth,outerCircleHeight/2));

            Curves.moveTo(innerCircleX,innerCircleY);
            Curves.arcTo(innerCircleX-innerCircleWidth/2,innerCircleY-innerCircleHeight/2,innerCircleWidth,innerCircleHeight,90.0,90.0);
            Curves.moveTo(innerCircleX-innerCircleWidth/2,innerCircleY);
            Curves.addRegion(QRect(innerCircleX-innerCircleWidth/2,innerCircleY,this->width(),this->height()));

            //Curves.addRegion(QRect(innerCircleX,innerCircleY-innerCircleHeight/2,this->width(),innerCircleHeight/2));

            Curves.closeSubpath();
            painter.drawPath(Curves.simplified());
 //if (this->property("endCurves").toBool() == true) {

            //! { BOTTOM LEFT }
            painter.drawPie(0,this->height()-bottomCurveHeight*2,bottomCurveLeftWidth/*outerCircleWidth-innerCircleWidth/2*/,bottomCurveHeight*2,360*8,360*4);
            //! { BOTTOM RIGHT }
            int rectWidth = innerCircleX-innerCircleWidth/2-bottomCurveRightWidth/2-bottomCurveLeftWidth/2;
            painter.drawRect(bottomCurveLeftWidth/2,this->height()-bottomCurveHeight,rectWidth,bottomCurveHeight);
            painter.drawPie(innerCircleX-innerCircleWidth/2-bottomCurveRightWidth,this->height()-bottomCurveHeight*2,bottomCurveRightWidth/*outerCircleWidth-innerCircleWidth/2*/,bottomCurveHeight*2,360*12,360*4);


            painter.eraseRect(this->width()-rightCurveWidth,0,rightCurveWidth*2,this->height());


            qreal sizeY = innerCircleY-innerCircleHeight/2-rightCurveBottomHeight;
            qreal sizeRectRight = innerCircleY-innerCircleHeight/2-rightCurveBottomHeight;

            painter.drawPie(this->width()-rightCurveWidth*2,0,rightCurveWidth*2,rightCurveTopHeight,360*16,360*4);

            painter.drawPie(this->width()-rightCurveWidth*2,sizeY,rightCurveWidth*2,rightCurveBottomHeight,360*12,360*4);

            painter.drawRect(this->width()-rightCurveWidth,rightCurveTopHeight/2,rightCurveWidth,sizeRectRight);



//}

            painter.eraseRect(innerCircleX-innerCircleWidth/2,innerCircleY,this->width(),this->height());

            painter.eraseRect(innerCircleX,innerCircleY-innerCircleHeight/2,this->width(),this->height());
            //qDebug() << innerCircleY-innerCircleHeight/2;
        }
            break;
        case 1:
        {
            QMatrix matrix;

            //! [ VERTICAL FLIP (TO BOTTOM LEFT)]
               matrix.translate(0, this->height());
               matrix.rotate(0);
               matrix.scale(1, -1);
               painter.setMatrix(matrix);


            //! [ HORIZONTAL / VERTICAL FLIP (TO BOTTOM RIGHT)]
               matrix.translate(this->height(), 0);
               matrix.rotate(0);
               matrix.scale(-1, 1);
               //painter.setMatrix(matrix);

            //! [ HORIZONTAL FLIP (TO TOP RIGHT)]
               matrix.translate(0, this->width());
               matrix.rotate(0);
               matrix.scale(1, -1);
               //painter.setMatrix(matrix);

            QPainterPath Curves;
            //painter.shear(15,0);
            int outerCircleWidth = this->property("outerCurveWidth").toInt(); //16;
            int outerCircleHeight = this->property("outerCurveHeight").toInt();//16;

            int outerCircleX = 0;
            int outerCircleY = 0;

            int innerCircleX = this->property("innerCurveX").toInt();//15;
            int innerCircleY = this->property("innerCurveY").toInt();//10;

            int innerCircleWidth = this->property("innerCurveWidth").toInt();//10;
            int innerCircleHeight = this->property("innerCurveHeight").toInt();//10;

            Curves.moveTo(outerCircleWidth/2,outerCircleHeight/2);
            Curves.arcTo(outerCircleX,outerCircleY,outerCircleWidth,outerCircleHeight,-90.0*2,-90.0);

            Curves.setFillRule(Qt::WindingFill);
            Curves.addRegion(QRect(outerCircleX,outerCircleHeight/2,this->width(),this->height()-outerCircleHeight/2-bottomCurveHeight));
            Curves.addRegion(QRect(outerCircleWidth/2,outerCircleY,this->width(),outerCircleHeight/2));

            Curves.moveTo(innerCircleX,innerCircleY);
            Curves.arcTo(innerCircleX-innerCircleWidth/2,innerCircleY-innerCircleHeight/2,innerCircleWidth,innerCircleHeight,90.0,90.0);
            Curves.moveTo(innerCircleX-innerCircleWidth/2,innerCircleY);
            Curves.addRegion(QRect(innerCircleX-innerCircleWidth/2,innerCircleY,this->width(),this->height()));
            Curves.closeSubpath();
            painter.drawPath(Curves.simplified());

//            if (this->property("endCurves").toBool() == true) {

            //! { BOTTOM LEFT }
            painter.drawPie(0,this->height()-bottomCurveHeight*2,bottomCurveLeftWidth/*outerCircleWidth-innerCircleWidth/2*/,bottomCurveHeight*2,360*8,360*4);
            //! { BOTTOM RIGHT }
            int rectWidth = innerCircleX-innerCircleWidth/2-bottomCurveRightWidth/2-bottomCurveLeftWidth/2;
            painter.drawRect(bottomCurveLeftWidth/2,this->height()-bottomCurveHeight,rectWidth,bottomCurveHeight);
            painter.drawPie(innerCircleX-innerCircleWidth/2-bottomCurveRightWidth,this->height()-bottomCurveHeight*2,bottomCurveRightWidth/*outerCircleWidth-innerCircleWidth/2*/,bottomCurveHeight*2,360*12,360*4);


            painter.eraseRect(this->width()-rightCurveWidth,0,rightCurveWidth*2,this->height());


            qreal sizeY = innerCircleY-innerCircleHeight/2-rightCurveBottomHeight;
            qreal sizeRectRight = innerCircleY-innerCircleHeight/2-rightCurveBottomHeight;

            painter.drawPie(this->width()-rightCurveWidth*2,0,rightCurveWidth*2,rightCurveTopHeight,360*16,360*4);

            painter.drawPie(this->width()-rightCurveWidth*2,sizeY,rightCurveWidth*2,rightCurveBottomHeight,360*12,360*4);

            painter.drawRect(this->width()-rightCurveWidth,rightCurveTopHeight/2,rightCurveWidth,sizeRectRight);

//            }

            painter.eraseRect(innerCircleX-innerCircleWidth/2,innerCircleY,this->width(),this->height());

            painter.eraseRect(innerCircleX,innerCircleY-innerCircleHeight/2,this->width(),this->height());



        }
            break;
        case 2:
        {
            QMatrix matrix;

            //! [ VERTICAL FLIP (TO BOTTOM LEFT)]
               /*matrix.translate(0, this->height());
               matrix.rotate(0);
               matrix.scale(1, -1);*/
               //painter.setMatrix(matrix);


            //! [ HORIZONTAL / VERTICAL FLIP (TO BOTTOM RIGHT)]
               matrix.translate(this->width(), 0);
               matrix.rotate(0);
               matrix.scale(-1, 1);
               painter.setMatrix(matrix);

            //! [ HORIZONTAL FLIP (TO TOP RIGHT)]
               /*matrix.translate(0, this->width());
               matrix.rotate(0);
               matrix.scale(1, -1);*/
               //painter.setMatrix(matrix);

            QPainterPath Curves;
            //painter.shear(15,0);
            int outerCircleWidth = this->property("outerCurveWidth").toInt(); //16;
            int outerCircleHeight = this->property("outerCurveHeight").toInt();//16;

            int outerCircleX = 0;
            int outerCircleY = 0;

            int innerCircleX = this->property("innerCurveX").toInt();//15;
            int innerCircleY = this->property("innerCurveY").toInt();//10;

            int innerCircleWidth = this->property("innerCurveWidth").toInt();//10;
            int innerCircleHeight = this->property("innerCurveHeight").toInt();//10;

            Curves.moveTo(outerCircleWidth/2,outerCircleHeight/2);
            Curves.arcTo(outerCircleX,outerCircleY,outerCircleWidth,outerCircleHeight,-90.0*2,-90.0);

            Curves.setFillRule(Qt::WindingFill);
            Curves.addRegion(QRect(outerCircleX,outerCircleHeight/2,this->width(),this->height()-outerCircleHeight/2));
            Curves.addRegion(QRect(outerCircleWidth/2,outerCircleY,this->width(),outerCircleHeight/2));

            Curves.moveTo(innerCircleX,innerCircleY);
            Curves.arcTo(innerCircleX-innerCircleWidth/2,innerCircleY-innerCircleHeight/2,innerCircleWidth,innerCircleHeight,90.0,90.0);
            Curves.moveTo(innerCircleX-innerCircleWidth/2,innerCircleY);
            Curves.addRegion(QRect(innerCircleX-innerCircleWidth/2,innerCircleY,this->width(),this->height()));
            //Curves.addRegion(QRect(innerCircleX,innerCircleY-innerCircleHeight/2,this->width(),innerCircleHeight/2));

            Curves.closeSubpath();
            painter.drawPath(Curves.simplified());

 //if (this->property("endCurves").toBool() == true) {
            //! { BOTTOM LEFT }
            painter.drawPie(0,this->height()-bottomCurveHeight*2,bottomCurveLeftWidth/*outerCircleWidth-innerCircleWidth/2*/,bottomCurveHeight*2,360*8,360*4);
            //! { BOTTOM RIGHT }
            int rectWidth = innerCircleX-innerCircleWidth/2-bottomCurveRightWidth/2-bottomCurveLeftWidth/2;
            painter.drawRect(bottomCurveLeftWidth/2,this->height()-bottomCurveHeight,rectWidth,bottomCurveHeight);
            painter.drawPie(innerCircleX-innerCircleWidth/2-bottomCurveRightWidth,this->height()-bottomCurveHeight*2,bottomCurveRightWidth/*outerCircleWidth-innerCircleWidth/2*/,bottomCurveHeight*2,360*12,360*4);


            painter.eraseRect(this->width()-rightCurveWidth,0,rightCurveWidth*2,this->height());


            qreal sizeY = innerCircleY-innerCircleHeight/2-rightCurveBottomHeight;
            qreal sizeRectRight = innerCircleY-innerCircleHeight/2-rightCurveBottomHeight;

            painter.drawPie(this->width()-rightCurveWidth*2,0,rightCurveWidth*2,rightCurveTopHeight,360*16,360*4);

            painter.drawPie(this->width()-rightCurveWidth*2,sizeY,rightCurveWidth*2,rightCurveBottomHeight,360*12,360*4);

            painter.drawRect(this->width()-rightCurveWidth,rightCurveTopHeight/2,rightCurveWidth,sizeRectRight);

//}
            painter.eraseRect(innerCircleX-innerCircleWidth/2,innerCircleY,this->width(),this->height());

            painter.eraseRect(innerCircleX,innerCircleY-innerCircleHeight/2,this->width(),this->height());
        }
            break;
        case 3:
        {
            QMatrix matrix;


            //! [ VERTICAL FLIP (TO BOTTOM LEFT)]
               /*matrix.translate(0, this->height());
               matrix.rotate(0);
               matrix.scale(1, -1);*/
               //painter.setMatrix(matrix);


            //! [ HORIZONTAL / VERTICAL FLIP (TO BOTTOM RIGHT)]
               matrix.translate(this->width(), this->height());
               matrix.rotate(0);
               matrix.scale(-1, -1);
               painter.setMatrix(matrix);

            //! [ HORIZONTAL FLIP (TO TOP RIGHT)]
               /*matrix.translate(0, this->width());
               matrix.rotate(0);
               matrix.scale(1, -1);*/
               //painter.setMatrix(matrix);

            QPainterPath Curves;
            //painter.shear(15,0);
            int outerCircleWidth = this->property("outerCurveWidth").toInt(); //16;
            int outerCircleHeight = this->property("outerCurveHeight").toInt();//16;

            int outerCircleX = 0;
            int outerCircleY = 0;

            int innerCircleX = this->property("innerCurveX").toInt();//15;
            int innerCircleY = this->property("innerCurveY").toInt();//10;

            int innerCircleWidth = this->property("innerCurveWidth").toInt();//10;
            int innerCircleHeight = this->property("innerCurveHeight").toInt();//10;

            Curves.moveTo(outerCircleWidth/2,outerCircleHeight/2);
            Curves.arcTo(outerCircleX,outerCircleY,outerCircleWidth,outerCircleHeight,-90.0*2,-90.0);

            Curves.setFillRule(Qt::WindingFill);
            Curves.addRegion(QRect(outerCircleX,outerCircleHeight/2,this->width(),this->height()-outerCircleHeight/2));
            Curves.addRegion(QRect(outerCircleWidth/2,outerCircleY,this->width(),outerCircleHeight/2));

            Curves.moveTo(innerCircleX,innerCircleY);
            Curves.arcTo(innerCircleX-innerCircleWidth/2,innerCircleY-innerCircleHeight/2,innerCircleWidth,innerCircleHeight,90.0,90.0);
            Curves.moveTo(innerCircleX-innerCircleWidth/2,innerCircleY);
            Curves.addRegion(QRect(innerCircleX-innerCircleWidth/2,innerCircleY,this->width(),this->height()));

            Curves.closeSubpath();
            painter.drawPath(Curves.simplified());
 //if (this->property("endCurves").toBool() == true) {
            //! { BOTTOM LEFT }
            painter.drawPie(0,this->height()-bottomCurveHeight*2,bottomCurveLeftWidth/*outerCircleWidth-innerCircleWidth/2*/,bottomCurveHeight*2,360*8,360*4);
            //! { BOTTOM RIGHT }
            int rectWidth = innerCircleX-innerCircleWidth/2-bottomCurveRightWidth/2-bottomCurveLeftWidth/2;
            painter.drawRect(bottomCurveLeftWidth/2,this->height()-bottomCurveHeight,rectWidth,bottomCurveHeight);
            painter.drawPie(innerCircleX-innerCircleWidth/2-bottomCurveRightWidth,this->height()-bottomCurveHeight*2,bottomCurveRightWidth/*outerCircleWidth-innerCircleWidth/2*/,bottomCurveHeight*2,360*12,360*4);


            painter.eraseRect(this->width()-rightCurveWidth,0,rightCurveWidth*2,this->height());


            qreal sizeY = innerCircleY-innerCircleHeight/2-rightCurveBottomHeight;
            qreal sizeRectRight = innerCircleY-innerCircleHeight/2-rightCurveBottomHeight;

            painter.drawPie(this->width()-rightCurveWidth*2,0,rightCurveWidth*2,rightCurveTopHeight,360*16,360*4);

            painter.drawPie(this->width()-rightCurveWidth*2,sizeY,rightCurveWidth*2,rightCurveBottomHeight,360*12,360*4);

            painter.drawRect(this->width()-rightCurveWidth,rightCurveTopHeight/2,rightCurveWidth,sizeRectRight);
//}

            painter.eraseRect(innerCircleX-innerCircleWidth/2,innerCircleY,this->width(),this->height());

            painter.eraseRect(innerCircleX,innerCircleY-innerCircleHeight/2,this->width(),this->height());
        }
            break;
        default:
        {
            QPainterPath Curves;

            int outerCircleWidth = this->property("outerCurveWidth").toInt(); //16;
            int outerCircleHeight = this->property("outerCurveHeight").toInt();//16;

            int outerCircleX = 0;
            int outerCircleY = 0;

            int innerCircleX = this->property("innerCurveX").toInt();//15;
            int innerCircleY = this->property("innerCurveY").toInt();//10;

            int innerCircleWidth = this->property("innerCurveWidth").toInt();//10;
            int innerCircleHeight = this->property("innerCurveHeight").toInt();//10;

            Curves.moveTo(outerCircleWidth/2,outerCircleHeight/2);
            Curves.arcTo(outerCircleX,outerCircleY,outerCircleWidth,outerCircleHeight,-90.0*2,-90.0);

            Curves.setFillRule(Qt::WindingFill);
            Curves.addRegion(QRect(outerCircleX,outerCircleHeight/2,this->width(),this->height()-outerCircleHeight/2));
            Curves.addRegion(QRect(outerCircleWidth/2,outerCircleY,this->width(),outerCircleHeight/2));

            Curves.moveTo(innerCircleX,innerCircleY);
            Curves.arcTo(innerCircleX-innerCircleWidth/2,innerCircleY-innerCircleHeight/2,innerCircleWidth,innerCircleHeight,90.0,90.0);
            Curves.moveTo(innerCircleX-innerCircleWidth/2,innerCircleY);
            Curves.addRegion(QRect(innerCircleX-innerCircleWidth/2,innerCircleY,this->width(),this->height()));
            //Curves.addRegion(QRect(innerCircleX,innerCircleY-innerCircleHeight/2,this->width(),innerCircleHeight/2));

            Curves.closeSubpath();
            painter.drawPath(Curves.simplified());
//            if (this->property("endCurves").toBool() == true) {
                       //! { BOTTOM LEFT }
                       painter.drawPie(0,this->height()-bottomCurveHeight*2,bottomCurveLeftWidth/*outerCircleWidth-innerCircleWidth/2*/,bottomCurveHeight*2,360*8,360*4);
                       //! { BOTTOM RIGHT }
                       int rectWidth = innerCircleX-innerCircleWidth/2-bottomCurveRightWidth/2-bottomCurveLeftWidth/2;
                       painter.drawRect(bottomCurveLeftWidth/2,this->height()-bottomCurveHeight,rectWidth,bottomCurveHeight);
                       painter.drawPie(innerCircleX-innerCircleWidth/2-bottomCurveRightWidth,this->height()-bottomCurveHeight*2,bottomCurveRightWidth/*outerCircleWidth-innerCircleWidth/2*/,bottomCurveHeight*2,360*12,360*4);


                       painter.eraseRect(this->width()-rightCurveWidth,0,rightCurveWidth*2,this->height());


                       qreal sizeY = innerCircleY-innerCircleHeight/2-rightCurveBottomHeight;
                       qreal sizeRectRight = innerCircleY-innerCircleHeight/2-rightCurveBottomHeight;

                       painter.drawPie(this->width()-rightCurveWidth*2,0,rightCurveWidth*2,rightCurveTopHeight,360*16,360*4);

                       painter.drawPie(this->width()-rightCurveWidth*2,sizeY,rightCurveWidth*2,rightCurveBottomHeight,360*12,360*4);

                       painter.drawRect(this->width()-rightCurveWidth,rightCurveTopHeight/2,rightCurveWidth,sizeRectRight);
//           }
            painter.eraseRect(innerCircleX-innerCircleWidth/2,innerCircleY,this->width(),this->height());

            painter.eraseRect(innerCircleX,innerCircleY-innerCircleHeight/2,this->width(),this->height());
        }
            break;
        }

}
