#include "process_data.h"
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslError>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QDataStream>

Process_Data::Process_Data(QObject *parent) : QObject(parent)
{

}

QString Process_Data::SELECT_DB_RETURN(const QStringList& ergebnis, const QString& request) {

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


QVector<QStringList> Process_Data::deserialize(const QByteArray& byteArray)
{
    QVector<QStringList> data;
    QDataStream stream(byteArray);
        stream.setVersion(QDataStream::Qt_4_5);

        stream >> data;
        return data;
}

QByteArray Process_Data::serialize(QVector<QStringList> data)
{
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);
    out << data;
    return byteArray;
}
