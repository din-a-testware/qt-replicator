#include "musicplayer_control.h"
#include <QDebug>
#include <QMediaPlayer>
#include <QtWidgets>
#include <QAudioBuffer>
#include <QAudioProbe>
#include <QMediaMetaData>
#include "extra_functions.h"
/*#include <taglib/tag.h>
#include <taglib/taglib.h>
#include <taglib/id3v2tag.h>
#include <taglib/fileref.h>
#include <taglib/id3v2frame.h>
#include <taglib/id3v2header.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/mpegfile.h>
#include <taglib/mpegheader.h>
#include <taglib/mpegproperties.h>*/
#include <QMediaPlaylist>
#include "listdelegate.h"
#include "musicdb_control.h"
#include "globalvars.h"

static qreal getPeakValue(const QAudioFormat &format);
static QVector<qreal> getBufferLevels(const QAudioBuffer &buffer);

template <class T>
static QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels);


MusicPlayer_Control::MusicPlayer_Control(QWidget *parent)
{
    Q_UNUSED(parent);
    musicPlaylist = new QMediaPlaylist;
}


void MusicPlayer_Control::initPlayer(
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
                                QButtonGroup *btnGrpPlayPrev)
{

    Q_UNUSED(loop);

    // ##### LINK ARGS TO VARS
    artist_label = artist;
    title_label = title;
    album_label = album;
    cover_art = album_pic;
    duration_label = time;
    play_pause_button =play_pause;
    slider_left=left_channel;
    slider_right=right_channel;
    playlist_list=playlist_widget;
    buttonGroupPlayPause=btnGrpPlayPause;
    buttonGroupStop=btnGrpPlayStop;
    buttonGroupNext=btnGrpPlayNext;
    buttonGroupPrev=btnGrpPlayPrev;

    //QList<QPushButton> *playbuttonlist;

    // ##### SET SOME VALUES: MUSIC PLAYER VOLUME; CLEAR PLAYLIST; SET PLAYLIST LOOP; SET PLAYLIST FOR PLAYER

    MusicPlayer.setVolume((baseVolume*Volume->value())/100);
    musicPlaylist->clear();
    musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    MusicPlayer.setPlaylist(musicPlaylist);

    // ##### NEXT CLICKED

    connect(buttonGroupNext, static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked),
        [=](QAbstractButton *button){ MusicPlayer.playlist()->next();
        //qDebug() << musicPlaylist->mediaCount();
        QString dateiname;
        dateiname = MusicPlayer.currentMedia().canonicalUrl().toString().remove("file://");
        playlist_widget->setCurrentRow(MusicPlayer.playlist()->currentIndex());
        updateTags(dateiname);});

    /*connect(next,&QPushButton::clicked,[=](){

            MusicPlayer.playlist()->next();
            //qDebug() << musicPlaylist->mediaCount();
            QString dateiname;
            dateiname = MusicPlayer.currentMedia().canonicalUrl().toString().remove("file://");
            playlist_widget->setCurrentRow(MusicPlayer.playlist()->currentIndex());
            updateTags(dateiname);

        });*/

    // ##### PREVIOUS CLICKED

    connect(buttonGroupPrev, static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked),
        [=](QAbstractButton *button){ MusicPlayer.playlist()->previous();
        //qDebug() << musicPlaylist->mediaCount();
        QString dateiname;
        dateiname = MusicPlayer.currentMedia().canonicalUrl().toString().remove("file://");
        playlist_widget->setCurrentRow(MusicPlayer.playlist()->currentIndex());
        });

    /*connect(prev,&QPushButton::clicked,[=](){

            MusicPlayer.playlist()->previous();
            //qDebug() << musicPlaylist->mediaCount();
            QString dateiname;
            dateiname = MusicPlayer.currentMedia().canonicalUrl().toString().remove("file://");
            playlist_widget->setCurrentRow(MusicPlayer.playlist()->currentIndex());
            //(dateiname);
        });*/

    // ##### PLAY/PAUSE CLICKED
    /*foreach (QPushButton *button, playbuttonlist) {
        connect(button,&QPushButton::clicked,[=](){
           play();
        });
    }
    connect(play_pause,&QPushButton::clicked,[=](){
            play();
        });*/

    connect(buttonGroupPlayPause, static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked),
        [=](QAbstractButton *button){ play(); });


    connect(buttonGroupStop, static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked),
        [=](QAbstractButton *button){ foreach (QAbstractButton *button, buttonGroupPlayPause->buttons()) {
            button->setText("T");
            MusicPlayer.stop();
        }
        });

    // ##### STOP CLICKED
    /*connect(stop,&QPushButton::clicked,[=](){
            //---play_pause_button->setText("T");MusicPlayer.stop();

            foreach (QAbstractButton *button, buttonGroupPlayPause->buttons()) {
                    button->setText("T");
                }

        });*/

    // ##### POSITION SLIDER RELEASED
    connect(music_Position,&QSlider::sliderReleased,[=](){
        //qDebug() << "released";
            setPosition(music_Position->value());
        });

    // ##### PLAYER POSITION CHANGED
    connect(&MusicPlayer, &QMediaPlayer::positionChanged,[=]() {
            positionChanged(music_Position,MusicPlayer.position());

        });

    // ##### PLAYER DURATION CHANGED
    connect(&MusicPlayer, &QMediaPlayer::durationChanged,[=]() {
        //qDebug() << "durationChanged";
            durationChanged(music_Position,MusicPlayer.duration());
        });

    // ##### PLAYER MEDIA CHANGED
    connect(&MusicPlayer, &QMediaPlayer::mediaChanged,[=]() {
            //qDebug() << "mediaChanged:" << MusicPlayer.currentMedia().canonicalUrl().toString();
        });

    // ##### VOLUME SLIDER CHANGED
    connect(Volume,&QSlider::valueChanged,[=](){
            MusicPlayer.setVolume((baseVolume*Volume->value())/100);
        });

    // ##### PLAY ITEM FROM PLAYLIST
    connect(music_playlist_play,&QPushButton::clicked,[=](){
            MusicPlayer.playlist()->setCurrentIndex(playlist_widget->currentRow());
        });

    // ##### CREATE AUDIO PROBE
    QAudioProbe *probe = new QAudioProbe;

    // ##### CONNECT PROBE TO PROCESS BUFFER AND SET PROBE SOURCE TO MUSIC PLAYER
    connect(probe, &QAudioProbe::audioBufferProbed, this, &MusicPlayer_Control::processBuffer);
    probe->setSource(&MusicPlayer);

}

void MusicPlayer_Control::syncPlaylists() {

}

QString MusicPlayer_Control::getTag(QString fileName,QString tagType) {

    // ##### GET THE REQUIRED TAGS
    if (fileName != "") {
    //!TagLib::MPEG::File ffile(fileName.toStdString().c_str());

    //!TagLib::ID3v2::Tag *tag = ffile.ID3v2Tag();

    if (tagType=="artist") {
//!        return tag->artist().toCString();
    }

    else if (tagType=="album") {
//!        return tag->album().toCString();
    }

    else if (tagType=="track") {
//!        return QString::number(tag->track());
    }

    else if (tagType=="title") {
//!        return tag->title().toCString();
    }

    else if (tagType=="image") {

    }

    else if (tagType=="duration") {

    }

    }
    return 0;

}

void MusicPlayer_Control::updateTags(QString fileName) {

    // ##### UPDATE THE TAG LABELS

    if (!fileName.isEmpty()) {

        // ##### CHECK IF FILENAME IS HTTP STREAM
        if (fileName.contains("http://")) {
            artist_label->setText(fileName.remove("http://"));
            title_label->setText(fileName.remove("http://"));
            album_label->setText("");
        }

        // ##### ELSE IT SHOULD BE A LOCAL FILE
        else {

            QStringList dbTags = musicDB_control->getMetadata(playlist_list->currentItem()->data(Qt::UserRole-1).toString());

//qDebug() << "MPC:" << dbTags;

            artist_label->setText(dbTags.at(0));

            album_label->setText(dbTags.at(1));

            title_label->setText(dbTags.at(2));

            //cover_art->setText(dbTags.at(3));
            // ##### INITIALIZE TAGLIB
/*!            TagLib::MPEG::File ffile(fileName.toStdString().c_str());
            TagLib::ID3v2::Tag *tag = ffile.ID3v2Tag();
            TagLib::ID3v2::FrameList l = tag->frameList("APIC");*/


            // ##### THAT'S FOR THE COVER ART
            //QImage image;

            /*!if(l.isEmpty()) {

                // ##### IF TAG HAS NO COVER ART SET PLACEHOLDER FOR THE IMAGE
                cover_art->setPixmap(QPixmap(":/images/UFP-Logo_even.png"));
            }
            else {

                // ##### CREATE COVER ART

                TagLib::ID3v2::AttachedPictureFrame *f =
                static_cast<TagLib::ID3v2::AttachedPictureFrame *>(l.front());
                image.loadFromData((const uchar *) f->picture().data(), f->picture().size());
                cover_art->setPixmap(QPixmap::fromImage(image));
            }*/

            QString bild = dbTags.at(3);
            QByteArray base64Data = bild.toStdString().c_str();
            QImage image;
            image.loadFromData(QByteArray::fromBase64(base64Data)/*, "PNG"*/);
            // QImage resized = image->scaled(480,332,Qt::KeepAspectRatio);
            if (image.loadFromData(QByteArray::fromBase64(base64Data)/*, "PNG"*/) == false) {
                cover_art->setPixmap(QPixmap::fromImage(QImage(":/images/UFP-Logo_even.png")));
            } else {
            QImage ResizedImage =
                image.scaled(200, 200, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                cover_art->setPixmap(QPixmap::fromImage(ResizedImage));
            }


        // ##### DURATION LABEL
        QTime duration(0, MusicPlayer.duration() / 60000,
                               qRound((MusicPlayer.duration() % 60000) / 1000.0));
        duration_label->setText(duration.toString(tr("hh:mm:ss")));

        // ##### ARTIST, TITLE, ALBUM LABEL
/*!        artist_label->setText(tag->artist().toCString());
        title_label->setText(tag->title().toCString());
        album_label->setText(tag->album().toCString());*/
}
    }

}

void MusicPlayer_Control::playMusic(QStringList *mediaFiles) {


    // ##### ADD FILES TO PLAYLIST AND PLAY

    // ##### IF THERE IS NONE, DO NOTHING
    if (mediaFiles->count() == 0) {

    }
    // ##### IF THERE IS AT LEAST ONE IN THE STRINGLIST LET'S PLAY WITH IT
    else if (mediaFiles->count() >=1) {

        // ##### FOREACH ITEM IN THE STRINGLIST ADD TO PLAYLIST AND PLAYLIST WIDGET
        for (int i=0;i<mediaFiles->count();++i) {

            // ##### ADD MEDIA TO PLAYLIST
            musicPlaylist->addMedia(QUrl::fromUserInput(mediaFiles->at(i)));

            // ##### ADD MEDIA TO PLAYLIST WIDGET
            QString fullFilePath = mediaFiles->at(i);
            QListWidgetItem *item = new QListWidgetItem();
            item->setData(Qt::DisplayRole, getTag(mediaFiles->at(i),"artist").toUpper() + " - " +getTag(mediaFiles->at(i),"title"));
            item->setData(Qt::UserRole + 1, QString::number(musicPlaylist->mediaCount()-1));
            item->setData(Qt::UserRole + 2, getTag(mediaFiles->at(i),"track"));
            item->setData(Qt::UserRole + 3, fullFilePath);
            playlist_list->addItem(item);
        }

        // ##### CHECK IF PLAYER IS PLAYING; IF NOT, PLAY!
        switch(MusicPlayer.state()) {
        case QMediaPlayer::PlayingState:
            break;
        default:
            //---play_pause_button->setText("[");
            foreach (QAbstractButton *button, buttonGroupPlayPause->buttons()) {
                    button->setText("[");
                }
            MusicPlayer.play();

            updateTags(MusicPlayer.currentMedia().canonicalUrl().toString().remove("file://"));
            break;
        }
    }
}

void MusicPlayer_Control::play()
{
    // ##### CHANGE PLAY PAUSE STATE AND BUTTON ICON
    switch(MusicPlayer.state()) {
    case QMediaPlayer::PlayingState:
        // ##### 4 IS FOR DISPLAYING > FROM THE FONT
        //---play_pause_button->setText("T");


        foreach (QAbstractButton *button,buttonGroupPlayPause->buttons() ) {
                button->setText("T");
            }



        MusicPlayer.pause();
        break;
    default:
        // ##### ; IS FOR DISPLAYING || FROM THE FONT
        //---play_pause_button->setText("[");

        foreach (QAbstractButton *button, buttonGroupPlayPause->buttons()) {
                button->setText("[");
            }

        MusicPlayer.play();
        break;
    }
}

void MusicPlayer_Control::setPosition(int position)
{
    // ##### SET TIME POSITION OF THE CURRENT MEDIA
    MusicPlayer.setPosition(position);
}

void MusicPlayer_Control::durationChanged(QSlider *music_Position,qint64 duration)
{
    // ##### CHANGE RANGE OF THE POSITION SLIDER
    music_Position->setRange(0, duration);
    playlist_list->setCurrentRow(MusicPlayer.playlist()->currentIndex());
    updateTags(MusicPlayer.currentMedia().canonicalUrl().toString().remove("file://"));
}

void MusicPlayer_Control::positionChanged(QSlider *music_Position,qint64 position)
{
    // ##### IS POS CHANGED IN MEDIA, CHANGE IT ON THE SLIDER TOO
    music_Position->setValue(position);

    QTime duration(0, (MusicPlayer.duration()-MusicPlayer.position()) / 60000,
                           qRound(((MusicPlayer.duration()-MusicPlayer.position()) % 60000) / 1000.0));
    duration_label->setText(duration.toString(tr("hh:mm:ss")));
}




// This function returns the maximum possible sample value for a given audio format
qreal getPeakValue(const QAudioFormat& format)
{
    // Note: Only the most common sample formats are supported
    if (!format.isValid())
        return qreal(0);

    if (format.codec() != "audio/pcm")
        return qreal(0);

    switch (format.sampleType()) {
    case QAudioFormat::Unknown:
        break;
    case QAudioFormat::Float:
        if (format.sampleSize() != 32) // other sample formats are not supported
            return qreal(0);
        return qreal(1.00003);
    case QAudioFormat::SignedInt:
        if (format.sampleSize() == 32)
            return qreal(INT_MAX);
        if (format.sampleSize() == 16)
            return qreal(SHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(CHAR_MAX);
        break;
    case QAudioFormat::UnSignedInt:
        if (format.sampleSize() == 32)
            return qreal(UINT_MAX);
        if (format.sampleSize() == 16)
            return qreal(USHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(UCHAR_MAX);
        break;
    }

    return qreal(0);
}

// returns the audio level for each channel
QVector<qreal> getBufferLevels(const QAudioBuffer& buffer)
{
    QVector<qreal> values;

    if (!buffer.format().isValid() || buffer.format().byteOrder() != QAudioFormat::LittleEndian)
        return values;

    if (buffer.format().codec() != "audio/pcm")
        return values;

    int channelCount = buffer.format().channelCount();
    values.fill(0, channelCount);
    qreal peak_value = getPeakValue(buffer.format());
    if (qFuzzyCompare(peak_value, qreal(0)))
        return values;

    switch (buffer.format().sampleType()) {
    case QAudioFormat::Unknown:
    case QAudioFormat::UnSignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<quint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<quint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<quint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] = qAbs(values.at(i) - peak_value / 2) / (peak_value / 2);
        break;
    case QAudioFormat::Float:
        if (buffer.format().sampleSize() == 32) {
            values = getBufferLevels(buffer.constData<float>(), buffer.frameCount(), channelCount);
            for (int i = 0; i < values.size(); ++i)
                values[i] /= peak_value;
        }
        break;
    case QAudioFormat::SignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<qint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<qint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<qint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] /= peak_value;
        break;
    }

    return values;
}

template <class T>
QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels)
{
    QVector<qreal> max_values;
    max_values.fill(0, channels);

    for (int i = 0; i < frames; ++i) {
        for (int j = 0; j < channels; ++j) {
            qreal value = qAbs(qreal(buffer[i * channels + j]));
            if (value > max_values.at(j))
                max_values.replace(j, value);
        }
    }

    return max_values;
}

void MusicPlayer_Control::processBuffer(const QAudioBuffer& buffer)
{
    QVector<qreal> levels = getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i) {
        //qDebug() << levels.count();
        //qDebug() << int(levels.at(i)*100);
        slider_left->setValue(int(levels.at(0)*100));
        //m_audioLevels.at(i)->setLevel(levels.at(i));
        slider_right->setValue(int(levels.at(1)*100));
    }
}

