#include "videofullscreen.h"
#include "ui_videofullscreen.h"
#include <QVideoWidget>
#include "globalvars.h"
VideoFullscreen::VideoFullscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoFullscreen)
{
    ui->setupUi(this);

    QGridLayout *asdf = new QGridLayout();
    asdf->addWidget(videoWidget);
    ui->VideoSurfaceWidget->setLayout(asdf);

}

VideoFullscreen::~VideoFullscreen()
{

    delete ui;
}

void VideoFullscreen::on_VIDEO_MAXIMIZE_clicked()
{
    ui->VIDEO_PLAY_PAUSE->clicked();
    this->close();
}

void VideoFullscreen::on_VIDEO_BUTTON_CONTROLS_clicked()
{
    switch (ui->VidControls->currentIndex()) {
    case 0:
        ui->VidControls->setCurrentIndex(1);
        break;
    case 1:
        ui->VidControls->setCurrentIndex(0);
        break;
    default:
        break;
    }
}

void VideoFullscreen::on_VIDEO_BUTTON_BACK_clicked()
{

}

void VideoFullscreen::on_VIDEO_FILES_PLAY_clicked()
{

    qDebug() << "videWidget";
    qDebug() << VideoFile;
    VideoPlayerClass->initPlayer(ui->VIDEO_FILES_PLAY,
                                 ui->VIDEO_PLAY_PAUSE,
                                 ui->VIDEO_STOP,
                                 ui->VIDEO_MAXIMIZE,
                                 ui->VIDEO_POSITION,
                                 ui->VIDEO_VOLUME,
                                 videoWidget,
                                 VideoFile);
}
