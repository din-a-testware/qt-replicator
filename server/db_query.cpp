#include "db_query.h"
#include <QVector>
#include <QDebug>
#include <QtXml>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDomElement>
#include <QTextStream>
#include <stdio.h>
#include <QFile>
#include <QRegExp>
#include <QRegularExpression>
#include "globalvars.h"
#include <QXmlQuery>
#include <QtSql>

db_query::db_query()
{

}

QVector<QStringList> db_query::DB_EXEC(const QString& SELECT_UPDATE, const QString& WHERE, const QString& LIKE_EQUAL) {

    /**** BASE VARS ****/
    QVector<QStringList> matrix;
    matrix.resize(0);
    QStringList list1;

    QString pathXML_DATABASE;

    deploySystem = "linux";

            pathXML_DATABASE = QDir::currentPath() + "replicator-data/databases/Replicator_Database_Test.xml";
            qDebug() << pathXML_DATABASE;
    // The QDomDocument class represents an XML document.
    QDomDocument xmlBOM;

    // Load xml file as raw data

    QFile fxml(pathXML_DATABASE);
    if (!fxml.open(QIODevice::ReadOnly)) {
      // Error while loading file
      qDebug() << "Error while loading file" << pathXML_DATABASE;
      // return 1;
    }

    // Set data into the QDomDocument before processing
    xmlBOM.setContent(&fxml);
    fxml.close();

    // Extract the root markup
    QDomElement root = xmlBOM.documentElement();

    // Get the first child of the root (Markup COMPONENT is expected)
    QDomElement Component = root.firstChild().toElement();

    /**** END BASE VARS ****/



    /**** SELECT_UPDATE ****/
    QStringList SELECT_UPDATE_partCommand;
        SELECT_UPDATE_partCommand = SELECT_UPDATE.split(" ");
    QStringList SELECT_UPDATE_partContents;
        SELECT_UPDATE_partContents = SELECT_UPDATE_partCommand[1].split(",");

    foreach (QString elem, SELECT_UPDATE_partContents) {
        //qDebug() << "SELECT " << elem;
        //result.append(elem);
    }
    /**** END SELECT_UPDATE ****/

    /**** WHERE ****/
    QStringList WHERE_partCommand;
        WHERE_partCommand = WHERE.split(" ");
    QStringList WHERE_partContents;
        WHERE_partContents = WHERE_partCommand[1].split(",");

    QString WHERE_IS;

    foreach (QString elem, WHERE_partContents) {
        //qDebug() << "WHERE " << elem;
        WHERE_IS = elem;
    }
    //qDebug() << "WHERE_IS" << WHERE_IS;

    /**** END WHERE ****/

    /**** LIKE_EQUAL ****/
    QStringList LIKE_EQUAL_partCommand;
        LIKE_EQUAL_partCommand = LIKE_EQUAL.split(" ");
    QStringList LIKE_EQUAL_partContents;
        LIKE_EQUAL_partContents = LIKE_EQUAL_partCommand[1].split(",");
if (LIKE_EQUAL_partCommand[0] == "LIKE") {
    foreach (QString elem, LIKE_EQUAL_partContents) {
        //qDebug() << "LIKE_ELEM " << elem;
        QRegularExpression re(elem,QRegularExpression::CaseInsensitiveOption);

        // Loop while there is a child
        while (!Component.isNull()) {
          // Check if the child tag name is COMPONENT
          if (Component.tagName() == "ENTRY") {
              // Read and display the component ID
              QString CURRENT_ELEMENT_TAG = Component.attribute(WHERE_IS.toUpper(), WHERE_IS.toUpper());
              //qDebug() << Component.attribute("TITEL","TITEL");

              // Get the first child of the component
              QDomElement Child = Component.firstChild().toElement();
                //qDebug() << "CURRENT_ELEMENT_TAG: " << CURRENT_ELEMENT_TAG;
            // Read each child of the component node
              //result.append(CURRENT_ELEMENT_TAG);
            while (!Child.isNull()) {


                QRegularExpressionMatch match = re.match(CURRENT_ELEMENT_TAG);
                if (match.hasMatch()) {


                    QString matched = match.captured(0); // matched == "23 def"

                    //qDebug() << matched;
                    //qDebug() << CURRENT_ELEMENT_TAG;
                    foreach (QString elem_2, SELECT_UPDATE_partContents) {

                        //qDebug() << elem_2;
                        if (Child.tagName() == elem_2.toUpper()) {
                          //  qDebug() << Child.tagName();
                            QString tempStringVolume=Child.firstChild().toText().data();
                            list1.append(elem_2.toUpper()+":::"+tempStringVolume);
                        }
                    }

                }


                Child = Child.nextSibling().toElement();

              }
          }
          if (!list1.isEmpty()) {
              list1.append("TITEL:::"+Component.attribute("TITEL","TITEL"));
              list1.append("ID:::"+Component.attribute("ID","ID"));
              matrix.append(list1);
              list1.clear();
          }

          // Next component
          Component = Component.nextSibling().toElement();
        }
    }
}
else if(LIKE_EQUAL_partCommand[0]=="EQUAL") {
    foreach (QString elem, LIKE_EQUAL_partContents) {
        //qDebug() << "LIKE_ELEM " << elem;
        QRegularExpression re(elem,QRegularExpression::CaseInsensitiveOption);

        // Loop while there is a child
        while (!Component.isNull()) {
          // Check if the child tag name is COMPONENT
          if (Component.tagName() == "ENTRY") {
              // Read and display the component ID
              QString CURRENT_ELEMENT_TAG = Component.attribute(WHERE_IS.toUpper(), WHERE_IS.toUpper());
              //qDebug() << Component.attribute("TITEL","TITEL");

              // Get the first child of the component
              QDomElement Child = Component.firstChild().toElement();
                //qDebug() << "CURRENT_ELEMENT_TAG: " << CURRENT_ELEMENT_TAG;
            // Read each child of the component node
              //result.append(CURRENT_ELEMENT_TAG);
            while (!Child.isNull()) {

                //qDebug() << CURRENT_ELEMENT_TAG.toUpper() << elem.toUpper();
                //QRegularExpressionMatch match = re.match(CURRENT_ELEMENT_TAG);
                //if (match.hasMatch()) {
                if ( CURRENT_ELEMENT_TAG.toUpper() == elem.toUpper()) {


                    //QString matched = match.captured(0); // matched == "23 def"

                    //qDebug() << matched;
                    //qDebug() << CURRENT_ELEMENT_TAG;
                    foreach (QString elem_2, SELECT_UPDATE_partContents) {

                        //qDebug() << elem_2;
                        if (Child.tagName() == elem_2.toUpper()) {
                          //  qDebug() << Child.tagName();
                            QString tempStringVolume=Child.firstChild().toText().data();
                            list1.append(elem_2.toUpper()+":::"+tempStringVolume);
                        }
                    }

                }


                Child = Child.nextSibling().toElement();

              }
          }
          if (!list1.isEmpty()) {
              list1.append("TITEL:::"+Component.attribute("TITEL","TITEL"));
              list1.append("ID:::"+Component.attribute("ID","ID"));

              QStringList temp;

              foreach (QString elem, SELECT_UPDATE_partContents) {
                  //qDebug() << "SELECT " << elem;
                  //result.append(elem);
              }

              matrix.append(list1);
              list1.clear();
          }

          // Next component
          Component = Component.nextSibling().toElement();
        }
    }
}
    /**** END LIKE_EQUAL ****/
qDebug() << matrix;
    return matrix;

}

QString db_query::SELECT_DB_RETURN(const QStringList& ergebnis, const QString& request) {

QString result;
QStringList temp;

foreach (QString element, ergebnis) {
    temp = element.split(":::");

    if (temp[0]==request.toUpper()) {
        //qDebug() << temp[1];
        result = temp[1];
        break;
    }
}

return result;

}

QVector<QStringList> db_query::DB_QUERY_COMMAND(const QString& SELECT_UPDATE, const QString& WHERE, const QString& LIKE_EQUAL) {

    QVector<QStringList> StringVectorReturn;

    //QFile datei("/home/ankalagon/Projekte/Qt/Replicator-xml/Replicator_Database_Test.xml");
    //QFile datei("/home/pi/Projekte/Qt/Replicator-xml/Replicator_Database_Test.xml");
    //QFile datei(QDir::currentPath()+"/Replicator_Database_Test.xml");
    //!QFile datei(QDir::homePath()+"/.replicator/server/Replicator_Database.xml");
/*    QFile datei(QDir::homePath()+"/replicator-data/databases/Replicator_Database.xml");

    QBuffer device;
    datei.open(QIODevice::ReadOnly);
    device.setData(datei.readAll());
    device.open(QIODevice::ReadOnly);


    /* < SELECT/UPDATE >*/

/*    QStringList splitSELECT_UPDATE;
        splitSELECT_UPDATE = SELECT_UPDATE.split(" ");
    QStringList SELECT_UPDATE_values;
        SELECT_UPDATE_values = splitSELECT_UPDATE[1].split(",");

    QString queryItems;

    foreach (QString elem,SELECT_UPDATE_values) {
        queryItems+="|descendant::"+elem.toUpper();
    }

    /* </ SELECT/UPDATE > */

    //qDebug() << WHERE;
    //qDebug() <<"doc($inputDocument)/REPLIKATOR/ENTRY[@ID='"+LIKE_EQUAL+"']/(@ID"+queryItems+")/string()";


/*    QXmlQuery query;

    //foldername=ui->listWidget_2->currentItem()->text();
    query.bindVariable("inputDocument", &device);

    if (WHERE.toUpper() =="ID") {

    query.setQuery("doc($inputDocument)/REPLIKATOR/ENTRY[@ID='"+LIKE_EQUAL+"']/(@ID"+queryItems+")/string()");

    } else if (WHERE.toUpper() == "TITLE") {
        //qDebug() << "doc($inputDocument)/REPLIKATOR/ENTRY[matches(TITLE, '"+LIKE_EQUAL.toUpper()+"','i')]/(@ID"+queryItems+")/string()";
        query.setQuery("doc($inputDocument)/REPLIKATOR/ENTRY[matches(TITLE, '"+LIKE_EQUAL.toUpper()+"','i')]/(@ID"+queryItems+")/string()");
    }

    QStringList returnStringList;


    QStringList tempStringElements;
    if (query.isValid()) {
    qDebug() << "valid";

    QStringList sl;

        query.evaluateTo(&sl);
        //qDebug() << sl;
        int iterator = 0;
        for (QStringList::iterator it = sl.begin(); it != sl.end(); ++it){

            tempStringElements.append(SELECT_UPDATE_values[iterator].toUpper()+":::"+*it);
            iterator++;
        if (tempStringElements.count()==SELECT_UPDATE_values.count()) {
            StringVectorReturn.append(tempStringElements);
            iterator=0;
            returnStringList.append(tempStringElements);
            tempStringElements.clear();
        }

        }

    } else{        qDebug() << "query invalid"; }

    /*for (int i=0;i<returnStringList.count();i++) {
        ui->listWidget->addItem(returnStringList[i]);
    }*/


/*    if (devmode==true) {
        qDebug() << StringVectorReturn;
    }
*/

    QString recipeDB_file = "/home/pi/replicator-data/databases/replicator_recipes.db";

    QVector<QStringList> returnList;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

        db.setDatabaseName(recipeDB_file);
        if(db.open()){} /*QMessageBox::information(0,"Ok","Connection ok")*/
        else{} /*QMessageBox::information(0,"Error","some error.")*/;

        QSqlQuery query("SELECT * FROM recipes");
         if (query.lastError().isValid()) {}/*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
         query.exec();

         while (query.next()) {

             QStringList queryResultRow;

             for (int i=0; i<query.record().count();i++) {
                queryResultRow.append(query.value(i).toString());
             }

             returnList.append(queryResultRow);

             }
        db.close();
        db.removeDatabase("QSQLITE");
    qDebug() << returnList;
    return StringVectorReturn;

}

QVector<QStringList> GET_SQL_DATA(QString queryString) {

    QString recipeDB_file = "/home/pi/replicator-data/databases/replicator_recipes.db";

    QVector<QStringList> returnList;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

        db.setDatabaseName(recipeDB_file);
        if(db.open()){} /*QMessageBox::information(0,"Ok","Connection ok")*/
        else{} /*QMessageBox::information(0,"Error","some error.")*/;

        QSqlQuery query("SELECT `id`,`title`,`thumbnail` FROM recipes");
         if (query.lastError().isValid()) {}/*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
         query.exec();

         /*QListWidgetItem newItem;
         newItem.setData(Qt::UserRole, query.value(1).toString());*/
            //query.size()
         qDebug() << query.size();
         while (query.next()) {
/*
                QListWidgetItem newItem;
                newItem.setData(Qt::UserRole, query.value(0).toString());
                newItem.setData(Qt::UserRole+1, query.value(1).toString());
                 if (query.value(0)=="") { newItem.setText("Unbenannter Ordner"); }
                 else { newItem.setText(query.value(0).toString()); }
                 asdf.append(newItem);*/

             QStringList queryList;
             returnList.append(queryList);

             }
        db.close();
        db.removeDatabase("QSQLITE");

    return returnList;

}
