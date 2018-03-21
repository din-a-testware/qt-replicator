//** TODO: LEARN C++

#include <QDebug>
#include "mainwindow.h"
#include "extra_functions.h"
#include "red_alert_class.h"
#include "settings.h"
#include <QApplication>
#include "globalvars.h"
#include <QDir>
#include <QtSql>
QString VideoFile = "";
QStringList code_temp;

bool StopTheRedAlert;

int main(int argc, char *argv[])
{

    QString scaleFacor;
    QSqlDatabase dbInitial = QSqlDatabase::addDatabase("QSQLITE", "initialSetting");
    dbInitial.setDatabaseName(settingsDbFile);

    if(dbInitial.open()) {
        QSqlQuery query(QSqlDatabase::database("initialSetting"));
        query.prepare("SELECT `scale_factor` FROM settings WHERE id='0'");
        query.exec();
        while (query.next()) {
            scaleFacor=query.value(0).toString();
        }

    }

    scaleFacor = 1;

    qputenv("QT_SCALE_FACTOR", qPrintable(scaleFacor));
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //HiDPI pixmaps

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

    if (QDir::homePath() == "/home/pi" || QDir::homePath() == "/root") {
        w.showFullScreen();
    } else {
        w.show();
    }
    //w.showFullScreen();
    //QString dat = QString::fromLocal8Bit(BUILDDATE);
    //qDebug() << dat;
    //QString compilationTime = QString("%1T%2").arg(__DATE__).arg(__TIME__);
    qDebug() << "Compilation timestamp:" << compilationTime;
    //qDebug() << "shown";

    return a.exec();
}
