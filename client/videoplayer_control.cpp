#include "videoplayer_control.h"
#include <QDebug>
#include <QMediaPlayer>
#include <QtWidgets>
VideoPlayer_Control::VideoPlayer_Control(QWidget *parent)
{
Q_UNUSED(parent);
}

void VideoPlayer_Control::blubber() {
qDebug() << bla(20);
}

void VideoPlayer_Control::initPlayer(
                                QPushButton *open_Video,
                                QPushButton *play_pause,
                                QPushButton *stop,
                                QPushButton *fullscreenButton,
                                QSlider *position_slider,
                                QSlider *volume_slider,
                                QVideoWidget *VideoWidget,
                                /*QListWidget *VideoFilesListWidget*/
                                QString fileURL
                                ) {
Q_UNUSED(fullscreenButton);
//VideoFilesList=VideoFilesListWidget;
    fileName=fileURL;
testPlayer.setVolume(volume_slider->value());

// ##### VOLUME SLIDER CHANGED
connect(volume_slider,&QSlider::valueChanged,[=](){
        testPlayer.setVolume(volume_slider->value());
    });

/*connect(VideoFilesList,&QListWidget::itemClicked,[=](){

    QListWidgetItem *currentItem;
    currentItem = VideoFilesList->currentItem();

    QFileInfo Fileitem;
    Fileitem.setFile(currentItem->data(Qt::UserRole+3).toString());


    if (Fileitem.isDir()) {
        listDirVideos(Fileitem.filePath());
    } else if (Fileitem.isFile()) {
        //qDebug() << "file:" << Fileitem.filePath();
        //MusicPlaylistEntries.append(Fileitem.filePath());
        fileName = Fileitem.filePath();
    }

});*/
connect(open_Video,&QPushButton::clicked,[=](){playVideo(VideoWidget);});
connect(play_pause,&QPushButton::clicked,[=](){play(play_pause);});
connect(stop,&QPushButton::clicked,[=](){play_pause->setText("T");testPlayer.stop();});


//connect(position_slider,&QSlider::sliderMoved,setPosition());
connect(position_slider,&QSlider::sliderReleased,[=](){setPosition(position_slider->value());});

    //VideoPlayerClass.playVideo(&VideoPlayer,VideoWidget);
    /*
    VideoPlayer.setVideoOutput(videoWidget);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());

    if (!fileName.isEmpty()) {
        VideoPlayer.setMedia(QUrl::fromLocalFile(fileName));
        //playButton->setEnabled(true);
        VideoPlayer.play();
    }*/

    /*connect(&testPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(mediaStateChanged(QMediaPlayer::State,play_pause)));*/
    //qDebug() << "getz2";
    connect(&testPlayer, &QMediaPlayer::stateChanged,[=]() {
        qDebug() << testPlayer.PlayingState;
        mediaStateChanged(testPlayer.PlayingState,play_pause);
    });

    /*connect(&testPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&testPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(&testPlayer, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(handleError()));*/

    //connect(position_slider,&QSlider::sliderMoved,[=](){setPosition(position_slider->value());});

    connect(&testPlayer, &QMediaPlayer::positionChanged,[=]() {
        positionChanged(position_slider,testPlayer.position());
    });

    connect(&testPlayer, &QMediaPlayer::durationChanged,[=]() {
        durationChanged(position_slider,testPlayer.duration());
    });


}


int VideoPlayer_Control::bla(int rofl) {
    return rofl;
}

void VideoPlayer_Control::mediaStateChanged(QMediaPlayer::State state, QPushButton *play_pause)
{
    qDebug()<< state;

    switch(state) {
    case QMediaPlayer::PlayingState:
        //play_pause->setText("[");
        //qDebug()<< state << "-playing-";
        break;
    case QMediaPlayer::StoppedState:
        //play_pause->setText("T");
        //qDebug()<< state << "-stopped-";
    case QMediaPlayer::PausedState:
        //play_pause->setText("T");
        //qDebug()<< state << "-paused-";
        break;
    default:
        //play_pause->setText("T");
        //qDebug()<< state << "default";
        break;
    }
}

void VideoPlayer_Control::playVideo(QVideoWidget *videoWidget) {

    //VideoPlayer->setVideoOutput(videoWidget);

    testPlayer.setVideoOutput(videoWidget);
    //qDebug() << "/home/ankalagon/Laufwerke/Videos/Filme/asterix_und_kleopatra.mkv";
    //fileName = /*QFileDialog::getOpenFileName(parent, tr("Open Movie"),QDir::homePath());*/
    //        "/home/ankalagon/Laufwerke/Videos/Filme/asterix_und_kleopatra.mkv";

    if (!fileName.isEmpty()) {
        testPlayer.setMedia(QUrl::fromLocalFile(fileName));
        //playButton->setEnabled(true);
        testPlayer.play();
    }
}

void VideoPlayer_Control::play(QPushButton *play_pause)
{
    qDebug() << "schon mal hier";
    switch(testPlayer.state()) {
    case QMediaPlayer::PlayingState:
        play_pause->setText("T");
        testPlayer.pause();
        break;
    default:
        play_pause->setText("[");
        testPlayer.play();
        break;
    }
}

void VideoPlayer_Control::setPosition(int position)
{
    testPlayer.setPosition(position);
}

void VideoPlayer_Control::durationChanged(QSlider *video_Position,qint64 duration)
{
    video_Position->setRange(0, duration);
}

void VideoPlayer_Control::positionChanged(QSlider *video_Position,qint64 position)
{
    video_Position->setValue(position);
}

void VideoPlayer_Control::listDirVideos(QString dirString) {
  VideoFilesList->clear();
  VideoFilesList->scrollToTop();
  QDir dir(dirString);

  int i = 0;
  int cnt_list = int(log10(dir.entryList().count()) + 1);





  {
      QFont helvetica_Spaced("Helvetica Ultra Compressed",28);
      helvetica_Spaced.setLetterSpacing(QFont::PercentageSpacing,120);

      /*****************************************************************/
      //for (int i = 0; i < 20; i++) {

      QString cnt_String;

      foreach (QFileInfo Fileitem, dir.entryInfoList()) {

        cnt_String = QString("%1").arg(i, cnt_list, 10, QChar('0'));

      QListWidgetItem *item = new QListWidgetItem();
      item->setData(Qt::DisplayRole, Fileitem.fileName().toUpper());
      item->setData(Qt::UserRole + 1, QString::number(i));
      item->setData(Qt::UserRole + 3, Fileitem.filePath());
      VideoFilesList->addItem(item);


      i++;
      }

  }
}
