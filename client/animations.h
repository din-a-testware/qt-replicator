#ifndef ANIMATIONS_H
#define ANIMATIONS_H
#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

class animations: public QObject
{
    Q_OBJECT
public:
    animations(QWidget *parent=0);
     void startAnimation(QVector<QVector< QWidget *> >  red_alert_widgets, QList<QWidget *> all_widgets,QString id_type, int duration, int delay);
     void closeAnimation(QVector<QVector< QWidget *> >  red_alert_widgets, QList<QWidget *> all_widgets,QString id_type, int duration, int delay);
     void outAnimation(QList< QWidget *> widget_list, QString id_type, int duration, int delay);
     void inAnimation(QList< QWidget *> widget_list, QString id_type, int duration, int delay);
     void InOutAnimation(QVector<QVector< QWidget *> >  red_alert_widgets, int duration,int delay,int in_out);
     int RedAlertFlash(/*QVector<QVector< QWidget *> >  red_alert_widgets, QList<QWidget*>all_widgets,QTimer *timerPointer*/);
     void TextFlash(QWidget *label,qreal duration, int loopCount, int status);
    QGraphicsOpacityEffect *effectOpacity = new QGraphicsOpacityEffect();
    QPropertyAnimation *animOpacity = new QPropertyAnimation(effectOpacity,"opacity");
    void RedAlertFlashNew(QList<QWidget*>all_widgets, QTimer *timerPointer);

    QVector<QVector< QWidget *> >  red_alert_widgets;
    QList<QWidget*>all_widgets;
    QTimer *timerPointer;
};

#endif // ANIMATIONS_H
