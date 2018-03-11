#ifndef MUSICDB_CONTROL_H
#define MUSICDB_CONTROL_H
#include <QObject>
#include <QListWidget>
#include <QString>


class musicdb_control: public QObject
{
    Q_OBJECT
public:
    musicdb_control(QObject *parent=0);

public slots:
    QVector <QListWidgetItem> getFolderList();

    QVector <QListWidgetItem> getArtist(QString selected_Folder);
    QVector <QListWidgetItem> getAlbum(QString selected_Folder, QString selected_Artist);
    QVector <QListWidgetItem> getTitle(QString selected_Folder, QString selected_Artist, QString selected_Album);
    QVector <QListWidgetItem> addToPlaylist(QString selected_Folder,QString selected_Titles);
    QVector <QListWidgetItem> getSearch(QString selected_Folder, QString selected_Combo, QString searchInput);
    QStringList getMetadata(QString selected_Titles);

};

#endif // MUSICDB_CONTROL_H
