#ifndef PROCESS_DATA_H
#define PROCESS_DATA_H

#include <QObject>
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslError>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>

class Process_Data : public QObject
{
    Q_OBJECT
public:
    explicit Process_Data(QObject *parent = nullptr);

    static QByteArray serialize(QVector<QStringList> data);
    static QVector<QStringList> deserialize(const QByteArray& byteArray);
    static QString SELECT_DB_RETURN(const QStringList& ergebnis, const QString& request);

signals:

public slots:
};

#endif // PROCESS_DATA_H
