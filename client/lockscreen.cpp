#include "lockscreen.h"
#include "ui_lockscreen.h"
#include "extra_functions.h"
#include "settings.h"
#include <QMediaPlayer>
#include "lcars_button.h"
#include <globalvars.h>
#include <QtSql>

LockScreen::LockScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LockScreen)
{
    ui->setupUi(this);
    code << "alpha" << "beta" << "gamma" << "delta";
    //codeString << "" << "" << "" << "";


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(settingsDbFile);

    if(db.open()) /*QMessageBox::information(0,"Ok","Connection ok")*/{}
    else {}

    QSqlQuery query("SELECT `vol_main`,`vol_menu` FROM settings WHERE `id` = '0'");
     if (query.lastError().isValid()){};
     query.exec();

     int mainVolume;
     int menuVolume;

     while (query.next()) {
         mainVolume = query.value(0).toInt();
         menuVolume = query.value(1).toInt();
         }
    db.close();

    int menuBeeperVolume = (mainVolume*menuVolume)/100;

    //qDebug() << menuBeeperVolume;
    menuBeeper = new QMediaPlayer;
    menuBeeper->setVolume(menuBeeperVolume);
    codeButtons= new QVector<QObject*>();
    ui->BUTTON_ALPHA->setText(
                settings::LOAD(
                    "interface",
                    "LOCKSCREEN/ALPHA/text()")[1].toUpper());
    ui->BUTTON_BETA->setText(
                settings::LOAD(
                    "interface",
                    "LOCKSCREEN/BETA/text()")[1].toUpper());
    ui->BUTTON_GAMMA->setText(
                settings::LOAD(
                    "interface",
                    "LOCKSCREEN/GAMMA/text()")[1].toUpper());
    ui->BUTTON_DELTA->setText(
                settings::LOAD(
                    "interface",
                    "LOCKSCREEN/DELTA/text()")[1].toUpper());
    ui->TOP_TITLE->setText(
                settings::LOAD(
                    "interface",
                    "LOCKSCREEN/TOPTITLE/text()")[1].toUpper());
    ui->BOTTOM_TITLE->setText(
                settings::LOAD(
                    "interface",
                    "LOCKSCREEN/BOTTOMTITLE/text()")[1].toUpper());
    ui->CONTROL_TITLE->setText(
                settings::LOAD(
                    "interface",
                    "LOCKSCREEN/CONTROLTITLE/text()")[1].toUpper());
    ui->MESSAGE->setText(
                settings::LOAD(
                    "interface",
                    "LOCKSCREEN/MESSAGE_LOCKED/text()")[1].toUpper());
}

LockScreen::~LockScreen()
{
    delete ui;
}

void LockScreen::on_MovingButton_1_clicked()
{
    menuBeeper->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    menuBeeper->play();

    codeString.append("alpha");
    codeButtons->append(this->sender());

    QVector<QVariant> codeResult;
    codeResult = extra_functions::checkCode(code,codeString,"alpha",this->sender(),codeButtons);
    processCodeResult(codeResult[0].toInt());
}

void LockScreen::on_MovingButton_2_clicked()
{
    menuBeeper->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    menuBeeper->play();

    codeString.append("beta");
    codeButtons->append(this->sender());
    QVector<QVariant> codeResult;
    codeResult = extra_functions::checkCode(code,codeString,"beta",this->sender(),codeButtons);
    processCodeResult(codeResult[0].toInt());

}

void LockScreen::on_MovingButton_3_clicked()
{
   menuBeeper->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
   menuBeeper->play();

   codeString.append("gamma");
   codeButtons->append(this->sender());
   QVector<QVariant> codeResult;
   codeResult = extra_functions::checkCode(code,codeString,"gamma",this->sender(),codeButtons);
    processCodeResult(codeResult[0].toInt());
}

void LockScreen::on_MovingButton_5_clicked()
{
    menuBeeper->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    menuBeeper->play();

    codeString.append("delta");
    codeButtons->append(this->sender());

    QVector<QVariant> codeResult;
    codeResult = extra_functions::checkCode(code,codeString,"delta",this->sender(),codeButtons);
    processCodeResult(codeResult[0].toInt());

}

void LockScreen::processCodeResult(int resultValue) {
    switch (resultValue) {
        case 0:

        break;
        case 1:
            codeButtons->clear();
            codeString.clear();
            menuBeeper->setMedia(QUrl("qrc:/sounds/denybeep3.mp3"));
            menuBeeper->play();
        break;
        case 2:
            codeButtons->clear();
            codeString.clear();
            menuBeeper->setMedia(QUrl("qrc:/sounds/tng_viewscreen_on.mp3"));
            menuBeeper->play();
            ui->MESSAGE->setText(
                        settings::LOAD(
                            "interface",
                            "LOCKSCREEN/MESSAGE_ACCESSGRANTED/text()")[1].toUpper());
            extra_functions::delay(1000);
            this->close();
        break;
    }
}

void LockScreen::on_BUTTON_ALPHA_clicked()
{
    menuBeeper->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    menuBeeper->play();

    codeString.append("alpha");
    codeButtons->append(this->sender());

    QVector<QVariant> codeResult;
    codeResult = extra_functions::checkCode(code,codeString,"alpha",this->sender(),codeButtons);
    processCodeResult(codeResult[0].toInt());
}

void LockScreen::on_BUTTON_BETA_clicked()
{
    menuBeeper->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    menuBeeper->play();

    codeString.append("beta");
    codeButtons->append(this->sender());
    QVector<QVariant> codeResult;
    codeResult = extra_functions::checkCode(code,codeString,"beta",this->sender(),codeButtons);
    processCodeResult(codeResult[0].toInt());
}

void LockScreen::on_BUTTON_GAMMA_clicked()
{
    menuBeeper->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    menuBeeper->play();

    codeString.append("gamma");
    codeButtons->append(this->sender());
    QVector<QVariant> codeResult;
    codeResult = extra_functions::checkCode(code,codeString,"gamma",this->sender(),codeButtons);
     processCodeResult(codeResult[0].toInt());
}

void LockScreen::on_BUTTON_DELTA_clicked()
{
    menuBeeper->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    menuBeeper->play();

    codeString.append("delta");
    codeButtons->append(this->sender());

    QVector<QVariant> codeResult;
    codeResult = extra_functions::checkCode(code,codeString,"delta",this->sender(),codeButtons);
    processCodeResult(codeResult[0].toInt());
}
