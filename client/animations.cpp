#include "animations.h"
#include <QVector>
#include <QWidget>
#include <QtGui>
#include "extra_functions.h"
#include "globalvars.h"
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

animations::animations(QWidget *parent)
{
    Q_UNUSED(devmode);
    Q_UNUSED(redActive);
    Q_UNUSED(redAlertKillMe);
    Q_UNUSED(parent);
}

void animations::InOutAnimation(QVector<QVector< QWidget *> >  red_alert_widgets, int duration=240, int delay=30,int in_out=0) {

    /*******************************************************************
     *
     * The Vector is for iterating through the id-groups of the widgets,
     * because widgets may have the same id for the animation.
     *
     *
     *  ╔════════════════════════════════════════╗
     *  ║ Container QVector                      ║
     *  ╟──────────────────────────────────────┬─╢
     *  ║ ┌─────────────────────────────────┐  │▲║
     *  ║ │ QVector Widgets1:id=0           │  │ ║
     *  ║ ├─────────────────────────────────┤  │ ║
     *  ║ │ ui->PushButton1                 │  │ ║
     *  ║ │ ui->Border1                     │  │ ║
     *  ║ │ ui->WidgetX                     │  │ ║
     *  ║ └─────────────────────────────────┘  │ ║
     *  ║ ┌─────────────────────────────────┐  │ ║
     *  ║ │ QVector Widgets2:id=1           │  │█║
     *  ║ ├─────────────────────────────────┤  │ ║
     *  ║ │ ui->PushButton7                 │  │ ║
     *  ║ └─────────────────────────────────┘  │ ║
     *  ║ ┌─────────────────────────────────┐  │ ║
     *  ║ │ QVector Widgets3:id=2           │  │ ║
     *  ║ ├─────────────────────────────────┤  │ ║
     *  ║ │ ui->Border0                     │  │ ║
     *  ║ │ ui->Widget8                     │  │ ║
     *  ║ └─────────────────────────────────┘  │ ║
     *  ║ ┌─────────────────────────────────┐  │ ║
     *  ║ │ QVector Widgets4:id=3           │  │ ║
     *  ║ ├─────────────────────────────────┤  │▼║
     *  ╚══════════════════════════════════════╧═╝
     *
     *
     * Function arguments:
     * int duration: duration of the QPropertyAnimation effect in msecs
     * int delay: the delay until the next Widget ist processed in msecs
     * int in_out: select if the animation is forward or backward.
     *             Default is forward.
     *             0: forward
     *             1: backward
     *
     * Widget Property arguments:
     * w1->property("delay_animation"): if the animation-duration of a
     *                                  widget-group shall take longer
     *                                  than the others, it's specified
     *                                  in this property
     *
     *******************************************************************/

    int startValue;
    int endValue;



    switch (systemSpeed) {
    case 0:

        break;
    case 1:
        delay=delay/2;
        break;
    case 2:
        delay=delay/2;
        break;
    default:
        break;
    }

    switch (in_out) {
    case 0:
        foreach (QVector<QWidget *> v, red_alert_widgets) {
            for (int i=0; i<v.count(); i++) {
                foreach (QWidget *w1, v) {
                    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(w1);
                    eff->setOpacity(0);
                    w1->setGraphicsEffect(eff);
               }
           }
         }
        startValue =0;
        endValue=1;
        break;
    case 1:
        startValue=1;
        endValue=0;
        std::reverse(red_alert_widgets.begin(),red_alert_widgets.end());
        break;
    default:
        foreach (QVector<QWidget *> v, red_alert_widgets) {
            for (int i=0; i<v.count(); i++) {
                foreach (QWidget *w1, v) {
                    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(w1);
                    eff->setOpacity(0);
                    w1->setGraphicsEffect(eff);
               }
           }
         }
        startValue =0;
        endValue=1;
        break;
    }



    foreach (QVector<QWidget *> v, red_alert_widgets) {

            foreach (QWidget *w1, v) {

                if (w1->property("delay_animation").isValid()) {

                    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(w1);
                    w1->setGraphicsEffect(eff);
                    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
                    a->setDuration(w1->property("delay_animation").toInt());
                    a->setStartValue(startValue);
                    a->setEndValue(endValue);

                    a->setEasingCurve(QEasingCurve::Linear);
                    a->start(QPropertyAnimation::DeleteWhenStopped);

                } else {

                    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(w1);
                    w1->setGraphicsEffect(eff);
                    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
                    a->setDuration(duration);
                    a->setStartValue(startValue);
                    a->setEndValue(endValue);
                    a->setEasingCurve(QEasingCurve::Linear);
                    a->start(QPropertyAnimation::DeleteWhenStopped);
                }
            }
        extra_functions::delay(delay);
    }
}

int animations::RedAlertFlash() {
/*QVector<QVector< QWidget *> >  red_alert_widgets, QList<QWidget*>all_widgets,QTimer *timerPointer*/
    int delay=90;
        foreach (QVector<QWidget *> v, red_alert_widgets) {

            if (StopTheRedAlert==false) {
            for (int i=0; i<v.count(); i++) {
                foreach (QWidget *w1, v) {
                    /*if (w1->property("css_weiss").isValid()) {
                    w1->setStyleSheet(w1->property("css_weiss").toString());
                    }*/
                    if (w1->property("RedAlertState").isValid()) {
                        w1->setProperty("RedAlertState",2);
                        w1->repaint();

                    }
                }
            }


            switch (systemSpeed) {
            case 0:
                extra_functions::delay(delay);
                break;
            case 1:
                extra_functions::delay(delay/2);
                break;
            case 2:
                extra_functions::delay(delay/2);
                break;
            default:
                break;
            }

            for (int i=0; i<v.count(); i++) {
                foreach (QWidget *w1, v) {
                    /*if (w1->property("css_red").isValid()) {
                    w1->setStyleSheet(w1->property("css_red").toString());
                    }*/
                    if (w1->property("RedAlertState").isValid()) {
                        w1->setProperty("RedAlertState",1);
                        w1->repaint();

                    }
                }
            }
            }
            else {
                foreach (QWidget* q,all_widgets) {
                        /*if (q->property("css_standard").isValid()) {
                            q->setStyleSheet(q->property("css_standard").toString());

                        }*/
                        if (q->property("RedAlertState").isValid()) {
                            q->setProperty("RedAlertState",0);
                            q->repaint();

                        }
                    }
                timerPointer->stop();
                break;
            }
        }

    return 0;
}

void animations::RedAlertFlashNew(QList<QWidget*>alertwidgets, QTimer *timerPointer) {

    Q_UNUSED(timerPointer);
    qDebug() << "neu" << alertwidgets;

    foreach (QWidget *w1,alertwidgets) {

    //if (StopTheRedAlert==false) {

        if (w1->property("RedAlertState").isValid()) {
            w1->setProperty("RedAlertState",2);
            w1->repaint();

    }
    extra_functions::delay(110);
    if (w1->property("RedAlertState").isValid()) {
        w1->setProperty("RedAlertState",1);
        w1->repaint();
    }

/*} else {

    }

    if (w1->property("RedAlertState").isValid()) {
        w1->setProperty("RedAlertState",0);
        w1->repaint();
    }*/

    }


}

void animations::TextFlash(QWidget *label, qreal duration=1500, int loopCount=1, int status=0) {

    label->setGraphicsEffect(effectOpacity);


    /* QAbstractAnimation::Stopped	0
     * QAbstractAnimation::Paused	1
     * QAbstractAnimation::Running	2
     */

    switch (status) {
    case 0:
        animOpacity->setDuration(duration);
        animOpacity->setStartValue(0.0);
        animOpacity->setKeyValueAt(0.5,1.0);
        animOpacity->setEndValue(0.0);
        animOpacity->setEasingCurve(QEasingCurve::OutQuad);
        animOpacity->setLoopCount(loopCount);
        animOpacity->start();
        break;
    case 1:
        if (animOpacity->state() ==2) {
            //animOpacity->setLoopCount(2);
            //qDebug() << "ende";
            animOpacity->stop();
        }

        break;
    default:
        animOpacity->setDuration(duration);
        animOpacity->setStartValue(0.0);
        animOpacity->setKeyValueAt(0.5,1.0);
        animOpacity->setEndValue(0.0);
        animOpacity->setEasingCurve(QEasingCurve::OutQuad);
        animOpacity->setLoopCount(loopCount);
        animOpacity->start();
        break;
    }

    //QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();

     //QPropertyAnimation *anim = new QPropertyAnimation(effect,"opacity");
     /*animOpacity->setDuration(1500);
     animOpacity->setStartValue(0.0);
     animOpacity->setKeyValueAt(0.5,1.0);
     animOpacity->setEndValue(0.0);
     animOpacity->setEasingCurve(QEasingCurve::InOutQuad);
     animOpacity->setLoopCount(-1);
     animOpacity->start(QAbstractAnimation::DeleteWhenStopped);*/

}
