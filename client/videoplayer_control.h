#ifndef VIDEOPLAYER_CONTROL_H
#define VIDEOPLAYER_CONTROL_H
#include <QMediaPlayer>
#include <QtWidgets>

class VideoPlayer_Control: public QObject
{
    Q_OBJECT
public:
    VideoPlayer_Control(QWidget *parent=0);
        //~VideoPlayer_Control();
    void blubber();
    int bla(int rofl);
    void mediaStateChanged(QMediaPlayer::State state, QPushButton *play_pause);
    void playVideo(QVideoWidget *videoWidget);
    void play(QPushButton *play_pause);
    void setPosition(int position);
    QMediaPlayer testPlayer;
    QListWidget *VideoFilesList;
    QString fileName;
    void initPlayer(QPushButton *open_Video,
                    QPushButton *play_pause,
                    QPushButton *stop,
                    QPushButton *fullscreenButton,
                    QSlider *position_slider,
                    QSlider *volume_slider,
                    QVideoWidget *VideoWidget,
                    /*QListWidget *VideoFilesListWidget*/
                    QString fileURL);
    void positionChanged(QSlider *video_Position,qint64 position);
    void durationChanged(QSlider *video_Position,qint64 duration);
    void listDirVideos(QString dirString);
};

#endif // VIDEOPLAYER_H
