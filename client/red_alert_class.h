#ifndef RED_ALERT_CLASS_H
#define RED_ALERT_CLASS_H
#include <QWidget>

class red_alert_class
{
public:
    red_alert_class();
    static int RedAlertFlash(QVector<QVector< QWidget *> >  red_alert_widgets, QList<QWidget *> all_widgets,QTimer *timerPointer);
    static int RedAlertFlash2(QVector<QVector< QWidget *> >  red_alert_widgets, QList<QWidget*>all_widgets,QTimer *timerPointer);

};

#endif // RED_ALERT_CLASS_H
