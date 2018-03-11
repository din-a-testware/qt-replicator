#ifndef VIDEOFULLSCREEN_H
#define VIDEOFULLSCREEN_H
#include <QVideoWidget>
#include <QWidget>
#include "videoplayer_control.h"

class VideoPlayer_Control;

namespace Ui {
class VideoFullscreen;
}

class VideoFullscreen : public QWidget
{
    Q_OBJECT

public:
    explicit VideoFullscreen(QWidget *parent = 0);
    ~VideoFullscreen();

    VideoPlayer_Control *VideoPlayerClass = new VideoPlayer_Control;

    QVideoWidget *videoWidget = new QVideoWidget;

private slots:
    void on_VIDEO_MAXIMIZE_clicked();

    void on_VIDEO_BUTTON_CONTROLS_clicked();

    void on_VIDEO_BUTTON_BACK_clicked();

    void on_VIDEO_FILES_PLAY_clicked();

private:
    Ui::VideoFullscreen *ui;
};

#endif // VIDEOFULLSCREEN_H
