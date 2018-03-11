//** TODO: LEARN C++

#include <QDebug>
#include "mainwindow.h"
#include "extra_functions.h"
#include "red_alert_class.h"
#include "settings.h"
#include <QApplication>
#include "globalvars.h"

QString VideoFile = "";
QStringList code_temp;

bool StopTheRedAlert;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_UNUSED(systemSpeed);
    Q_UNUSED(devmode);
    Q_UNUSED(redActive);
    Q_UNUSED(redAlertKillMe);

    if (devmode == false) {
        a.setOverrideCursor(Qt::BlankCursor);
    }
    MainWindow w;
    w.setWindowIcon(QIcon(":/images/replicator_database_orange_red.png"));
    //w.setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

    w.setWindowFlags(Qt::FramelessWindowHint| Qt::Window);
    w.setAttribute(Qt::WA_OpaquePaintEvent);
    w.show();
    //w.showFullScreen();
    //QString dat = QString::fromLocal8Bit(BUILDDATE);
    //qDebug() << dat;
    //QString compilationTime = QString("%1T%2").arg(__DATE__).arg(__TIME__);
    qDebug() << "Compilation timestamp:" << compilationTime;
    //qDebug() << "shown";

    return a.exec();
}
