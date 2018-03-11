#include "settings.h"
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
#include <QXmlResultItems>
#include <QtSql>
#include <QObject>
#include <QListWidgetItem>


settings::settings(QObject *parent)
{
    Q_UNUSED(systemSpeed);
    Q_UNUSED(devmode);
    Q_UNUSED(redActive);
    Q_UNUSED(redAlertKillMe);
    newdb.setDatabaseName(settingsDbFile);
    db.setDatabaseName(settingsDbFile);
}

QString settings::writeSetting(QString queryString) {
    QString returnError;

    /*QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(settingsDbFile);*/

    if(db.open()) {
        QSqlQuery query(QSqlDatabase::database("mydb"));
        query.prepare(queryString);
         if (query.lastError().isValid()){
            returnError = "is Valid";
         } else {
             returnError = "Query is invalid: " + query.lastError().text();
         }
         query.exec();

    } else{ returnError = "DB is not open!";}
    //db.close();
    return returnError;
}

QList<QListWidgetItem> settings::languageList() {

    QList<QListWidgetItem> listReturner;
    if(db.open()) {
        //QSqlQuery query("SELECT `code`,`text` FROM translation_codes");
        QSqlQuery query(QSqlDatabase::database("langDB",true));
        query.prepare("SELECT `code`,`text` FROM translation_codes");

         if (query.lastError().isValid()){

         };
         query.exec();

         while (query.next()) {
            QListWidgetItem item;
            item.setData(Qt::UserRole,query.value(0));
            item.setData(Qt::DisplayRole,query.value(1));
            listReturner.append(item);
             }
    } else{ }
    //db.close();

    return listReturner;


}

QList<QStringList> settings::loadSetting(QString queryString) {
    QList<QStringList> listReturner;

    if(newdb.open()) {
        QSqlQuery query(QSqlDatabase::database("mydb",true));
        query.prepare(queryString);
         if (query.lastError().isValid()){};
         query.exec();

         while (query.next()) {
             QStringList returnValue;

             for (int i=0; i<=query.last()+1;i++) {

                 returnValue.append(query.value(i).toString());
                }
             listReturner.append(returnValue);
             }

    }
    else{ }



    return listReturner;
}

QString settings::loadLanguage(QString langCode, QString element) {

    QString returnValue;

    if(db.open()) {}
    else{ }
    db.setDatabaseName(settingsDbFile);

    QSqlQuery query(QSqlDatabase::database("langDB",true));
            query.prepare("SELECT `"+element+"` FROM translations WHERE `lang_code` = '"+langCode+"'");
     if (query.lastError().isValid()){};
     query.exec();


     while (query.next()) {

             returnValue = query.value(0).toString();
         }

    return returnValue;

}

QStringList settings::LOAD(/*const QString& LANG_CODE,*/
                                 const QString& TYPE,
                                 const QString& SETTING,
                                 const QString& LANG_CODE) {

    /* */

    QStringList retVal;
    QStringList retVal2;
    QString returnValue1;
    deploySystem = "linux";
    QFile structure1;
    /*if (deploySystem == "linux") {
        if (devmode == true) {
            qDebug() << "blubber";
            structure1.setFileName("/home/ankalagon/Dokumente/Replikator_Settings.xml");


        } else {
            structure1.setFileName("../Replikator_Settings.xml");
        }

    } else if (deploySystem == "android") {
        structure1.setFileName(":/settings/Replikator_Settings.xml");
    }*/
    //structure1.setFileName("/home/ankalagon/Dokumente/Replikator_Settings.xml");
    //structure1.setFileName("/home/ankalagon/Projekte/Qt/Replicator-xml/Replikator_Settings.xml");
    //structure1.setFileName(QDir::currentPath() + "/../Replikator_Settings.xml");
    structure1.setFileName(":/settings/Replikator_Settings.xml");
    //qDebug() << QDir::currentPath();
    //qDebug() << structure1.fileName();

    structure1.open(QFile::ReadOnly);

    QString base_Name1 = "REPLICATOR/";
    QXmlQuery query1;
    query1.setFocus(&structure1);

    query1.setQuery(base_Name1+"INTERFACE");
    QXmlResultItems results1;
    query1.evaluateTo(&results1);

    QXmlQuery childQuery1(query1.namePool());
    while (!results1.next().isNull()) {
        childQuery1.setFocus(results1.current());
        //childQuery.setQuery("nam/text()");

        childQuery1.setQuery("TRANSLATIONS/data(@DEFAULT)");
        //childQuery1.setQuery(SETTING);
        QString r1;
        childQuery1.evaluateTo(&r1);
        returnValue1 = r1.remove("\n");
        //returnValue.append(r.remove("\n"));

    }
    structure1.close();
    retVal.append(returnValue1);



    /* */



    //QVector<QVariant> returnValue;
    QString returnValue;

    QFile structure(":/settings/Replikator_Settings.xml");
    structure.open(QFile::ReadOnly);

    QString base_Name = "REPLICATOR/";
    QXmlQuery query;
    query.setFocus(&structure);

    //query.setQuery(base_Name+"/INTERFACE/TRANSLATIONS/LANG[@CODE = '"+LANG_CODE+"']");
    if (TYPE.toUpper() == "INTERFACE") {

        if (LANG_CODE!=NULL) {
            base_Name+="INTERFACE/TRANSLATIONS/LANG[@CODE = '"+LANG_CODE+"']";
        } else {
            base_Name+="INTERFACE/TRANSLATIONS/LANG[@CODE = '"+returnValue1+"']";
        }
    } else if( TYPE.toUpper() == "LANG_LIST") {
        base_Name+="INTERFACE/TRANSLATIONS";
    } else {
        base_Name+=TYPE.toUpper();
    }

    query.setQuery(base_Name);


    QXmlResultItems results;
    query.evaluateTo(&results);

    QXmlQuery childQuery(query.namePool());
    while (!results.next().isNull()) {

        childQuery.setFocus(results.current());
        //childQuery.setQuery("nam/text()");

        //childQuery.setQuery("MAINMENU/SEARCH/text()");
        childQuery.setQuery(SETTING);
        QString r;
        childQuery.evaluateTo(&r);
        returnValue.append(r.remove("\n"));
   }

    retVal.append(returnValue);
    return retVal;
}

settings::~settings() {
    newdb.close();
    db.close();
    QSqlDatabase::removeDatabase("mydb");
    QSqlDatabase::removeDatabase("langDB");
}
