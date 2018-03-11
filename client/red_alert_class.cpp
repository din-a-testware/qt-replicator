#include "red_alert_class.h"
#include <QVector>
#include <QWidget>
#include <QtGui>
#include "extra_functions.h"
#include "globalvars.h"

red_alert_class::red_alert_class()
{
    Q_UNUSED(devmode);
    Q_UNUSED(redActive);
    Q_UNUSED(redAlertKillMe);
}


int red_alert_class::RedAlertFlash(/*QWidget *base*/ QVector<QVector< QWidget *> >  red_alert_widgets, QList<QWidget *> all_widgets, QTimer *timerPointer) {

    //if (StopTheRedAlert == false) {
        //qDebug() << "active";
        //QCoreApplication::processEvents(QEventLoop::AllEvents);
        /*foreach (QWidget* q,all_widgets) {
                if (q->property("css_red").isValid()) {
                    q->setStyleSheet(q->property("css_red").toString());
                }
            }*/

        foreach (QWidget *q, all_widgets) {

            if (q->property("alarm_id").isValid()) {
                red_alert_widgets[q->property("alarm_id").toInt()] += q;
            }
        }

        foreach (QVector<QWidget *> v, red_alert_widgets) {
            for (int i=0; i<v.count(); i++) {
                foreach (QWidget *w1, v) {
                   w1->setStyleSheet(w1->property("css_red").toString());
               }
           }
         }

        foreach (QVector<QWidget *> v, red_alert_widgets) {

            if (StopTheRedAlert==false) {
            for (int i=0; i<v.count(); i++) {
                foreach (QWidget *w1, v) {
                    w1->setStyleSheet(w1->property("css_weiss").toString());
                }
            }
            extra_functions::delay(110);
            for (int i=0; i<v.count(); i++) {
                foreach (QWidget *w1, v) {
                    w1->setStyleSheet(w1->property("css_red").toString());
                }
            }
            }
            else {
                foreach (QWidget* q,all_widgets) {
                        if (q->property("css_standard").isValid()) {
                            q->setStyleSheet(q->property("css_standard").toString());
                        }
                    }
                timerPointer->stop();
                break;
            }
        }

    return 0;
    /*} else if (StopTheRedAlert == true) {
        qDebug() << "INactive";

        foreach (QWidget* q,all_widgets) {
                if (q->property("css_standard").isValid()) {
                    q->setStyleSheet(q->property("css_standard").toString());
                }
            }
        timerPointer->stop();
        }*/
}


int red_alert_class::RedAlertFlash2(/*QWidget *base*/ QVector<QVector< QWidget *> >  red_alert_widgets, QList<QWidget*>all_widgets,QTimer *timerPointer) {

    //if (StopTheRedAlert == false) {
        //qDebug() << "active";
        //QCoreApplication::processEvents(QEventLoop::AllEvents);
        /*foreach (QWidget* q,all_widgets) {
                if (q->property("css_red").isValid()) {
                    q->setStyleSheet(q->property("css_red").toString());
                }
            }*/

        /*foreach (QWidget *q, all_widgets) {

            if (q->property("alarm_id").isValid()) {
                red_alert_widgets[q->property("alarm_id").toInt()] += q;
            }
        }*/

        foreach (QVector<QWidget *> v, red_alert_widgets) {
            for (int i=0; i<v.count(); i++) {
                foreach (QWidget *w1, v) {
                   w1->setStyleSheet(w1->property("css_red").toString());
               }
           }
         }

        foreach (QVector<QWidget *> v, red_alert_widgets) {

            if (StopTheRedAlert==false) {
            for (int i=0; i<v.count(); i++) {
                foreach (QWidget *w1, v) {
                    w1->setStyleSheet(w1->property("css_weiss").toString());
                }
            }
            extra_functions::delay(110);
            for (int i=0; i<v.count(); i++) {
                foreach (QWidget *w1, v) {
                    w1->setStyleSheet(w1->property("css_red").toString());
                }
            }
            }
            else {
                foreach (QWidget* q,all_widgets) {
                        if (q->property("css_standard").isValid()) {
                            q->setStyleSheet(q->property("css_standard").toString());
                        }
                    }
                timerPointer->stop();
                break;
            }
        }

    return 0;
    /*} else if (StopTheRedAlert == true) {
        qDebug() << "INactive";

        foreach (QWidget* q,all_widgets) {
                if (q->property("css_standard").isValid()) {
                    q->setStyleSheet(q->property("css_standard").toString());
                }
            }
        timerPointer->stop();
        }*/
}

