#ifndef SETTINGS_H
#define SETTINGS_H
#include <QVector>
#include <QVariant>
#include <QObject>
#include <QListWidgetItem>
#include <QtSql>

class settings: public QObject
{
    Q_OBJECT
public:
    settings(QObject *parent=0);
    ~settings();
    QSqlDatabase newdb = QSqlDatabase::addDatabase("QSQLITE","mydb");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","langDB");

    static /*QVector<QVariant>*/ QStringList LOAD(/*const QString& LANG_CODE,*/
                                  const QString& TYPE,
                                  const QString& SETTING,
                                  const QString& LANG_CODE=NULL);

    QString loadLanguage(QString langCode, QString element);
    QList<QStringList> loadSetting(QString queryString);
    QList<QListWidgetItem> languageList();
    QString writeSetting(QString queryString);
};

#endif // SETTINGS_H
