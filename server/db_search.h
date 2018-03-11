#ifndef DB_SEARCH_H
#define DB_SEARCH_H
#include <QObject>
#include <QVector>
#include <QStringList>
#include <QString>
#include <QtSql>

class db_search: public QObject
{
    Q_OBJECT
public:
    db_search(QObject *parent=0);
    ~db_search();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","recipeDB");

public slots:

    QVector<QStringList> get_Data(QString file, QString queryString);
};

#endif // DB_SEARCH_H
