#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include "QTimer"
#include <QMediaPlayer>
#include "globalvars.h"
#include "lockscreen.h"

class remote_control: public QObject
{
    Q_OBJECT
public:
    remote_control(QWidget *parent=0);
    void process_message(QString message,
                         QTimer &timer,
                         QMediaPlayer &Sound_RedAlert,
                         QList<QWidget *> all_widgets );
    /*Qtimer timer
            bool StopTheRedAlert
            QMediaPlayer Sound_RedAlert
            QList<QWidget> all_widgets;*/
    LockScreen *lockform;
};

#endif // REMOTE_CONTROL_H
