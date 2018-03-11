#include "musicdb_control.h"
#include <QtSql>
#include <QDebug>
#include <QListWidget>
#include "globalvars.h"


musicdb_control::musicdb_control(QObject *parent)
{
Q_UNUSED(parent);
}


QVector <QListWidgetItem> musicdb_control::musicdb_control::getFolderList() {

    QVector <QListWidgetItem> asdf;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

        db.setDatabaseName(musicDB_file);
        if(db.open()){} /*QMessageBox::information(0,"Ok","Connection ok")*/
        else{} /*QMessageBox::information(0,"Error","some error.")*/;

        QSqlQuery query("SELECT `name`,`path` FROM folders");
         if (query.lastError().isValid()) {}/*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
         query.exec();

         /*QListWidgetItem newItem;
         newItem.setData(Qt::UserRole, query.value(1).toString());*/

         while (query.next()) {

                QListWidgetItem newItem;
                newItem.setData(Qt::UserRole, query.value(0).toString());
                newItem.setData(Qt::UserRole+1, query.value(1).toString());
                 if (query.value(0)=="") { newItem.setText("Unbenannter Ordner"); }
                 else { newItem.setText(query.value(0).toString()); }
                 asdf.append(newItem);
             }
        db.close();
        db.removeDatabase("QSQLITE");

        return asdf;
}

QVector <QListWidgetItem> musicdb_control::getArtist(QString selected_Folder) {

    QVector <QListWidgetItem> returnList;


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

           db.setDatabaseName(musicDB_file);
           if(db.open()) /*QMessageBox::information(0,"Ok","Connection ok")*/{}
           else /*QMessageBox::information(0,"Error","some error.")*/{};
           QSqlQuery query("SELECT DISTINCT `artist` FROM music WHERE `folder` = '"+selected_Folder+"' COLLATE NOCASE");
            //qDebug() << "[ getArtist ]: " << "selected_Folder: " << selected_Folder;

           if (query.lastError().isValid()) /*QMessageBox::critical(0,"Error",query.lastError().text() )*/{};
            query.exec();

            while (query.next()) {
                QListWidgetItem newItem;
                newItem.setData(Qt::UserRole, query.value(0).toString());

                if (query.value(0)=="") { newItem.setText("Unbekannter Interpret"); }
                else { newItem.setText(query.value(0).toString()); }

                    returnList.append(newItem);
                }
            query.clear();

            db.close();
            db.removeDatabase("QSQLITE");


    return returnList;

}

QVector <QListWidgetItem> musicdb_control::getAlbum(QString selected_Folder, QString selected_Artist) {

    QVector <QListWidgetItem> returnList;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

     db.setDatabaseName(musicDB_file);
     if(db.open()) /*QMessageBox::information(0,"Ok","Connection ok")*/{}
     else /*QMessageBox::information(0,"Error","some error.")*/{};
     QSqlQuery query("SELECT DISTINCT `album` FROM music WHERE `folder` = '"+selected_Folder+"' COLLATE NOCASE AND `artist` = '"+selected_Artist+"' COLLATE NOCASE");

     //qDebug() << "[ getAlbum ]: " << "selected_Folder: " << selected_Folder<<" | selected_Artist:" << selected_Artist;

     if (query.lastError().isValid()) /*QMessageBox::critical(0,"Error",query.lastError().text() )*/{};
      query.exec();

      while (query.next()) {
          QListWidgetItem newItem;
          newItem.setData(Qt::UserRole, query.value(0).toString());

          if (query.value(0)=="") { newItem.setText("Unbekanntes Album"); }
          else { newItem.setText(query.value(0).toString()); }

          returnList.append(newItem);
          }
      query.clear();

      db.close();
      db.removeDatabase("QSQLITE");

return returnList;

}

QVector <QListWidgetItem> musicdb_control::getTitle(QString selected_Folder, QString selected_Artist, QString selected_Album) {
    QVector <QListWidgetItem> returnList;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

     db.setDatabaseName(musicDB_file);
     if(db.open()) /*QMessageBox::information(0,"Ok","Connection ok")*/{}
     else /*QMessageBox::information(0,"Error","some error.")*/{};


     QString queryString="SELECT DISTINCT `id`,`title`,`track`,`album` FROM music WHERE `folder` = '"+selected_Folder+"' COLLATE NOCASE AND `artist` = '"+selected_Artist+"' COLLATE NOCASE AND `album` = '"+selected_Album+"' COLLATE NOCASE ORDER BY `album` ASC,`track` ASC, `title` ASC";
     //"SELECT DISTINCT `id`,`title` FROM music WHERE `folder` = '"+selected_Folder+"' AND `artist` = '"+selected_Artist+"' AND `album` = '"+selected_Album+"' COLLATE NOCASE"
     //"SELECT DISTINCT `id`,`track`,`title` FROM music WHERE `folder` = '"+selected_Folder+"' AND `artist` = '"+selected_Artist+"' AND `album` = '"+selected_Album+"' COLLATE NOCASE ORDER BY `album` ASC, `track` ASC, `title` ASC";
     QSqlQuery query(queryString);
     //qDebug() << "[ getAlbum ]: " << "selected_Folder: " << selected_Folder<<" | selected_Artist:" << selected_Artist << " | selected_Album: " << selected_Album;
     if (query.lastError().isValid()) /*QMessageBox::critical(0,"Error",query.lastError().text() )*/{};
      query.exec();

      while (query.next()) {
            QListWidgetItem newItem;
            newItem.setData(Qt::UserRole, query.value(0).toString());

                if (query.value(1)=="") { newItem.setText("Unbekannter Titel"); }
                else { newItem.setText(query.value(1).toString()); }

            returnList.append(newItem);
          }
      query.clear();

      db.close();
      db.removeDatabase("QSQLITE");

    return returnList;
}

QVector <QListWidgetItem> musicdb_control::addToPlaylist(QString selected_Folder,QString selected_Titles) {

        QVector <QListWidgetItem> returnList;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

     db.setDatabaseName(musicDB_file);
     if(db.open()){}
     else {};

                 QSqlQuery query("SELECT DISTINCT `title`,`filepath`,`filename`,`track`,`local` FROM music WHERE `id` = '"+selected_Titles+"' COLLATE NOCASE");
                 if (query.lastError().isValid()){};
                 query.exec();

                 while (query.next()) {


                     QListWidgetItem newItem;
                     newItem.setData(Qt::UserRole-1, selected_Titles); // id
                     newItem.setData(Qt::UserRole, query.value(0).toString()); // title
                     if (query.value(4)=="1") {
                        newItem.setData(Qt::UserRole+1,query.value(2).toString()); // complete path
                     } else {
                        newItem.setData(Qt::UserRole+1,selected_Folder+query.value(1).toString()+query.value(2).toString()); // complete path
                     }

                     newItem.setData(Qt::UserRole+2,query.value(3).toString()); // track number

                     if (query.value(0)=="") { newItem.setText("Unbekannter Titel"); }
                     else { newItem.setText(query.value(0).toString()); }

                     returnList.append(newItem);

                     }
                 query.clear();

    db.close();
    db.removeDatabase("QSQLITE");

  return returnList;
}

QVector <QListWidgetItem> musicdb_control::getSearch(QString selected_Folder, QString selected_Combo, QString searchInput) {

    QString whichType =selected_Combo;

    /*if (selected_Combo == "Artist") {
        whichType="artist";
    }
    else if (selected_Combo == "Album") {
        whichType="album";
    }
    if (selected_Combo == "Title") {
        whichType="title";
    }*/

    QVector <QListWidgetItem> returnList;


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

           db.setDatabaseName(musicDB_file);
           if(db.open()) /*QMessageBox::information(0,"Ok","Connection ok")*/{}
           else /*QMessageBox::information(0,"Error","some error.")*/{};
           QSqlQuery query("SELECT DISTINCT `"+whichType+"` FROM music WHERE `folder` = '"+selected_Folder+"' COLLATE NOCASE AND `"+whichType+"` LIKE '%"+searchInput+"%' COLLATE NOCASE");
            //qDebug() << "[ getArtist ]: " << "selected_Folder: " << selected_Folder;

           if (query.lastError().isValid()) /*QMessageBox::critical(0,"Error",query.lastError().text() )*/{};
            query.exec();

            while (query.next()) {
                QListWidgetItem newItem;
                newItem.setData(Qt::UserRole, query.value(0).toString());

                if (query.value(0)=="") { newItem.setText("Unbekannt"); }
                else { newItem.setText(query.value(0).toString()); }

                    returnList.append(newItem);
                }
            query.clear();

            db.close();
            db.removeDatabase("QSQLITE");


    return returnList;

}

QStringList musicdb_control::getMetadata(QString selected_Titles) {

        QStringList returnList;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

     db.setDatabaseName(musicDB_file);
     if(db.open()) {}
     else {};

                 QSqlQuery query("SELECT DISTINCT `artist`,`album`,`title`,`coverart` FROM music WHERE `id` = '"+selected_Titles+"' COLLATE NOCASE");
                 if (query.lastError().isValid()){};
                 query.exec();

                 while (query.next()) {

                    QString artist;
                    QString album;
                    QString title;
                    QString coverart;

                     if (query.value(0)=="") { artist="Unbekannter Interpret"; }
                     else { artist=query.value(0).toString(); }

                     if (query.value(1)=="") { album="Unbekanntes Album"; }
                     else { album=query.value(1).toString(); }

                     if (query.value(2)=="") { title="Unbekannter Titel"; }
                     else { title=query.value(2).toString(); }

                     if (query.value(3)=="") { coverart=""; }
                     else { coverart=query.value(3).toString(); }

                     returnList.append(artist);
                     returnList.append(album);
                     returnList.append(title);
                     returnList.append(coverart);



                     }
                 query.clear();

    db.close();
    db.removeDatabase("QSQLITE");

  return returnList;
}
