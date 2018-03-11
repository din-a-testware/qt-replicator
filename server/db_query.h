#ifndef DB_QUERY_H
#define DB_QUERY_H
#include <QVector>
#include "globalvars.h"

class db_query
{
public:
    db_query();

    static QVector<QStringList> DB_EXEC(const QString& SELECT, const QString& WHERE, const QString& LIKE_EQUAL);
    static QString SELECT_DB_RETURN(const QStringList& ergebnis, const QString& request);
    static QVector<QStringList> DB_QUERY_COMMAND(const QString& SELECT, const QString& WHERE, const QString& LIKE_EQUAL);

    static QVector<QStringList> GET_SQL_DATA(QString queryString);

};

#endif // DB_QUERY_H
