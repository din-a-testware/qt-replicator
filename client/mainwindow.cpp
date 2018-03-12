#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videofullscreen.h"
#include "extra_functions.h"

#include "listdelegate.h"
#include "listdelegaterecipe.h"
#include "listdelegate_mediadatabase.h"
#include "listdelegateedge.h"
#include "listdelegatenotes.h"
#include "listdelegatelanguages.h"

#include "videoplayer_control.h"
#include "musicplayer_control.h"
#include "gradientprogressbar.h"
#include "lcars_roundededgeconnector.h"
#include "lcars_button.h"

#include <QColorDialog>
#include "ExifTool.h"
#include "settings.h"
#include <QTimer>
#include <QTime>
#include <QMediaPlayer>
#include "globalvars.h"
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>
#include <QDataStream>
#include <QGridLayout>
#include "process_data.h"
#include <QSignalMapper>
#include <QScrollBar>
#include "animations.h"
#include <QMultimedia>
#include <QAudioProbe>
#include <QAudioBuffer>
#include <QFileDialog>
#include <QMediaMetaData>
#include <QtWidgets>
#include <qvideowidget.h>
#include <qvideosurfaceformat.h>
#include <QPropertyAnimation>

#include "musicdb_control.h"
#include <QtSql>
#include <QNetworkInterface>
#include <QHostInfo>

#include "videofullscreen.h"
#include <QListWidgetItem>




static qreal getPeakValue(const QAudioFormat &format);
static QVector<qreal> getBufferLevels(const QAudioBuffer &buffer);

template <class T>
static QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    Q_UNUSED(systemSpeed);
    Q_UNUSED(devmode);
    Q_UNUSED(redActive);
    Q_UNUSED(redAlertKillMe);

    ui->setupUi(this);

    initReplicator();

}


void MainWindow::initReplicator() {

        ui->MEDIA_CONTROLS_CONTENTS->setVisible(false);
        ui->controls->setVisible(false);
        ui->PAGES->setCurrentWidget(ui->START);
        qDebug() << "[0]";
        currentLanguage = settingsClass->loadSetting("SELECT `code` FROM translation_codes WHERE `selected` = '1'").at(0).at(0);
        qDebug() << "[1]";
        loadSettings(currentLanguage);
        serverSettings();
        widgetsSettings();
        timerSettings();
        fontSettings();
        codeSettings();
        mediaSettings();
        delegateSettings();
        pathSettings();
        connections();
        rippedOffTab = ui->VIDEO_PLAYER;

}

void MainWindow::widgetsSettings() {

    QList<QListWidgetItem> listItemLanguage;
    listItemLanguage = settingsClass->languageList();


    int i=0;
    foreach (QListWidgetItem itemIterator, listItemLanguage) {
    qDebug() << itemIterator.text();
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setData(Qt::DisplayRole,itemIterator.data(Qt::DisplayRole));
        newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
        ui->SET_AVAIL_LANG_LIST->addItem(newItem);
        i++;
    }


    ui->VersionLabel->setText(QString("Compilation time: "+compilationTime).toUpper());

    QGridLayout *asdf = new QGridLayout();
    asdf->addWidget(videoWidget);
    ui->VideoSurfaceWidget->setLayout(asdf);

    /************************************** < CREATE LIST OF WIDGETS FOR RED ALERT FLASH ANIM > **************************************/
    //QList<QWidget *> alarm_id_widgets;
    all_widgets = this->findChildren<QWidget *>();

    red_alert_widgets = extra_functions::WidgetReturnVector(all_widgets,"alarm_id");

    AnimationClass->all_widgets=all_widgets;
    AnimationClass->red_alert_widgets=red_alert_widgets;
    /*** MUSIC PLAYBACK MODES **/


    {


       QListWidgetItem *modeOneStop = new QListWidgetItem;
       QListWidgetItem *modeOne = new QListWidgetItem;
       QListWidgetItem *modeNone = new QListWidgetItem;
       QListWidgetItem *modeAll = new QListWidgetItem;
       QListWidgetItem *modeRandom = new QListWidgetItem;

       modeOneStop->setText("EINZELNEN TITEL");
       modeOne->setText("TITEL WIEDERHOLEN");
       modeNone->setText("KEINE WIEDERHOLUNG");
       modeAll->setText("ALLE WIEDERHOLEN");
       modeRandom->setText("ZUFALL");

       modeOneStop->setData(Qt::UserRole,QMediaPlaylist::CurrentItemOnce);
       modeOneStop->setData(Qt::UserRole+1,"2");
       modeOne->setData(Qt::UserRole,QMediaPlaylist::CurrentItemInLoop);
       modeOne->setData(Qt::UserRole+1,"1");
       modeNone->setData(Qt::UserRole,QMediaPlaylist::Sequential);
       modeNone->setData(Qt::UserRole+1,"3");
       modeAll->setData(Qt::UserRole,QMediaPlaylist::Loop);
       modeAll->setData(Qt::UserRole+1,"0");
       modeRandom->setData(Qt::UserRole,QMediaPlaylist::Random);
       modeRandom->setData(Qt::UserRole+1,"4");

       ui->MUSIC_MODE_SELECT->addItem(modeAll);
       ui->MUSIC_MODE_SELECT->addItem(modeOne);
       ui->MUSIC_MODE_SELECT->addItem(modeOneStop);
       ui->MUSIC_MODE_SELECT->addItem(modeNone);
       ui->MUSIC_MODE_SELECT->addItem(modeRandom);

       ui->MUSIC_MODE_SELECT->setCurrentRow(0);
    }

}
void MainWindow::codeSettings() {
    //!*qDebug() << "13";
    code << "alpha" << "beta" << "gamma" << "delta";
    //codeString << "" << "" << "" << "";



    codeButtons= new QVector<QObject*>();
}

void MainWindow::delegateSettings() {

    ui->MUSIC_DATABASE_LIST->setItemDelegate(new ListDelegate(ui->MUSIC_DATABASE_LIST));
    ui->MUSIC_PLAYLIST->setItemDelegate(new ListDelegate(ui->MUSIC_PLAYLIST));
    ui->MUSIC_MODE_SELECT->setItemDelegate(new ListDelegate(ui->MUSIC_PLAYLIST));
    ui->MUSIC_RADIO_LIST->setItemDelegate(new ListDelegate(ui->MUSIC_RADIO_LIST));
    ui->VIDEO_FILES_LIST->setItemDelegate(new ListDelegate(ui->VIDEO_FILES_LIST));
    ui->SR_RESULT->setItemDelegate(new listDelegateRecipe(ui->SR_RESULT));
    ui->TIMER_LIST->setItemDelegate(new listdelegateEdge(ui->TIMER_LIST));
    ui->RECIPE_Ingredient_list->setItemDelegate(new listdelegateEdge(ui->RECIPE_Ingredient_list));
    ui->MARKED_RECIPES->setItemDelegate(new ListDelegateNotes(ui->MARKED_RECIPES));
    ui->NOTES_LIST->setItemDelegate(new ListDelegateNotes(ui->NOTES_LIST));
    ui->SET_AVAIL_LANG_LIST->setItemDelegate(new ListDelegateLanguages(ui->SET_AVAIL_LANG_LIST));
    //ui->SET_AVAIL_LANG_COMBO->setItemDelegate(new ListDelegateLanguages(ui->SET_AVAIL_LANG_COMBO));

    ui->MUSIC_DATABASE_LIST_FOLDERS->setItemDelegate(new ListDelegate_MediaDatabase(ui->MUSIC_DATABASE_LIST_FOLDERS));
    ui->MUSIC_DATABASE_LIST_ARTISTS->setItemDelegate(new ListDelegate_MediaDatabase(ui->MUSIC_DATABASE_LIST_ARTISTS));
    ui->MUSIC_DATABASE_LIST_ALBUMS->setItemDelegate(new ListDelegate_MediaDatabase(ui->MUSIC_DATABASE_LIST_ALBUMS));
    ui->MUSIC_DATABASE_LIST_TITLES->setItemDelegate(new ListDelegate_MediaDatabase(ui->MUSIC_DATABASE_LIST_TITLES));
    ui->MUSIC_DB_SETTINGS_FOLDERS->setItemDelegate(new ListDelegate_MediaDatabase(ui->MUSIC_DB_SETTINGS_FOLDERS));
    ui->AMBIENT_SOUND_LIST->setItemDelegate(new ListDelegate_MediaDatabase(ui->AMBIENT_SOUND_LIST));

}

void MainWindow::timerSettings() {
    /************************************** < TIMER FOR CURRENT TIME LABEL > **************************************/
    QTimer *timer_zeit = new QTimer(this);
        connect(timer_zeit, SIGNAL(timeout()), this, SLOT(showTime()));
        timer_zeit->start(1000);
        showTime();
    /************************************** </ TIMER FOR CURRENT TIME LABEL > **************************************/

    /************************************** < TIMER RED ALERT > **************************************/

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this,
            SLOT(timedout()));



    timer_countdown = new QTimer(); // Construct the timer
    timer_countdown->setInterval(1000); // One second interval
    timer_countdown->setSingleShot(false); // Multiple shot. This means that the signal
                                     // timeout will be signed each second
    connect(timer_countdown, SIGNAL(timeout()), this,
            SLOT(countdown())); // Connects the timeout signal to my slot timeOut

 AnimationClass->timerPointer=timer;

    /************************************** </ TIMER RED ALERT > **************************************/
}

void MainWindow::mediaSettings() {


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(settingsDbFile);

    if(db.open()) /*QMessageBox::information(0,"Ok","Connection ok")*/;
    else QMessageBox::information(0,"Error","some error.");

    QSqlQuery query("SELECT `vol_main`,`vol_menu`,`vol_media`,`vol_music`,`vol_video`,`vol_alarm`,`brightness` FROM settings WHERE `id` = '0'");
     if (query.lastError().isValid()){};
     query.exec();

     QSqlQuery queryAmbient("SELECT `id`,`title`,`file`,`volume` FROM ambient_sounds");
     //qDebug() << "SELECT `id`,`title`,file`,`volume` FROM ambient_sounds";
     if (queryAmbient.lastError().isValid()){} else {qDebug() << queryAmbient.lastError().text();}
     queryAmbient.exec();


     while (query.next()) {
         mainVolume = query.value(0).toInt();
         menuVolume = query.value(1).toInt();
         mediaVolume = query.value(2).toInt();
         musicVolume = query.value(3).toInt();
         videoVolume = query.value(4).toInt();
         alarmVolume = query.value(5).toInt();
         brightness = query.value(6).toInt();
         }

     while (queryAmbient.next()) {

         QListWidgetItem *newItem = new QListWidgetItem;
         newItem->setText(queryAmbient.value(1).toString());
         newItem->setData(Qt::UserRole-1,queryAmbient.value(0).toString());
         newItem->setData(Qt::UserRole,queryAmbient.value(2).toString());
         newItem->setData(Qt::UserRole+1,queryAmbient.value(3).toInt());
         //qDebug() << queryAmbient.value(3).toInt();
         ui->AMBIENT_SOUND_LIST->addItem(newItem);
         QMediaPlayer *ambPlayer = new QMediaPlayer;
         QMediaPlaylist *ambPlaylist = new QMediaPlaylist;
         //ambPlaylist->addMedia(QUrl::fromUserInput("/home/pi/replicator-data/media/ambient/"+queryAmbient.value(2).toString()));
         ambPlayer->setPlaylist(ambPlaylist);
         ambPlayer->setVolume(queryAmbient.value(3).toInt());
         ambPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
         ambientSounds.append(ambPlayer);

         }

    db.close();
    //qDebug() << settingsDbFile;

    /*VideoPlayerClass->initPlayer(ui->VIDEO_FILES_PLAY,
                                 ui->VIDEO_PLAY_PAUSE,
                                 ui->VIDEO_STOP,
                                 ui->VIDEO_MAXIMIZE,
                                 ui->VIDEO_POSITION,
                                 ui->VIDEO_VOLUME,
                                 videoWidget,
                                 ui->VIDEO_FILES_LIST);*/

    /*QVector<LCARS_Button> qPlaypauseList;
    qPlaypauseList.append(ui->TOP_MENU_MUSIC_PLAYPAUSE);
    qPlaypauseList.append(ui->MUSIC_PLAYER_PLAY_PAUSE);*/

    buttonGroupPLayPause = new QButtonGroup(this);
    buttonGroupPLayPause->addButton(ui->TOP_MENU_MUSIC_PLAYPAUSE);
    buttonGroupPLayPause->addButton(ui->MUSIC_PLAYER_PLAY_PAUSE);

    buttonGroupStop = new QButtonGroup(this);
    buttonGroupStop->addButton(ui->TOP_MENU_MUSIC_STOP);
    buttonGroupStop->addButton(ui->MUSIC_PLAYER_STOP);

    buttonGroupNext = new QButtonGroup(this);
    buttonGroupNext->addButton(ui->TOP_MENU_MUSIC_NEXT);
    buttonGroupNext->addButton(ui->MUSIC_PLAYER_NEXT);

    buttonGroupPrev = new QButtonGroup(this);
    buttonGroupPrev->addButton(ui->TOP_MENU_MUSIC_PREV);
    buttonGroupPrev->addButton(ui->MUSIC_PLAYER_PREV);

    MusicPlayerClass->initPlayer(
                                 ui->MUSIC_PLAYER_LABEL_TITLE,
                                 ui->MUSIC_PLAYER_ARTIST,
                                 ui->MUSIC_PLAYER_ALBUM,
                                 ui->MUSIC_PLAYER_TIME,
                                 ui->MUSIC_PLAYER_COVERART,
                                 ui->MUSIC_PLAYER_POSITION,
                                 ui->MUSIC_PLAYER_VOLUME,
                                 ui->MUSIC_PLAYER_PLAY_PAUSE,
                                 ui->MUSIC_PLAYER_STOP,
                                 ui->MUSIC_PLAYER_NEXT,
                                 ui->MUSIC_PLAYER_PREV,
                                 ui->MUSIC_PLAYER_LOOP,
                                 ui->MUSIC_PLAYER_LEFT_VIS,
                                 ui->MUSIC_PLAYER_RIGHT_VIS,
                                 ui->MUSIC_PLAYLIST,
                                 ui->MUSIC_PLAYLIST_PLAY,
                                 buttonGroupPLayPause,
                                 buttonGroupStop,
                                 buttonGroupNext,
                                 buttonGroupPrev
                                 );




    menuBeeper = new QMediaPlayer;
    menuBeeper->setVolume((mainVolume*menuVolume)/100);

    menuVolumeGlobal = (mainVolume*menuVolume)/100;



    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_RedAlert->setMedia(QUrl("qrc:/sounds/tng_red_alert1.mp3"));
    Sound_MenuBeep->setVolume((mainVolume*menuVolume)/100);
    Sound_RedAlert->setVolume((mainVolume*alarmVolume)/100);

    MusicPlayerClass->baseVolume=mainVolume;

    ui->MAIN_VOLUME_SLIDER->setValue(mainVolume);
    ui->MENU_VOLUME_SLIDER->setValue(menuVolume);
    ui->MEDIA_VOLUME_SLIDER->setValue(mediaVolume);
    ui->ALARM_VOLUME_SLIDER->setValue(alarmVolume);
    ui->MUSIC_PLAYER_VOLUME->setValue(musicVolume);
    ui->VIDEO_VOLUME->setValue(videoVolume);
    ui->DISPLAY_BRIGHTNESS_SLIDER->setValue(brightness);
    ui->DISPLAY_BRIGHTNESS_SLIDER_2->setValue(brightness);



}

void MainWindow::pathSettings() {
    //!*qDebug() << "15";
    QString basePath;
    if (devmode==true) {
        basePath=QDir::homePath();
    } else {
        basePath="/home/pi";
    }

    listDirMusic(basePath+"/Laufwerke/Musik/");
    //VideoPlayerClass->listDirVideos(basePath+"/Laufwerke/Videos/");
    listDirVideos(basePath+"/Laufwerke/Videos/");
}

void MainWindow::serverSettings() {

    QString settingsServer;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(settingsDbFile);

    if(db.open()) /*QMessageBox::information(0,"Ok","Connection ok")*/;
    else QMessageBox::information(0,"Error","some error.");

    QSqlQuery query("SELECT `server_ip`,`server_port`,`server_local`,`server_protocol`,`server_bin` FROM settings WHERE `id` = '0'");
     if (query.lastError().isValid()){} /*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
     query.exec();


     while (query.next()) {
         settingsServer=query.value(3).toString()+query.value(0).toString()+":"+query.value(1).toString();
         serverIP = query.value(0).toString();
         serverPort = query.value(1).toString();
         serverLocal = query.value(2).toString();
         serverProtocol = query.value(3).toString();
         //url=QStringLiteral(serverProtocol+serverIP+":"+serverPort);
         url=serverProtocol+serverIP+":"+serverPort;
         serverBin = query.value(4).toString();
         }
    db.close();

    ui->BINServerLabel->setText(serverBin);

    m_webSocket = new QWebSocket(QString(settingsServer),QWebSocketProtocol::VersionLatest,this);
    m_webSocket->ignoreSslErrors();

    connect(m_webSocket, &QWebSocket::connected, this, &MainWindow::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &MainWindow::onDisconnected);

    //connect(ui->SR_RESULT->verticalScrollBar(), QScrollBar::hideEvent(QHideEvent *),this,&MainWindow::hideshowscroll);
    //typedef void (QWebSocket:: *sslErrorsSignal)(const QList<QSslError> &);
    /*connect(m_webSocket, static_cast<sslErrorsSignal>(&QWebSocket::sslErrors),
            this, &MainWindow::onSslErrors);*/

    if ( m_webSocket->state() == QAbstractSocket::UnconnectedState) {
        //!*qDebug() << "0.1";
        m_webSocket->open(QUrl(url));
        //m_webSocket->sendTextMessage("Hallo?");



    }
}

void MainWindow::fontSettings() {
    //!*qDebug() << "12";
    /************************************** < FONT DATABASE > *******************************************************/
        QFontDatabase LCARS_fonts;
        LCARS_fonts.addApplicationFont(":/fonts/Helvetica_Ultra_Compressed.ttf");
        LCARS_fonts.addApplicationFont(":/fonts/LCARS_EXTRA_GLYPHS.ttf");
        LCARS_fonts.addApplicationFont(":/fonts/webdings.ttf");
        LCARS_fonts.addApplicationFont(":/fonts/DroidSansMono.ttf");

    /************************************** < /FONT DATABASE > *******************************************************/

}

//remote_settings_ip

void MainWindow::searchOK(QString section, QString searchTerm) {

    if (section == "recipeList") {
        ui->PAGES->setCurrentWidget(ui->SCREEN_PROCESSING);
        QMediaPlayer mplayProcessing;
        mplayProcessing.setMedia(QUrl::fromUserInput("qrc:/sounds/processing.mp3"));
        mplayProcessing.play();
        AnimationClass->TextFlash(ui->PROCESSING_LABEL,1500,-1,0);
        m_webSocket->sendTextMessage("RECIPE_LIST::SELECT `id`,`title`,`thumbnail` FROM recipes WHERE `title` LIKE '%"+searchTerm+"%'");
        processData(searchTerm);
        extra_functions::delay(1500);
        ui->SEARCHTEXT->clear();
        ui->PAGES->setCurrentWidget(ui->SEARCH_RECIPE);
        AnimationClass->TextFlash(ui->PROCESSING_LABEL,1500,-1,1);
    }
    else if (section == "editNote") {
        ui->NOTES_EDIT_TEXTFIELD->clear();
        ui->NOTES_EDIT_TEXTFIELD->appendPlainText(searchTerm);
        ui->PAGES->setCurrentWidget(ui->NOTES);
        ui->SEARCHTEXT->clear();
    }
    else if (section=="editNoteTitle") {
        ui->NOTES_EDIT_TITLE_LINE->clear();
        ui->NOTES_EDIT_TITLE_LINE->setText(searchTerm);
        ui->PAGES->setCurrentWidget(ui->NOTES);
        ui->SEARCHTEXT->clear();
    }
    else if  (section == "serverIP") {

        serverIP=searchTerm;

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(settingsDbFile);

        if(db.open()) {
            QSqlQuery query("UPDATE settings SET `server_ip` = '"+serverIP+"' WHERE `id`=0;");
             if (query.lastError().isValid()){};
             query.exec();}
        else {};

        db.close();
        ui->IPServerLabel->setText(serverIP);
        ui->PAGES->setCurrentWidget(ui->SERVER_STATUS);

    } else if (section == "serverPort") {

        serverPort=searchTerm;

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(settingsDbFile);

        if(db.open()) {
            QSqlQuery query("UPDATE settings SET `server_port` = '"+serverPort+"' WHERE `id`=0;");
             if (query.lastError().isValid()){};
             query.exec();}
        else {};

        db.close();
        ui->PortServerLabel->setText(serverPort);
        ui->PAGES->setCurrentWidget(ui->SERVER_STATUS);

    } else if (section == "serverBin") {

        serverBin=searchTerm;

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(settingsDbFile);

        if(db.open()) {
            QSqlQuery query("UPDATE settings SET `server_bin` = '"+serverBin+"' WHERE `id`=0;");
             if (query.lastError().isValid()){};
             query.exec();}
        else {};

        db.close();
        ui->BINServerLabel->setText(serverBin);
        ui->PAGES->setCurrentWidget(ui->SERVER_STATUS);

    } else if (section == "serverLocal") {

        serverLocal=searchTerm;

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(settingsDbFile);

        if(db.open()) {
            QSqlQuery query("UPDATE settings SET `server_local` = '"+serverLocal+"' WHERE `id`=0;");
             if (query.lastError().isValid()){};
             query.exec();}
        else {};

        db.close();
        ui->PAGES->setCurrentWidget(ui->SERVER_STATUS);

    } else if (section == "serverProtocol") {

        serverProtocol=searchTerm;

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(settingsDbFile);

        if(db.open()) {
            QSqlQuery query("UPDATE settings SET `server_protocol` = '"+serverProtocol+"' WHERE `id`=0;");
             if (query.lastError().isValid()){};
             query.exec();}
        else {};

        db.close();
        ui->PAGES->setCurrentWidget(ui->SERVER_STATUS);

    }
    else if (section == "remote_settings_ip") {

        m_webSocket->sendTextMessage("SYSTEM::UPDATE settings SET `ip` = '"+searchTerm+"' WHERE `id` = '0'");
        ui->PAGES->setCurrentWidget(ui->SETTINGS);
        ui->SETTINGS_TABS->setCurrentWidget(ui->SET_SERVER);

    } else if (section == "remote_settings_port") {

        m_webSocket->sendTextMessage("SYSTEM::UPDATE settings SET `port` = '"+searchTerm+"' WHERE `id` = '0'");
        ui->PAGES->setCurrentWidget(ui->SETTINGS);
        ui->SETTINGS_TABS->setCurrentWidget(ui->SET_SERVER);
    } else if (section == "remote_settings_protocol") {

        m_webSocket->sendTextMessage("SYSTEM::UPDATE settings SET `protocol` = '"+searchTerm+"' WHERE `id` = '0'");
        ui->PAGES->setCurrentWidget(ui->SETTINGS);
        ui->SETTINGS_TABS->setCurrentWidget(ui->SET_SERVER);
    } else if (section == "remote_settings_path") {

        m_webSocket->sendTextMessage("SYSTEM::UPDATE settings SET `db_path` = '"+searchTerm+"' WHERE `id` = '0'");
        ui->PAGES->setCurrentWidget(ui->SETTINGS);
        ui->SETTINGS_TABS->setCurrentWidget(ui->SET_SERVER);
    }
    else if (section == "folders") {


        QVector<QListWidgetItem> folderItems = musicDB_control->getSearch(selected_folder,"folder",searchTerm);
        int i=0;
        ui->MUSIC_DATABASE_LIST_FOLDERS->clear();
        ui->MUSIC_DB_FOLDER_SEARCH_TEXT->clear();
        ui->MUSIC_DB_FOLDER_SEARCH_TEXT->setText(searchTerm);
        foreach (QListWidgetItem itemIterator, folderItems) {

            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setText(itemIterator.text());
            newItem->setData(Qt::UserRole-1,i);
            newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
            ui->MUSIC_DATABASE_LIST_FOLDERS->addItem(newItem);
            i++;
        }

        ui->SEARCHTEXT->clear();
        ui->PAGES->setCurrentWidget(ui->MEDIA);
        ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
        ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_DATABASE);
        ui->MUSIC_DATABASE_TABS->setCurrentWidget(ui->MUSIC_DB_FOLDERS);
    } else if (section == "artists") {


        QVector<QListWidgetItem> folderItems = musicDB_control->getSearch(selected_folder,"artist",searchTerm);
        int i=0;
        ui->MUSIC_DATABASE_LIST_ARTISTS->clear();
        ui->MUSIC_DB_ARTISTS_SEARCH_TEXT->clear();
        ui->MUSIC_DB_ARTISTS_SEARCH_TEXT->setText(searchTerm);
        foreach (QListWidgetItem itemIterator, folderItems) {

            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setText(itemIterator.text());
            newItem->setData(Qt::UserRole-1,i);
            newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
            ui->MUSIC_DATABASE_LIST_ARTISTS->addItem(newItem);
            i++;
        }
        ui->SEARCHTEXT->clear();
        ui->PAGES->setCurrentWidget(ui->MEDIA);
        ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
        ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_DATABASE);
        ui->MUSIC_DATABASE_TABS->setCurrentWidget(ui->MUSIC_DB_ARTISTS);
    } else if (section == "albums") {


        QVector<QListWidgetItem> folderItems = musicDB_control->getSearch(selected_folder,"album",searchTerm);
        int i=0;
        ui->MUSIC_DATABASE_LIST_ALBUMS->clear();
        ui->MUSIC_DB_ALBUMS_SEARCH_TEXT->clear();
        ui->MUSIC_DB_ALBUMS_SEARCH_TEXT->setText(searchTerm);
        foreach (QListWidgetItem itemIterator, folderItems) {

            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setText(itemIterator.text());
            newItem->setData(Qt::UserRole-1,i);
            newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
            ui->MUSIC_DATABASE_LIST_ALBUMS->addItem(newItem);
            i++;
        }
        ui->SEARCHTEXT->clear();
        ui->PAGES->setCurrentWidget(ui->MEDIA);
        ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
        ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_DATABASE);
        ui->MUSIC_DATABASE_TABS->setCurrentWidget(ui->MUSIC_DB_ALBUMS);
    } else if (section == "titles") {


        QVector<QListWidgetItem> folderItems = musicDB_control->getSearch(selected_folder,"title",searchTerm);
        int i=0;
        ui->MUSIC_DATABASE_LIST_TITLES->clear();
        ui->MUSIC_DB_TITLES_SEARCH_TEXT->clear();
        ui->MUSIC_DB_TITLES_SEARCH_TEXT->setText(searchTerm);
        foreach (QListWidgetItem itemIterator, folderItems) {

            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setText(itemIterator.text());
            newItem->setData(Qt::UserRole-1,i);
            newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
            ui->MUSIC_DATABASE_LIST_TITLES->addItem(newItem);
            i++;
        }
        ui->SEARCHTEXT->clear();
        ui->PAGES->setCurrentWidget(ui->MEDIA);
        ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
        ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_DATABASE);
        ui->MUSIC_DATABASE_TABS->setCurrentWidget(ui->MUSIC_DB_TITLES);
    } else if (section == "name") {


        QVector<QListWidgetItem> folderItems = musicDB_control->getSearch(selected_folder,"name",searchTerm);
        int i=0;
        ui->MUSIC_DATABASE_LIST_ARTISTS->clear();
        ui->MUSIC_DB_ADD_NAME->clear();
        ui->MUSIC_DB_ADD_NAME->setText(searchTerm);
        foreach (QListWidgetItem itemIterator, folderItems) {

            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setText(itemIterator.text());
            newItem->setData(Qt::UserRole-1,i);
            newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
            //ui->MUSIC_DATABASE_LIST_ARTISTS->addItem(newItem);
            ui->MUSIC_DB_EDIT_NAME->setText(itemIterator.text());
            i++;
        }
        ui->SEARCHTEXT->clear();
        ui->PAGES->setCurrentWidget(ui->MEDIA);
        ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
        ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_SETTINGS);
    } else if (section == "path") {


        QVector<QListWidgetItem> folderItems = musicDB_control->getSearch(selected_folder,"path",searchTerm);
        int i=0;
        ui->MUSIC_DATABASE_LIST_ARTISTS->clear();
        ui->MUSIC_DB_ADD_PATH->clear();
        ui->MUSIC_DB_ADD_PATH->setText(searchTerm);
        foreach (QListWidgetItem itemIterator, folderItems) {

            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setText(itemIterator.text());
            newItem->setData(Qt::UserRole-1,i);
            newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
            //ui->MUSIC_DATABASE_LIST_ARTISTS->addItem(newItem);
            ui->MUSIC_DB_EDIT_PATH->setText(itemIterator.text());
            i++;
        }
        ui->SEARCHTEXT->clear();
        ui->PAGES->setCurrentWidget(ui->MEDIA);
        ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
        ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_SETTINGS);

    } else if (section=="command") {


        //system("su pi -c \"xset dpms force off\"");
        //system("sudo shutdown -P now");

       ui->PAGES->setCurrentWidget(ui->SETTINGS);
       ui->SETTINGS_TABS->setCurrentWidget(ui->SET_COMMAND);
       ui->terminal_Text->clear();
       ui->terminal_Text->appendPlainText(searchTerm);
    } else if (section=="timer_name") {
       ui->PAGES->setCurrentWidget(ui->TIMER);
       ui->TIMER_NAME_EDIT->setText(searchTerm);
    }

}
void MainWindow::loadSettings(const QString& LANG_CODE) {

qDebug() << "loadSettings";





    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(settingsDbFile);

    if(db.open()) /*QMessageBox::information(0,"Ok","Connection ok")*/;
    else QMessageBox::information(0,"Error","some error.");

    QSqlQuery query("SELECT `root_command`,`timer_sound_repeat` FROM settings WHERE `id` = '0'");
     if (query.lastError().isValid()){};
     query.exec();


     while (query.next()) {

             rootCommand = query.value(0).toString();
             system(qPrintable(rootCommand+" xset s off"));
             system(qPrintable(rootCommand+" xset -dpms"));
             system(qPrintable(rootCommand+" xset s noblank"));

             timer_sound_repeat=query.value(0).toInt();
             if (timer_sound_repeat==0) {

             } else {

             }

         }
    db.close();



    //[@CODE = '"+LANG_CODE+"']"

    /*db.setDatabaseName(musicDB_file);

    if(db.open()) /*QMessageBox::information(0,"Ok","Connection ok")*/;
    /*else QMessageBox::information(0,"Error","some error.");

    QSqlQuery query("SELECT `vol_main`,`vol_menu`,`vol_media`,`vol_music`,`vol_video`,`brightness`,`scale`,`language`,`server_ip`,`server_port`,`server_local`,`timer_repeat`,`timer_list`,`system_notes`,`equalizer_levels`,`root_command` FROM settings WHERE `id` = '0'");
     if (query.lastError().isValid()){} /*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
     /*query.exec();

     /*QListWidgetItem newItem;
     newItem.setData(Qt::UserRole, query.value(1).toString());*/

     /*while (query.next()) {

         }
    db.close();*/


{
    //** TODO: Das muss in den loadsettings passieren, tuts aber nicht: 1
    QString languageCode = currentLanguage.toLower();

    QImage baseImage(":/images/flags/flag_"+languageCode+".png");
    QImage overlayImage(":/images/UFP_LAYER.png");
    //QImage SideImage = extra_functions::createImageWithOverlay(baseImage, overlayImage,228,180);
    QImage SideImage = extra_functions::createImageWithOverlay(baseImage, overlayImage,
                                                               1,
                                                               {},{},
                                                               130,130,
                                                               {},{},
                                                               {},16,
                                                               {},{},
                                                               true,false);
    ui->CURRENT_LANG_FLAG->setPixmap(QPixmap::fromImage(SideImage));
    //****


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(settingsDbFile);

    if(db.open()) /*QMessageBox::information(0,"Ok","Connection ok")*/;
    else QMessageBox::information(0,"Error","some error.");

    QSqlQuery query("SELECT `id`,`name`,`hh`,`mm`,`ss` FROM timer");
     if (query.lastError().isValid()){};
     query.exec();


     while (query.next()) {

         QListWidgetItem *item = new QListWidgetItem;
         item->setData(Qt::UserRole,query.value(0).toString());
         item->setData(Qt::UserRole+1,query.value(2).toString());
         item->setData(Qt::UserRole+2,query.value(3).toString());
         item->setData(Qt::UserRole+3,query.value(4).toString());
         item->setData(Qt::DisplayRole,query.value(1).toString());
         ui->TIMER_LIST->addItem(item);



         }

    db.close();


    QString scaleFactor = settingsClass->loadSetting("SELECT `scale_factor` FROM `settings` WHERE id='0';").at(0).at(0);
    ui->GUI_SCALE_SPINNER->setValue(scaleFactor.toDouble());

    //qDebug() << "languageItems" << settingsClass->loadSetting("SELECT `code`,`text` FROM `translation_codes`");

    currentLanguageName = settingsClass->loadSetting("SELECT `text` FROM `translation_codes` WHERE `code` = '"+currentLanguage+"'").at(0).at(0);



    generic_back = settingsClass->loadLanguage(currentLanguage,"generic_back").toUpper();
    main_title = settingsClass->loadLanguage(currentLanguage,"main_title").toUpper();
    main_timer = settingsClass->loadLanguage(currentLanguage,"main_timer").toUpper();
    main_media = settingsClass->loadLanguage(currentLanguage,"main_media").toUpper();
    main_recipe = settingsClass->loadLanguage(currentLanguage,"main_recipe").toUpper();
    main_search = settingsClass->loadLanguage(currentLanguage,"main_search").toUpper();
    main_system = settingsClass->loadLanguage(currentLanguage,"main_system").toUpper();
    timer_title = settingsClass->loadLanguage(currentLanguage,"timer_title").toUpper();
    timer_timername = settingsClass->loadLanguage(currentLanguage,"timer_timername").toUpper();
    timer_newtimer = settingsClass->loadLanguage(currentLanguage,"timer_newtimer").toUpper();
    timer_pause = settingsClass->loadLanguage(currentLanguage,"timer_pause").toUpper();
    timer_stop = settingsClass->loadLanguage(currentLanguage,"timer_stop").toUpper();
    timer_edit = settingsClass->loadLanguage(currentLanguage,"timer_edit").toUpper();
    timer_start = settingsClass->loadLanguage(currentLanguage,"timer_start").toUpper();
    timer_reset = settingsClass->loadLanguage(currentLanguage,"timer_reset").toUpper();
    timer_save = settingsClass->loadLanguage(currentLanguage,"timer_save").toUpper();
    timer_delete = settingsClass->loadLanguage(currentLanguage,"timer_delete").toUpper();
    timer_new = settingsClass->loadLanguage(currentLanguage,"timer_new").toUpper();
    timer_current = settingsClass->loadLanguage(currentLanguage,"timer_current").toUpper();
    timer_timeout = settingsClass->loadLanguage(currentLanguage,"timer_timeout").toUpper();
    timer_inactive = settingsClass->loadLanguage(currentLanguage,"timer_inactive").toUpper();
    system_title = settingsClass->loadLanguage(currentLanguage,"system_title").toUpper();
    system_settings = settingsClass->loadLanguage(currentLanguage,"system_settings").toUpper();
    system_notes = settingsClass->loadLanguage(currentLanguage,"system_notes").toUpper();
    system_lock = settingsClass->loadLanguage(currentLanguage,"system_lock").toUpper();
    recipe_next = settingsClass->loadLanguage(currentLanguage,"recipe_next").toUpper();
    recipe_previous = settingsClass->loadLanguage(currentLanguage,"recipe_previous").toUpper();
    recipe_read = settingsClass->loadLanguage(currentLanguage,"recipe_read").toUpper();
    recipe_mark = settingsClass->loadLanguage(currentLanguage,"recipe_mark").toUpper();
    search_title = settingsClass->loadLanguage(currentLanguage,"search_title").toUpper();
    search_search = settingsClass->loadLanguage(currentLanguage,"search_search").toUpper();
    discon_title = settingsClass->loadLanguage(currentLanguage,"discon_title").toUpper();
    discon_controltitle = settingsClass->loadLanguage(currentLanguage,"discon_controltitle").toUpper();
    discon_error = settingsClass->loadLanguage(currentLanguage,"discon_error").toUpper();
    discon_retry = settingsClass->loadLanguage(currentLanguage,"discon_retry").toUpper();
    lock_toptitle = settingsClass->loadLanguage(currentLanguage,"lock_toptitle").toUpper();
    lock_controltitle = settingsClass->loadLanguage(currentLanguage,"lock_controltitle").toUpper();
    lock_locked = settingsClass->loadLanguage(currentLanguage,"lock_locked").toUpper();
    lock_granted = settingsClass->loadLanguage(currentLanguage,"lock_granted").toUpper();
    lock_bottomtitle = settingsClass->loadLanguage(currentLanguage,"lock_bottomtitle").toUpper();
    lock_alpha = settingsClass->loadLanguage(currentLanguage,"lock_alpha").toUpper();
    lock_beta = settingsClass->loadLanguage(currentLanguage,"lock_beta").toUpper();
    lock_gamma = settingsClass->loadLanguage(currentLanguage,"lock_gamma").toUpper();
    lock_delta = settingsClass->loadLanguage(currentLanguage,"lock_delta").toUpper();
    red_disable = settingsClass->loadLanguage(currentLanguage,"red_disable").toUpper();
    topcontrols_mplayer = settingsClass->loadLanguage(currentLanguage,"topcontrols_mplayer").toUpper();
    topcontrols_vplayer = settingsClass->loadLanguage(currentLanguage,"topcontrols_vplayer").toUpper();
    set_lang_title = settingsClass->loadLanguage(currentLanguage,"set_lang_title").toUpper();
    set_lang_title_description = settingsClass->loadLanguage(currentLanguage,"set_lang_title_description").toUpper();
    set_lang_description = settingsClass->loadLanguage(currentLanguage,"set_lang_description").toUpper();
    media_title = settingsClass->loadLanguage(currentLanguage,"media_title").toUpper();
    media_music_db = settingsClass->loadLanguage(currentLanguage,"media_music_db").toUpper();
    media_music_files = settingsClass->loadLanguage(currentLanguage,"media_music_files").toUpper();
    media_music_db_settings = settingsClass->loadLanguage(currentLanguage,"media_music_db_settings").toUpper();
    media_music_general_settings = settingsClass->loadLanguage(currentLanguage,"media_music_general_settings").toUpper();
    media_music_player = settingsClass->loadLanguage(currentLanguage,"media_music_player").toUpper();
    media_video_db = settingsClass->loadLanguage(currentLanguage,"media_video_db").toUpper();
    media_video_files = settingsClass->loadLanguage(currentLanguage,"media_video_files").toUpper();
    media_video_db_settings = settingsClass->loadLanguage(currentLanguage,"media_video_db_settings").toUpper();
    media_video_general_settings = settingsClass->loadLanguage(currentLanguage,"media_video_general_settings").toUpper();
    media_video_player = settingsClass->loadLanguage(currentLanguage,"media_video_player").toUpper();
    notes_title = settingsClass->loadLanguage(currentLanguage,"notes_title").toUpper();
    notes_marked = settingsClass->loadLanguage(currentLanguage,"notes_marked").toUpper();
    notes_notes = settingsClass->loadLanguage(currentLanguage,"notes_notes").toUpper();
    notes_show_recipe = settingsClass->loadLanguage(currentLanguage,"notes_show_recipe").toUpper();
    notes_remove_recipe = settingsClass->loadLanguage(currentLanguage,"notes_remove_recipe").toUpper();
    notes_new_note = settingsClass->loadLanguage(currentLanguage,"notes_new_note").toUpper();
    notes_edit_note = settingsClass->loadLanguage(currentLanguage,"notes_edit_note").toUpper();
    notes_remove_note = settingsClass->loadLanguage(currentLanguage,"notes_remove_note").toUpper();
    generic_edit = settingsClass->loadLanguage(currentLanguage,"generic_edit").toUpper();
    server_status_client_ip = settingsClass->loadLanguage(currentLanguage,"server_status_client_ip").toUpper();
    server_status_client_port = settingsClass->loadLanguage(currentLanguage,"server_status_client_port").toUpper();
    server_status_client_bin = settingsClass->loadLanguage(currentLanguage,"server_status_client_bin").toUpper();
    server_status_status = settingsClass->loadLanguage(currentLanguage,"server_status_status").toUpper();
    server_status_ip = settingsClass->loadLanguage(currentLanguage,"server_status_ip").toUpper();
    server_status_port = settingsClass->loadLanguage(currentLanguage,"server_status_port").toUpper();
    server_status_pid = settingsClass->loadLanguage(currentLanguage,"server_status_pid").toUpper();
    server_status_client = settingsClass->loadLanguage(currentLanguage,"server_status_client").toUpper();
    server_status_server = settingsClass->loadLanguage(currentLanguage,"server_status_server").toUpper();
    server_status_title = settingsClass->loadLanguage(currentLanguage,"server_status_title").toUpper();
    server_status_running = settingsClass->loadLanguage(currentLanguage,"server_status_running").toUpper();
    server_status_stopped = settingsClass->loadLanguage(currentLanguage,"server_status_stopped").toUpper();
    server_status_settings = settingsClass->loadLanguage(currentLanguage,"server_status_settings").toUpper();
    server_settings_ip = settingsClass->loadLanguage(currentLanguage,"server_settings_ip").toUpper();
    server_settings_port = settingsClass->loadLanguage(currentLanguage,"server_settings_port").toUpper();
    server_settings_protocol = settingsClass->loadLanguage(currentLanguage,"server_settings_protocol").toUpper();
    server_settings_dbpath = settingsClass->loadLanguage(currentLanguage,"server_settings_dbpath").toUpper();
    server_settings_pid = settingsClass->loadLanguage(currentLanguage,"server_settings_pid").toUpper();
    server_settings_status = settingsClass->loadLanguage(currentLanguage,"server_settings_status").toUpper();
    server_settings_running = settingsClass->loadLanguage(currentLanguage,"server_settings_running").toUpper();
    server_settings_stopped = settingsClass->loadLanguage(currentLanguage,"server_settings_stopped").toUpper();
    server_settings_start = settingsClass->loadLanguage(currentLanguage,"server_settings_start").toUpper();
    server_settings_restart = settingsClass->loadLanguage(currentLanguage,"server_settings_restart").toUpper();
    server_settings_shutdown = settingsClass->loadLanguage(currentLanguage,"server_settings_shutdown").toUpper();
    server_settings_kill = settingsClass->loadLanguage(currentLanguage,"server_settings_kill").toUpper();
    server_settings_title = settingsClass->loadLanguage(currentLanguage,"server_settings_title").toUpper();
    settings_title = settingsClass->loadLanguage(currentLanguage,"settings_title").toUpper();
    settings_display = settingsClass->loadLanguage(currentLanguage,"settings_display").toUpper();
    settings_advanced = settingsClass->loadLanguage(currentLanguage,"settings_advanced").toUpper();
    settings_network = settingsClass->loadLanguage(currentLanguage,"settings_network").toUpper();
    settings_language = settingsClass->loadLanguage(currentLanguage,"settings_language").toUpper();
    settings_audio = settingsClass->loadLanguage(currentLanguage,"settings_audio").toUpper();
    settings_run = settingsClass->loadLanguage(currentLanguage,"settings_run").toUpper();
    settings_warpcore = settingsClass->loadLanguage(currentLanguage,"settings_warpcore").toUpper();
    display_title = settingsClass->loadLanguage(currentLanguage,"display_title").toUpper();
    display_off = settingsClass->loadLanguage(currentLanguage,"display_off").toUpper();
    display_timeout = settingsClass->loadLanguage(currentLanguage,"display_timeout").toUpper();
    display_brightness = settingsClass->loadLanguage(currentLanguage,"display_brightness").toUpper();
    advanced_title = settingsClass->loadLanguage(currentLanguage,"advanced_title").toUpper();
    advanced_ambient = settingsClass->loadLanguage(currentLanguage,"advanced_ambient").toUpper();
    advanced_alarmsound = settingsClass->loadLanguage(currentLanguage,"advanced_alarmsound").toUpper();
    advanced_alarmrepeat = settingsClass->loadLanguage(currentLanguage,"advanced_alarmrepeat").toUpper();
    advanced_alarmsingle = settingsClass->loadLanguage(currentLanguage,"advanced_alarmsingle").toUpper();
    ambient_title = settingsClass->loadLanguage(currentLanguage,"ambient_title").toUpper();
    ambient_name = settingsClass->loadLanguage(currentLanguage,"ambient_name").toUpper();
    ambient_path = settingsClass->loadLanguage(currentLanguage,"ambient_path").toUpper();
    ambient_add = settingsClass->loadLanguage(currentLanguage,"ambient_add").toUpper();
    generic_add = settingsClass->loadLanguage(currentLanguage,"generic_add").toUpper();
    generic_remove = settingsClass->loadLanguage(currentLanguage,"generic_remove").toUpper();
    network_title = settingsClass->loadLanguage(currentLanguage,"network_title").toUpper();
    network_hostname = settingsClass->loadLanguage(currentLanguage,"network_hostname").toUpper();
    network_interface = settingsClass->loadLanguage(currentLanguage,"network_interface").toUpper();
    network_ip = settingsClass->loadLanguage(currentLanguage,"network_ip").toUpper();
    network_mac = settingsClass->loadLanguage(currentLanguage,"network_mac").toUpper();
    network_wlan_settings = settingsClass->loadLanguage(currentLanguage,"network_wlan_settings").toUpper();
    network_eth_settings = settingsClass->loadLanguage(currentLanguage,"network_eth_settings").toUpper();
    audio_title = settingsClass->loadLanguage(currentLanguage,"audio_title").toUpper();
    autio_main = settingsClass->loadLanguage(currentLanguage,"autio_main").toUpper();
    audio_menu = settingsClass->loadLanguage(currentLanguage,"audio_menu").toUpper();
    audio_media = settingsClass->loadLanguage(currentLanguage,"audio_media").toUpper();
    audio_alarm = settingsClass->loadLanguage(currentLanguage,"audio_alarm").toUpper();
    audio_equalizer = settingsClass->loadLanguage(currentLanguage,"audio_equalizer").toUpper();
    eq_title = settingsClass->loadLanguage(currentLanguage,"eq_title").toUpper();
    eq_disabled = settingsClass->loadLanguage(currentLanguage,"eq_disabled").toUpper();
    eq_enabled = settingsClass->loadLanguage(currentLanguage,"eq_enabled").toUpper();
    run_title = settingsClass->loadLanguage(currentLanguage,"run_title").toUpper();
    run_asroot = settingsClass->loadLanguage(currentLanguage,"run_asroot").toUpper();
    run_run = settingsClass->loadLanguage(currentLanguage,"run_run").toUpper();
    run_edit = settingsClass->loadLanguage(currentLanguage,"run_edit").toUpper();
    run_example = settingsClass->loadLanguage(currentLanguage,"run_example").toUpper();
    generic_searchfor = settingsClass->loadLanguage(currentLanguage,"generic_searchfor").toUpper();
    keyboard_ok = settingsClass->loadLanguage(currentLanguage,"keyboard_ok").toUpper();
    keyboard_cancel = settingsClass->loadLanguage(currentLanguage,"keyboard_cancel").toUpper();
    generic_processing = settingsClass->loadLanguage(currentLanguage,"generic_processing").toUpper();
    music_player_title = settingsClass->loadLanguage(currentLanguage,"music_player_title").toUpper();
    music_player_details_title = settingsClass->loadLanguage(currentLanguage,"music_player_details_title").toUpper();
    music_player_details_artist = settingsClass->loadLanguage(currentLanguage,"music_player_details_artist").toUpper();
    music_player_details_album = settingsClass->loadLanguage(currentLanguage,"music_player_details_album").toUpper();
    music_player_details_left = settingsClass->loadLanguage(currentLanguage,"music_player_details_left").toUpper();
    music_player_details_right = settingsClass->loadLanguage(currentLanguage,"music_player_details_right").toUpper();
    music_player_mode = settingsClass->loadLanguage(currentLanguage,"music_player_mode").toUpper();
    music_player_playlist = settingsClass->loadLanguage(currentLanguage,"music_player_playlist").toUpper();
    music_player_details = settingsClass->loadLanguage(currentLanguage,"music_player_details").toUpper();
    music_player_menu = settingsClass->loadLanguage(currentLanguage,"music_player_menu").toUpper();
    music_player_database = settingsClass->loadLanguage(currentLanguage,"music_player_database").toUpper();
    music_player_files = settingsClass->loadLanguage(currentLanguage,"music_player_files").toUpper();
    music_player_playlist_play = settingsClass->loadLanguage(currentLanguage,"music_player_playlist_play").toUpper();
    music_player_mode_all = settingsClass->loadLanguage(currentLanguage,"music_player_mode_all").toUpper();
    music_player_mode_one = settingsClass->loadLanguage(currentLanguage,"music_player_mode_one").toUpper();
    music_player_mode_onerepeat = settingsClass->loadLanguage(currentLanguage,"music_player_mode_onerepeat").toUpper();
    music_player_mode_norepeat = settingsClass->loadLanguage(currentLanguage,"music_player_mode_norepeat").toUpper();
    music_player_mode_random = settingsClass->loadLanguage(currentLanguage,"music_player_mode_random").toUpper();
    video_player_title = settingsClass->loadLanguage(currentLanguage,"video_player_title").toUpper();
    video_player_controls = settingsClass->loadLanguage(currentLanguage,"video_player_controls").toUpper();
    musicdb_settings_title = settingsClass->loadLanguage(currentLanguage,"musicdb_settings_title").toUpper();
    musicdb_settings_name = settingsClass->loadLanguage(currentLanguage,"musicdb_settings_name").toUpper();
    musicdb_settings_path = settingsClass->loadLanguage(currentLanguage,"musicdb_settings_path").toUpper();
    musicdb_settings_add = settingsClass->loadLanguage(currentLanguage,"musicdb_settings_add").toUpper();
    musicdb_settings_update = settingsClass->loadLanguage(currentLanguage,"musicdb_settings_update").toUpper();
    musicdb_settings_status = settingsClass->loadLanguage(currentLanguage,"musicdb_settings_status").toUpper();
    musicdb_settings_idle = settingsClass->loadLanguage(currentLanguage,"musicdb_settings_idle").toUpper();
    musicdb_setttings_counting = settingsClass->loadLanguage(currentLanguage,"musicdb_setttings_counting").toUpper();
    musicdb_settings_adding = settingsClass->loadLanguage(currentLanguage,"musicdb_settings_adding").toUpper();
    musicdb_folder = settingsClass->loadLanguage(currentLanguage,"musicdb_folder").toUpper();
    musicdb_letter = settingsClass->loadLanguage(currentLanguage,"musicdb_letter").toUpper();
    musicdb_artist = settingsClass->loadLanguage(currentLanguage,"musicdb_artist").toUpper();
    musicdb_album = settingsClass->loadLanguage(currentLanguage,"musicdb_album").toUpper();
    musicdb_songtitle = settingsClass->loadLanguage(currentLanguage,"musicdb_songtitle").toUpper();
    musicdb_playlist = settingsClass->loadLanguage(currentLanguage,"musicdb_playlist").toUpper();
    musicdb_add = settingsClass->loadLanguage(currentLanguage,"musicdb_add").toUpper();
    musicdb_play = settingsClass->loadLanguage(currentLanguage,"musicdb_play").toUpper();
    musicfiles_title = settingsClass->loadLanguage(currentLanguage,"musicfiles_title").toUpper();
    musicfiles_add = settingsClass->loadLanguage(currentLanguage,"musicfiles_add").toUpper();
    musicfiles_play = settingsClass->loadLanguage(currentLanguage,"musicfiles_play").toUpper();
    musicfiles_player = settingsClass->loadLanguage(currentLanguage,"musicfiles_player").toUpper();
    videofiles_title = settingsClass->loadLanguage(currentLanguage,"videofiles_title").toUpper();
    videofiles_play = settingsClass->loadLanguage(currentLanguage,"videofiles_play").toUpper();
    videofiles_add = settingsClass->loadLanguage(currentLanguage,"videofiles_add").toUpper();
    videofiles_player = settingsClass->loadLanguage(currentLanguage,"videofiles_player").toUpper();
    system_restart = settingsClass->loadLanguage(currentLanguage,"system_restart").toUpper();
    system_shutdown = settingsClass->loadLanguage(currentLanguage,"system_shutdown").toUpper();
    system_server_status = settingsClass->loadLanguage(currentLanguage,"system_server_status").toUpper();
    system_disable_gui = settingsClass->loadLanguage(currentLanguage,"system_deactivate_gui").toUpper();
    red_alert_text = settingsClass->loadLanguage(currentLanguage,"red_disable").toUpper();
    audio_main = settingsClass->loadLanguage(currentLanguage,"audio_main").toUpper();

        ui->TITLE->setText(main_title);
        ui->MM_BUTTON_MEDIA->setText(main_media);
        ui->MM_BUTTON_RECIPE->setText(main_recipe);
        ui->MM_BUTTON_SEARCH->setText(main_search);
        ui->MM_BUTTON_SYSTEM->setText(main_system);
        ui->MM_BUTTON_TIMER->setText(main_timer);
        ui->TIMER_CURRENT_TIME->setText(timer_title);
        ui->TIMER_DELETE->setText(timer_delete);
        ui->TIMER_NEW->setText(timer_new);
        ui->TIMER_EDIT->setText(timer_edit);
        ui->TIMER_PAUSE->setText(timer_pause);
        ui->TIMER_STOP->setText(timer_stop);
        ui->TIMER_START->setText(timer_start);
        ui->TIMER_SAVE->setText(timer_save);
        ui->TIMER_TIMEOUT->setText(timer_timeout);
        ui->TIMER_RESET->setText(timer_reset);
        ui->calendarWidget->setLocale(currentLanguage);
        ui->TIMER_NAME_EDIT->setText(timer_newtimer);
        ui->SR_DIS_TITLE_LABEL->setText(discon_controltitle);
        ui->SR_DIS_ERROR_LABEL->setText(discon_error);
        ui->SR_DIS_RETRY->setText(discon_retry);
        ui->SET_AVAIL_LANG_DESC_TITLE->setText(set_lang_title_description);
        ui->SET_AVAIL_LANG_DESC->setText(set_lang_description);
        ui->MEDIA_BUTTON_MUSIC_DATABASE->setText(media_music_db);
        ui->MEDIA_BUTTON_MUSIC_FILES->setText(media_music_files);
        ui->MEDIA_BUTTON_MUSIC_GENERAL_SETTINGS->setText(media_music_general_settings);
        ui->MEDIA_BUTTON_MUSIC_SETTINGS->setText(media_music_db_settings);
        ui->MEDIA_BUTTON_MUSIC_PLAYER->setText(media_music_player);
        ui->MEDIA_BUTTON_VIDEOS_DATABASE->setText(media_music_db);
        ui->MEDIA_BUTTON_VIDEO_FILES->setText(media_music_files);
        ui->MEDIA_BUTTON_VIDEOS_GENERAL_SETTINGS->setText(media_music_general_settings);
        ui->MEDIA_BUTTON_VIDEOS_SETTINGS->setText(media_music_db_settings);
        ui->MEDIA_BUTTON_VIDEO_PLAYER->setText(media_video_player);
        ui->NEW_NOTE->setText(notes_new_note);
        ui->EDIT_NOTE->setText(notes_edit_note);
        ui->REMOVE_NOTE->setText(generic_remove);
        ui->REMOVE_MARKED_RECIPE->setText(generic_remove);
        ui->SHOW_RECIPE->setText(notes_show_recipe);
        ui->NOTES_BACK->setText(generic_back);
        ui->PROCESSING_LABEL->setText(generic_processing);
        ui->pushButton_16->setText(recipe_previous);
        ui->pushButton_19->setText(recipe_next);
        ui->RECIPE_MARK->setText(recipe_mark);
        ui->RECIPE_READ->setText(recipe_read);
        ui->SR_SEARCH->setText(search_search);
        ui->SR_DIS_ERROR_LABEL->setText(discon_error);
        ui->SR_DIS_RETRY->setText(discon_retry);
        ui->SR_DIS_TITLE_LABEL->setText(discon_controltitle);
        ui->pushButton_45->setText(system_restart);
        ui->pushButton_44->setText(system_shutdown);
        ui->SYSTEM_BUTTON_NOTES->setText(system_notes);
        ui->SYSTEM_SERVER_STATUS->setText(system_server_status);
        ui->pushButton->setText(system_lock);
        ui->pushButton_5->setText(system_disable_gui);
        ui->SYSTEM_SETTINGS_BUTTON->setText(system_settings);
        ui->SERVER_IP_CLIENT->setText(generic_edit);
        ui->SERVER_PORT_CLIENT->setText(generic_edit);
        ui->SERVER_PATH_CLIENT->setText(generic_edit);
        ui->pushButton_47->setText(server_status_client);
        ui->pushButton_48->setText(server_status_server);
        ui->SETTINGS_SERVER->setText(server_status_settings);
        ui->SETTINGS_SERVER_BACK->setText(generic_back);
        ui->AW_RED_ALERT_TEXT->setText(red_alert_text);
        ui->SET_AVAIL_LANG_BACK->setText(generic_back);
        ui->pushButton_4->setText(eq_disabled);
        ui->SYSTEM_BUTTON_EQUALIZER_BACK_2->setText(generic_back);
        ui->label_25->setText(audio_main);
        ui->label_30->setText(audio_menu);
        ui->label_36->setText(audio_media);
        ui->label_47->setText(audio_alarm);
        ui->pushButton_46->setText(generic_back);
        ui->pushButton_69->setText(audio_equalizer);
        ui->SETTINGS_BUTTON_ADVANCED_3->setText(settings_display);
        ui->SETTINGS_BUTTON_EXTENDED->setText(settings_advanced);
        ui->SETTINGS_BUTTON_LANGUAGE->setText(settings_language);
        ui->SETTINGS_BUTTON_NETWORK->setText(settings_network);
        ui->SETTINGS_BUTTON_AUDIO->setText(settings_audio);
        ui->SETTINGS_BUTTON_ADVANCED_2->setText(settings_run);
        ui->SETTINGS_BUTTON_BACK->setText(generic_back);
        ui->label_6->setText(settings_warpcore);
        ui->label_9->setText(display_brightness);
        ui->SETTINGS_BUTTON_DISPLAY_OFF->setText(display_off);
        ui->SETTINGS_BUTTON_DISPLAY_OFF_2->setText(display_timeout);
        ui->SETTINGS_BUTTON_BACK_2->setText(generic_back);
        ui->terminal_RunRoot->setText(run_asroot);
        ui->terminal_Edit->setText(generic_edit);
        ui->terminal_Run->setText(run_run);
        ui->MM_BUTTON_TIMER_2->setText(generic_back);
        ui->SERVER_IP_EDIT->setText(generic_edit);
        ui->SERVER_PORT_EDIT->setText(generic_edit);
        ui->SERVER_PROTOCOL_EDIT->setText(generic_edit);
        ui->SERVER_DB_PATH_EDIT->setText(generic_edit);
        ui->label_22->setText(server_settings_ip);
        ui->label_17->setText(server_settings_port);
        ui->label_37->setText(server_settings_protocol);
        ui->label_46->setText(server_settings_dbpath);
        ui->label_48->setText(server_settings_pid);
        ui->label_49->setText(server_settings_status);
        ui->SET_START_SERVER->setText(server_settings_start);
        ui->SET_RESTART_SERVER->setText(server_settings_restart);
        ui->SET_SHUTDOWN_SERVER->setText(server_settings_shutdown);
        ui->SET_KILL_SERVER->setText(server_settings_kill);
        ui->SET_SERVER_BACK->setText(generic_back);
        ui->pushButton_39->setText(generic_back);
        ui->MUSIC_DB_REMOVE_FOLDER_2->setText(generic_remove);
        ui->pushButton_40->setText(generic_edit);
        ui->label_83->setText(ambient_name);
        ui->label_84->setText(ambient_path);
        ui->MUSIC_DB_EDIT_NAME_2->setText(generic_edit);
        ui->MUSIC_DB_EDIT_PATH_2->setText(generic_edit);
        ui->MUSIC_DB_ADD_2->setText(musicdb_add);
        ui->SET_START_SERVER_2->setText(advanced_ambient);
        ui->TOP_BAR_HH_2->setText(advanced_alarmsound);
        ui->LCARS_PushButton->setText(advanced_alarmrepeat);
        ui->LCARS_PushButton_6->setText(advanced_alarmsingle);
        ui->SETTINGS_SERVER_BACK->setText(generic_back);
        ui->hostnameLabel->setText(network_hostname);
        ui->SET_SERVER_BACK_3->setText(generic_back);
        ui->LCARS_PushButton_7->setText(network_wlan_settings);
        ui->LCARS_PushButton_8->setText(network_eth_settings);
        ui->MUSIC_FILES_BACK_2->setText(generic_back);
        ui->MUSIC_FILES_OKAYER_2->setText(videofiles_player);
        ui->MUSIC_FILES_ADD_2->setText(videofiles_add);
        ui->VIDEO_FILES_PLAY->setText(videofiles_play);
        ui->VIDEO_BUTTON_CONTROLS->setText(video_player_controls);
        ui->VIDEO_BUTTON_BACK->setText(generic_back);
        ui->MUSIC_FILES_BACK->setText(generic_back);
        ui->MUSIC_FILES_OKAYER->setText(musicfiles_player);
        ui->MUSIC_FILES_ADD->setText(musicfiles_add);
        ui->MUSIC_FILES_PLAY->setText(musicfiles_play);
        ui->pushButton_35->setText(generic_searchfor);
        ui->pushButton_36->setText(generic_back);
        ui->MUSIC_DB_REMOVE_FOLDER->setText(generic_remove);
        ui->pushButton_38->setText(musicdb_settings_update);
        ui->MUSIC_DB_EDIT_NAME->setText(generic_edit);
        ui->MUSIC_DB_EDIT_PATH->setText(generic_edit);
        ui->label_79->setText(musicdb_settings_name);
        ui->label_80->setText(musicdb_settings_path);
        ui->MUSIC_DB_STATUS_BAR->setText(musicdb_settings_status);
        ui->MUSIC_DB_ADD->setText(generic_add);
        ui->MUSIC_DB_SEARCH_FOLDERS->setText(generic_searchfor);
        ui->pushButton_30->setText(generic_back);
        ui->pushButton_33->setText(musicdb_playlist);
        ui->MUSIC_DB_SEARCH_ARTISTS->setText(generic_searchfor);
        ui->MUSIC_DB_BACK_ARTISTS->setText(generic_back);
        ui->pushButton_34->setText(musicdb_playlist);
        ui->pushButton_27->setText(generic_add);
        ui->pushButton_28->setText(musicdb_play);
        ui->MUSIC_DB_SEARCH_ALBUMS->setText(generic_searchfor);
        ui->MUSIC_DB_BACK_ALBUMS->setText(generic_back);
        ui->pushButton_32->setText(musicdb_playlist);
        ui->pushButton_22->setText(generic_add);
        ui->pushButton_23->setText(musicdb_play);
        ui->MUSIC_DB_SEARCH_TITLES->setText(generic_searchfor);
        ui->MUSIC_DB_BACK_TITLES->setText(generic_back);
        ui->pushButton_31->setText(musicdb_playlist);
        ui->MUSIC_DB_ADD_TITLES->setText(generic_add);
        ui->MUSIC_DB_PLAY_TITLE->setText(musicdb_play);
        ui->label_35->setText(music_player_details_title);
        ui->label_41->setText(music_player_details_artist);
        ui->label_43->setText(music_player_details_album);
        ui->SYSTEM_TIME_LABEL_12->setText(music_player_details_left);
        ui->SYSTEM_TIME_LABEL_13->setText(music_player_details_right);
        ui->MUSIC_PLAYER_LOOP->setText(music_player_mode);
        ui->MUSIC_PLAYER_PLAYLIST->setText(music_player_playlist);
        ui->MUSIC_PLAYER_DETAILS->setText(music_player_details);
        ui->MM_BUTTON_TIMER_7->setText(music_player_menu);
        ui->MUSIC_PLAYER_DATABASE->setText(music_player_database);
        ui->MUSIC_PLAYER_FILES->setText(music_player_files);
        ui->MUSIC_FILES_OKAYER_3->setText(generic_remove);
        ui->MUSIC_PLAYLIST_PLAY->setText(music_player_playlist_play);
        ui->MC_BUTTON_M_PLAYER->setText(topcontrols_mplayer);
        ui->MC_BUTTON_V_PLAYER->setText(topcontrols_vplayer);



}
}

void MainWindow::changeTitle(QString title) {
    ui->TITLE->setText(title);
}

void MainWindow::listDirVideos(QString dirString) {
  ui->VIDEO_FILES_LIST->clear();
  ui->VIDEO_FILES_LIST->scrollToTop();
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
      ui->VIDEO_FILES_LIST->addItem(item);


      i++;
      }

  }
}

void MainWindow::connections() {

    void (QDoubleSpinBox::*mySignal)(double) = &QDoubleSpinBox::valueChanged;
    connect(ui->GUI_SCALE_SPINNER, mySignal, this, [this](double value){
        qDebug() << qgetenv("Q_SCALE_FACTOR");
        QString scaleFactor = QString::number(value);
        qputenv("Q_SCALE_FACTOR",qPrintable(scaleFactor));
    });


    connect(ui->GUI_SCALE_SPINNER_LEFT,&QPushButton::clicked,[=](){
       ui->GUI_SCALE_SPINNER->setValue(ui->GUI_SCALE_SPINNER->value()-0.01);
    });

    connect(ui->GUI_SCALE_SPINNER_RIGHT,&QPushButton::clicked,[=](){
       ui->GUI_SCALE_SPINNER->setValue(ui->GUI_SCALE_SPINNER->value()+0.01);
    });

    connect(ui->GUI_SCALE_SPINNER_LEFT,&QPushButton::released,[=](){
       settingsClass->writeSetting("UPDATE settings SET `scale_factor` = '"+QString::number(ui->GUI_SCALE_SPINNER->value())+"' WHERE `id` = '0';");
    });

    connect(ui->GUI_SCALE_SPINNER_RIGHT,&QPushButton::released,[=](){
       settingsClass->writeSetting("UPDATE settings SET `scale_factor` = '"+QString::number(ui->GUI_SCALE_SPINNER->value())+"' WHERE `id` = '0';");
    });

    connect(ui->VIDEO_FILES_LIST,&QListWidget::itemClicked,[=](){

        QListWidgetItem *currentItem;
        currentItem = ui->VIDEO_FILES_LIST->currentItem();

        QFileInfo Fileitem;
        Fileitem.setFile(currentItem->data(Qt::UserRole+3).toString());


        if (Fileitem.isDir()) {
            listDirVideos(Fileitem.filePath());
        } else if (Fileitem.isFile()) {
            //qDebug() << "file:" << Fileitem.filePath();
            //MusicPlaylistEntries.append(Fileitem.filePath());
            QString fileName = Fileitem.filePath();
            VideoFile=fileName;
            VideoPlayerClass->initPlayer(ui->VIDEO_FILES_PLAY,
                                         ui->VIDEO_PLAY_PAUSE,
                                         ui->VIDEO_STOP,
                                         ui->VIDEO_MAXIMIZE,
                                         ui->VIDEO_POSITION,
                                         ui->VIDEO_VOLUME,
                                         videoWidget,
                                         VideoFile);

        }

    });


    QScrollBar *SET_AVAIL_LANGUAGE_LEFT_SCROLLBAR = ui->SET_AVAIL_LANG_LEFT_SCROLL;

    QScrollBar *SET_AVAIL_LANGUAGE_RIGHT_SCROLLBAR = ui->SET_AVAIL_LANG_RIGHT_SCROLL;

    QScrollBar *SET_AVAIL_LANGUAGE_LIST_SCROLLBAR = ui->SET_AVAIL_LANG_LIST->verticalScrollBar();


    SET_AVAIL_LANGUAGE_LEFT_SCROLLBAR->setMaximum(SET_AVAIL_LANGUAGE_LIST_SCROLLBAR->maximum());
    SET_AVAIL_LANGUAGE_RIGHT_SCROLLBAR->setMaximum(SET_AVAIL_LANGUAGE_LIST_SCROLLBAR->maximum());

    connect(SET_AVAIL_LANGUAGE_LIST_SCROLLBAR, &QScrollBar::rangeChanged,
            [=]
                {
                    SET_AVAIL_LANGUAGE_LEFT_SCROLLBAR->setMaximum(SET_AVAIL_LANGUAGE_LIST_SCROLLBAR->maximum());
                    SET_AVAIL_LANGUAGE_RIGHT_SCROLLBAR->setMaximum(SET_AVAIL_LANGUAGE_LIST_SCROLLBAR->maximum());
                }
            );
    connect(SET_AVAIL_LANGUAGE_LEFT_SCROLLBAR, &QAbstractSlider::valueChanged,
            [=](int aSliderPosition)
                {
                    SET_AVAIL_LANGUAGE_LIST_SCROLLBAR->setValue(aSliderPosition);
                    SET_AVAIL_LANGUAGE_RIGHT_SCROLLBAR->setValue(aSliderPosition);
                });

    connect(SET_AVAIL_LANGUAGE_RIGHT_SCROLLBAR, &QAbstractSlider::valueChanged,
            [=](int aSliderPosition)
                {
                    SET_AVAIL_LANGUAGE_LIST_SCROLLBAR->setValue(aSliderPosition);
                    SET_AVAIL_LANGUAGE_LEFT_SCROLLBAR->setValue(aSliderPosition);
                });


    connect(ui->AMBIENT_SOUND_LIST->verticalScrollBar(),&QScrollBar::rangeChanged,[=] {
        ui->AMBIENT_SOUND_SLIDER->setRange(ui->AMBIENT_SOUND_LIST->verticalScrollBar()->minimum(),ui->AMBIENT_SOUND_LIST->verticalScrollBar()->maximum());
    });
    connect(ui->AMBIENT_SOUND_SLIDER, &QAbstractSlider::valueChanged,
            [=](int aSliderPosition)
                {
                    ui->AMBIENT_SOUND_LIST->verticalScrollBar()->setValue(aSliderPosition);
                });




    connect(SET_AVAIL_LANGUAGE_LIST_SCROLLBAR, &QAbstractSlider::valueChanged,
            [=](int aSliderPosition)
                {
                    SET_AVAIL_LANGUAGE_RIGHT_SCROLLBAR->setValue(aSliderPosition);
                    SET_AVAIL_LANGUAGE_LEFT_SCROLLBAR->setValue(aSliderPosition);
                });



            connect(ui->AMBIENT_SOUND_VOLUME, &QAbstractSlider::valueChanged,
                    [=](int aSliderPosition)
                        {
                            ambientSounds.at(ui->AMBIENT_SOUND_LIST->currentItem()->data(Qt::UserRole-1).toInt())->setVolume(aSliderPosition);
                            ui->AMBIENT_SOUND_LIST->currentItem()->setData(Qt::UserRole+1,aSliderPosition);

                        });

            connect(ui->AMBIENT_SOUND_VOLUME,  &QAbstractSlider::sliderReleased, [=]{
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName(settingsDbFile);

                QString currentID = ui->AMBIENT_SOUND_LIST->currentItem()->data(Qt::UserRole-1).toString();
                QString volume = ui->AMBIENT_SOUND_LIST->currentItem()->data(Qt::UserRole+1).toString();
                if(db.open()) {
                    QSqlQuery query("UPDATE ambient_sounds SET `volume` = '"
                                    +volume+
                                    "' WHERE `id`='"
                                    +currentID+"';");
                    QString qstr = "UPDATE ambient_sounds SET `volume` = '"
                            +volume+
                            "' WHERE `id`='"
                            +currentID+"';";
                    //qDebug() << qstr;
                     if (query.lastError().isValid()){} /*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
                     query.exec();}
                else {};

                db.close();
            });

    connect(ui->MUSIC_MODE_SELECT, &QListWidget::itemClicked, [=]{
        int pbMode = ui->MUSIC_MODE_SELECT->currentItem()->data(Qt::UserRole).toInt();
        switch (pbMode) {
            case 0:
                MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
                break;
            case 1:
                MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
                break;
            case 2:
                MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::Sequential);
                break;
            case 3:
                MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::Loop);
                break;
            case 4:
                MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::Random);
                break;
            default:
                MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::Loop);
               break;
            }

    });

    connect(ui->AMBIENT_SOUND_LIST,&QListWidget::itemSelectionChanged, [=]{
        if (ui->AMBIENT_SOUND_LIST->currentItem()->isSelected()) {
            int selectedID =ui->AMBIENT_SOUND_LIST->currentItem()->data(Qt::UserRole-1).toInt();



            ambientSounds.at(selectedID)->playlist()->addMedia(QUrl::fromUserInput("/home/pi/replicator-data/media/ambient/"+ui->AMBIENT_SOUND_LIST->currentItem()->data(Qt::UserRole).toString()));
            ambientSounds.at(selectedID)->play();
            ui->AMBIENT_SOUND_VOLUME->setValue(ambientSounds.at(selectedID)->volume());
            //qDebug() << "selected" << ui->AMBIENT_SOUND_LIST->currentItem()->data(Qt::UserRole-1).toString() << ui->AMBIENT_SOUND_LIST->currentItem()->data(Qt::UserRole).toString();
        } else {
            ambientSounds.at(ui->AMBIENT_SOUND_LIST->currentItem()->data(Qt::UserRole-1).toInt())->stop();
            //qDebug() << "deselected" << ui->AMBIENT_SOUND_LIST->currentItem()->data(Qt::UserRole-1).toString() << ui->AMBIENT_SOUND_LIST->currentItem()->data(Qt::UserRole).toString();
        }
    });

    //!*qDebug() << "17";
    connect(this, SIGNAL(window_loaded), this, SLOT(window_shown()), Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));

    //! FOLDERS
    connect(ui->MUSIC_DB_LIST_FOLDERS_SLIDER,
            SIGNAL(valueChanged(int)),ui->MUSIC_DATABASE_LIST_FOLDERS->verticalScrollBar(),SLOT(setValue(int)));

    connect(ui->MUSIC_DATABASE_LIST_FOLDERS->verticalScrollBar(),&QScrollBar::rangeChanged,[=]() {
       ui->MUSIC_DB_LIST_FOLDERS_SLIDER->setMaximum(ui->MUSIC_DATABASE_LIST_FOLDERS->verticalScrollBar()->maximum());
    });

    //! ARTISTS

    connect(ui->MUSIC_DB_LIST_ARTISTS_SLIDER,
            SIGNAL(valueChanged(int)),ui->MUSIC_DATABASE_LIST_ARTISTS->verticalScrollBar(),SLOT(setValue(int)));

    connect(ui->MUSIC_DATABASE_LIST_ARTISTS->verticalScrollBar(),&QScrollBar::rangeChanged,[=]() {
       ui->MUSIC_DB_LIST_ARTISTS_SLIDER->setMaximum(ui->MUSIC_DATABASE_LIST_ARTISTS->verticalScrollBar()->maximum());
    });

    //! ALBUMS

    connect(ui->MUSIC_DB_LIST_ALBUMS_SLIDER,
            SIGNAL(valueChanged(int)),ui->MUSIC_DATABASE_LIST_ALBUMS->verticalScrollBar(),SLOT(setValue(int)));

    connect(ui->MUSIC_DATABASE_LIST_ALBUMS->verticalScrollBar(),&QScrollBar::rangeChanged,[=]() {
       ui->MUSIC_DB_LIST_ALBUMS_SLIDER->setMaximum(ui->MUSIC_DATABASE_LIST_ALBUMS->verticalScrollBar()->maximum());
    });

    //! TITLES

    connect(ui->MUSIC_DB_LIST_TITLES_SLIDER,
            SIGNAL(valueChanged(int)),ui->MUSIC_DATABASE_LIST_TITLES->verticalScrollBar(),SLOT(setValue(int)));

    connect(ui->MUSIC_DATABASE_LIST_TITLES->verticalScrollBar(),&QScrollBar::rangeChanged,[=]() {
        //qDebug() << ui->MUSIC_DATABASE_LIST_TITLES->verticalScrollBar()->maximum() << "." << ui->MUSIC_DB_LIST_FOLDERS_SLIDER->maximum();
       ui->MUSIC_DB_LIST_TITLES_SLIDER->setMaximum(ui->MUSIC_DATABASE_LIST_TITLES->verticalScrollBar()->maximum());
    });


    //!* PLAYLIST
    connect(ui->MUSIC_PLAYLIST->verticalScrollBar(),&QScrollBar::rangeChanged,[=]() {
       ui->MUSIC_PLAYLIST_SLIDER->setMaximum(ui->MUSIC_PLAYLIST->verticalScrollBar()->maximum());
    });

    //!*qDebug() << "18";
    connect(ui->MUSIC_PLAYLIST_SLIDER,SIGNAL(valueChanged(int)),ui->MUSIC_PLAYLIST->verticalScrollBar(),SLOT(setValue(int)));

    connect(ui->SR_RESULT->verticalScrollBar(),&QScrollBar::rangeChanged,[=]() {
       ui->SR_SLIDER->setMaximum(ui->SR_RESULT->verticalScrollBar()->maximum());
    });


    connect(ui->MUSIC_FILES_SLIDER,SIGNAL(valueChanged(int)),ui->MUSIC_DATABASE_LIST->verticalScrollBar(),SLOT(setValue(int)));


    connect(ui->MUSIC_DATABASE_LIST->verticalScrollBar(),&QScrollBar::rangeChanged,[=]() {
       ui->MUSIC_FILES_SLIDER->setMaximum(ui->MUSIC_DATABASE_LIST->verticalScrollBar()->maximum());
    });




    connect(ui->VIDEO_FILES_SLIDER,SIGNAL(valueChanged(int)),ui->VIDEO_FILES_LIST->verticalScrollBar(),SLOT(setValue(int)));

    connect(ui->VIDEO_FILES_LIST->verticalScrollBar(),&QScrollBar::rangeChanged,[=]() {
       ui->VIDEO_FILES_SLIDER->setMaximum(ui->VIDEO_FILES_LIST->verticalScrollBar()->maximum());
    });
    /*ui->MAIN_VOLUME_SLIDER->setValue(mainVolume);
    ui->MENU_VOLUME_SLIDER->setValue(menuVolume);
    ui->MEDIA_VOLUME_SLIDER->setValue(mediaVolume);
    ui->ALARM_VOLUME_SLIDER->setValue(alarmVolume);*/

    //!**** VOLUMES ***** /
    connect(ui->MAIN_VOLUME_SLIDER,&QSlider::valueChanged,[=]() {
        mainVolume = ui->MAIN_VOLUME_SLIDER->value();

        MusicPlayerClass->MusicPlayer.setVolume(((mainVolume*mediaVolume*musicVolume)/100)/100);
        Sound_MenuBeep->setVolume((mainVolume*menuVolume)/100);
        Sound_RedAlert->setVolume((mainVolume*alarmVolume)/100);
            menuVolumeGlobal = (mainVolume*menuVolume)/100;
    });

    connect(ui->MENU_VOLUME_SLIDER,&QSlider::valueChanged,[=]() {
        menuVolume = ui->MENU_VOLUME_SLIDER->value();
        Sound_MenuBeep->setVolume((mainVolume*menuVolume)/100);
            menuVolumeGlobal = (mainVolume*menuVolume)/100;
    });

    connect(ui->MEDIA_VOLUME_SLIDER,&QSlider::valueChanged,[=]() {
        mediaVolume = ui->MEDIA_VOLUME_SLIDER->value();

        MusicPlayerClass->MusicPlayer.setVolume(((mainVolume*mediaVolume*musicVolume)/100)/100);
    });

    connect(ui->ALARM_VOLUME_SLIDER,&QSlider::valueChanged,[=]() {
        alarmVolume = ui->ALARM_VOLUME_SLIDER->value();
        Sound_RedAlert->setVolume((mainVolume*alarmVolume)/100);
    });

    connect(ui->MUSIC_PLAYER_VOLUME,&QSlider::valueChanged,[=]() {
        musicVolume = ui->MUSIC_PLAYER_VOLUME->value();
    });

    connect(ui->MAIN_VOLUME_SLIDER,&QSlider::sliderReleased,[=]() {

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(settingsDbFile);

        if(db.open()) {
            QSqlQuery query("UPDATE settings SET `vol_main` = '"+QString::number(mainVolume)+"' WHERE `id`=0;");
             if (query.lastError().isValid()){} /*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
             query.exec();}
        else {};

        db.close();
    });

    connect(ui->MENU_VOLUME_SLIDER,&QSlider::sliderReleased,[=]() {

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(settingsDbFile);

        if(db.open()) {
            QSqlQuery query("UPDATE settings SET `vol_menu` = '"+QString::number(menuVolume)+"' WHERE `id`=0;");
             if (query.lastError().isValid()){} /*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
             query.exec();
        }
        else {};

        db.close();
    });

    connect(ui->MEDIA_VOLUME_SLIDER,&QSlider::sliderReleased,[=]() {

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(settingsDbFile);

        if(db.open()) {        QSqlQuery query("UPDATE settings SET `vol_media` = '"+QString::number(mediaVolume)+"' WHERE `id`=0;");
            if (query.lastError().isValid()){} /*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
            query.exec();}
        else {};


        db.close();
    });


    connect(ui->ALARM_VOLUME_SLIDER,&QSlider::sliderReleased,[=]() {

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(settingsDbFile);

        if(db.open()) {
            QSqlQuery query("UPDATE settings SET `vol_alarm` = '"+QString::number(alarmVolume)+"' WHERE `id`=0;");
             if (query.lastError().isValid()){} /*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
             query.exec();
        }
        else {};


        db.close();
    });


    connect(ui->MUSIC_PLAYER_VOLUME,&QSlider::sliderReleased,[=]() {

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(settingsDbFile);

        if(db.open()) {
            QSqlQuery query("UPDATE settings SET `vol_music` = '"+QString::number(musicVolume)+"' WHERE `id`=0;");
             if (query.lastError().isValid()){} /*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
             query.exec();
        }
        else {};


        db.close();
    });

    connect(ui->DISPLAY_BRIGHTNESS_SLIDER,&QSlider::sliderReleased,[=]() {

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(settingsDbFile);

        if(db.open()) {
            QSqlQuery query("UPDATE settings SET `brightness` = '"+QString::number(brightness)+"' WHERE `id`=0;");
             if (query.lastError().isValid()){} /*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
             query.exec();
        }
        else {};


        db.close();
    });

    connect(ui->DISPLAY_BRIGHTNESS_SLIDER_2,&QSlider::sliderReleased,[=]() {

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(settingsDbFile);

        if(db.open()) {
            QSqlQuery query("UPDATE settings SET `brightness` = '"+QString::number(brightness)+"' WHERE `id`=0;");
             if (query.lastError().isValid()){} /*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
             query.exec();
        }
        else {};


        db.close();
    });
    //!**** /VOLUMES ***** /


    connect(ui->SET_START_SERVER,&QPushButton::clicked,[=]() { QProcess::startDetached(serverBin.toStdString().c_str()); });

    connect(ui->SET_SHUTDOWN_SERVER,&QPushButton::clicked,[=]() { m_webSocket->sendTextMessage("shutdown"); });

    connect(ui->SET_RESTART_SERVER,&QPushButton::clicked,[=]() {

        system(qPrintable(rootCommand+" killall replicator-server "));
        //system(qPrintable(rootCommand+" kill "+serverPID));

        extra_functions::delay(2000);

        QProcess::startDetached(serverBin.toStdString().c_str());
    });

    connect(ui->SET_KILL_SERVER,&QPushButton::clicked,[=]() { system(qPrintable(rootCommand+" killall replicator-server")); });

    connect(ui->SETTINGS_SERVER,&QPushButton::clicked,[=]() { ui->PAGES->setCurrentWidget(ui->SETTINGS);
    ui->SETTINGS_TABS->setCurrentWidget(ui->SET_SERVER);
    changeTitle(server_settings_title);
    });

    connect(ui->SET_SERVER_BACK,&QPushButton::clicked,[=]() { ui->PAGES->setCurrentWidget(ui->SERVER_STATUS);changeTitle(server_status_title); });



    connect(ui->SR_SLIDER,SIGNAL(valueChanged(int)),ui->SR_RESULT->verticalScrollBar(),SLOT(setValue(int)));


    //!*qDebug() << "19";

    /******* KEYBOARD BUTTONS *************/
    QPlainTextEdit *searchEdit = ui->SEARCHTEXT;
    qApp->setCursorFlashTime(0);


    connect(ui->KEYBOARD_LINE_UP,&QPushButton::clicked,[=]() {

    if (ui->SEARCHTEXT->verticalScrollBar()->value() >= ui->SEARCHTEXT->verticalScrollBar()->minimum()+1) {
        ui->SEARCHTEXT->verticalScrollBar()->setValue(ui->SEARCHTEXT->verticalScrollBar()->value() - 1);
    }
        Sound_MenuBeep->play();
    });

    connect(ui->KEYBOARD_LINE_DOWN,&QPushButton::clicked,[=]() {

    if (ui->SEARCHTEXT->verticalScrollBar()->value() <= ui->SEARCHTEXT->verticalScrollBar()->maximum()-1) {
        ui->SEARCHTEXT->verticalScrollBar()->setValue(ui->SEARCHTEXT->verticalScrollBar()->value() + 1);
    }
        Sound_MenuBeep->play();
    });


    connect(ui->KEYBOARD_TOGGLE_NUM,&QPushButton::clicked,[=]() { switch (ui->KEYBOARD_GLYPHS->isVisible()) {
        case 0:
            ui->KEYBOARD_GLYPHS->setVisible(true);
            break;
        case 1:
            ui->KEYBOARD_GLYPHS->setVisible(false);
        break;

        default:
            break;
        } Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_0,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("0"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_1,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("1"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_2,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("2"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_3,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("3");Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_4,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("4"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_5,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("5"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_6,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("6"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_7,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("7"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_8,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("8"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_9,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("9"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_AND,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("&"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_ENTER,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("\n"); Sound_MenuBeep->play();});

    connect(ui->KEYBOARD_A,&QPushButton::clicked,[=]() {
        if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("A"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("a"); }Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_B,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("B"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("b"); } Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_C,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("C"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("c"); }Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_D,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("D"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("d"); }Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_E,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("E"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("e"); }Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_F,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("F"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("f"); }Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_G,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("G"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("g"); }Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_H,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("H"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("h"); }Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_I,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("I"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("i"); }Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_J,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("J"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("j"); }Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_K,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("K"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("k"); }Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_L,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("L"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("l"); }Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_M,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("M"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("m"); }Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_N,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("N"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("n"); }Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_O,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("O"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("o"); }Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_P,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("P"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("p"); }Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_Q,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("Q"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("q"); }Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_R,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("R"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("r"); }Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_S,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("S"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("s"); } Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_T,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("T"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("t"); } Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_U,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("U"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("u"); }Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_V,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("V"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("v"); } Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_W,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("W"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("w"); }Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_X,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("X"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("x"); }Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_Y,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("Y"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("y"); }Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_Z,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("Z"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("z"); }Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_SS,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("ß"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_AE,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("Ä"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("ä"); }Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_OE,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("Ö"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("ö"); }Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_UE,&QPushButton::clicked,[=]() {         if (shiftPressed==true || capslockPressed == true) { searchEdit->insertPlainText("Ü"); shiftPressed=false; ui->KEYBOARD_SHIFT->setChecked(false);
        } else { searchEdit->insertPlainText("ü"); }Sound_MenuBeep->play(); });
    //connect(ui->KEYBOARD_NEXT,&QPushButton::clicked,[=]() { });
    //connect(ui->KEYBOARD_PREV,&QPushButton::clicked,[=]() { });
    connect(ui->KEYBOARD_LEFT,&QPushButton::clicked,[=]() { searchEdit->moveCursor(QTextCursor::Left, QTextCursor::MoveAnchor); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_RIGHT,&QPushButton::clicked,[=]() { searchEdit->moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_BACKSPACE,&QPushButton::clicked,[=]() { searchEdit->moveCursor(QTextCursor::Left, QTextCursor::KeepAnchor);  searchEdit->textCursor().removeSelectedText(); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_DELETE,&QPushButton::clicked,[=]() { searchEdit->moveCursor(QTextCursor::Right, QTextCursor::KeepAnchor);  searchEdit->textCursor().removeSelectedText(); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_HYPHEN,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("-"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_SLASH,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("/"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_BACKSLASH,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("\\"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_COLON,&QPushButton::clicked,[=]() { searchEdit->insertPlainText(":"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_COMMA,&QPushButton::clicked,[=]() { searchEdit->insertPlainText(","); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_SPACE,&QPushButton::clicked,[=]() { searchEdit->insertPlainText(" "); Sound_MenuBeep->play();});

    connect(ui->KEYBOARD_BRACKET_CURLY_LEFT,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("{"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_BRACKET_CURLY_RIGHT,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("}"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_BRACKET_NORM_LEFT,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("("); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_BRACKET_NORM_RIGHT,&QPushButton::clicked,[=]() { searchEdit->insertPlainText(")"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_BRACKET_SQUARE_LEFT,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("["); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_BRACKET_SQUARE_RIGHT,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("]"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_SEMICOLON,&QPushButton::clicked,[=]() { searchEdit->insertPlainText(";"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_QUESTIONMARK,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("?"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_DOLLAR,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("$");Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_PERCENT,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("%"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_PERIOD,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("."); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_SINGLEQUOTE,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("'"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_QUOTE,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("\""); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_POWEROF,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("^");Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_PIPE,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("|"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_UNDERLINE,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("_"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_HASH,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("#");Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_PLUS,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("+");Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_ASTERISK,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("*"); Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_EQUAL,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("=");Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_SMALLER,&QPushButton::clicked,[=]() { searchEdit->insertPlainText("<");Sound_MenuBeep->play(); });
    connect(ui->KEYBOARD_GREATER,&QPushButton::clicked,[=]() { searchEdit->insertPlainText(">"); Sound_MenuBeep->play();});

    connect(ui->SYSTEM_BUTTON_NOTES,&QPushButton::clicked,[=]() {

        changeTitle(notes_title);

        m_webSocket->sendTextMessage("MARKED::SELECT `id`,`title` FROM recipes WHERE `selected`='1'");

        m_webSocket->sendTextMessage("NOTES::SELECT `id`,`title`,`text` FROM notes");

        ui->PAGES->setCurrentWidget(ui->NOTES);Sound_MenuBeep->play();
    });

    connect(ui->KEYBOARD_SHIFT,&QPushButton::clicked,[=]() {
        if (ui->KEYBOARD_SHIFT->isChecked() == false)
            { shiftPressed=false; }
        else { shiftPressed = true;} Sound_MenuBeep->play();});
    connect(ui->KEYBOARD_CAPS,&QPushButton::clicked,[=]() {
                if (ui->KEYBOARD_CAPS->isChecked() == false)
                { capslockPressed=false; }
        else { capslockPressed= true;} Sound_MenuBeep->play();});


    connect(ui->KEYBOARD_CANCEL,&QPushButton::clicked,[=]() {
        ui->SEARCHTEXT->clear();

        ui->PAGES->setCurrentWidget(current_widget); Sound_MenuBeep->play();});



            connect(ui->NOTES_BACK,&QPushButton::clicked,[=]() {

                ui->PAGES->setCurrentWidget(ui->SYSTEM); Sound_MenuBeep->play();
            });

    //!! NOTES -----------------
    // NEW
    connect(ui->NEW_NOTE,&QPushButton::clicked,[=]() {
        ui->SEARCHTEXT->clear();
        current_widget=ui->NOTES;

        ui->NOTES_LIST->clearSelection();

        ui->NOTES_TABS->setCurrentWidget(ui->NOTES_EDIT);
        Sound_MenuBeep->play();});

    // CANCEL
    connect(ui->NOTES_EDIT_CANCEL,&QPushButton::clicked,[=]() {
        ui->SEARCHTEXT->clear();

        current_widget=ui->NOTES;
        ui->NOTES_TABS->setCurrentWidget(ui->NOTES_START);
        Sound_MenuBeep->play();});

    // REMOVE
    connect(ui->REMOVE_NOTE,&QPushButton::clicked,[=]() {
        ui->SEARCHTEXT->clear();

        current_widget=ui->NOTES;
        ui->NOTES_TABS->setCurrentWidget(ui->NOTES_START);

        if (ui->NOTES_LIST->selectedItems().count() > 0) {
            m_webSocket->sendTextMessage("UPDATE::DELETE FROM notes WHERE `id`='"+ui->NOTES_LIST->currentItem()->data(Qt::UserRole).toString()+"'");
            delete ui->NOTES_LIST->currentItem();
        } else {

        }
        Sound_MenuBeep->play();});
    // SAVE
    connect(ui->NOTES_EDIT_SAVE,&QPushButton::clicked,[=]() {
        ui->SEARCHTEXT->clear();

        current_widget=ui->NOTES;
        ui->NOTES_TABS->setCurrentWidget(ui->NOTES_START);

        if (ui->NOTES_LIST->selectedItems().count() > 0) {

            ui->NOTES_LIST->currentItem()->setData(Qt::DisplayRole,ui->NOTES_EDIT_TITLE_LINE->text());
            m_webSocket->sendTextMessage("UPDATE::UPDATE notes SET `title` = '"+ui->NOTES_EDIT_TITLE_LINE->text()+"',`text` = '"+ui->NOTES_EDIT_TEXTFIELD->toPlainText()+"' WHERE `id`='"+ui->NOTES_LIST->currentItem()->data(Qt::UserRole).toString()+"';");

        } else {



            m_webSocket->sendTextMessage("UPDATE::INSERT INTO notes(title,text) VALUES ('"+ui->NOTES_EDIT_TITLE_LINE->text()+"','"+ui->NOTES_EDIT_TEXTFIELD->toPlainText()+"')");


            /*QSqlQuery query("select max(id) from timer");
                 if (query.lastError().isValid()){};
                 query.exec();

                while (query.next()) {
                    maxval = query.value(0).toInt();
                }
            }



            if(db.open()) {
                //qDebug()<<ui->TIMER_LIST->currentItem()->data(Qt::UserRole).toBool();
                if (maxval+1 == ui->TIMER_LIST->currentItem()->data(Qt::UserRole).toInt()) {
                     QString queryString;
                     queryString = "INSERT INTO timer(id,name, hh, mm, ss) VALUES ('"
                            +ui->TIMER_LIST->currentItem()->data(Qt::UserRole).toString()+"','"
                            +ui->TIMER_NAME_EDIT->text()+"','"
                            +QString::number(ui->ALARM_SPINNER_H->value())+"','"
                            +QString::number(ui->ALARM_SPINNER_M->value())+"','"
                            +QString::number(ui->ALARM_SPINNER_S->value())+"')";
                     //qDebug()<< queryString;
                     QSqlQuery query(queryString);

                      if (query.lastError().isValid()){};
                      query.exec();
                } else {
                     QString queryString;
                    queryString="UPDATE timer SET `name` = '"+ui->TIMER_NAME_EDIT->text()+"',"
                                                 " `hh` = '"+QString::number(ui->ALARM_SPINNER_H->value())+"',"
                                                 " `mm` = '"+QString::number(ui->ALARM_SPINNER_M->value())+"',"
                                                 " `ss` = '"+QString::number(ui->ALARM_SPINNER_S->value())+"'"
                                                 " WHERE `id`='"+ui->TIMER_LIST->currentItem()->data(Qt::UserRole).toString()+"';";
                    QSqlQuery query(queryString);
                     if (query.lastError().isValid()){};
                     query.exec();
                }



                }
            else {};
            db.close();
            */


        QListWidgetItem *item = new QListWidgetItem;
        item->setData(Qt::DisplayRole,ui->NOTES_EDIT_TITLE_LINE->text());
        item->setData(Qt::UserRole,"id");
        item->setData(Qt::UserRole+1,ui->NOTES_EDIT_TEXTFIELD->toPlainText());
        ui->NOTES_LIST->addItem(item);
        }
        Sound_MenuBeep->play();});

    // EDIT NOTE
    connect(ui->EDIT_NOTE,&QPushButton::clicked,[=]() {
        ui->SEARCHTEXT->clear();

        if (ui->NOTES_LIST->selectedItems().count() > 0) {

            current_widget=ui->NOTES;
            ui->NOTES_TABS->setCurrentWidget(ui->NOTES_EDIT);
            ui->NOTES_EDIT_TEXTFIELD->clear();
            ui->NOTES_EDIT_TEXTFIELD->appendPlainText(ui->NOTES_LIST->currentItem()->data(Qt::UserRole+1).toString());
            ui->NOTES_EDIT_TITLE_LINE->clear();
            ui->NOTES_EDIT_TITLE_LINE->setText(ui->NOTES_LIST->currentItem()->data(Qt::DisplayRole).toString());
            Sound_MenuBeep->play();
        } else {
            QMediaPlayer deny;
            deny.setVolume(menuVolume);
            deny.setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
            deny.play();
        }
        });

    //!! /NOTES -----------------

    /* KEYBOARD OK CLICKED */
    connect(ui->KEYBOARD_OK,&QPushButton::clicked,[=]() {
       searchOK(searchSection,searchEdit->toPlainText()); Sound_MenuBeep->play();
    });

    connect(ui->MUSIC_DB_SEARCH_FOLDERS,&QPushButton::clicked,[=]() {
        searchSection="folders";
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });

    connect(ui->NOTES_EDIT_BUTTON,&QPushButton::clicked,[=]() {
        searchSection="editNote";
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });

    connect(ui->NOTES_EDIT_EDIT_TITLE,&QPushButton::clicked,[=]() {
        searchSection="editNoteTitle";
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });

    connect(ui->MUSIC_DB_SEARCH_ARTISTS,&QPushButton::clicked,[=]() {
        searchSection="artists";
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });

    connect(ui->MUSIC_DB_SEARCH_ALBUMS,&QPushButton::clicked,[=]() {
        searchSection="albums";
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });

    connect(ui->MUSIC_DB_SEARCH_TITLES,&QPushButton::clicked,[=]() {
        searchSection="titles";
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });

    connect(ui->MUSIC_DB_EDIT_NAME,&QPushButton::clicked,[=]() {
        searchSection="name";
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });

    connect(ui->MUSIC_DB_EDIT_PATH,&QPushButton::clicked,[=]() {
        searchSection="path";
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });


    connect(ui->SR_SEARCH,&QPushButton::clicked,[=](){
        ui->SR_TEXT->clear();
        searchSection="recipeList";
        current_widget = ui->SEARCH_RECIPE;
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });

    connect(ui->terminal_Edit,&QPushButton::clicked,[=](){
        searchSection="command";
        current_widget=ui->SETTINGS;
        ui->SEARCHTEXT->clear();
        ui->SEARCHTEXT->appendPlainText(ui->terminal_Text->toPlainText());
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });

    connect(ui->TIMER_EDIT,&QPushButton::clicked,[=](){
        searchSection="timer_name";
        ui->SEARCHTEXT->clear();
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });


    connect(ui->SERVER_IP_CLIENT,&QPushButton::clicked,[=]() {
        current_widget=ui->SERVER_STATUS;
        searchSection="serverIP";
        ui->SEARCHTEXT->clear();
        ui->SEARCHTEXT->appendPlainText(serverIP);
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });

    connect(ui->SERVER_PORT_CLIENT,&QPushButton::clicked,[=]() {
          current_widget=ui->SERVER_STATUS;
        searchSection="serverPort";
        ui->SEARCHTEXT->clear();
        ui->SEARCHTEXT->appendPlainText(serverPort);
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });

    connect(ui->SERVER_PATH_CLIENT,&QPushButton::clicked,[=]() {
          current_widget=ui->SERVER_STATUS;
        searchSection="serverBin";
        ui->SEARCHTEXT->clear();
        ui->SEARCHTEXT->appendPlainText(serverBin);
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
    });

    /*DBPathServerLabel_remote
            ui->SERVER_IP_EDIT*/

    connect(ui->SERVER_IP_EDIT,&QPushButton::clicked,[=]() {
        current_widget=ui->SETTINGS;
        searchSection="remote_settings_ip";
        ui->SEARCHTEXT->clear();
        ui->SEARCHTEXT->appendPlainText(ui->IPServerLabel_remote->text());
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
        Sound_MenuBeep->play();
    });

    connect(ui->SERVER_PORT_EDIT,&QPushButton::clicked,[=]() {
        current_widget=ui->SETTINGS;
        searchSection="remote_settings_port";
        ui->SEARCHTEXT->clear();
        ui->SEARCHTEXT->appendPlainText(ui->PortServerLabel_remote->text());
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
        Sound_MenuBeep->play();
    });
    connect(ui->SERVER_PROTOCOL_EDIT,&QPushButton::clicked,[=]() {
        current_widget=ui->SETTINGS;
        searchSection="remote_settings_protocol";
        ui->SEARCHTEXT->clear();
        ui->SEARCHTEXT->appendPlainText(ui->ProtocolServerLabel_remote->text());
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
        Sound_MenuBeep->play();
    });
    connect(ui->SERVER_DB_PATH_EDIT,&QPushButton::clicked,[=]() {
        current_widget=ui->SETTINGS;
        searchSection="remote_settings_path";
        ui->SEARCHTEXT->clear();
        ui->SEARCHTEXT->appendPlainText(ui->DBPathServerLabel_remote->text());
        ui->PAGES->setCurrentWidget(ui->KEYBOARD); Sound_MenuBeep->play();
        Sound_MenuBeep->play();
    });

    connect(ui->terminal_Run,&QPushButton::clicked,[=](){
        QString commandToRun=ui->terminal_Text->toPlainText();
Sound_MenuBeep->play();
         system(qPrintable(commandToRun));
    });

    connect(ui->terminal_RunRoot,&QPushButton::clicked,[=](){
        QString commandToRun=rootCommand+ui->terminal_Text->toPlainText();
Sound_MenuBeep->play();
         system(qPrintable(commandToRun));

    });


    connect(ui->SR_CLEAR,&QPushButton::clicked,[=](){
        ui->SR_TEXT->clear(); Sound_MenuBeep->play();
    });

}

QString MainWindow::searchRequest(QString returnText) {
    //!*qDebug() << "20";
    ui->SR_RESULT->clear();

    if ( m_webSocket->state() == QAbstractSocket::UnconnectedState) {
        m_webSocket->open(QUrl(url));
        ui->PAGES->setCurrentWidget(ui->SEARCH_DISCONNECTED);
    }
    else if (m_webSocket->state() == QAbstractSocket::ConnectedState) {
        processData(returnText);
    }
    return 0;

}

MainWindow::~MainWindow()
{
    //!*qDebug() << "21";
    delete ui;
}

void MainWindow::showTime()
{
    ////!*qDebug() << "22";
    /* TIMER FOR CURRENT TIME LABEL */
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    QDate cd = QDate::currentDate();


    ui->SYSTEM_TIME_LABEL->setText(text);

    ui->SYSTEM_TIME_LABEL_4->setText(cd.toString("dd.MM.yyyy"));
    ui->TOP_BAR_HH->setText(text);
    ui->TIMER_CURRENT_TIME_TIME->setText(text);


    QDate date = QDateTime::currentDateTime().date();
    QLocale locale  = QLocale(currentLanguage);
    QString currentDay = locale.toString(date, "dddd");
    ui->SYSTEM_TIME_LABEL_5->setText(currentDay.toUpper());

     m_webSocket->sendTextMessage("ping");
     //m_webSocket->sendTextMessage("pid");
     if ( m_webSocket->state() == QAbstractSocket::UnconnectedState) {
        ui->SERVER_STATUS_RESPONSE->setText("NOT RESPONDING");
        ui->SERVER_STATUS_RESPONSE_2->setText("NOT RESPONDING");
        ui->serverPID_label->setText("");
        ui->serverPID_label_2->setText("");
     }

     if ( m_webSocket->state() == QAbstractSocket::UnconnectedState) {
         m_webSocket->open(QUrl(url));

     }
     else if (m_webSocket->state() == QAbstractSocket::ConnectedState) {

     }
     extra_functions::delay(200);
     if ( m_webSocket->state() == QAbstractSocket::UnconnectedState) {
         m_webSocket->open(QUrl(url));

     }
     else if (m_webSocket->state() == QAbstractSocket::ConnectedState) {

     }

}


void MainWindow::on_MM_BUTTON_SYSTEM_clicked()
{
    changeTitle(system_title);
    //!*qDebug() << "23";
    /* CHANGE TO THE SYSTEM TAB */
    ui->PAGES->setCurrentWidget(ui->SYSTEM);
}

void MainWindow::on_MM_BUTTON_RECIPE_clicked()
{
    //!*qDebug() << "24";
    /* CHANGE TO THE VIEW RECIPE TAB */
    ui->PAGES->setCurrentWidget(ui->VIEW_RECIPE);

}


void MainWindow::on_AW_RED_ALERT_TEXT_clicked()
{

    /* STOP THE RED ALERT */
    if (timer_countdown->isActive()) {
        timer_countdown->stop();
        ui->TIMER_ACTIVE_HH->setText("00");
        ui->TIMER_ACTIVE_MM->setText("00");
        ui->TIMER_ACTIVE_SS->setText("00");
        ui->TIMER_TIMEOUT_TIME->setText(timer_inactive.toUpper());

    }

    Sound_RedAlert->stop();
    StopTheRedAlert=true;
    //timer->stop();
    Sound_RedAlert->setMedia(QUrl("qrc:/sounds/tng_red_alert1.mp3"));
    ui->PAGES->setCurrentWidget(last_widget);
}



void MainWindow::on_MM_BUTTON_SEARCH_clicked()
{
    changeTitle(search_title);

    if ( m_webSocket->state() == QAbstractSocket::UnconnectedState) {
        ui->PAGES->setCurrentWidget(ui->SEARCH_DISCONNECTED);
        ui->SERVER_STATUS_RESPONSE->setText("NOT RESPONDING");
        ui->serverPID_label->setText("");
        ui->SERVER_STATUS_RESPONSE_2->setText("NOT RESPONDING");
        ui->serverPID_label_2->setText("");
    }
    else if (m_webSocket->state() == QAbstractSocket::ConnectedState) {
        ui->PAGES->setCurrentWidget(ui->SEARCH_RECIPE);
    }


}

void MainWindow::on_pushButton_clicked()
{
    /*lockScreen(red_alert_widgets,all_widgets);

    ui->PAGES->setCurrentWidget(ui->LOCK);*/

    lockform = new LockScreen(this);
    //connect(keyb,SIGNAL(closed()),this,SLOT(updateVarTextFeld()));
    lockform->setAttribute(Qt::WA_DeleteOnClose);
    lockform->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    lockform->showFullScreen(); //opening second form
}
/*
void MainWindow::on_SR_SEARCH_clicked()
{
    ui->SR_RESULT->clear();
    if ( m_webSocket->state() == QAbstractSocket::UnconnectedState) {
        m_webSocket->open(QUrl(url));
        ui->PAGES->setCurrentWidget(ui->SEARCH_DISCONNECTED);
    }
    else if (m_webSocket->state() == QAbstractSocket::ConnectedState) {
        processData();
    }
}
*/
/*** DATABASE CONNECTION ***/

void MainWindow::onDisconnected() {
    //!*qDebug() << "27";
    //!*qDebug() << "Disconnected.";

}



void MainWindow::onConnected() {
    //!*qDebug() << "28";
    //!*qDebug() << "Connected to replicator database server.";

    //!*qDebug() << m_webSocket->state();

    connect(m_webSocket, &QWebSocket::textMessageReceived,
            this, &MainWindow::onTextMessageReceived);
    connect(m_webSocket, &QWebSocket::binaryMessageReceived,
            this, &MainWindow::onBinaryMessageRecieved);

    m_webSocket->sendTextMessage("Hi there!");
}

void MainWindow::onTextMessageReceived(QString message) {
    //qDebug() << "recieved";

    //qDebug() << "msg:" << message;

    QStringList messageRec = message.split(":");
    if (messageRec.at(0) == "pid") {
        //qDebug() << messageRec.at(1);

        QString current_serverPID = messageRec.at(1);
        serverPID=current_serverPID;
        ui->serverPID_label->setText(current_serverPID);
        ui->serverPID_label_2->setText(current_serverPID);
        ui->SERVER_STATUS_RESPONSE->setText("RUNNING");
        ui->SERVER_STATUS_RESPONSE_2->setText("RUNNING");
    } else {
        ui->SERVER_STATUS_RESPONSE->setText("NOT RESPONDING");
        ui->SERVER_STATUS_RESPONSE_2->setText("NOT RESPONDING");
        ui->serverPID_label->setText("");
        ui->serverPID_label_2->setText("");
    }
}

void MainWindow::onBinaryMessageRecieved(QByteArray message) {
    //!*qDebug() << "31";
    //!*qDebug() << "Binary Message received";
    replicationData.clear();
    replicationData << Process_Data::deserialize(message);
    //qDebug() << "repData:" << replicationData.at(0);
    ////!*qDebug() << replicationData.size();
    QStringList firstValue = replicationData.at(0);
    if (firstValue.at(0) == "RECIPE_LIST") {
        replicationData.removeAt(0);
        fillScrollArea(replicationData);
        //qDebug() << "repData1:" << replicationData;
    } else if (firstValue.at(0) == "RECIPE") {
        replicationData.removeAt(0);
        loadRecipe(replicationData);
        //qDebug() << "repData:2" << replicationData;
    } else if (firstValue.at(0) == "MARKED") {
        replicationData.removeAt(0);
        ui->MARKED_RECIPES->clear();
        for (int i=0; i< replicationData.size(); i++) {

            QListWidgetItem *itemSelected = new QListWidgetItem();

            QStringList resultItems = replicationData[i];

            itemSelected->setData(Qt::DisplayRole, resultItems.at(1));
            itemSelected->setData(Qt::UserRole, resultItems.at(0));

            ui->MARKED_RECIPES->addItem(itemSelected);
        }
        //qDebug() << "repData:3" << replicationData;

    }

    else if (firstValue.at(0) == "NOTES") {
           replicationData.removeAt(0);
           ui->NOTES_LIST->clear();
           for (int i=0; i< replicationData.size(); i++) {

               QListWidgetItem *itemSelected = new QListWidgetItem();

               QStringList resultItems = replicationData[i];
                //qDebug() << resultItems;

               qDebug() << resultItems.count();

               for (int ii=0; ii<resultItems.count(); ii++) {
                   qDebug() << resultItems.at(ii);
               }


               itemSelected->setData(Qt::DisplayRole, resultItems.at(1));
               itemSelected->setData(Qt::UserRole, resultItems.at(0));
               itemSelected->setData(Qt::UserRole+1, resultItems.at(2));

               ui->NOTES_LIST->addItem(itemSelected);
           }
    }
}

/*** /DATABASE CONNECTION ***/

void MainWindow::processData(QString searchString) {

    //!*qDebug() << "32";
    ui->SR_RESULT->clear();
    //qDebug() << "prdata";
//    QStringList requestString;
    /*requestString.append(ui->SELECT_UPDATE->currentText()+" "+ui->TEXT_SELECT->text());
    requestString.append("WHERE "+ui->TEXT_WHERE->text());
    requestString.append(ui->LIKE_EQUAL->currentText()+" "+ui->TEXT_LIKE->text());*/

//    requestString.append("SELECT id,title,thumbnail");
    //requestString.append("WHERE titel");
    //requestString.append("LIKE "+searchString);
//    requestString.append("title");
//    requestString.append(searchString);

    //!*qDebug() << requestString;

//    QVector<QStringList> requestVector;
//    requestVector.append(requestString);

    //m_webSocket->sendBinaryMessage(Process_Data::serialize(requestVector));
    //m_webSocket->sendTextMessage("SELECT `id`,`title`,`thumbnail` FROM recipes WHERE `title` LIKE '%"+searchString+"%'");
//    m_webSocket->sendTextMessage(searchString);

}

void MainWindow::fillScrollArea(QVector<QStringList> result) {

    ////!*qDebug() << result.count();
    //!*qDebug() << "33";
    QString bild;


    for (int i = 0; i < result.size(); i++) {


    QListWidgetItem *itemRecipe = new QListWidgetItem();

    QStringList resultItems = result[i];

    /*itemRecipe->setData(Qt::DisplayRole, Process_Data::SELECT_DB_RETURN(result[i],"title"));
    itemRecipe->setData(Qt::UserRole + 3, Process_Data::SELECT_DB_RETURN(result[i],"id"));*/

    itemRecipe->setData(Qt::DisplayRole, resultItems.at(1));
    itemRecipe->setData(Qt::UserRole + 3, resultItems.at(0));

    //itemRecipe->setData(Qt::UserRole + 2, QString::number(i));

    ui->SR_RESULT->addItem(itemRecipe);


    /* FINALLY ADD THE ITEM TO THE LISTVIEW: */
    /*ui->SR_RESULT->addItem(item);
    ui->SR_RESULT->setItemWidget(item,l_widget);*/

    }
}

void MainWindow::clickedSlot(QString id) {
    openRecipeBool = true;
    QStringList requestString;
    requestString.append("SELECT title,text,bild,rezept,markiert");
    requestString.append("WHERE id");
    requestString.append("EQUAL "+id);

    QVector<QStringList> requestVector;
    requestVector.append(requestString);

    m_webSocket->sendBinaryMessage(Process_Data::serialize(requestVector));
}

void MainWindow::loadRecipe(QVector<QStringList> result) {

    ////!*qDebug() << result;

    openRecipeBool = false;
    AnimationClass->TextFlash(ui->PROCESSING_LABEL,1500,-1,0);
    ui->PAGES->setCurrentWidget(ui->SCREEN_PROCESSING);
    QMediaPlayer mplayProcessing;
    mplayProcessing.setMedia(QUrl::fromUserInput("qrc:/sounds/processing.mp3"));
    mplayProcessing.play();
    extra_functions::delay(1500);


//m_webSocket->sendTextMessage("SELECT `id`,`title`,`ingredients`,`text`,`image`,`selected` FROM `recipes` WHERE `id` = "+itemData+"");
//! *

QStringList ingredients = QString(result[0].at(2)).split("\n");
QString text = result[0].at(3);
QString image64 = result[0].at(4);
QString title = result[0].at(1);

//qDebug() << result[0].at(2);

ui->RECIPE_Ingredient_list->clear();
for (int i=0; i< ingredients.size();i++) {

    ui->RECIPE_Ingredient_list->addItem(ingredients[i]);

}

ui->TITLE->setText(title.toUpper());
ui->RECIPE_TEXT->clear();
ui->RECIPE_TEXT->appendPlainText(text);

QByteArray base64Data = image64.toStdString().c_str();
QImage image;
image.loadFromData(QByteArray::fromBase64(base64Data));
// QImage resized = image->scaled(480,332,Qt::KeepAspectRatio);
if (image.loadFromData(QByteArray::fromBase64(base64Data)) == false) {
    ui->RECIPE_IMAGE->setPixmap(QPixmap::fromImage(QImage("qrc:/images/recipe_big_logo.png")));
} else {
QImage ResizedImage =
    image.scaled(480, 332, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
ui->RECIPE_IMAGE->setPixmap(QPixmap::fromImage(ResizedImage));
}


if(result[0].at(5) == "1") {
    ui->RECIPE_MARK->setProperty("Center_Color","#cc6666");
    ui->RECIPE_MARK->setText("MARKIERT");
    ui->RECIPE_MARK->setChecked(true);
} else {
    ui->RECIPE_MARK->setProperty("Center_Color","#ff9900");
    ui->RECIPE_MARK->setText("MERKEN");
    ui->RECIPE_MARK->setChecked(false);
}

/*for (int i = 0; i < result.size(); i++) {


QListWidgetItem *itemRecipe = new QListWidgetItem();

QStringList resultItems = result[i];


itemRecipe->setData(Qt::DisplayRole, resultItems.at(1));
itemRecipe->setData(Qt::UserRole + 3, resultItems.at(0));


ui->SR_RESULT->addItem(itemRecipe);



}*/

//!*

/*
QStringList list;
    list = Process_Data::SELECT_DB_RETURN(result[0],"ZUTATEN").split("\n");
    ui->RECIPE_Ingredient_list->clear();
    ui->TITLE->setText(Process_Data::SELECT_DB_RETURN(result[0],"TITLE").toUpper());
    ui->RECIPE_TEXT->clear();
    ui->RECIPE_TEXT->appendPlainText(Process_Data::SELECT_DB_RETURN(result[0],"TEXT"));
    for (int i=0; i< list.size();i++) {
        ui->RECIPE_Ingredient_list->addItem(list[i]);
    }


    ////!*qDebug() << Process_Data::SELECT_DB_RETURN(result[0],"BILD");
    ////!*qDebug() << Process_Data::SELECT_DB_RETURN(result[0],"TEXT");


        QString bild = Process_Data::SELECT_DB_RETURN(result[0],"BILD");
        QByteArray base64Data = bild.toStdString().c_str();
        QImage image;
        image.loadFromData(QByteArray::fromBase64(base64Data), "PNG");
        // QImage resized = image->scaled(480,332,Qt::KeepAspectRatio);
        if (image.loadFromData(QByteArray::fromBase64(base64Data), "PNG") == false) {
            ui->RECIPE_IMAGE->setPixmap(QPixmap::fromImage(QImage("qrc:/images/recipe_big_logo.png")));
        } else {
        QImage ResizedImage =
            image.scaled(480, 332, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->RECIPE_IMAGE->setPixmap(QPixmap::fromImage(ResizedImage));
    }
*/
    ui->PAGES->setCurrentWidget(ui->VIEW_RECIPE);
     ui->RECIPE_TABS->setCurrentWidget(ui->RECIPE_STARTSCREEN);
         AnimationClass->TextFlash(ui->PROCESSING_LABEL,1500,-1,1);
}

void MainWindow::on_pushButton_16_clicked()
{
    if (ui->RECIPE_TEXT->verticalScrollBar()->value() >= ui->RECIPE_TEXT->verticalScrollBar()->minimum()+1) {
    ui->RECIPE_TEXT->verticalScrollBar()->setValue(ui->RECIPE_TEXT->verticalScrollBar()->value() - 1);
    }
    extra_functions::delay(200);
}

void MainWindow::on_pushButton_18_clicked()
{
    ui->RECIPE_TABS->setCurrentWidget(ui->RECIPE_STARTSCREEN);
}

void MainWindow::on_RECIPE_READ_clicked()
{

    ui->RECIPE_TEXT->verticalScrollBar()->setValue(0);
    ui->RECIPE_TABS->setCurrentWidget(ui->RECIPE);

}

void MainWindow::on_pushButton_19_clicked()
{

}

void MainWindow::on_pushButton_19_pressed()
{
    if (ui->RECIPE_TEXT->verticalScrollBar()->value() == 0) {
    menuBeeper->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    menuBeeper->play();
    }

    if (ui->RECIPE_TEXT->verticalScrollBar()->value() <= ui->RECIPE_TEXT->verticalScrollBar()->maximum()-1) {
    ui->RECIPE_TEXT->verticalScrollBar()->setValue(ui->RECIPE_TEXT->verticalScrollBar()->value() + 1);
    }
    extra_functions::delay(200);
}

void MainWindow::on_ALARM_SPIN_H_Plus_clicked()
{
    int val = ui->ALARM_SPINNER_H->value()+1;

    if (val >=10 ) {
        ui->ALARM_SPINNER_H->setPrefix("");
    } else if (val<10){
        ui->ALARM_SPINNER_H->setPrefix("0");
    }
    ui->ALARM_SPINNER_H->setValue(val);

}

void MainWindow::on_ALARM_SPIN_H_Minus_clicked()
{
    int val = ui->ALARM_SPINNER_H->value()-1;

    if (val >=109 ) {
        ui->ALARM_SPINNER_H->setPrefix("");
    } else if (val<10) {
        ui->ALARM_SPINNER_H->setPrefix("0");
    }
    ui->ALARM_SPINNER_H->setValue(val);

}

void MainWindow::on_ALARM_SPIN_M_Plus_clicked()
{
    int val = ui->ALARM_SPINNER_M->value()+1;

    if (val >=10 ) {
        ui->ALARM_SPINNER_M->setPrefix("");
    } else if (val<10) {
        ui->ALARM_SPINNER_M->setPrefix("0");
    }
    ui->ALARM_SPINNER_M->setValue(val);

}

void MainWindow::on_ALARM_SPIN_M_Minus_clicked()
{
    int val = ui->ALARM_SPINNER_M->value()-1;

    if (val >= 10 ) {
        ui->ALARM_SPINNER_M->setPrefix("");
    } else{
        ui->ALARM_SPINNER_M->setPrefix("0");
    }
    ui->ALARM_SPINNER_M->setValue(val);
}

void MainWindow::on_ALARM_SPIN_S_Plus_clicked()
{
    int val = ui->ALARM_SPINNER_S->value()+1;

    if (val >= 10 ) {
        ui->ALARM_SPINNER_S->setPrefix("");
    } else if (val<10) {
        ui->ALARM_SPINNER_S->setPrefix("0");
    }
    ui->ALARM_SPINNER_S->setValue(val);
}

void MainWindow::on_ALARM_SPIN_S_Minus_clicked()
{
    int val = ui->ALARM_SPINNER_S->value()-1;

    if (val >=10 ) {
        ui->ALARM_SPINNER_S->setPrefix("");
    } else if (val<10) {
        ui->ALARM_SPINNER_S->setPrefix("0");
    }
    ui->ALARM_SPINNER_S->setValue(val);

}

void MainWindow::on_TIMER_RESET_clicked()
{
    ui->ALARM_SPINNER_H->setValue(0);
    ui->ALARM_SPINNER_M->setValue(0);
    ui->ALARM_SPINNER_S->setValue(0);
    ui->ALARM_SPINNER_H->setPrefix("0");
    ui->ALARM_SPINNER_M->setPrefix("0");
    ui->ALARM_SPINNER_S->setPrefix("0");
}

void MainWindow::on_TIMER_START_clicked()
{
    redActive=true;
    //!*qDebug() << "mainwindow" << redActive;

    if (!timer->isActive()) {
    StopTheRedAlert=false;
    ui->TIMER_NAME->setText(ui->TIMER_NAME_EDIT->text());
    //timer->start();
    //timer_countdown->start(2000);

    /*foreach (QWidget* q,all_widgets) {
        if (q->property("css_red").isValid()) {
            q->setStyleSheet(q->property("css_red").toString());
        }
    }*/
    //red_alert_class::RedAlertFlash(red_alert_widgets,all_widgets);
    timercntdown = (ui->ALARM_SPINNER_H->value()*60*60*1000) + (ui->ALARM_SPINNER_M->value()*60*1000) + (ui->ALARM_SPINNER_S->value()*1000);
    QTime t = QTime::currentTime();
    //out << t.toString("hh:mm:ss") << " -> ";
    QTime t2 = t.addSecs((ui->ALARM_SPINNER_H->value()*60*60) + (ui->ALARM_SPINNER_M->value()*60) + (ui->ALARM_SPINNER_S->value()));

    ui->TIMER_TIMEOUT_TIME->setText(t2.toString("hh:mm:ss"));
    //out << t2.toString("hh:mm:ss");
    //!*qDebug() << timercntdown;

    timer_countdown->start(1000);
    extra_functions::delay(timercntdown);
    last_widget = ui->PAGES->currentWidget();
    //extra_functions::delay(10);

    /*QMovie *redAlertAnim = new QMovie(":/images/red_alert.gif");
    ui->AW_REDALERT_LABEL->setMovie(redAlertAnim);
    ui->AW_REDALERT_LABEL->setScaledContents(true);
    redAlertAnim->start();*/
    timer->start(0);

    ui->PAGES->setCurrentWidget(ui->ALARM_WINDOW);

   foreach (QWidget* q,all_widgets) {

        /*if (q->property("css_red").isValid()) {
            q->setStyleSheet(q->property("css_red").toString());
        }*/
        if (q->property("RedAlertState").isValid()) {
            q->setProperty("RedAlertState",1);
            q->repaint();

        }

    }
    Sound_RedAlert->play();
    } else if (timer->isActive()){
        ui->TIMER_NAME->setText("Timer Aktiv!");
    }

    //red_alert_class::RedAlertFlash(red_alert_widgets,all_widgets,true);


}

void MainWindow::on_SR_DIS_RETRY_clicked()
{
    if ( m_webSocket->state() == QAbstractSocket::UnconnectedState) {
        m_webSocket->open(QUrl(url));

    }
    else if (m_webSocket->state() == QAbstractSocket::ConnectedState) {
    ui->PAGES->setCurrentWidget(ui->SEARCH_RECIPE);
    }
    extra_functions::delay(200);
    if ( m_webSocket->state() == QAbstractSocket::UnconnectedState) {
        m_webSocket->open(QUrl(url));

    }
    else if (m_webSocket->state() == QAbstractSocket::ConnectedState) {
    ui->PAGES->setCurrentWidget(ui->SEARCH_RECIPE);
    }

}

void MainWindow::on_MM_BUTTON_TIMER_clicked()
{

    //QList< QWidget *> timer_Widgets = ui->TIMER->findChildren<QWidget *>();
    //QList< QWidget *> previous_Widgets = current_widget->findChildren<QWidget *>();

    //AnimationClass->InOutAnimation(extra_functions::WidgetReturnVector(previous_Widgets,"open_animation"),120,30,1);
    ui->PAGES->setCurrentWidget(ui->TIMER);
    changeTitle(timer_title);
    //AnimationClass->InOutAnimation(extra_functions::WidgetReturnVector(timer_Widgets,"open_animation"),120,30,0);
    current_widget=ui->TIMER;
}

void MainWindow::saveSettings() {

}


void MainWindow::on_SYSTEM_SETTINGS_BUTTON_clicked()
{
    //loadSettings();
    changeTitle(settings_title);
    ui->PAGES->setCurrentWidget(ui->SETTINGS);
    ui->SETTINGS_TABS->setCurrentWidget(ui->SET_START);
}

void MainWindow::on_TIMER_STOP_clicked()
{

    if (timer_countdown->isActive()) {
        timer_countdown->stop();
        ui->TIMER_ACTIVE_HH->setText("00");
        ui->TIMER_ACTIVE_MM->setText("00");
        ui->TIMER_ACTIVE_SS->setText("00");
        ui->TIMER_TIMEOUT_TIME->setText(timer_timeout.toUpper());
    }
    StopTheRedAlert=true;
    Sound_RedAlert->stop();

}

void MainWindow::endAlert() {
    foreach (QWidget* q,all_widgets) {
        if (q->property("css_standard").isValid()) {
            q->setStyleSheet(q->property("css_standard").toString());
            if (q->property("RedAlertState").isValid()) {
                q->setProperty("RedAlertState",0);
                q->repaint();
            }
            }
        }
    }


void MainWindow::timedout()
{

    timer_countdown->stop();
    /*last_widget = ui->PAGES->currentWidget();
    extra_functions::delay(10);
    QMovie *redAlertAnim = new QMovie(":/images/red_alert.gif");
    ui->AW_REDALERT_LABEL->setMovie(redAlertAnim);
    ui->AW_REDALERT_LABEL->setScaledContents(true);
    redAlertAnim->start();
    ui->PAGES->setCurrentWidget(ui->ALARM_WINDOW);*/

    AnimationClass->RedAlertFlash();

}


void MainWindow::on_TIMER_LIST_itemClicked(QListWidgetItem *item)
{
    ui->TIMER_NAME_EDIT->setText(item->data(Qt::DisplayRole).toString());


        int hours = item->data(Qt::UserRole+1).toInt();
        int minutes = item->data(Qt::UserRole+2).toInt();
        int seconds = item->data(Qt::UserRole+3).toInt();

        if (hours >=10) {
            ui->ALARM_SPINNER_H->setPrefix("");
        } else {
            ui->ALARM_SPINNER_H->setPrefix("0");
        }

        if (minutes >=10) {
            ui->ALARM_SPINNER_M->setPrefix("");
        } else {
            ui->ALARM_SPINNER_M->setPrefix("0");
        }

        if (seconds >=10) {
            ui->ALARM_SPINNER_S->setPrefix("");
        } else {
            ui->ALARM_SPINNER_S->setPrefix("0");
        }
        ui->ALARM_SPINNER_H->setValue(hours);
        ui->ALARM_SPINNER_M->setValue(minutes);
        ui->ALARM_SPINNER_S->setValue(seconds);

}

void MainWindow::on_TIMER_SAVE_clicked()
{
   /* ui->TIMER_LIST->currentItem()->setText(QString::number(ui->ALARM_SPINNER_H->value())+":"+QString::number(ui->ALARM_SPINNER_M->value())+":"+QString::number(ui->ALARM_SPINNER_S->value())+" - " +ui->TIMER_NAME_EDIT->text());

   QString dataValue = QString::number(ui->ALARM_SPINNER_H->value())+":"+QString::number(ui->ALARM_SPINNER_M->value())+":"+QString::number(ui->ALARM_SPINNER_S->value());

    QVariant v;
    v.setValue(dataValue);

    ui->TIMER_LIST->currentItem()->setData(Qt::UserRole,v);*/


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(settingsDbFile);

    int maxval;

    if(db.open()) {


        QSqlQuery query("select max(id) from timer");
         if (query.lastError().isValid()){};
         query.exec();

        while (query.next()) {
            maxval = query.value(0).toInt();
        }
    }



    if(db.open()) {
        //qDebug()<<ui->TIMER_LIST->currentItem()->data(Qt::UserRole).toBool();
        if (maxval+1 == ui->TIMER_LIST->currentItem()->data(Qt::UserRole).toInt()) {
             QString queryString;
             queryString = "INSERT INTO timer(id,name, hh, mm, ss) VALUES ('"
                    +ui->TIMER_LIST->currentItem()->data(Qt::UserRole).toString()+"','"
                    +ui->TIMER_NAME_EDIT->text()+"','"
                    +QString::number(ui->ALARM_SPINNER_H->value())+"','"
                    +QString::number(ui->ALARM_SPINNER_M->value())+"','"
                    +QString::number(ui->ALARM_SPINNER_S->value())+"')";
             //qDebug()<< queryString;
             QSqlQuery query(queryString);

              if (query.lastError().isValid()){};
              query.exec();
        } else {
             QString queryString;
            queryString="UPDATE timer SET `name` = '"+ui->TIMER_NAME_EDIT->text()+"',"
                                         " `hh` = '"+QString::number(ui->ALARM_SPINNER_H->value())+"',"
                                         " `mm` = '"+QString::number(ui->ALARM_SPINNER_M->value())+"',"
                                         " `ss` = '"+QString::number(ui->ALARM_SPINNER_S->value())+"'"
                                         " WHERE `id`='"+ui->TIMER_LIST->currentItem()->data(Qt::UserRole).toString()+"';";
            QSqlQuery query(queryString);
             if (query.lastError().isValid()){};
             query.exec();
        }



        }
    else {};
    db.close();

    ui->TIMER_LIST->currentItem()->setData(Qt::DisplayRole,ui->TIMER_NAME_EDIT->text());
    ui->TIMER_LIST->currentItem()->setData(Qt::UserRole+1,QString::number(ui->ALARM_SPINNER_H->value()));
    ui->TIMER_LIST->currentItem()->setData(Qt::UserRole+2,QString::number(ui->ALARM_SPINNER_M->value()));
    ui->TIMER_LIST->currentItem()->setData(Qt::UserRole+3,QString::number(ui->ALARM_SPINNER_S->value()));

}

void MainWindow::on_TIMER_DELETE_clicked()
{
    //ui->TIMER_LIST->removeItemWidget(ui->TIMER_LIST->currentItem());
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(settingsDbFile);
    QString queryString = "DELETE FROM timer WHERE `id`='"+ui->TIMER_LIST->currentItem()->data(Qt::UserRole).toString()+"'";
    if(db.open()) {
         QSqlQuery query(queryString);
         if (query.lastError().isValid()){};
         query.exec();
    }

    delete(ui->TIMER_LIST->currentItem());
}

void MainWindow::on_TIMER_NEW_clicked()
{

    ui->ALARM_SPINNER_H->setValue(0);
    ui->ALARM_SPINNER_M->setValue(0);
    ui->ALARM_SPINNER_S->setValue(0);
    ui->TIMER_NAME_EDIT->setText("Neuer Timer");

    QListWidgetItem *item = new QListWidgetItem;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(settingsDbFile);
    QString queryString = "select max(id)+1 from timer";


    if(db.open()) {
        //qDebug() << queryString;
        QSqlQuery query(queryString);
         if (query.lastError().isValid()){};
         query.exec();

        while (query.next()) {
            //qDebug() << query.value(0).toString();
            item->setData(Qt::UserRole,query.value(0).toString());
        }
    }
    db.close();

         item->setData(Qt::UserRole+1,QString::number(ui->ALARM_SPINNER_H->value()));
         item->setData(Qt::UserRole+2,QString::number(ui->ALARM_SPINNER_M->value()));
         item->setData(Qt::UserRole+3,QString::number(ui->ALARM_SPINNER_S->value()));
         item->setData(Qt::DisplayRole,ui->TIMER_NAME_EDIT->text());
         ui->TIMER_LIST->addItem(item);
         ui->TIMER_LIST->setCurrentItem(item);
}

void MainWindow::countdown() {

      int Hours;
      int Minutes;
      int Seconds;
      if (timercntdown - 1000 >= 0) // If not timeout
      {
        timercntdown =
            timercntdown - 1000; // Reduce the milliseconds with 1 secod (1000)
        // Convert milliseconds to H:M:S
        Hours = timercntdown / (1000 * 60 * 60);
        Minutes = (timercntdown % (1000 * 60 * 60)) / (1000 * 60);
        Seconds = ((timercntdown % (1000 * 60 * 60)) % (1000 * 60)) / 1000;

        // Move the H:M:S to the labels
        if (Hours < 10) {
          ui->TIMER_ACTIVE_HH->setText("0" + QString::number(Hours));
        } else {
          ui->TIMER_ACTIVE_HH->setText(QString::number(Hours));
        }
        if (Minutes < 10) {
          ui->TIMER_ACTIVE_MM->setText("0" + QString::number(Minutes));
        } else {
          ui->TIMER_ACTIVE_MM->setText(QString::number(Minutes));
        }
        if (Seconds < 10) {
          ui->TIMER_ACTIVE_SS->setText("0" + QString::number(Seconds));
        } else {
          ui->TIMER_ACTIVE_SS->setText(QString::number(Seconds));
        }

        if (timercntdown <= 0) // If timeout
        {
          timer_countdown->stop(); // Stop the timer
        }
      } else {

        timer_countdown->stop(); // Stop the timer
        timer->stop();
      }

}

void MainWindow::on_TIMER_PAUSE_clicked()
{
//redActive=false;

    ui->TOP_Left_border_bottom->setProperty("RedAlertState",1);
}

void MainWindow::window_shown() {

  /*player->setMedia(QUrl::fromLocalFile(mediaDir + "processing.mp3"));
  // //!*qDebug() << mediaDir+"processing.mp3";

  player->play();*/
    ////ui->MEDIA_CONTROLS_CONTENTS->setVisible(false);
  //animations::startAnimation(start_animation_widgets,all_widgets,"start_animation_id",240,60);

  //animations::inAnimation(all_widgets,"start_animation",240,60);
  /*QList<QWidget *> startWidgets = this->findChildren<QWidget*>();
  AnimationClass->InOutAnimation(extra_functions::WidgetReturnVector(startWidgets,"start_animation_id"),240,120,0);
*/
  return;
}



void MainWindow::on_MM_BUTTON_MEDIA_clicked()
{
    //QList< QWidget *> timer_Widgets = ui->MEDIA_START->findChildren<QWidget *>();
    /*QList< QWidget *> previous_Widgets = current_widget->findChildren<QWidget *>();*/
    /*AnimationClass->InOutAnimation(extra_functions::WidgetReturnVector(previous_Widgets,"open_animation"),120,30,1);*/
    ui->PAGES->setCurrentWidget(ui->MEDIA);
    ui->MEDIA_TABS->setCurrentWidget(ui->MEDIA_START);
    changeTitle(media_title);
    //AnimationClass->InOutAnimation(extra_functions::WidgetReturnVector(timer_Widgets,"open_animation"),120,30,0);
    //current_widget=ui->MEDIA;
}

void MainWindow::on_pushButton_2_clicked()
{
    //ui->PAGES->setCurrentWidget(ui->COLORPAGE);
    //ui->PAGES->setCurrentWidget(ui->TESTPAGE);
    //ui->PAGES->setCurrentWidget(ui->INTERNET);
    //ui->PAGES->setCurrentWidget();
    //ui->VideoWindow_ControlBar->setVisible(false);
    //ui->controls->setVisible(true);
}

/*void MainWindow::on_SET_AVAIL_LANG_LEFT_SLIDER_clicked()
{
    if (ui->SET_AVAIL_LANG_COMBO->currentIndex()-1 >=0) {

        ui->SET_AVAIL_LANG_COMBO->setCurrentIndex(ui->SET_AVAIL_LANG_COMBO->currentIndex()-1);

        //loadSettings(ui->SET_AVAIL_LANG_COMBO->itemData(ui->SET_AVAIL_LANG_COMBO->currentIndex()).toString());
        currentLanguage = ui->SET_AVAIL_LANG_COMBO->itemData(ui->SET_AVAIL_LANG_COMBO->currentIndex()).toString();
        loadSettings(currentLanguage);
    }
}

void MainWindow::on_SET_AVAIL_LANG_RIGHT_SLIDER_clicked()
{
    if (ui->SET_AVAIL_LANG_COMBO->currentIndex()+1 < ui->SET_AVAIL_LANG_COMBO->count()) {
        ui->SET_AVAIL_LANG_COMBO->setCurrentIndex(ui->SET_AVAIL_LANG_COMBO->currentIndex()+1);

        //loadSettings(ui->SET_AVAIL_LANG_COMBO->itemData(ui->SET_AVAIL_LANG_COMBO->currentIndex()).toString());
        currentLanguage = ui->SET_AVAIL_LANG_COMBO->itemData(ui->SET_AVAIL_LANG_COMBO->currentIndex()).toString();
        loadSettings(currentLanguage);
    }
}
*/

void MainWindow::on_SET_AVAIL_LANG_SAVE_clicked()
{

}


void MainWindow::on_SETTINGS_BUTTON_LANGUAGE_clicked()
{
    changeTitle(settings_language);
    ui->SETTINGS_TABS->setCurrentWidget(ui->SET_LANGUAGE);
}

void MainWindow::on_SETTINGS_BUTTON_AUDIO_clicked()
{
    changeTitle(audio_title);
    ui->SETTINGS_TABS->setCurrentWidget(ui->AUDIO);
}

void MainWindow::on_SETTINGS_BUTTON_ADVANCED_clicked()
{
    //ui->SETTINGS_TABS->setCurrentWidget();
}

void MainWindow::on_SETTINGS_BUTTON_NETWORK_clicked()
{
    changeTitle(network_title);

    ui->SETTINGS_TABS->setCurrentWidget(ui->SET_NETWORK);

    ui->NETWORK_TABLE->clearContents();
    ui->LABEL_HOSTNAME->setText(QHostInfo::localHostName());

    QList<QNetworkInterface> allInterfaces = QNetworkInterface::allInterfaces();
        foreach(QNetworkInterface iFace, allInterfaces) {
            foreach (const QHostAddress &address, iFace.allAddresses()) {
                        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))

                            //qDebug() << "name:" << iFace.humanReadableName() << "hw:" << iFace.hardwareAddress() << "all: " <<iFace.allAddresses()<< "up:" <<iFace.IsUp;
                             //qDebug() << "add:" <<  address.toString();

                            ui->NETWORK_TABLE->insertRow(iFace.index()-1);
                            ui->NETWORK_TABLE->setItem(iFace.index()-1,0,new QTableWidgetItem(iFace.humanReadableName()));
                            ui->NETWORK_TABLE->setItem(iFace.index()-1,1,new QTableWidgetItem(iFace.allAddresses().at(2).toString()));
                            ui->NETWORK_TABLE->setItem(iFace.index()-1,2,new QTableWidgetItem(iFace.hardwareAddress()));

                            //qDebug() <<"index" << iFace.index() << "name:" << iFace.humanReadableName() << "hw:" << iFace.hardwareAddress() <<"add:" <<  address.toString() << "up:" << iFace.IsUp;

                    }
        }
}

void MainWindow::on_SETTINGS_BUTTON_DISPLAY_OFF_clicked()
{
    //ui->SETTINGS_TABS->setCurrentWidget();

    system(qPrintable(rootCommand+ " xset dpms force off"));
}

void MainWindow::on_pushButton_69_clicked()
{
    changeTitle(eq_title);
    ui->SETTINGS_TABS->setCurrentWidget(ui->EQUALIZER);
}


void MainWindow::on_MEDIA_BUTTON_VIDEO_PLAYER_clicked()
{
    changeTitle(video_player_title);
    ui->MEDIA_TABS->setCurrentWidget(ui->VIDEO);
    ui->VIDEO_TABS->setCurrentWidget(ui->VIDEO_PLAYER);
    //ui->VIDEO_TABS->setCurrentIndex(1);
    //qDebug() << "ci" << ui->VIDEO_TABS->currentIndex();
}

void MainWindow::on_MEDIA_BUTTON_MUSIC_PLAYER_clicked()
{
    changeTitle(music_player_title);
    ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
    ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_PLAYER);
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

void MainWindow::processBuffer(const QAudioBuffer& buffer)
{
    ////!*qDebug() << buffer.format().channelCount();
    /*
    if (m_audioLevels.count() != buffer.format().channelCount()) {
        qDeleteAll(m_audioLevels);
        m_audioLevels.clear();
        for (int i = 0; i < buffer.format().channelCount(); ++i) {
            AudioLevel *level = new AudioLevel(ui->centralwidget);
            m_audioLevels.append(level);
            ui->levelsLayout->addWidget(level);
        }
    }*/

    QVector<qreal> levels = getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i) {
        ////!*qDebug() << levels.count();
        ////!*qDebug() << int(levels.at(i)*100);
        ui->MUSIC_PLAYER_LEFT_VIS->setValue(int(levels.at(0)*100));
        //m_audioLevels.at(i)->setLevel(levels.at(i));
        ui->MUSIC_PLAYER_RIGHT_VIS->setValue(int(levels.at(1)*100));
    }
}

void MainWindow::on_MC_BUTTON_PAUSE_2_clicked()
{

}


void MainWindow::on_pushButton_3_clicked()
{
    /*ui->pushButton_3->setText("");

    ui->pushButton_3->setCheckable(true);
    ui->pushButton_3->setStyleSheet("* {"

                                    "color:#000000;"
                                    "}"
                                    "QPushButton {"
                                    "background:#ff9900;"
                                    "color:#000000;"
                                    "font-family:\"Helvetica Ultra Compressed\";"
                                    "height:35px;"
                                    "border-top-left-radius:17px;"
                                    "border-bottom-left-radius:17px;}"
                                    "QPushButton:pressed {"
                                    "background:#ffffff;"
                                    "}"
                                    "QPushButton:checked {"
                                    "background:#9999cc;"
                                    "}"
                                    "QPushButton:checked:pressed {"
                                    "background:#ffffff;"
                                    "}");

     QHBoxLayout *l_widgetLayout = new QHBoxLayout();
     QLabel *label1 = new QLabel();
     QLabel *label2 = new QLabel();
     label1->setObjectName("ListButton1");
     label1->setStyleSheet("QLabel#ListButton1 {"
                           "font-family:\"Helvetica Ultra Compressed\";"
                           "font-size:30px;"
                           "color:#ffffff;"
                           "border-radius:0px;"
                           "padding-left:5px;"
                           "padding-right:5px;"
                           "font-style:normal;"
                           "background:#000000;"
                           "text-align:left;"
                           "}");
     label1->setFixedHeight(35);
     label1->setMinimumWidth(50);
     label1->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

     label2->setObjectName("ListButton2");
     label2->setStyleSheet("QLabel#ListButton2 {"
                           "background:transparent;"
                           "font-family:\"Helvetica Ultra Compressed\";"
                           "font-size:25px;"
                           "color:#000000;"
                           "border-radius:0px;"
                           "font-style:normal;"
                           "text-align:left;"
                           "}");
     label2->setFixedHeight(35);
     label2->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);

     label1->setText("40");
     label2->setText("IRGEND EIN TEXT");

     l_widgetLayout->setContentsMargins(30,0,0,0);
     l_widgetLayout->setSpacing(5);
     l_widgetLayout->addWidget(label1);
     l_widgetLayout->addWidget(label2);
     ui->pushButton_3->setLayout(l_widgetLayout);*/
}

void MainWindow::on_pushButton_4_clicked()
{
}

void MainWindow::on_MEDIA_BUTTON_MUSIC_FILES_clicked()
{
    changeTitle(musicfiles_title);
//    listDirMusic("/home/ankalagon/Laufwerke/Musik/");
    ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
    ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_FILES);

}


void MainWindow::listDirMusic(QString dirString) {
  ui->MUSIC_DATABASE_LIST->clear();
  ui->MUSIC_DATABASE_LIST->scrollToTop();
  QDir dir(dirString);
  ////!*qDebug() << dir;
  //QDir dir(MusikPath);
  // //!*qDebug() << MusikPath;
  int i = 0;
  int cnt_list = int(log10(dir.entryList().count()) + 1);
  /*QString MusikAdressBarLabel = MusikPath;
  MusikAdressBarLabel.replace(homepath,"~");
  MusikAdressBarLabel.replace("Laufwerke/","DRIVE:");
  MusikAdressBarLabel.replace("/../","");
  ui->label_musik_adressbar->setText(MusikAdressBarLabel.toUpper());*/






  {
      QFont helvetica_Spaced("Helvetica Ultra Compressed",28);
      helvetica_Spaced.setLetterSpacing(QFont::PercentageSpacing,120);

      /*****************************************************************/
      //for (int i = 0; i < 20; i++) {

      QString cnt_String;

      foreach (QFileInfo Fileitem, dir.entryInfoList()) {

        cnt_String = QString("%1").arg(i, cnt_list, 10, QChar('0'));



      /*** LET'S CREATE THE LISTWIDGET ITEMS ***************************
       *
       *   THE ITEM IS MADE UP OF:
       *   <ITEM>
       *     <WIDGET>
       *       <HORIZONTAL LAYOUT>
       *          <CORNER WIDGET> <IMAGE LABEL> <PUSH BUTTON>
       *       </HORIZONTAL LAYOUT>
       *     <WIDGET>
       *   </ITEM>
       *
       ****************************************************************/

      /* FIRST DECLARE THE ITEM ITSELF WITH A FIXED HEIGHT: */
/*      QListWidgetItem *item = new QListWidgetItem();
      item->setSizeHint(QSize(0,45));

      QPushButton *bushButton1 = new QPushButton();
      bushButton1->setFixedHeight(45);
      bushButton1->setCheckable(true);
      bushButton1->setStyleSheet("* {"

                                      "color:#000000;"
                                      "}"
                                      "QPushButton {"
                                      "background:#ff9900;"
                                      "color:#000000;"
                                      "font-family:\"Helvetica Ultra Compressed\";"

                                      "border-top-left-radius:17px;"
                                      "border-bottom-left-radius:17px;"
                                      "margin-bottom:10px;"
                                      "}"
                                      "QPushButton:pressed {"
                                      "background:#ffffff;"
                                      "}"
                                      "QPushButton:checked {"
                                      "background:#9999cc;"
                                      "}"
                                      "QPushButton:checked:pressed {"
                                      "background:#ffffff;"
                                      "}");

       QHBoxLayout *l_widgetLayout_1 = new QHBoxLayout();
       QLabel *label1 = new QLabel();
       QLabel *label2 = new QLabel();
       label1->setObjectName("ListButton1");
       label1->setStyleSheet("QLabel#ListButton1 {"
                             "font-family:\"Helvetica Ultra Compressed\";"
                             "font-size:30px;"
                             "color:#ffffff;"
                             "border-radius:0px;"
                             "padding-left:5px;"
                             "padding-right:5px;"
                             "font-style:normal;"
                             "background:#000000;"
                             "text-align:left;"
                             "}");
       label1->setFixedHeight(35);
       label1->setMinimumWidth(50);
       label1->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

       label2->setObjectName("ListButton2");
       label2->setStyleSheet("QLabel#ListButton2 {"
                             "background:transparent;"
                             "font-family:\"Helvetica Ultra Compressed\";"
                             "font-size:25px;"
                             "color:#000000;"
                             "border-radius:0px;"
                             "font-style:normal;"
                             "text-align:left;"
                             "}");
       label2->setFixedHeight(35);
       label2->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
*/

       /*QListWidgetItem *eintrag = new QListWidgetItem;
       QString vollerPfad(item.absoluteFilePath());
       QVariant vollerPfadDaten(vollerPfad);

       eintrag->setData(Qt::UserRole, vollerPfadDaten);
       eintrag->setText(cnt_String +".   "+ Fileitem.fileName().toUpper());*/


/*
       label1->setText(QString::number(i));
       label2->setText(Fileitem.fileName().toUpper());
       item->setWhatsThis(Fileitem.filePath());

       l_widgetLayout_1->setContentsMargins(30,0,0,10);
       l_widgetLayout_1->setSpacing(5);
       l_widgetLayout_1->addWidget(label1);
       l_widgetLayout_1->addWidget(label2);
       bushButton1->setLayout(l_widgetLayout_1);
        bushButton1->setWhatsThis(Fileitem.filePath());

        connect(bushButton1,
                    &QPushButton::clicked,
                    [=]()
                    {
                    //!*qDebug() << Fileitem.filePath();
                        if (Fileitem.isDir()) {
                            listDirMusic(Fileitem.filePath());
                        } else if (Fileitem.isFile()) {
                            //!*qDebug() << "file:" << Fileitem.filePath();
                            MusicPlaylistEntries.append(Fileitem.filePath());
                        }

                        ////!*qDebug() << ui->MUSIC_DATABASE_LIST->currentItem()->whatsThis();

                    });
*/
      /* FINALLY ADD THE ITEM TO THE LISTVIEW: */
/*
      ui->MUSIC_DATABASE_LIST->addItem(item);
      ui->MUSIC_DATABASE_LIST->setItemWidget(item,bushButton1);
*/

      QListWidgetItem *item = new QListWidgetItem();
      item->setData(Qt::DisplayRole, Fileitem.fileName().toUpper());
      item->setData(Qt::UserRole + 1, QString::number(i));
      item->setData(Qt::UserRole + 3, Fileitem.filePath());
      ui->MUSIC_DATABASE_LIST->addItem(item);


      i++;
      }

  }

















/*

  QString cnt_String;

  foreach (QFileInfo item, dir.entryInfoList()) {

    cnt_String = QString("%1").arg(i, cnt_list, 10, QChar('0'));

    QListWidgetItem *eintrag = new QListWidgetItem;
    QString vollerPfad(item.absoluteFilePath());
    QVariant vollerPfadDaten(vollerPfad);

    eintrag->setData(Qt::UserRole, vollerPfadDaten);
    eintrag->setText(cnt_String +".   "+ item.fileName().toUpper());

    // //!*qDebug() << eintrag->data(Qt::UserRole);

    ui->MUSIC_DATABASE_LIST->insertItem(i, eintrag);

    i++;
  }*/
  /*ui->sliderMusikExplorer_4->setValue(0);
  ui->sliderMusikExplorer_4->setMaximum(ui->MUSIC_DATABASE_LIST->count() - 7);
  ui->MUSIC_DATABASE_LIST->verticalScrollBar()->setValue(0);*/


}

void MainWindow::on_MUSIC_FILES_BACK_clicked()
{
    //!*qDebug() << "moooooooooaaaaaaaaaaaaaaaaaaaaaaaaan";
    {
        //!*qDebug() << "rofl";
        QFont helvetica_Spaced("Helvetica Ultra Compressed",28);
        helvetica_Spaced.setLetterSpacing(QFont::PercentageSpacing,120);

        /*****************************************************************/
        for (int i = 0; i < 20; i++) {



        /*** LET'S CREATE THE LISTWIDGET ITEMS ***************************
         *
         *   THE ITEM IS MADE UP OF:
         *   <ITEM>
         *     <WIDGET>
         *       <HORIZONTAL LAYOUT>
         *          <CORNER WIDGET> <IMAGE LABEL> <PUSH BUTTON>
         *       </HORIZONTAL LAYOUT>
         *     <WIDGET>
         *   </ITEM>
         *
         ****************************************************************/

        /* FIRST DECLARE THE ITEM ITSELF WITH A FIXED HEIGHT: */
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(0,45));

        QPushButton *bushButton1 = new QPushButton();
        bushButton1->setFixedHeight(45);
        bushButton1->setCheckable(true);
        bushButton1->setStyleSheet("* {"

                                        "color:#000000;"
                                        "}"
                                        "QPushButton {"
                                        "background:#ff9900;"
                                        "color:#000000;"
                                        "font-family:\"Helvetica Ultra Compressed\";"

                                        "border-top-left-radius:17px;"
                                        "border-bottom-left-radius:17px;"
                                        "margin-bottom:10px;"
                                        "}"
                                        "QPushButton:pressed {"
                                        "background:#ffffff;"
                                        "}"
                                        "QPushButton:checked {"
                                        "background:#9999cc;"
                                        "}"
                                        "QPushButton:checked:pressed {"
                                        "background:#ffffff;"
                                        "}");

         QHBoxLayout *l_widgetLayout_1 = new QHBoxLayout();
         QLabel *label1 = new QLabel();
         QLabel *label2 = new QLabel();
         label1->setObjectName("ListButton1");
         label1->setStyleSheet("QLabel#ListButton1 {"
                               "font-family:\"Helvetica Ultra Compressed\";"
                               "font-size:30px;"
                               "color:#ffffff;"
                               "border-radius:0px;"
                               "padding-left:5px;"
                               "padding-right:5px;"
                               "font-style:normal;"
                               "background:#000000;"
                               "text-align:left;"
                               "}");
         label1->setFixedHeight(35);
         label1->setMinimumWidth(50);
         label1->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

         label2->setObjectName("ListButton2");
         label2->setStyleSheet("QLabel#ListButton2 {"
                               "background:transparent;"
                               "font-family:\"Helvetica Ultra Compressed\";"
                               "font-size:25px;"
                               "color:#000000;"
                               "border-radius:0px;"
                               "font-style:normal;"
                               "text-align:left;"
                               "}");
         label2->setFixedHeight(35);
         label2->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);

         label1->setText(QString::number(i*100));
         label2->setText("ZEILE NUMMER: "+QString::number(i));

         l_widgetLayout_1->setContentsMargins(30,0,0,10);
         l_widgetLayout_1->setSpacing(5);
         l_widgetLayout_1->addWidget(label1);
         l_widgetLayout_1->addWidget(label2);
         bushButton1->setLayout(l_widgetLayout_1);



        /* FINALLY ADD THE ITEM TO THE LISTVIEW: */

        ui->MUSIC_DATABASE_LIST->addItem(item);
        ui->MUSIC_DATABASE_LIST->setItemWidget(item,bushButton1);
        }

    }
}

void MainWindow::on_MUSIC_FILES_OKAYER_clicked()
{
    //!*qDebug() << "bla";
}


void MainWindow::on_pushButton_44_clicked()
{
    /*player->setMedia(QUrl::fromLocalFile(mediaDir + "computerbeep_15.mp3"));
    player->play();*/
    /*if (devmode == true) {
        this->close();
    } else {*/
    system("su pi -c \"xset dpms force off\"");
    system("sudo shutdown -P now");
      //}

}

void MainWindow::on_MC_BUTTON_HIDE_clicked()
{
    ui->MEDIA_CONTROLS_CONTENTS->setVisible(false);
    //animations::startAnimation(start_animation_widgets,all_widgets,"start_animation_id",240,120);
}


void MainWindow::showEvent(QShowEvent *event) {

  QMainWindow::showEvent(event);
  //QTimer::singleShot(120, this, SLOT(window_shown()));

  return;
}

void MainWindow::on_MEDIA_BUTTON_MUSIC_DEACTIVATED_clicked()
{
    //animations::closeAnimation(start_animation_widgets,all_widgets,"open_animation",240,30);
}

void MainWindow::on_TIMER_LIST_NEXT_clicked()
{
    //+++++++++++++++++++QList< QWidget *> timer_Widgets = ui->TIMER->findChildren<QWidget *>();
    //QList< QWidget *> previous_Widgets = current_widget->findChildren<QWidget *>();
    //++++++++++++++++++++++AnimationClass->InOutAnimation(extra_functions::WidgetReturnVector(timer_Widgets,"open_animation"),240,120,1);

    /*ui->PAGES->setCurrentWidget(ui->TIMER);
    animations::InOutAnimation(extra_functions::WidgetReturnVector(timer_Widgets,"open_animation"),240,120,0);
    current_widget=ui->TIMER;*/

    if (ui->TIMER_LIST->verticalScrollBar()->value() <= ui->TIMER_LIST->verticalScrollBar()->maximum()-1) {
    ui->TIMER_LIST->verticalScrollBar()->setValue(ui->TIMER_LIST->verticalScrollBar()->value() + 1);
    }
}

void MainWindow::on_pushButton_81_clicked()
{

}

void MainWindow::on_VIDEO_BUTTON_CONTROLS_clicked()
{
    switch (ui->VidControls->currentIndex()) {
    case 0:
        ui->VidControls->setCurrentIndex(ui->VidControls->currentIndex()+1);
        break;
    case 1:
        ui->VidControls->setCurrentIndex(ui->VidControls->currentIndex()+1);
        break;
    case 2:
        ui->VidControls->setCurrentIndex(ui->VidControls->currentIndex()-2);
        break;
    default:
        ui->VidControls->setCurrentIndex(ui->VidControls->currentIndex()+1);
        break;
    }

}


void MainWindow::on_MEDIA_BUTTON_VIDEOS_DEACTIVATED_clicked()
{

}

void MainWindow::on_MC_BUTTON_M_PLAYER_clicked()
{
    changeTitle(music_player_title);
    ui->PAGES->setCurrentWidget(ui->MEDIA);
    ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
    ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_PLAYER);
}

void MainWindow::on_MUSIC_PLAYER_FILES_clicked()
{
    changeTitle(musicfiles_title);
    ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
    ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_FILES);
}

void MainWindow::on_MUSIC_FILES_PLAY_clicked()
{
    MusicPlayerClass->playMusic(&MusicPlaylistEntries);
    MusicPlaylistEntries.clear();
}

void MainWindow::on_MUSIC_PLAYER_NEXT_clicked()
{
    //MusicPlayerClass->MusicPlayer.playlist()->next();
}

void MainWindow::on_MUSIC_PLAYER_PREV_clicked()
{
    //MusicPlayerClass->MusicPlayer.playlist()->previous();
}

void MainWindow::on_MUSIC_PLAYER_LOOP_toggled(bool checked)
{
    if (checked == true) {
        MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::Loop);
    } else {
        MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::Sequential);
    }

}


void MainWindow::on_MUSIC_PLAYER_LOOP_stateChanged(int arg1)
{
    //!*qDebug() << arg1;

    switch (arg1) {
    case 0:
        ui->MUSIC_PLAYER_LOOP->setText("NO LOOP");
        MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::Sequential);
        break;
    case 1:
        ui->MUSIC_PLAYER_LOOP->setText("LOOP ONE");
        MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;
    case 2:
        ui->MUSIC_PLAYER_LOOP->setText("LOOP ALL");
        MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    default:
        ui->MUSIC_PLAYER_LOOP->setText("NO LOOP");
        MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::Sequential);
        break;
    }
}

void MainWindow::on_MUSIC_PLAYER_PLAYLIST_stateChanged(int arg1)
{
    switch (arg1) {
    case 0:
        ui->MUSIC_PLAYER_TABS->setCurrentWidget(ui->MUSIC_PLAYER_DETAILS_TAB);
        ui->MUSIC_PLAYER_PLAYLIST->setText("PLAYLIST");
        break;
    case 1:
        ui->MUSIC_PLAYER_TABS->setCurrentWidget(ui->MUSIC_PLAYER_PLAYLIST_TAB);
        //ui->MUSIC_PLAYER_PLAYLIST->setText("VIS");
        ui->MUSIC_PLAYER_PLAYLIST->setText("DETAILS");
        break;
    /*case 2:
        ui->MUSIC_PLAYER_TABS->setCurrentWidget(ui->MUSIC_PLAYER_VIS_TAB);
        ui->MUSIC_PLAYER_PLAYLIST->setText("DETAILS");
        break;*/
    default:
        ui->MUSIC_PLAYER_TABS->setCurrentWidget(ui->MUSIC_PLAYER_DETAILS_TAB);
        ui->MUSIC_PLAYER_PLAYLIST->setText("PLAYLIST");
        break;
    }
}




void MainWindow::on_MUSIC_FILES_OKAYER_3_clicked()
{

    int start = ui->MUSIC_PLAYLIST->currentIndex().row();
    //int end = ui->MUSIC_PLAYLIST->currentIndex();
    MusicPlayerClass->musicPlaylist->removeMedia(start);

    delete ui->MUSIC_PLAYLIST->currentItem();
}

void MainWindow::on_MUSIC_DATABASE_LIST_itemClicked(QListWidgetItem *item)
{
    QFileInfo Fileitem;
    Fileitem.setFile(item->data(Qt::UserRole+3).toString());

    if (Fileitem.isDir()) {
        listDirMusic(Fileitem.filePath());
    } else if (Fileitem.isFile()) {
        //!*qDebug() << "file:" << Fileitem.filePath();
        MusicPlaylistEntries.append(Fileitem.filePath());
    }


    /*//!*qDebug() << item->data(Qt::UserRole+3).toString();
    listDirMusic(item->data(Qt::UserRole+3).toString());*/

}

void MainWindow::on_MUSIC_FILES_OKAYER_4_clicked()
{

    MusicPlayerClass->musicPlaylist->addMedia(QUrl::fromUserInput("www.schwarze-welle.de:7500"));

    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::DisplayRole, /*ui->MUSIC_RADIO_LIST->itemAt(0)->data(Qt::DisplayRole)*/"Radio Schwarze Welle");
    item->setData(Qt::UserRole + 1, QString::number(MusicPlayerClass->musicPlaylist->mediaCount()));
    item->setData(Qt::UserRole + 3, QUrl::fromUserInput("www.schwarze-welle.de:7500").toString());

    /*
     *     item->setData(Qt::DisplayRole, ui->MUSIC_RADIO_LIST->currentItem()->data(Qt::DisplayRole));
    item->setData(Qt::UserRole + 1, QString::number(MusicPlayerClass->musicPlaylist->mediaCount()));
    item->setData(Qt::UserRole + 3, QUrl::fromUserInput("www.schwarze-welle.de:7500").toString());
    */
    ui->MUSIC_PLAYLIST->addItem(item);
}

void MainWindow::on_MEDIA_BUTTON_VIDEO_FILES_clicked()
{
    changeTitle(videofiles_title);
    ui->MEDIA_TABS->setCurrentWidget(ui->VIDEO);
    ui->VIDEO_TABS->setCurrentWidget(ui->VIDEO_FILE_CHOOSER);
}

void MainWindow::on_SR_RESULT_itemClicked(QListWidgetItem *item)
{

        openRecipeBool = true;
        QStringList requestString;
        QString itemData = item->data(Qt::UserRole+3).toString();
            ////!*qDebug() << itemData;
        requestString.append("SELECT id,title,zutaten,text,bild,markiert");
        requestString.append("id");
        requestString.append(itemData);

        QVector<QStringList> requestVector;
        requestVector.append(requestString);

        //m_webSocket->sendBinaryMessage(Process_Data::serialize(requestVector));

        m_webSocket->sendTextMessage("RECIPE::SELECT `id`,`title`,`ingredients`,`text`,`image`,`selected` FROM `recipes` WHERE `id` = "+itemData+"");
        recipeID = itemData;
        requestString.clear();
        requestVector.clear();


}

void MainWindow::on_VIDEO_FILES_PLAY_clicked()
{
    ui->VIDEO_TABS->setCurrentWidget(ui->VIDEO_PLAYER);
}

void MainWindow::on_MC_BUTTON_V_PLAYER_clicked()
{
    changeTitle(video_player_title);
    ui->PAGES->setCurrentWidget(ui->MEDIA);
    ui->MEDIA_TABS->setCurrentWidget(ui->VIDEO);
    ui->VIDEO_TABS->setCurrentWidget(ui->VIDEO_PLAYER);
}
/*
void MainWindow::on_pushButton_45_clicked()
{
    player->setMedia(QUrl::fromLocalFile(mediaDir + "computerbeep_15.mp3"));
    player->play();
    if (devmode==false) {
        system("sudo shutdown -r now");
    }
}
*/

void MainWindow::on_RECIPE_Ingredients_next_clicked()
{
    if (ui->RECIPE_Ingredient_list->verticalScrollBar()->value() <= ui->RECIPE_Ingredient_list->verticalScrollBar()->maximum()-1) {
    ui->RECIPE_Ingredient_list->verticalScrollBar()->setValue(ui->RECIPE_Ingredient_list->verticalScrollBar()->value() + 1);
    }
}

void MainWindow::on_RECIPE_Ingredients_back_clicked()
{
    if (ui->RECIPE_Ingredient_list->verticalScrollBar()->value() >= ui->RECIPE_Ingredient_list->verticalScrollBar()->minimum()+1) {
    ui->RECIPE_Ingredient_list->verticalScrollBar()->setValue(ui->RECIPE_Ingredient_list->verticalScrollBar()->value() - 1);
    }
}


void MainWindow::on_controls_down_clicked()
{
    ui->VideoWindow_ControlBar->setVisible(false);
    ui->controls->setVisible(true);
}

void MainWindow::on_controls_butt_clicked()
{
    ui->VideoWindow_ControlBar->setVisible(true);
    ui->controls->setVisible(false);
}

void MainWindow::on_VIDEO_BUTTON_BACK_clicked()
{
    /*videoform->setAttribute(Qt::WA_DeleteOnClose);
    videoform->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    //vid_full->showFullScreen();
    videoform->showFullScreen();
    ui->VIDEO_STOP->toggle();*/



    /*rippedOffTab->setWindowFlags(Qt::Widget);
    //VIDEO_PLAYER->close();


    //rippedOffTab->setObjectName("VIDEO_PLAYER");
    //ui->VIDEO_TABS->setCurrentWidget(ui->VIDEO_PLAYER);

    //ui->VIDEO_TABS->addTab(rippedOffTab,"VPLAYER");
    ui->VIDEO_TABS->insertTab(ui->VIDEO_TABS->count()-1,rippedOffTab,"VPLAYER");

    ui->MEDIA_TABS->setCurrentWidget(ui->VIDEO);
    ui->MEDIA_TABS->setCurrentWidget(ui->VIDEO);
    ui->MEDIA_TABS->setCurrentWidget(ui->VIDEO);

    ui->VIDEO_TABS->setCurrentIndex(ui->VIDEO_TABS->count()-1);
    ui->VIDEO_TABS->setCurrentIndex(ui->VIDEO_TABS->count()-1);
    ui->VIDEO_TABS->setCurrentIndex(ui->VIDEO_TABS->count()-1);

    qDebug() << "ftcnt" << ui->VIDEO_TABS->count();
    qDebug() << "fs" << rippedOffTab->objectName();
    qDebug() << "fs2" << ui->MEDIA_TABS->currentWidget()->objectName();*/

    ui->VIDEO_TABS->addTab(rippedOffTab,"tab2");
    rippedOffTab->setWindowFlags(Qt::Widget);
    rippedOffTab->setMinimumHeight(0);
    rippedOffTab->setMinimumWidth(0);
    ui->VIDEO_TABS->setCurrentWidget(rippedOffTab);

}

void MainWindow::on_VIDEO_MAXIMIZE_2_clicked()
{

}

void MainWindow::on_SYSTEM_SERVER_STATUS_clicked()
{
    changeTitle(server_status_title);
    ui->PAGES->setCurrentWidget(ui->SERVER_STATUS);
}

void MainWindow::on_LCARS_PushButton_clicked()
{
    //qDebug() << serverBin.toStdString().c_str();

    QProcess::startDetached(serverBin.toStdString().c_str());
}

void MainWindow::on_ALARM_VOLUME_SLIDER_sliderReleased()
{

}

void MainWindow::on_ALARM_VOLUME_SLIDER_valueChanged(int value)
{
    Sound_RedAlert->setVolume(value);
}

void MainWindow::on_CHANGE_COLORS_clicked()
{

    QStringList colorlist;
    colorlist.append("#5b8ffe");
    colorlist.append("#6688cc");
    colorlist.append("#bb4411");
    colorlist.append("#ffffff");
    colorlist.append("#ffcc66");

    foreach (QWidget *q, all_widgets) {
        if (q->property("Color_Theme_ID").isValid() || q->property("ColorThemeID").isValid()) {
            q->setProperty("Center_Color",colorlist[q->property("Color_Theme_ID").toInt()]);
            q->setProperty("Edge_Left_Color",colorlist[q->property("Color_Theme_ID").toInt()]);
            q->setProperty("Edge_Right_Color",colorlist[q->property("Color_Theme_ID").toInt()]);
            if (q->property("bgColor").isValid()) {
                q->setProperty("bgColor",colorlist[q->property("Color_Theme_ID").toInt()]);
            }
            q->repaint();
        }


    }
}

void MainWindow::on_LCARS_PushButton_3_clicked()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->LCARS_Lines_12, "maximumWidth");
    animation->setDuration(ui->LCARS_Lines_12->width()
                           );
    animation->setStartValue(ui->LCARS_Lines_12->maximumWidth());
    animation->setEndValue(0);

    // to slide in call
    animation->start();

    // to slide in call widget_2
    animation->start();
    /*QPropertyAnimation *animation2 = new QPropertyAnimation(ui->LCARS_COLOR_TAB, "maximumWidth");
    animation2->setDuration(300
                           );
    animation2->setStartValue(0);
    animation2->setEndValue(300);

    // to slide in call widget_2
    animation2->start();*/
    //!*qDebug() << "in";
}

void MainWindow::on_LCARS_PushButton_4_clicked()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->LCARS_Lines_12, "maximumWidth");
    animation->setDuration(ui->LCARS_Lines_12->width()
                           );
    animation->setStartValue(0);
    animation->setEndValue(300);

    // to slide in call
    animation->start();

    // to slide in call widget_2
    animation->start();
    /*QPropertyAnimation *animation2 = new QPropertyAnimation(ui->LCARS_COLOR_TAB, "maximumWidth");
    animation2->setDuration(300
                           );
    animation2->setStartValue(ui->LCARS_COLOR_TAB->maximumWidth());
    animation2->setEndValue(0);

    // to slide in call widget_2
    animation2->start();*/
}

void MainWindow::on_ALARM_VOLUME_SLIDER_sliderMoved(int position)
{
    Q_UNUSED(position);
    //!*qDebug() << position;
}


void MainWindow::on_MEDIA_BUTTON_MUSIC_DATABASE_clicked()
{
    changeTitle(musicdb_folder);

    ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
    ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_DATABASE);

    ui->MUSIC_DATABASE_TABS->setCurrentWidget(ui->MUSIC_DB_FOLDERS);
    ui->MUSIC_DATABASE_LIST_FOLDERS->clear();
    /*ui->folders->clear();
    ui->artists->clear();
    ui->albums->clear();
    ui->titles->clear();*/


    ui->TITLE->setText("MUSIK • ORDNER");
    QVector<QListWidgetItem> folderItems = musicDB_control->getFolderList();
    int i=0;
    foreach (QListWidgetItem itemIterator, folderItems) {
        //!*qDebug() << itemIterator.text() << itemIterator.data(Qt::UserRole).toString() << itemIterator.data(Qt::UserRole+1).toString();
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(itemIterator.text());

        newItem->setData(Qt::UserRole-1,i);
        newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
        newItem->setData(Qt::UserRole+1,itemIterator.data(Qt::UserRole+1));
        i++;
        ui->MUSIC_DATABASE_LIST_FOLDERS->addItem(newItem);

    }

    //ui->MUSIC_DB_LIST_FOLDERS_SLIDER->setMaximum(ui->MUSIC_DATABASE_LIST_FOLDERS->verticalScrollBar()->maximum());
        //ui->MUSIC_DB_LIST_FOLDERS_SLIDER->setValue(0);
}

void MainWindow::on_MUSIC_DATABASE_LIST_FOLDERS_itemClicked(QListWidgetItem *item)
{
    /*ui->artists->clear();
    ui->albums->clear();
    ui->titles->clear();
*/
    ui->MUSIC_DATABASE_LIST_ARTISTS->clear();
    ui->TITLE->setText("MUSIK • INTERPRETEN");

    selected_folder=item->data(Qt::UserRole).toString();

    selected_folder_real=item->data(Qt::UserRole+1).toString();
    QVector<QListWidgetItem> folderItems = musicDB_control->getArtist(item->text());
    int i=0;
    foreach (QListWidgetItem itemIterator, folderItems) {

        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(itemIterator.text());
        newItem->setData(Qt::UserRole-1,i);
        newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
        ui->MUSIC_DATABASE_LIST_ARTISTS->addItem(newItem);
        i++;
    }
    //ui->MUSIC_DB_LIST_ARTISTS_SLIDER->setMaximum(ui->MUSIC_DATABASE_LIST_ARTISTS->verticalScrollBar()->maximum());
    //ui->MUSIC_DB_LIST_ARTISTS_SLIDER->setValue(0);
    ui->MUSIC_DATABASE_TABS->setCurrentWidget(ui->MUSIC_DB_ARTISTS);
}

void MainWindow::on_MUSIC_DATABASE_LIST_ARTISTS_itemClicked(QListWidgetItem *item)
{
    ui->MUSIC_DATABASE_LIST_ALBUMS->clear();
    ui->TITLE->setText("MUSIK • ALBEN");

    selected_artist=item->data(Qt::UserRole).toString();

    QVector<QListWidgetItem> folderItems = musicDB_control->getAlbum(selected_folder, selected_artist);
    int i=0;
    foreach (QListWidgetItem itemIterator, folderItems) {

        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(itemIterator.text());
        newItem->setData(Qt::UserRole-1,i);
        newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
        ui->MUSIC_DATABASE_LIST_ALBUMS->addItem(newItem);
        i++;
    }
    //ui->MUSIC_DB_LIST_ALBUMS_SLIDER->setMaximum(ui->MUSIC_DATABASE_LIST_ALBUMS->verticalScrollBar()->maximum());
    //ui->MUSIC_DB_LIST_ALBUMS_SLIDER->setValue(0);
    ui->MUSIC_DATABASE_TABS->setCurrentWidget(ui->MUSIC_DB_ALBUMS);
}

void MainWindow::on_MUSIC_DATABASE_LIST_ALBUMS_itemClicked(QListWidgetItem *item)
{

          ui->MUSIC_DATABASE_LIST_TITLES->clear();
          ui->TITLE->setText("MUSIK • TITEL");

          selected_album=item->data(Qt::UserRole).toString();

          QVector<QListWidgetItem> folderItems = musicDB_control->getTitle(selected_folder, selected_artist, selected_album);

          int i=0;
          foreach (QListWidgetItem itemIterator, folderItems) {
              QListWidgetItem *newItem = new QListWidgetItem;
              newItem->setText(itemIterator.text());
              newItem->setData(Qt::UserRole-1,i);
              newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
              ui->MUSIC_DATABASE_LIST_TITLES->addItem(newItem);
              i++;
          }
          //ui->MUSIC_DB_LIST_TITLES_SLIDER->setMaximum(ui->MUSIC_DATABASE_LIST_TITLES->verticalScrollBar()->maximum());
          //ui->MUSIC_DB_LIST_TITLES_SLIDER->setValue(0);
          ui->MUSIC_DATABASE_TABS->setCurrentWidget(ui->MUSIC_DB_TITLES);
}

void MainWindow::on_MUSIC_DB_ADD_TITLES_clicked()
{
    for (int row=0;row<ui->MUSIC_DATABASE_LIST_TITLES->count();row++) {

        QListWidgetItem *item = ui->MUSIC_DATABASE_LIST_TITLES->item(row);
        if (item->isSelected()) {

            QString title_ID = item->data(Qt::UserRole).toString();

            QVector<QListWidgetItem> playlistItems = musicDB_control->addToPlaylist(selected_folder_real,title_ID);

            foreach (QListWidgetItem itemIterator, playlistItems) {



                /*QListWidgetItem *newItem = new QListWidgetItem;
                QString title_String = itemIterator.text() + " : " + itemIterator.data(Qt::UserRole+2).toString();
                newItem->setText(title_String);
                newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
                newItem->setData(Qt::UserRole+1,itemIterator.data(Qt::UserRole+1));
                newItem->setData(Qt::UserRole+2,itemIterator.data(Qt::UserRole+2));
                ui->MUSIC_PLAYLIST->addItem(newItem);
                */

                MusicPlayerClass->musicPlaylist->addMedia(QUrl::fromUserInput(itemIterator.data(Qt::UserRole+1).toString()));

                QListWidgetItem *item = new QListWidgetItem();
                item->setData(Qt::DisplayRole, itemIterator.data(Qt::UserRole).toString());
                item->setData(Qt::UserRole-1, itemIterator.data(Qt::UserRole-1).toString());
                item->setData(Qt::UserRole + 1, QString::number(MusicPlayerClass->musicPlaylist->mediaCount()));
                item->setData(Qt::UserRole + 3, QUrl::fromUserInput(itemIterator.data(Qt::UserRole+1).toString()).toString());

                /*
                 *     item->setData(Qt::DisplayRole, ui->MUSIC_RADIO_LIST->currentItem()->data(Qt::DisplayRole));
                item->setData(Qt::UserRole + 1, QString::number(MusicPlayerClass->musicPlaylist->mediaCount()));
                item->setData(Qt::UserRole + 3, QUrl::fromUserInput("www.schwarze-welle.de:7500").toString());
                */
                ui->MUSIC_PLAYLIST->addItem(item);

            }

        }
    }
    ui->MUSIC_DATABASE_LIST_TITLES->clearSelection();

}

void MainWindow::on_MUSIC_DB_PLAY_TITLE_clicked()
{
    ui->MEDIA_CONTROLS_CONTENTS->setVisible(true);


    foreach (QAbstractButton *button, buttonGroupPLayPause->buttons()) {
        button->setText("[");
    }

    for (int row=0;row<ui->MUSIC_DATABASE_LIST_TITLES->count();row++) {

        QListWidgetItem *item = ui->MUSIC_DATABASE_LIST_TITLES->item(row);
        if (item->isSelected()) {

            QString title_ID = item->data(Qt::UserRole).toString();

            QVector<QListWidgetItem> playlistItems = musicDB_control->addToPlaylist(selected_folder_real,title_ID);

            foreach (QListWidgetItem itemIterator, playlistItems) {



                /*QListWidgetItem *newItem = new QListWidgetItem;
                QString title_String = itemIterator.text() + " : " + itemIterator.data(Qt::UserRole+2).toString();
                newItem->setText(title_String);
                newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
                newItem->setData(Qt::UserRole+1,itemIterator.data(Qt::UserRole+1));
                newItem->setData(Qt::UserRole+2,itemIterator.data(Qt::UserRole+2));
                ui->MUSIC_PLAYLIST->addItem(newItem);
                */

                MusicPlayerClass->musicPlaylist->addMedia(QUrl::fromUserInput(itemIterator.data(Qt::UserRole+1).toString()));

                QListWidgetItem *item = new QListWidgetItem();
                item->setData(Qt::DisplayRole, itemIterator.data(Qt::UserRole).toString());
                item->setData(Qt::UserRole-1, itemIterator.data(Qt::UserRole-1).toString());
                item->setData(Qt::UserRole + 1, QString::number(MusicPlayerClass->musicPlaylist->mediaCount()));
                item->setData(Qt::UserRole + 3, QUrl::fromUserInput(itemIterator.data(Qt::UserRole+1).toString()).toString());

                /*
                 *     item->setData(Qt::DisplayRole, ui->MUSIC_RADIO_LIST->currentItem()->data(Qt::DisplayRole));
                item->setData(Qt::UserRole + 1, QString::number(MusicPlayerClass->musicPlaylist->mediaCount()));
                item->setData(Qt::UserRole + 3, QUrl::fromUserInput("www.schwarze-welle.de:7500").toString());
                */
                ui->MUSIC_PLAYLIST->addItem(item);

            }

        }
    }
    //MusicPlayerClass->MusicPlayer.playlist()->setCurrentIndex(playlist_widget->currentRow());
    MusicPlayerClass->MusicPlayer.playlist()->setCurrentIndex(ui->MUSIC_PLAYLIST->count()-1);
    MusicPlayerClass->MusicPlayer.play();
    ui->MUSIC_DATABASE_LIST_TITLES->clearSelection();

}

void MainWindow::on_MUSIC_FILES_ADD_clicked()
{

}

void MainWindow::on_MUSIC_PLAYLIST_PLAY_clicked()
{

}

void MainWindow::on_MUSIC_FILES_PLAY_4_clicked()
{

}

void MainWindow::on_MUSIC_DB_BACK_ARTISTS_clicked()
{
    ui->MUSIC_DATABASE_TABS->setCurrentWidget(ui->MUSIC_DB_FOLDERS);
    ui->TITLE->setText("MUSIK • ORDNER");
}

void MainWindow::on_MUSIC_DB_BACK_ALBUMS_clicked()
{
    ui->MUSIC_DATABASE_TABS->setCurrentWidget(ui->MUSIC_DB_ARTISTS);
    ui->TITLE->setText("MUSIK • INTERPRETEN");
}

void MainWindow::on_MUSIC_DB_BACK_TITLES_clicked()
{
    ui->MUSIC_DATABASE_TABS->setCurrentWidget(ui->MUSIC_DB_ALBUMS);
    ui->TITLE->setText("MUSIK • ALBEN");
}

void MainWindow::on_pushButton_30_clicked()
{
    ui->MEDIA_TABS->setCurrentWidget(ui->MEDIA_START);
    ui->TITLE->setText("MEDIA");
}

void MainWindow::on_MEDIA_BUTTON_MUSIC_SETTINGS_clicked()
{

    changeTitle(musicdb_settings_title);

    ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
    ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_SETTINGS);
    ui->MUSIC_DB_SETTINGS_FOLDERS->clear();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");



    QString tempFile = musicDB_file;
    tempFile.append("_temp");

    QFile file (tempFile);
    file.remove();


    db.setDatabaseName(musicDB_file);

    if(db.open()) /*QMessageBox::information(0,"Ok","Connection ok")*/;
    else QMessageBox::information(0,"Error","some error.");

    QSqlQuery query("SELECT `name`,`path` FROM folders");
     if (query.lastError().isValid()){} /*QMessageBox::critical(0,"Error",query.lastError().text() )*/;
     query.exec();

     /*QListWidgetItem newItem;
     newItem.setData(Qt::UserRole, query.value(1).toString());*/

     while (query.next()) {

            QListWidgetItem *newItem = new QListWidgetItem;
            newItem->setData(Qt::UserRole-1,ui->MUSIC_DB_SETTINGS_FOLDERS->count());
            newItem->setData(Qt::UserRole, query.value(0).toString());
            newItem->setData(Qt::UserRole+1, query.value(1).toString());
             if (query.value(0)=="") { newItem->setText("Unbenannter Ordner"); }
             else { newItem->setText(query.value(0).toString()); }
             ui->MUSIC_DB_SETTINGS_FOLDERS->addItem(newItem);
         }
    db.close();

}

void MainWindow::on_pushButton_38_clicked()
{
    QString tempFile = musicDB_file;
    tempFile.append("_temp");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");



    db.setDatabaseName(tempFile);

    if(db.open()) /*QMessageBox::information(0,"Ok","Connection ok")*/;
    else QMessageBox::information(0,"Error","some error.");
/*
     // REMOVE OLD FOLDERS
     QSqlQuery queryTruncate("DELETE FROM folders;");
     queryTruncate.exec();
     //QSqlQuery queryTruncate("TRUNCATE music;");
*/
    QSqlQuery createFolder("CREATE TABLE `folders` ("
                           "`id`	INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "`name`	INTEGER,"
                           "`path`	INTEGER)");

    QSqlQuery createMusic("CREATE TABLE \"music\" ("
                          "`id`	INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "`artist`	TEXT,"
                          "`album`	TEXT,"
                          "`title`	TEXT,"
                          "`coverart`	TEXT,"
                          "`length`	INTEGER,"
                          "`filepath`	TEXT,"
                          "`filename`	TEXT,"
                          "`folder`	TEXT,"
                          "`genre`	TEXT,"
                          "`track`	INTEGER)");

    createFolder.exec();
    createMusic.exec();

     foreach(QListWidgetItem* listItem, ui->MUSIC_DB_SETTINGS_FOLDERS->findItems("*", Qt::MatchWildcard))
      {
         //QListWidgetItem *listItem = ui->MUSIC_DB_SETTINGS_FOLDERS->item(row);
         QSqlQuery queryInsert("INSERT INTO folders (`name`, `path`) VALUES ('"+listItem->data(Qt::UserRole).toString()+"', '"+listItem->data(Qt::UserRole+1).toString()+"');");
         //if (query.lastError().isValid()) QMessageBox::critical(0,"Error",query.lastError().text() );
         queryInsert.clear();
      }

     db.close();


       ui->MUSIC_DB_STATUS_BAR->setText("STATUS: Zähle Dateien");

       ExifTool *et = new ExifTool();

       QVector<QStringList> DBDirs;


           db.setDatabaseName(tempFile);
           if(db.open()) /*QMessageBox::information(0,"Ok","Connection ok")*/;
           else QMessageBox::information(0,"Error","some error.");
           QSqlQuery query("SELECT * FROM folders");
            if (query.lastError().isValid()) QMessageBox::critical(0,"Error",query.lastError().text() );
            query.exec();

            while (query.next()) {
                    QString path = query.value(2).toString();
                    QString name = query.value(1).toString();
                    int id = query.value(0).toInt();
                    Q_UNUSED(id);

                    QStringList folder;
                    folder.append(name);
                    folder.append(path);

                    //!*qDebug() << id << name << path;
                    DBDirs.append(folder);
                }
            query.clear();

            db.close();




               db.setDatabaseName(tempFile);
               if(db.open()) {}/*QMessageBox::information(0,"Ok","Connection ok second")*/
               else {}/*QMessageBox::information(0,"Error","some error.")*/;


       int fileCount = 0;
       ui->MUSIC_DB_PROGRESS->setValue(0);


       foreach (QStringList q, DBDirs) {

           QDirIterator it(q[1], QStringList() << "*.mp3" << "*.wav" << "*.ogg" << "*.aiff" << "*.flac" << "*.wma", QDir::NoFilter, QDirIterator::Subdirectories);

       while (it.hasNext()) {
           it.next();
           {
               ui->MUSIC_DB_STATUS_BAR->setText("STATUS: Zähle Dateien: " + QString::number(fileCount) + " " +it.fileName() );
            fileCount+=1;
           }
       }
       }
       ui->MUSIC_DB_PROGRESS->setMaximum(fileCount);
        ui->MUSIC_DB_STATUS_BAR->setText("STATUS: Füge Dateien hinzu");

       foreach (QStringList q, DBDirs) {

           QDirIterator it(q[1], QStringList() << "*.mp3" << "*.wav" << "*.ogg" << "*.aiff" << "*.flac" << "*.wma", QDir::NoFilter, QDirIterator::Subdirectories);

       while (it.hasNext()) {
           it.next();
           {



               QString artist = "";
               QString album = "";
               QString title = "";
               QString genre = "";
               QString track = "";
               QByteArray picture = "";
               QString filepath=it.filePath().remove(it.fileName()).remove(q[1]);
               QString filename=it.fileName();
               QString folder = q[0];
               QString complete=it.filePath();
               QVector <QByteArray> values;


                TagInfo *info = et->ImageInfo(complete.toStdString().c_str(),"-artist\n"
                                                                          "-album\n"
                                                                          "-title\n"
                                                                          /*"-duration\n"*/
                                                                          "-genre\n"
                                                                          "-track\n"
                                                                          "-picture\n"
                                                                          "--a\n"
                                                                          "-e\n"
                                                                          "-b");
            if (info) {


                    // print returned information
                    for (TagInfo *i=info->next; i; i=i->next) {

                        QString nameString= i->name;

                        if (nameString == "Picture") {
                            //!*qDebug() << "Picture:";
                            picture = i->value;
                            //!*qDebug() << "Pic Size:" << picture.size();
                        } else if (nameString == "Artist") {
                            //!*qDebug() << "Artist:";
                            artist=i->value;
                            //!*qDebug() << artist;
                        } else if (nameString == "Album") {
                            //!*qDebug() << "Album:";
                            album=i->value;
                            //!*qDebug() << album;
                        } else if (nameString == "Genre") {
                            //!*qDebug() << "Genre:";
                            genre=i->value;
                            //!*qDebug() << genre;
                        } else if (nameString == "Track") {
                            //!*qDebug() << "Track:";
                            track=i->value;
                            //!*qDebug() << track;
                        } else if (nameString == "Title") {
                            //!*qDebug() << "Title:";
                            title=i->value;
                            ui->MUSIC_DB_STATUS_BAR->setText(title);
                            //!*qDebug() << title;
                        }


                    }



                    // we are responsible for deleting the information when done

                    delete info;
                } else if (et->LastComplete() <= 0) {
                    //!*qDebug() << "Error executing exiftool!";
                }

            //!*qDebug() << filename;
            //!*qDebug() << title;
            if (title=="") {
                title=filename;
            }
            //!*qDebug() << title;

            QByteArray ba = picture;
            QBuffer bu(&ba);
            //bu.open(QBuffer::ReadWrite);
            bu.open(QIODevice::WriteOnly);

            //bu.close();
            //QString imgBase64 = ba.toBase64();
            QString imgBase64 = QString::fromLatin1(ba.toBase64().data());

            ////!*qDebug() << "image base64: " << imgBase64;

            //!*qDebug() << "--------------------------";
             QSqlQuery query;
             query.exec("INSERT INTO music (`artist`,`album`,`title`,`coverart`,`length`,`genre`,`track`,`filepath`,`filename`,`folder`) "
                        "VALUES ('"+artist+"','"+album+"','"+title+"','"+picture+"','0','"+genre+"','"+track+"','"+filepath+"','"+filename+"','"+folder+"');");
            //!*qDebug() << query.isValid();
            ui->MUSIC_DB_PROGRESS->setValue(ui->MUSIC_DB_PROGRESS->value()+1);



                }
            }
       }
       db.close();



     QString oldBackup = musicDB_file+"_backup.1";

     QFile::rename(musicDB_file+"_backup",oldBackup);

     QFile::rename(musicDB_file,musicDB_file+"_backup");


     QFile fileOldBackup (oldBackup);
     fileOldBackup.remove();

     QFile::rename(tempFile,musicDB_file);
}


void MainWindow::on_MUSIC_DB_ADD_clicked()
{

    QListWidgetItem *newItem = new QListWidgetItem;
    QString title_String = ui->MUSIC_DB_ADD_NAME->text();
    newItem->setText(title_String);
    newItem->setData(Qt::UserRole-1,ui->MUSIC_DB_SETTINGS_FOLDERS->count());
    newItem->setData(Qt::UserRole,ui->MUSIC_DB_ADD_NAME->text());
    newItem->setData(Qt::UserRole+1,ui->MUSIC_DB_ADD_PATH->text());
    ui->MUSIC_DB_SETTINGS_FOLDERS->addItem(newItem);
}

void MainWindow::on_MUSIC_DB_REMOVE_FOLDER_clicked()
{
    delete ui->MUSIC_DB_SETTINGS_FOLDERS->currentItem();
}

void MainWindow::on_pushButton_34_clicked()
{
    ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
    ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_PLAYER);
    ui->MUSIC_PLAYER_TABS->setCurrentWidget(ui->MUSIC_PLAYER_PLAYLIST_TAB);
}

void MainWindow::on_pushButton_32_clicked()
{
    ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
    ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_PLAYER);
    ui->MUSIC_PLAYER_TABS->setCurrentWidget(ui->MUSIC_PLAYER_PLAYLIST_TAB);
}

void MainWindow::on_pushButton_31_clicked()
{
    ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
    ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_PLAYER);
    ui->MUSIC_PLAYER_TABS->setCurrentWidget(ui->MUSIC_PLAYER_PLAYLIST_TAB);
}

void MainWindow::on_pushButton_33_clicked()
{
    ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
    ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_PLAYER);
    ui->MUSIC_PLAYER_TABS->setCurrentWidget(ui->MUSIC_PLAYER_PLAYLIST_TAB);
}

void MainWindow::on_MUSIC_PLAYER_DATABASE_clicked()
{
    ui->MEDIA_TABS->setCurrentWidget(ui->MUSIC);
    ui->MUSIC_TABS->setCurrentWidget(ui->MUSIC_DATABASE);

    ui->MUSIC_DATABASE_TABS->setCurrentWidget(ui->MUSIC_DB_FOLDERS);
    ui->MUSIC_DATABASE_LIST_FOLDERS->clear();

    //ui->TITLE->setText("MUSIK • ORDNER");
    changeTitle(musicdb_folder);
    QVector<QListWidgetItem> folderItems = musicDB_control->getFolderList();
    int i=0;
    foreach (QListWidgetItem itemIterator, folderItems) {
        //!*qDebug() << itemIterator.text() << itemIterator.data(Qt::UserRole).toString() << itemIterator.data(Qt::UserRole+1).toString();
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(itemIterator.text());

        newItem->setData(Qt::UserRole-1,i);
        newItem->setData(Qt::UserRole,itemIterator.data(Qt::UserRole));
        newItem->setData(Qt::UserRole+1,itemIterator.data(Qt::UserRole+1));
        i++;
        ui->MUSIC_DATABASE_LIST_FOLDERS->addItem(newItem);

    }

    ui->MUSIC_DB_LIST_FOLDERS_SLIDER->setMaximum(ui->MUSIC_DATABASE_LIST_FOLDERS->verticalScrollBar()->maximum());
        ui->MUSIC_DB_LIST_FOLDERS_SLIDER->setValue(0);
}

//!* MENU BEEPS ----------------------------------------------------

void MainWindow::on_MM_BUTTON_TIMER_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MM_BUTTON_SYSTEM_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MM_BUTTON_SEARCH_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MM_BUTTON_RECIPE_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MM_BUTTON_MEDIA_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MC_BUTTON_M_PLAYER_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MC_BUTTON_HIDE_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MC_BUTTON_V_PLAYER_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MC_BUTTON_NEXT_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MC_BUTTON_STOP_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MC_BUTTON_PAUSE_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MC_BUTTON_PAUSE_2_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_TIMER_PAUSE_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_TIMER_STOP_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_3_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_TIMER_DELETE_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_TIMER_NEW_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_TIMER_SAVE_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}


void MainWindow::on_TIMER_EDIT_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_TIMER_START_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_TIMER_RESET_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_ALARM_SPIN_H_Plus_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_ALARM_SPIN_M_Plus_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}


void MainWindow::on_ALARM_SPIN_S_Plus_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_ALARM_SPIN_H_Minus_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_ALARM_SPIN_M_Minus_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_ALARM_SPIN_S_Minus_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_TIMER_LIST_PREV_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_TIMER_LIST_NEXT_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_RECIPE_MARK_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_RECIPE_READ_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MEDIA_BUTTON_MUSIC_DATABASE_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MEDIA_BUTTON_MUSIC_FILES_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MEDIA_BUTTON_MUSIC_SETTINGS_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MEDIA_BUTTON_MUSIC_DEACTIVATED_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/denybeep3.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MEDIA_BUTTON_MUSIC_PLAYER_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MEDIA_BUTTON_VIDEO_PLAYER_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MEDIA_BUTTON_VIDEO_FILES_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MEDIA_BUTTON_VIDEOS_DATABASE_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MEDIA_BUTTON_VIDEOS_SETTINGS_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MEDIA_BUTTON_VIDEOS_DEACTIVATED_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/denybeep3.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_PLAYER_FILES_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_PLAYER_DATABASE_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_PLAYER_PLAY_PAUSE_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_PLAYER_STOP_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_PLAYER_NEXT_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_PLAYER_PREV_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_FILES_OKAYER_3_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_PLAYLIST_PLAY_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_35_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_36_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_DB_REMOVE_FOLDER_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_38_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_41_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_42_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_DB_ADD_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_29_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_30_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_33_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_DATABASE_LIST_FOLDERS_itemSelectionChanged()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_34_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_DB_BACK_ARTISTS_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_24_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_27_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/denybeep3.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_28_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/denybeep3.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_18_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_DB_BACK_ALBUMS_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_32_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_22_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/denybeep3.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_23_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/denybeep3.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_12_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_DB_BACK_TITLES_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_31_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_DB_ADD_TITLES_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_MUSIC_DB_PLAY_TITLE_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SR_SEARCH_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SR_DIS_RETRY_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_45_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_44_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SYSTEM_SETTINGS_BUTTON_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SYSTEM_SERVER_STATUS_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SYSTEM_BUTTON_NOTES_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_LCARS_PushButton_8_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_LCARS_PushButton_7_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_47_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_48_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_LCARS_PushButton_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_LCARS_PushButton_9_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_LCARS_PushButton_6_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_LCARS_PushButton_11_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SET_AVAIL_LANG_SAVE_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SET_AVAIL_LANG_RIGHT_SLIDER_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SET_AVAIL_LANG_LEFT_SLIDER_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SET_AVAIL_LANG_LIST_itemPressed(QListWidgetItem *item)
{
    Q_UNUSED(item);
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_4_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SYSTEM_BUTTON_EQUALIZER_BACK_2_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_46_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_69_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SETTINGS_BUTTON_DISPLAY_OFF_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SETTINGS_BUTTON_NETWORK_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SETTINGS_BUTTON_ADVANCED_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SETTINGS_BUTTON_LANGUAGE_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SETTINGS_BUTTON_AUDIO_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_SETTINGS_BUTTON_BACK_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_pushButton_16_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_RECIPE_Ingredients_back_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

void MainWindow::on_RECIPE_Ingredients_next_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}


void MainWindow::on_MM_BUTTON_TIMER_7_pressed()
{
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
}

//!** MENU BEEP ---------------------------------------------------------


void MainWindow::on_MM_BUTTON_TIMER_7_clicked()
{
    switch (ui->MEDIA_CONTROLS_CONTENTS->isVisible()) {
    case 0:
        ui->MEDIA_CONTROLS_CONTENTS->setVisible(true);
        break;
    case 1:
        ui->MEDIA_CONTROLS_CONTENTS->setVisible(false);
        break;
    default:
        break;
    }

}

void MainWindow::on_MUSIC_PLAYER_LOOP_clicked()
{
    ui->MUSIC_PLAYER_TABS->setCurrentWidget(ui->MUSIC_PLAYER_MODE);
    /*
    int arg1=2;

    switch (arg1) {
    case 0:
        ui->MUSIC_PLAYER_LOOP->setText("NO LOOP");
        MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::Sequential);
        break;
    case 1:
        ui->MUSIC_PLAYER_LOOP->setText("LOOP ONE");
        MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;
    case 2:
        ui->MUSIC_PLAYER_LOOP->setText("LOOP ALL");
        MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    default:
        ui->MUSIC_PLAYER_LOOP->setText("NO LOOP");
        MusicPlayerClass->MusicPlayer.playlist()->setPlaybackMode(QMediaPlaylist::Sequential);
        break;
    }*/
}



void MainWindow::on_MUSIC_PLAYER_PLAYLIST_clicked()
{
    ui->MUSIC_PLAYER_TABS->setCurrentWidget(ui->MUSIC_PLAYER_PLAYLIST_TAB);
    ui->TITLE->setText("MUSIK PLAYER");
}

void MainWindow::on_MUSIC_PLAYER_DETAILS_clicked()
{
    ui->MUSIC_PLAYER_TABS->setCurrentWidget(ui->MUSIC_PLAYER_DETAILS_TAB);
    ui->TITLE->setText("MUSIK PLAYER");
}


void MainWindow::on_DISPLAY_BRIGHTNESS_SLIDER_valueChanged(int value)
{
            brightness = value;
    if (value == 0) {
      // system("su pi -c \"xset dpms force off\"");
      system("xset dpms force off");
      QString syscall = rootCommand+" echo " + QString::number(10) +
                        " > /sys/class/backlight/rpi_backlight/brightness";
      QByteArray ba = syscall.toLatin1();
      const char *c_str2 = ba.data();
      system(c_str2);
    } else {
      QString syscall = rootCommand+ " echo " + QString::number(value) +
                        " > /sys/class/backlight/rpi_backlight/brightness";
      QByteArray ba = syscall.toLatin1();
      const char *c_str2 = ba.data();
      system(c_str2);
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    /*player->setMedia(QUrl::fromLocalFile(mediaDir + "computerbeep_15.mp3"));
    player->play();*/
    Sound_MenuBeep->setMedia(QUrl("qrc:/sounds/computerbeep_15.mp3"));
    Sound_MenuBeep->play();
        this->close();


}

void MainWindow::on_pushButton_45_clicked()
{
    system("su pi -c \"xset dpms force off\"");
    system("sudo shutdown -r now");
}

void MainWindow::on_LCARS_PushButton_12_clicked()
{

}

void MainWindow::on_MUSIC_PLAYER_RANDOM_clicked()
{

}

void MainWindow::on_SETTINGS_BUTTON_ADVANCED_2_clicked()
{
    changeTitle(run_title);
     ui->SETTINGS_TABS->setCurrentWidget(ui->SET_COMMAND);
}

void MainWindow::on_SETTINGS_BUTTON_DISPLAY_OFF_2_clicked()
{
    system(qPrintable(rootCommand+ " xset s off"));
    system(qPrintable(rootCommand+ " xset -dpms"));
    system(qPrintable(rootCommand+ " xset s noblank"));

}

void MainWindow::on_SETTINGS_BUTTON_ADVANCED_3_clicked()
{
    changeTitle(display_title);
    ui->PAGES->setCurrentWidget(ui->SETTINGS);
    ui->SETTINGS_TABS->setCurrentWidget(ui->SET_DISPLAY);
}

void MainWindow::on_SETTINGS_BUTTON_BACK_2_clicked()
{
    changeTitle(settings_title);
    ui->PAGES->setCurrentWidget(ui->SETTINGS);
    ui->SETTINGS_TABS->setCurrentWidget(ui->SET_START);
}

void MainWindow::on_DISPLAY_BRIGHTNESS_SLIDER_2_valueChanged(int value)
{

         brightness = value;
        if (value == 0) {
          // system("su pi -c \"xset dpms force off\"");
          system(qPrintable(rootCommand+" xset dpms force off"));
          QString syscall = rootCommand+ " echo " + QString::number(10) +
                            " > /sys/class/backlight/rpi_backlight/brightness";
          QByteArray ba = syscall.toLatin1();
          const char *c_str2 = ba.data();
          system(c_str2);
        } else {
          QString syscall = rootCommand+ " echo " + QString::number(value) +
                            " > /sys/class/backlight/rpi_backlight/brightness";
          QByteArray ba = syscall.toLatin1();
          const char *c_str2 = ba.data();
          system(c_str2);
        }

}

void MainWindow::on_LCARS_PushButton_11_clicked()
{
    //m_webSocket->sendTextMessage("ping");

    system(qPrintable(rootCommand+" killall replicator-server"));
    //system(qPrintable(rootCommand+" kill "+serverPID));
    //qDebug() << qPrintable(rootCommand+" kill "+serverPID);
}

void MainWindow::on_LCARS_PushButton_6_clicked()
{
    m_webSocket->sendTextMessage("shutdown");
}

void MainWindow::on_SET_AVAIL_LANG_BACK_clicked()
{
    changeTitle(settings_title);
    ui->SETTINGS_TABS->setCurrentWidget(ui->SET_START);
}

void MainWindow::on_SETTINGS_BUTTON_EXTENDED_clicked()
{
    changeTitle(advanced_title);
    ui->SETTINGS_TABS->setCurrentWidget(ui->SET_EXTENDED);
}

void MainWindow::on_SET_START_SERVER_2_clicked()
{
    changeTitle(ambient_title);
    ui->SETTINGS_TABS->setCurrentWidget(ui->SET_AMBIENT);
}

void MainWindow::on_TIMER_LIST_PREV_clicked()
{
    if (ui->TIMER_LIST->verticalScrollBar()->value() >= ui->TIMER_LIST->verticalScrollBar()->minimum()+1) {
    ui->TIMER_LIST->verticalScrollBar()->setValue(ui->TIMER_LIST->verticalScrollBar()->value() - 1);
    }
}

void MainWindow::on_TIMER_SOUND_REPEAT_clicked()
{


}

void MainWindow::on_TIMER_SOUND_REPEAT_toggled(bool checked)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(settingsDbFile);

    if (checked==true) {
        timer_sound_repeat=1;
        this->sender()->setProperty("Center_Color","#cc6666");
    } else { timer_sound_repeat=0;
    this->sender()->setProperty("Center_Color","#ff9900");}


    if(db.open()) {
        QSqlQuery query("UPDATE settings SET `timer_sound_repeat` = '"+QString::number(timer_sound_repeat)+"' WHERE `id`=0;");
         if (query.lastError().isValid()){};
         query.exec();}
    else {};

    db.close();
}

void MainWindow::on_SYSTEM_BUTTON_NOTES_clicked()
{

}

void MainWindow::on_REMOVE_MARKED_RECIPE_clicked()
{
    m_webSocket->sendTextMessage("UPDATE::UPDATE recipes SET `selected` = '0' WHERE `id`='"+ui->MARKED_RECIPES->currentItem()->data(Qt::UserRole).toString()+"';");
    delete ui->MARKED_RECIPES->currentItem();
}

void MainWindow::on_RECIPE_MARK_toggled(bool checked)
{
    if (checked == true) {
        m_webSocket->sendTextMessage("UPDATE::UPDATE recipes SET `selected` = '1' WHERE `id`='"+recipeID+"';");
        ui->RECIPE_MARK->setProperty("Center_Color","#cc6666");
        ui->RECIPE_MARK->setText("MARKIERT");
    } else {
        m_webSocket->sendTextMessage("UPDATE::UPDATE recipes SET `selected` = '0' WHERE `id`='"+recipeID+"';");
        ui->RECIPE_MARK->setProperty("Center_Color","#ff9900");
        ui->RECIPE_MARK->setText("MERKEN");
    }
}

void MainWindow::on_SHOW_RECIPE_clicked()
{
    QString itemData= ui->MARKED_RECIPES->currentItem()->data(Qt::UserRole).toString();
    m_webSocket->sendTextMessage("RECIPE::SELECT `id`,`title`,`ingredients`,`text`,`image`,`selected` FROM `recipes` WHERE `id` = "+itemData+"");
    recipeID = itemData;
}

void MainWindow::on_SET_SERVER_BACK_3_clicked()
{
    changeTitle(settings_title);
    ui->SETTINGS_TABS->setCurrentWidget(ui->SET_START);
}

void MainWindow::on_VIDEO_MAXIMIZE_clicked()
{
    /////videoform = new VideoFullscreen(this);
    //connect(keyb,SIGNAL(closed()),this,SLOT(updateVarTextFeld()));
    /////videoform->setAttribute(Qt::WA_DeleteOnClose);
    /////videoform->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    /////videoform->showFullScreen(); //opening second form
    //videoform->videoFileString=videoFile;
    /////ui->VIDEO_PLAY_PAUSE->clicked();


    //////ui->MEDIA_TABS->removeTab(ui->MEDIA_TABS->currentIndex());

    //rippedOffTab->setAttribute(Qt::WA_DeleteOnClose);
    //////rippedOffTab->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    //////rippedOffTab->showFullScreen();

    qDebug() << ui->VIDEO_MAXIMIZE->isChecked();

    switch (videoIsFullscreen) {
    case true:
        ui->VIDEO_TABS->addTab(rippedOffTab,"tab2");
        rippedOffTab->setWindowFlags(Qt::Widget);
        rippedOffTab->setMinimumHeight(0);
        rippedOffTab->setMinimumWidth(0);
        ui->SR_TOP_LINES_6->setVisible(true);
        //ui->VideoWindow_ControlBar->setVisible(true);
        ui->VIDEO_TABS->setCurrentWidget(rippedOffTab);
        videoIsFullscreen = false;

        ui->VIDEO_MAXIMIZE->setText("d");

        //ui->VIDEO_MAXIMIZE->setChecked(false);
        break;
    case false:
        ui->VIDEO_TABS->removeTab(ui->VIDEO_TABS->currentIndex());
        ui->SR_TOP_LINES_6->setVisible(false);
        //ui->VideoWindow_ControlBar->setVisible(false);
        rippedOffTab->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
        rippedOffTab->setMinimumHeight(480);
        rippedOffTab->setMinimumWidth(800);
        rippedOffTab->showFullScreen();
        ui->VIDEO_MAXIMIZE->setText("c");
        videoIsFullscreen = true;
        //ui->VIDEO_MAXIMIZE->setChecked(true);


        break;
    default:
        break;
    }



}

void MainWindow::on_KEYBOARD_GLYPH_LEFT_clicked()
{


            /*switch (ui->KEYBOARD_GLYPHS_TAB->currentIndex()) {
            case 0:

                break;
            case 1:*/
                //ui->KEYBOARD_GLYPHS_TAB->setCurrentIndex(-1);
                ui->KEYBOARD_GLYPHS_TAB->setCurrentWidget(ui->GLYPH_NUMBERS);
                /*break;
        default:
            break;
    }*/
}

void MainWindow::on_KEYBOARD_GLYPH_RIGHT_clicked()
{
    /*switch (ui->KEYBOARD_GLYPHS_TAB->currentIndex()) {
    case 0:*/
        //ui->KEYBOARD_GLYPHS_TAB->setCurrentIndex(+1);
        ui->KEYBOARD_GLYPHS_TAB->setCurrentWidget(ui->GLYPH_CHARS);
        /*break;
    case 1:

        break;
default:
    break;
}*/
}

void MainWindow::on_SETTINGS_SERVER_BACK_clicked()
{
    changeTitle(system_title);
    ui->PAGES->setCurrentWidget(ui->SYSTEM);
}

void MainWindow::on_MEDIA_BUTTON_MUSIC_GENERAL_SETTINGS_clicked()
{
    //changeTitle(media_music_general_settings);
}

void MainWindow::on_MEDIA_BUTTON_VIDEOS_DATABASE_clicked()
{
    //changeTitle(timer_title);
}

void MainWindow::on_MEDIA_BUTTON_VIDEOS_GENERAL_SETTINGS_clicked()
{

}

void MainWindow::on_SETTINGS_BUTTON_BACK_clicked()
{

}

void MainWindow::on_MM_BUTTON_TIMER_2_clicked()
{
    //changeTitle(settings_title);
}

void MainWindow::on_SET_SERVER_BACK_clicked()
{

}

void MainWindow::on_SET_AVAIL_LANG_LIST_itemClicked(QListWidgetItem *item)
{
    currentLanguage = item->data(Qt::UserRole).toString();
    settingsClass->writeSetting("UPDATE translation_codes SET `selected` = '0';");
    settingsClass->writeSetting("UPDATE translation_codes SET `selected` = '1' WHERE `code` = '"+currentLanguage+"';");
    loadSettings(currentLanguage);
}
