#include "remote_control.h"
#include <QTimer>
#include <QMediaPlayer>
#include <QWidget>
#include <QDebug>
#include "lockscreen.h"
#include <QApplication>
remote_control::remote_control(QWidget *parent)
{
Q_UNUSED(parent);
}

void remote_control::process_message(QString message,
                     QTimer &timer,
                     QMediaPlayer &Sound_RedAlert,
                     QList<QWidget *> all_widgets )  {

    qDebug() << message;
        if (message=="connect") {

            qDebug() << "client connected";

        } else if (message=="alert_blue"){



            Sound_RedAlert.stop();



           if (timer.isActive()) {
                    StopTheRedAlert=true;
                    timer.stop();
                }


            qDebug() << "alert condition blue";


        } else if (message=="alert_yellow"){

            qDebug() << "alert condition yellow";

        } else if (message=="alert_red") {



            Sound_RedAlert.play();



           if (!timer.isActive()) {
                    StopTheRedAlert=false;
                    timer.start(0);
                }
           foreach (QWidget* q,all_widgets) {
                if (q->property("RedAlertState").isValid()) {
                    q->setProperty("RedAlertState",1);
                    q->repaint();
                }
            }




            qDebug() << "alert condition red";

        } else if (message=="system_reboot"){

            qDebug() << "system reboot";
            system("su pi -c \"xset dpms force off\"");
            system("sudo shutdown -r now");

        } else if (message=="system_shutdown"){

            qDebug() << "system shutdown";
            system("su pi -c \"xset dpms force off\"");
            system("sudo shutdown -P now");

        } else if (message=="system_lock") {
            lockform = new LockScreen();
            //connect(keyb,SIGNAL(closed()),this,SLOT(updateVarTextFeld()));
            lockform->setAttribute(Qt::WA_DeleteOnClose);
            lockform->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
            lockform->showFullScreen(); //opening second
        } else if (message=="shutdown_server") {

        } else if (message=="start_server") {

        } else if (message=="search_recipe") {

        } else if (message=="list_timer") {

        } else if (message=="stop_timer") {

        }
        else if (message=="close_gui") {
            QApplication::closeAllWindows();
        }

}
