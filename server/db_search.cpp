#include "db_search.h"
#include <QtSql>
#include <QVector>
#include <QStringList>
#include <QString>
#include <QObject>
#include <QDebug>

db_search::db_search(QObject *parent)
{
Q_UNUSED(parent);


}

QVector<QStringList> db_search::get_Data(QString file,QString queryString) {


    QVector<QStringList> returnList;



        db.setDatabaseName(file);
        if(db.open()){
            QSqlQuery query(QSqlDatabase::database("recipeDB",true));
            query.prepare(queryString);
             if (query.lastError().isValid()) {};
             query.exec();

             while (query.next()) {

                 QStringList queryResultRow;

                 for (int i=0; i<query.record().count();i++) {
                    queryResultRow.append(query.value(i).toString());
                 }

                 returnList.append(queryResultRow);

                 }
        }
        else{

        };


        db.close();
    return returnList;

}

db_search::~db_search() {
    QSqlDatabase::removeDatabase("recipeDB");
}
