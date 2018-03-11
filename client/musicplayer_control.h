#ifndef MUSICPLAYER_CONTROL_H
#define MUSICPLAYER_CONTROL_H
#include <QMediaPlayer>
#include <QtWidgets>
#include <QAudioBuffer>
//#include <taglib/tag.h>
#include <QMediaPlaylist>
#include "musicdb_control.h"

class musicdb_control;

class MusicPlayer_Control: public QObject
{
    Q_OBJECT
public:
    MusicPlayer_Control(QWidget *parent=0);

    musicdb_control *musicDB_control = new musicdb_control;

    QMediaPlayer MusicPlayer;

    QMediaPlaylist *musicPlaylist;

    void initPlayer(
                    QLabel *title,
                    QLabel *artist,
                    QLabel *album,
                    QLabel *time,
                    QLabel *album_pic,
                    QSlider *music_Position,
                    QSlider *Volume,
                    QPushButton *play_pause,
                    QPushButton *stop,
                    QPushButton *next,
                    QPushButton *prev,
                    QPushButton *loop,
                    QProgressBar *left_channel,
                    QProgressBar *right_channel,
                    QListWidget *playlist_widget,
                    QPushButton *music_playlist_play,
                    QButtonGroup *btnGrpPlayPause,
                    QButtonGroup *btnGrpPlayStop,
                    QButtonGroup *btnGrpPlayNext,
                    QButtonGroup *btnGrpPlayPrev);

    QProgressBar *slider_left;
    QListWidget *playlist_list;
    QProgressBar *slider_right;
    QPushButton *play_pause_button;
    QLabel *artist_label;
    QLabel *title_label;
    QLabel *album_label;
    QLabel *cover_art;
    QLabel *duration_label;
    QButtonGroup *buttonGroupPlayPause;
    QButtonGroup *buttonGroupStop;
    QButtonGroup *buttonGroupNext;
    QButtonGroup *buttonGroupPrev;

    int baseVolume;

//    void mediaStateChanged(QMediaPlayer::State state, QPushButton *play_pause);
    void updateTags(QString fileName);
    void playMusic(QStringList *mediaFiles);
    void play();
    QString getTag(QString fileName,QString tagType);
    void processBuffer(const QAudioBuffer& buffer);

    void setPosition(int position);

    void positionChanged(QSlider *music_Position,
                         qint64 position);

    void durationChanged(QSlider *music_Position,
                         qint64 duration);

    void syncPlaylists();
};

#endif // MUSICPLAYER_CONTROL_H


