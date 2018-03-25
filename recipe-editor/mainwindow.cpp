#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "process_data.h"
#include <QDebug>
#include <QtWebSockets/QWebSocket>
#include <QFileDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QSettings>
#include <QBuffer>
#include <QIODevice>
#include <QProcess>
#include <QTimer>
#include <QtSql>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString defaultIP="127.0.0.1";
    QString defaultPort="1701";
    QString defaultBin=QDir::homePath()+"/bin/replicator-server";
    QString defaultDB=QDir::homePath()+"/replicator-data/databases/replicator_recipes.db";

    serverPath=settings.value("serverPath",defaultBin).toString();
    serverPort=settings.value("serverPort",defaultPort).toString();
    serverIP=settings.value("serverIP",defaultIP).toString();
    local_DB=settings.value("local_DB",defaultDB).toString();
    useLocalDb=settings.value("useLocalDB",false).toBool();

    serverURL = "ws://"+serverIP+":"+serverPort;

    qDebug() << serverURL;

    ui->server_ip->setText(serverIP);
    ui->server_port->setText(serverPort);
    ui->server_path->setText(serverPath);
    ui->local_DB->setText(local_DB);
    ui->useLocalFile->setChecked(useLocalDb);

    m_webSocket = new QWebSocket("Replicator Editor");
    m_webSocket->ignoreSslErrors();
    connect(m_webSocket, &QWebSocket::connected, this, &MainWindow::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &MainWindow::onDisconnected);

    connect(m_webSocket, &QWebSocket::textMessageReceived,
                this, &MainWindow::onTextMessageReceived);
        connect(m_webSocket, &QWebSocket::binaryMessageReceived,
                this, &MainWindow::onBinaryMessageRecieved);

    if ( m_webSocket->state() == QAbstractSocket::UnconnectedState) {


        m_webSocket->open(QUrl(serverURL.toStdString().c_str()));
        m_webSocket->sendTextMessage("Hi there!");
        m_webSocket->sendTextMessage("ping");
    }

        QCoreApplication::setOrganizationName("din-a-testware");
        QCoreApplication::setOrganizationDomain("github.com/din-a-testware");
        QCoreApplication::setApplicationName("Replicator");

        connect(ui->server_start,&QPushButton::clicked,[=]() {
            QProcess::startDetached(serverPath.toStdString().c_str());
        });

        connect(ui->server_shutdown,&QPushButton::clicked,[=]() {
            m_webSocket->sendTextMessage("shutdown");
        });

        QTimer *pingTimer = new QTimer(this);
        connect(pingTimer,&QTimer::timeout,[=](){
            m_webSocket->sendTextMessage("ping");
        });
        pingTimer->setInterval(1000);
        pingTimer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_6_clicked()
{
    /*selectPicDialog = new SelectPicture;
    selectPicDialog->setModal(true);

    if (selectPicDialog->exec() == QDialog::Accepted) {
        qDebug() << "accepted";

        ui->recipe_image->setPixmap(QPixmap(selectPicDialog->getPicturePath()));
    } else {
        qDebug() << "rejected";
    }*/


    fileName = QFileDialog::getOpenFileName(this,
         tr("Open Image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));

    if (fileName!="") {
        ui->recipe_image->setPixmap(QPixmap(fileName));
    }
}


void MainWindow::on_add_ingredient_clicked()
{
    //!! ##########################################################################################################


    if (ui->ingredient_new->text()=="") {
    QMessageBox::StandardButton reply;
      reply = QMessageBox::warning(this, "Hinweis", "Eingabefeld darf nicht leer sein!",
                                    QMessageBox::Ok);
      if (reply == QMessageBox::Ok) {
        qDebug() << "Yes was clicked";
      }
    } else {
        ui->ingredients_list->addItem(ui->ingredient_new->text());
    }

}

void MainWindow::on_remove_ingredient_clicked()
{
    delete ui->ingredients_list->currentItem();

}

void MainWindow::on_connectServer_clicked()
{
    //m_webSocket->sendTextMessage("hallo");
    if ( m_webSocket->state() == QAbstractSocket::UnconnectedState) {
        QString tempServerURL="ws://"+ui->server_ip->text()+":"+ui->server_port->text();
        m_webSocket->open(QUrl(tempServerURL.toStdString().c_str()));
        m_webSocket->sendTextMessage("Hi there!");
    }
}

void MainWindow::onConnected() {

    /*connect(m_webSocket, &QWebSocket::textMessageReceived,
            this, &MainWindow::onTextMessageReceived);
    connect(m_webSocket, &QWebSocket::binaryMessageReceived,
            this, &MainWindow::onBinaryMessageRecieved);
    */
    m_webSocket->sendTextMessage("Hi there!");
    ui->server_status->setText("Running");
}

void MainWindow::onDisconnected() {
    qDebug() << "Disconnected";
    ui->server_status->setText("Disconnected");
}


void MainWindow::onTextMessageReceived(QString message) {

    //qDebug() << message;
    QStringList messageRec = message.split(":");
    if (messageRec.at(0) == "pid") {
        //qDebug() << messageRec.at(1);

        /*QString current_serverPID = messageRec.at(1);
        serverPID=current_serverPID;
        ui->serverPID_label->setText(current_serverPID);
        ui->serverPID_label_2->setText(current_serverPID);*/
        ui->server_status->setText("Running");

    } /*else {
        ui->server_status->setText("Not responding");
        ui->SERVER_STATUS_RESPONSE_2->setText("NOT RESPONDING");
        ui->serverPID_label->setText("");
        ui->serverPID_label_2->setText("");
    }*/

}
void MainWindow::onBinaryMessageRecieved(QByteArray message) {

    replicationData.clear();
    replicationData << Process_Data::deserialize(message);
    QStringList firstValue = replicationData.at(0);
    if (firstValue.at(0) == "RECIPE_LIST") {
        replicationData.removeAt(0);
        populateRecipeList(replicationData);
    }  else if (firstValue.at(0) == "RECIPE") {
        replicationData.removeAt(0);
        loadRecipe(replicationData);
    }
}
void MainWindow::processData(QString searchString) {}

void MainWindow::on_pushButton_5_clicked()
{


     QString sql = "SELECT `id`,`title` FROM recipes";

     if (ui->useLocalFile->isChecked() == true) {

         db.setDatabaseName(local_DB);
         if(db.open()){
             QSqlQuery query(QSqlDatabase::database("recipeDB",true));
             query.prepare(sql);

             QVector<QStringList> returnList;
             query.prepare(sql);
             if (query.lastError().isValid()) {} else {qDebug()<<query.lastError().text();};
             query.exec();
             while (query.next()) {

                 QStringList queryResultRow;

                 for (int i=0; i<query.record().count();i++) {
                    queryResultRow.append(query.value(i).toString());
                 }

                 returnList.append(queryResultRow);

                 }
             populateRecipeList(returnList);

         } else {

         }
         db.close();

     } else {
         m_webSocket->sendTextMessage("RECIPE_LIST::"+sql);
     }

}

void MainWindow::on_pushButton_4_clicked()
{
    m_webSocket->sendTextMessage("RECIPE_LIST::SELECT `id`,`title` FROM recipes WHERE `title` LIKE '%"+ui->recipe_search->text()+"%'");
}

void MainWindow::populateRecipeList(QVector<QStringList> result) {

    ui->recipe_list->clear();

    for (int i = 0; i < result.size(); i++) {
        QListWidgetItem *itemRecipe = new QListWidgetItem();
        QStringList resultItems = result[i];
        itemRecipe->setData(Qt::DisplayRole, resultItems.at(1));
        itemRecipe->setData(Qt::UserRole, resultItems.at(0));
        ui->recipe_list->addItem(itemRecipe);
    }
}


void MainWindow::on_pushButton_clicked()
{
    if (ui->recipe_list->selectedItems().count() < 1) {

    QMessageBox::StandardButton reply;
      reply = QMessageBox::warning(this, "Hinweis", "Bitte ein Rezept aus der Liste auswählen!",
                                    QMessageBox::Ok);
      if (reply == QMessageBox::Ok) {
        qDebug() << "Yes was clicked";
      }
    } else {

    QString sql="SELECT `id`,`title`,`ingredients`,`text`,`selected`,`image` FROM recipes WHERE `id`='"+ui->recipe_list->currentItem()->data(Qt::UserRole).toString()+"'";
        if (ui->useLocalFile->isChecked() == true) {

            db.setDatabaseName(local_DB);
            if(db.open()){
                QSqlQuery query(QSqlDatabase::database("recipeDB",true));
                query.prepare(sql);

                QVector<QStringList> returnList;
                query.prepare(sql);
                if (query.lastError().isValid()) {} else {qDebug()<<query.lastError().text();};
                query.exec();
                while (query.next()) {

                    QStringList queryResultRow;

                    for (int i=0; i<query.record().count();i++) {
                       queryResultRow.append(query.value(i).toString());
                    }

                    returnList.append(queryResultRow);

                    }
                loadRecipe(returnList);

            } else {

            }
            db.close();

        } else {
            m_webSocket->sendTextMessage("RECIPE::"+sql);
        }
    }





}

void MainWindow::loadRecipe(QVector<QStringList> result) {

    QStringList resultList = result.at(0);
    QStringList ingredientsItems = QString(resultList[2]).split("\n");

    ui->recipe_text->clear();
    ui->recipe_image->setPixmap(QPixmap());
    ui->recipe_title->clear();
    ui->ingredients_list->clear();
    ui->recipe_marked->setChecked(false);
    ui->recipe_id->setValue(-1);

    ui->recipe_title->setText(resultList[1]);
    ui->recipe_text->insertPlainText(resultList[3]);
    for (int i=0;i<ingredientsItems.count();i++) {
        ui->ingredients_list->addItem(ingredientsItems.at(i));
    }

    ui->recipe_marked->setChecked(QString(resultList[4]).toInt());
    ui->recipe_id->setValue(QString(resultList[0]).toInt());




    QByteArray base64Data = QString(resultList[5]).toStdString().c_str();
    QImage image;
    image.loadFromData(QByteArray::fromBase64(base64Data));
    ui->recipe_image->setPixmap(QPixmap::fromImage(image));

    ui->tabs->setCurrentWidget(ui->tab_recipe);

    // QImage resized = image->scaled(480,332,Qt::KeepAspectRatio);
    /*if (image.loadFromData(QByteArray::fromBase64(base64Data)) == false) {
        ui->RECIPE_IMAGE->setPixmap(QPixmap::fromImage(QImage("qrc:/images/recipe_big_logo.png")));
    } else {
    QImage ResizedImage =
        image.scaled(480, 332, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);*/
    //ui->RECIPE_IMAGE->setPixmap(QPixmap::fromImage(ResizedImage));


}

void MainWindow::on_pushButton_7_clicked()
{

    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Bestätigen", "Alle Eingaben werden gelöscht.",
                                    QMessageBox::Ok|QMessageBox::Cancel);
      if (reply == QMessageBox::Ok) {
        qDebug() << "Yes was clicked";
        ui->recipe_text->clear();
        ui->recipe_image->setPixmap(QPixmap());
        ui->recipe_title->clear();
        ui->ingredients_list->clear();
        ui->recipe_marked->setChecked(false);
        ui->recipe_id->setValue(-1);
        ui->tabs->setCurrentWidget(ui->tab_list);
      } else {
        qDebug() << "Yes was *not* clicked";
      }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->recipe_text->clear();
    ui->recipe_image->setPixmap(QPixmap());
    ui->recipe_title->clear();
    ui->ingredients_list->clear();
    ui->recipe_marked->setChecked(false);
    ui->recipe_id->setValue(-1);

    ui->tabs->setCurrentWidget(ui->tab_recipe);
}

void MainWindow::on_pushButton_8_clicked()
{
    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Bestätigen", "Eventuell vorhandene Daten werden überschrieben!",
                                    QMessageBox::Ok|QMessageBox::Cancel);
      if (reply == QMessageBox::Ok) {
        qDebug() << "Yes was clicked";
        if (ui->recipe_id->value()==-1) {
            qDebug() << "insert";


            QString ingredients;
            QString selected;

            for (int i=0;i<ui->ingredients_list->count();i++) {

                ingredients.append(ui->ingredients_list->item(i)->data(Qt::DisplayRole).toString()+"\n");
            }


            if (ui->recipe_marked->isChecked()) {
                selected="1";
            } else {
                selected="0";
            }


            const QPixmap* pixmap1 = ui->recipe_image->pixmap();
            QImage image(pixmap1->toImage());
            QPixmap pixmap(QPixmap::fromImage(image));
            // Preparation of our QPixmap
            QByteArray bArray;
            QBuffer buffer(&bArray);
            buffer.open(QIODevice::WriteOnly);
            pixmap.save(&buffer, "PNG");

            //bu.close();
            //QString imgBase64 = ba.toBase64();
            QString imgBase64 = QString::fromLatin1(bArray.toBase64().data());


            QString sql="INSERT INTO recipes (`title`,`ingredients`,`text`,`thumbnail`,`image`,`selected`) "
                        "VALUES ('"+ui->recipe_title->text()+"','"+ingredients+"','"+ui->recipe_text->toPlainText()+"','"+""+"','"+imgBase64+"','"+selected+"')";

            if (ui->useLocalFile->isChecked() == true) {

                db.setDatabaseName(local_DB);
                if(db.open()){
                    QSqlQuery query(QSqlDatabase::database("recipeDB",true));
                    query.prepare(sql);
                    if (query.lastError().isValid()) {} else {qDebug()<<query.lastError().text();};
                    query.exec();


                    ui->recipe_text->clear();
                    ui->recipe_image->setPixmap(QPixmap());
                    ui->recipe_title->clear();
                    ui->ingredients_list->clear();
                    ui->recipe_marked->setChecked(false);
                    ui->recipe_id->setValue(-1);

                    QVector<QStringList> returnList;
                    query.prepare("SELECT `id`,`title` FROM recipes");
                    if (query.lastError().isValid()) {} else {qDebug()<<query.lastError().text();};
                    query.exec();
                    while (query.next()) {

                        QStringList queryResultRow;

                        for (int i=0; i<query.record().count();i++) {
                           queryResultRow.append(query.value(i).toString());
                        }

                        returnList.append(queryResultRow);

                        }
                    populateRecipeList(returnList);

                } else {

                }
                db.close();

            } else {
                m_webSocket->sendTextMessage("UPDATE::"+sql);
                m_webSocket->sendTextMessage("RECIPE_LIST::SELECT `id`,`title` FROM recipes");
            }





        } else {
            qDebug() << "update";

            QString ingredients;
            QString selected;

            for (int i=0;i<ui->ingredients_list->count();i++) {

                ingredients.append(ui->ingredients_list->item(i)->data(Qt::DisplayRole).toString()+"\n");
            }


            if (ui->recipe_marked->isChecked()) {
                selected="1";
            } else {
                selected="0";
            }


            const QPixmap* pixmap1 = ui->recipe_image->pixmap();
            QImage image(pixmap1->toImage());
            QPixmap pixmap(QPixmap::fromImage(image));
            // Preparation of our QPixmap
            QByteArray bArray;
            QBuffer buffer(&bArray);
            buffer.open(QIODevice::WriteOnly);
            pixmap.save(&buffer, "PNG");

            //bu.close();
            //QString imgBase64 = ba.toBase64();
            QString imgBase64 = QString::fromLatin1(bArray.toBase64().data());


            QString sql="UPDATE recipes SET `title`='"+ui->recipe_title->text()+"',`ingredients`='"+ingredients+"',`text`='"+ui->recipe_text->toPlainText()+"',`thumbnail`='"+""+"',`image`='"+imgBase64+"',`selected`='"+selected+"' WHERE `id`="+QString::number(ui->recipe_id->value())+"";
            qDebug() << sql;
            if (ui->useLocalFile->isChecked() == true) {

                db.setDatabaseName(local_DB);
                if(db.open()){
                    QSqlQuery query(QSqlDatabase::database("recipeDB",true));
                    query.prepare(sql);
                    if (query.lastError().isValid()) {} else {qDebug()<<query.lastError().text();};
                    query.exec();


                    ui->recipe_text->clear();
                    ui->recipe_image->setPixmap(QPixmap());
                    ui->recipe_title->clear();
                    ui->ingredients_list->clear();
                    ui->recipe_marked->setChecked(false);
                    ui->recipe_id->setValue(-1);

                    QVector<QStringList> returnList;
                    query.prepare("SELECT `id`,`title` FROM recipes");
                    if (query.lastError().isValid()) {} else {qDebug()<<query.lastError().text();};
                    query.exec();
                    while (query.next()) {

                        QStringList queryResultRow;

                        for (int i=0; i<query.record().count();i++) {
                           queryResultRow.append(query.value(i).toString());
                        }

                        returnList.append(queryResultRow);

                        }
                    populateRecipeList(returnList);

                } else {

                }
                db.close();

            } else {
                m_webSocket->sendTextMessage("UPDATE::"+sql);
                m_webSocket->sendTextMessage("RECIPE_LIST::SELECT `id`,`title` FROM recipes");
            }

        }

      } else {
        qDebug() << "Yes was *not* clicked";
      }
}

void MainWindow::on_pushButton_3_clicked()
{
    if (ui->recipe_list->selectedItems().count()<1) {
    QMessageBox::StandardButton reply;
      reply = QMessageBox::warning(this, "Hinweis", "Bitte ein Rezept aus der Liste auswählen!",
                                    QMessageBox::Ok);
      if (reply == QMessageBox::Ok) {
        qDebug() << "Yes was clicked";

      }
    } else {

        QString sql = "DELETE FROM recipes WHERE `id` = '"+ui->recipe_list->currentItem()->data(Qt::UserRole).toString()+"'";

        if (ui->useLocalFile->isChecked() == true) {

            db.setDatabaseName(local_DB);
            if(db.open()){
                QSqlQuery query(QSqlDatabase::database("recipeDB",true));
                query.prepare(sql);
                if (query.lastError().isValid()) {} else {qDebug()<<query.lastError().text();};
                query.exec();


                ui->recipe_text->clear();
                ui->recipe_image->setPixmap(QPixmap());
                ui->recipe_title->clear();
                ui->ingredients_list->clear();
                ui->recipe_marked->setChecked(false);
                ui->recipe_id->setValue(-1);

                QVector<QStringList> returnList;
                query.prepare("SELECT `id`,`title` FROM recipes");
                if (query.lastError().isValid()) {} else {qDebug()<<query.lastError().text();};
                query.exec();
                while (query.next()) {

                    QStringList queryResultRow;

                    for (int i=0; i<query.record().count();i++) {
                       queryResultRow.append(query.value(i).toString());
                    }

                    returnList.append(queryResultRow);

                    }
                populateRecipeList(returnList);

            } else {

            }
            db.close();

        } else {
            m_webSocket->sendTextMessage("UPDATE::"+sql);
            m_webSocket->sendTextMessage("RECIPE_LIST::SELECT `id`,`title` FROM recipes");
            //m_webSocket->sendTextMessage("RECIPE::SELECT `id`,`title`,`ingredients`,`text`,`selected`,`image` FROM recipes WHERE `id`='"+ui->recipe_list->currentItem()->data(Qt::UserRole).toString()+"'");
        }



    }



}

void MainWindow::on_recipe_search_returnPressed()
{
    m_webSocket->sendTextMessage("RECIPE_LIST::SELECT `id`,`title` FROM recipes WHERE `title` LIKE '%"+ui->recipe_search->text()+"%'");
}

void MainWindow::on_pushButton_10_clicked()
{
    settings.setValue("serverIP",ui->server_ip->text());
    settings.setValue("serverPort",ui->server_port->text());
    settings.setValue("serverPath",ui->server_path->text());
    settings.setValue("local_DB",ui->local_DB->text());
    settings.setValue("useLocalDB",ui->useLocalFile->isChecked());
}
