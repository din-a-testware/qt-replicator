#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFontDatabase>
#include <QMediaPlayer>
#include "lockscreen.h"
#include <QWebSocket>
#include <QListWidgetItem>
#include <QAudioBuffer>
#include <QVideoWidget>
#include "videoplayer_control.h"
#include "musicplayer_control.h"
#include "animations.h"
#include "lcars_button.h"
#include "lcars_roundededgeconnector.h"
#include "musicdb_control.h"
#include <QSqlDatabase>
#include "videofullscreen.h"
#include "settings.h"
#include <QTextToSpeech>

#include "remote_control.h"

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtNetwork/QSslError>
//#include "globalvars.h"
//#include "db_search.h"
#include <QAction>

class remote_control;
class VideoPlayer_Control;
class MusicPlayer_Control;
class animations;
class musicdb_control;
class VideoFullscreen;
class settings;

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(quint16 port,QWidget *parent = 0);
    virtual ~MainWindow();

    QAction *exitAct;

    QVector<QTimer *> timerList;

    //! LANGUAGE SETTINGS
    QString currentLanguage;
    QString currentLanguageName;
    QString lang_code;
    QString generic_back;
    QString main_title;
    QString main_timer;
    QString main_media;
    QString main_recipe;
    QString main_search;
    QString main_system;
    QString timer_title;
    QString timer_timername;
    QString timer_newtimer;
    QString timer_pause;
    QString timer_stop;
    QString timer_edit;
    QString timer_start;
    QString timer_reset;
    QString timer_save;
    QString timer_delete;
    QString timer_new;
    QString timer_current;
    QString timer_timeout;
    QString timer_inactive;
    QString system_title;
    QString system_settings;
    QString system_notes;
    QString system_lock;
    QString recipe_next;
    QString recipe_previous;
    QString recipe_read;
    QString recipe_mark;
    QString search_title;
    QString search_search;
    QString discon_title;
    QString discon_controltitle;
    QString discon_error;
    QString discon_retry;
    QString lock_toptitle;
    QString lock_controltitle;
    QString lock_locked;
    QString lock_granted;
    QString lock_bottomtitle;
    QString lock_alpha;
    QString lock_beta;
    QString lock_gamma;
    QString lock_delta;
    QString red_disable;
    QString topcontrols_hide;
    QString topcontrols_mplayer;
    QString topcontrols_vplayer;
    QString set_lang_title;
    QString set_lang_title_description;
    QString set_lang_description;
    QString media_title;
    QString media_music_db;
    QString media_music_files;
    QString media_music_db_settings;
    QString media_music_general_settings;
    QString media_music_player;
    QString media_video_db;
    QString media_video_files;
    QString media_video_db_settings;
    QString media_video_general_settings;
    QString media_video_player;
    QString notes_title;
    QString notes_marked;
    QString notes_notes;
    QString notes_show_recipe;
    QString notes_remove_recipe;
    QString notes_new_note;
    QString notes_edit_note;
    QString notes_remove_note;
    QString generic_edit;
    QString server_status_client_ip;
    QString server_status_client_port;
    QString server_status_client_bin;
    QString server_status_status;
    QString server_status_ip;
    QString server_status_port;
    QString server_status_pid;
    QString server_status_client;
    QString server_status_server;
    QString server_status_title;
    QString server_status_running;
    QString server_status_stopped;
    QString server_status_settings;
    QString server_settings_ip;
    QString server_settings_port;
    QString server_settings_protocol;
    QString server_settings_dbpath;
    QString server_settings_pid;
    QString server_settings_status;
    QString server_settings_running;
    QString server_settings_stopped;
    QString server_settings_start;
    QString server_settings_restart;
    QString server_settings_shutdown;
    QString server_settings_kill;
    QString server_settings_title;
    QString settings_title;
    QString settings_display;
    QString settings_advanced;
    QString settings_network;
    QString settings_language;
    QString settings_audio;
    QString settings_run;
    QString settings_warpcore;
    QString display_title;
    QString display_off;
    QString display_timeout;
    QString display_brightness;
    QString advanced_title;
    QString advanced_ambient;
    QString advanced_alarmsound;
    QString advanced_alarmrepeat;
    QString advanced_alarmsingle;
    QString ambient_title;
    QString ambient_name;
    QString ambient_path;
    QString ambient_add;
    QString generic_add;
    QString generic_remove;
    QString network_title;
    QString network_hostname;
    QString network_interface;
    QString network_ip;
    QString network_mac;
    QString network_wlan_settings;
    QString network_eth_settings;
    QString audio_title;
    QString autio_main;
    QString audio_menu;
    QString audio_media;
    QString audio_alarm;
    QString audio_equalizer;
    QString eq_title;
    QString eq_disabled;
    QString eq_enabled;
    QString run_title;
    QString run_asroot;
    QString run_run;
    QString run_edit;
    QString run_example;
    QString generic_searchfor;
    QString keyboard_ok;
    QString keyboard_cancel;
    QString generic_processing;
    QString music_player_title;
    QString music_player_details_title;
    QString music_player_details_artist;
    QString music_player_details_album;
    QString music_player_details_left;
    QString music_player_details_right;
    QString music_player_mode;
    QString music_player_playlist;
    QString music_player_details;
    QString music_player_menu;
    QString music_player_database;
    QString music_player_files;
    QString music_player_playlist_play;
    QString music_player_mode_all;
    QString music_player_mode_one;
    QString music_player_mode_onerepeat;
    QString music_player_mode_norepeat;
    QString music_player_mode_random;
    QString video_player_title;
    QString video_player_controls;
    QString musicdb_settings_title;
    QString musicdb_settings_name;
    QString musicdb_settings_path;
    QString musicdb_settings_add;
    QString musicdb_settings_update;
    QString musicdb_settings_status;
    QString musicdb_settings_idle;
    QString musicdb_setttings_counting;
    QString musicdb_settings_adding;
    QString musicdb_folder;
    QString musicdb_letter;
    QString musicdb_artist;
    QString musicdb_album;
    QString musicdb_songtitle;
    QString musicdb_playlist;
    QString musicdb_add;
    QString musicdb_play;
    QString musicfiles_title;
    QString musicfiles_add;
    QString musicfiles_play;
    QString musicfiles_player;
    QString videofiles_title;
    QString videofiles_play;
    QString videofiles_add;
    QString videofiles_player;
    QString system_restart;
    QString system_shutdown;
    QString system_server_status;
    QString system_disable_gui;
    QString red_alert_text;
    QString audio_main;



    //! /LANGUAGE SETTINGS

    QString mediaFile;
    QTimer *timer;
    QTimer *timer_countdown;
    QVector<QVector< QWidget *> >  red_alert_widgets;
    QVector<QVector< QWidget *> >  start_animation_widgets;
    QList<QWidget *> all_widgets;
    QList<QWidget *> alarm_id_widgets;
    QList<QWidget *> second_alarm_id_widgets;
    QFontDatabase LCARS_fonts;
    QStringList code;
    QStringList code_temp;
    QMediaPlayer *menuBeeper;
    QStringList codeString;
    QVector<QObject*> *codeButtons;
    LockScreen *lockform;
    VideoFullscreen *videoform = new VideoFullscreen;
    QWidget *last_widget;
    QWidget *current_widget;
    QWebSocket *m_webSocket;
    QUrl url;
    QVector<QStringList> replicationData;
    bool openRecipeBool = false;
    int timercntdown;
    QMediaPlayer *MusicPlayer = new QMediaPlayer;
    QMediaPlayer *Sound_MenuBeep = new QMediaPlayer;
    QMediaPlayer *Sound_RedAlert = new QMediaPlayer;
    QVideoWidget *videoWidget = new QVideoWidget;
    VideoPlayer_Control *VideoPlayerClass = new VideoPlayer_Control;
    MusicPlayer_Control *MusicPlayerClass = new MusicPlayer_Control;
    animations *AnimationClass = new animations;

    remote_control *Remote_Control = new remote_control;


    QStringList MusicPlaylistEntries;
    QString selected_folder;
    QString selected_folder_real;
    QString selected_artist;
    QString selected_album;
    QStringList selected_titles;
    musicdb_control *musicDB_control = new musicdb_control;

    settings *settingsClass = new settings;

    bool shiftPressed = false;
    bool capslockPressed = false;


    QWidget *rippedOffTab;

    int menuVolume;
    int mediaVolume;
    int mainVolume;
    int musicVolume;
    int videoVolume;
    int alarmVolume;
    int brightness;

    int timer_sound_repeat;

    bool videoIsFullscreen = false;

    QString recipeID;

    QString rootCommand;

    QString serverBin;
    QString serverPID;
    QString serverIP;
    QString serverPort;
    QString serverLocal;
    QString serverProtocol;

    QString remoteServerIP;
    QString remoteServerPort;
    QString remoteServerProtocol;
    QString remoteServerDBPath;

    QButtonGroup *buttonGroupPLayPause;

    QButtonGroup *buttonGroupStop;

    QButtonGroup *buttonGroupNext;

    QButtonGroup *buttonGroupPrev;

    QSqlDatabase db;

    QList<QMediaPlayer *> ambientSounds;
    //QList<QMediaPlaylist *> ambientSoundsPlaylist;

    QTextToSpeech *m_speech;

protected:
    void showEvent(QShowEvent *event);

private Q_SLOTS:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(QString message);
    //void onSslErrors(const QList<QSslError> &errors);
    void onBinaryMessageRecieved(QByteArray message);
    void processData(QString searchString);
    void fillScrollArea(QVector<QStringList> result);
    void clickedSlot(QString id);
    void loadRecipe(QVector<QStringList> result) ;
    //void SR_RESULT_ITEM_CLICKEDSlot(QString id);
    //void hideshowscroll();
    void timedout();

    void timerSettings();
    void mediaSettings();
    void pathSettings();
    void serverSettings();
    void fontSettings();
    void delegateSettings();
    void codeSettings();
    void widgetsSettings();

    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
    void onSslErrors(const QList<QSslError> &errors);
    QByteArray serialize(QVector<QStringList> data);
    QVector<QStringList> deserialize(const QByteArray& byteArray);
    void createActions();
    void createMenus(QString iface);

private slots:

    //! -- COUNTDOWN TIMER
    void countdownTimerTimeout(QTimer *newTimer, QString title, QListWidgetItem *currentListItem);

    void setTimerLabel(qint64 mseconds, QString title, QDateTime timeoutDate);
    //! --------

    void changeTitle(QString title);

    void listDirVideos(QString dirString);

    void initReplicator();

    void listDirMusic(QString dirString);

    void processBuffer(const QAudioBuffer& buffer);

    void on_MM_BUTTON_SYSTEM_clicked();

    void showTime();



    void on_MM_BUTTON_RECIPE_clicked();



    void on_AW_RED_ALERT_TEXT_clicked();



    void on_MM_BUTTON_SEARCH_clicked();

    void on_pushButton_clicked();

    //void on_SR_SEARCH_clicked();

    //void lockScreen(QVector<QVector< QWidget *> >  red_alert_widgets, QList<QWidget *> all_widgets);

    /*void on_MovingButton_1_clicked();

    void on_MovingButton_2_clicked();

    void on_MovingButton_3_clicked();

    void on_MovingButton_5_clicked();*/

    //void processCodeResult(int resultValue);

    //void on_SR_CLEAR_clicked();

    /*void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();*/

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_RECIPE_READ_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_19_pressed();



    void on_ALARM_SPIN_H_Plus_clicked();

    void on_ALARM_SPIN_H_Minus_clicked();

    void on_ALARM_SPIN_M_Plus_clicked();

    void on_ALARM_SPIN_M_Minus_clicked();

    void on_ALARM_SPIN_S_Plus_clicked();

    void on_ALARM_SPIN_S_Minus_clicked();

    void on_TIMER_RESET_clicked();

    void on_TIMER_START_clicked();

    void on_SR_DIS_RETRY_clicked();

    void on_MM_BUTTON_TIMER_clicked();

    void loadSettings(const QString& LANG_CODE=NULL);

    void saveSettings();

    void endAlert() ;

    void on_SYSTEM_SETTINGS_BUTTON_clicked();

    void on_TIMER_STOP_clicked();

    void on_TIMER_LIST_itemClicked(QListWidgetItem *item);

    void on_TIMER_SAVE_clicked();

    void on_TIMER_DELETE_clicked();

    void on_TIMER_NEW_clicked();

    void countdown();

    void on_TIMER_PAUSE_clicked();

    void window_shown();

    void on_MM_BUTTON_MEDIA_clicked();

    void on_pushButton_2_clicked();



    void on_SET_AVAIL_LANG_SAVE_clicked();

    //void on_SET_AVAIL_LANG_LEFT_SCROLL_valueChanged(int value);

    void on_pushButton_3_clicked();



    void on_SETTINGS_BUTTON_LANGUAGE_clicked();

    void on_SETTINGS_BUTTON_AUDIO_clicked();

    void on_SETTINGS_BUTTON_ADVANCED_clicked();

    void on_SETTINGS_BUTTON_NETWORK_clicked();

    void on_SETTINGS_BUTTON_DISPLAY_OFF_clicked();

    void on_pushButton_69_clicked();


    void on_MEDIA_BUTTON_VIDEO_PLAYER_clicked();

    void on_MEDIA_BUTTON_MUSIC_PLAYER_clicked();

    void on_MC_BUTTON_PAUSE_2_clicked();



    void on_pushButton_4_clicked();

    void on_MEDIA_BUTTON_MUSIC_FILES_clicked();

    void on_MUSIC_FILES_BACK_clicked();

    void on_MUSIC_FILES_OKAYER_clicked();


    void on_pushButton_44_clicked();

    void on_MC_BUTTON_HIDE_clicked();

    void on_MEDIA_BUTTON_MUSIC_DEACTIVATED_clicked();

    void on_TIMER_LIST_NEXT_clicked();



    void on_pushButton_81_clicked();

    void on_MEDIA_BUTTON_VIDEOS_DEACTIVATED_clicked();

    void on_VIDEO_BUTTON_CONTROLS_clicked();


    void on_MC_BUTTON_M_PLAYER_clicked();

    void on_MUSIC_PLAYER_FILES_clicked();

    void on_MUSIC_FILES_PLAY_clicked();

    void on_MUSIC_PLAYER_NEXT_clicked();

    void on_MUSIC_PLAYER_PREV_clicked();

    void on_MUSIC_PLAYER_LOOP_toggled(bool checked);

    void on_MUSIC_PLAYER_LOOP_stateChanged(int arg1);

    void on_MUSIC_PLAYER_PLAYLIST_stateChanged(int arg1);

    void on_MUSIC_PLAYER_PLAYLIST_clicked();

    void on_MUSIC_FILES_OKAYER_3_clicked();

    void on_MUSIC_DATABASE_LIST_itemClicked(QListWidgetItem *item);

    void on_MUSIC_FILES_OKAYER_4_clicked();



    void on_MEDIA_BUTTON_VIDEO_FILES_clicked();

    void connections();

    QString searchRequest(QString returnText);

    void on_SR_RESULT_itemClicked(QListWidgetItem *item);

    void on_VIDEO_FILES_PLAY_clicked();

    void on_MC_BUTTON_V_PLAYER_clicked();

    
    void on_RECIPE_Ingredients_next_clicked();

    void on_RECIPE_Ingredients_back_clicked();

    void on_controls_down_clicked();

    void on_controls_butt_clicked();

    void on_VIDEO_BUTTON_BACK_clicked();

    void on_VIDEO_MAXIMIZE_2_clicked();

    void on_SYSTEM_SERVER_STATUS_clicked();

    void on_LCARS_PushButton_clicked();

    void on_ALARM_VOLUME_SLIDER_sliderReleased();

    void on_ALARM_VOLUME_SLIDER_valueChanged(int value);

    void on_CHANGE_COLORS_clicked();

    void on_LCARS_PushButton_3_clicked();

    void on_LCARS_PushButton_4_clicked();

    void on_ALARM_VOLUME_SLIDER_sliderMoved(int position);

    void on_MEDIA_BUTTON_MUSIC_DATABASE_clicked();

    void on_MUSIC_DATABASE_LIST_FOLDERS_itemClicked(QListWidgetItem *item);

    void on_MUSIC_DATABASE_LIST_ARTISTS_itemClicked(QListWidgetItem *item);

    void on_MUSIC_DATABASE_LIST_ALBUMS_itemClicked(QListWidgetItem *item);

    void on_MUSIC_DB_ADD_TITLES_clicked();

    void on_MUSIC_DB_PLAY_TITLE_clicked();

    void on_MUSIC_FILES_ADD_clicked();

    void on_MUSIC_PLAYLIST_PLAY_clicked();

    void on_MUSIC_FILES_PLAY_4_clicked();

    void on_MUSIC_DB_BACK_ARTISTS_clicked();

    void on_MUSIC_DB_BACK_ALBUMS_clicked();

    void on_MUSIC_DB_BACK_TITLES_clicked();

    void on_pushButton_30_clicked();

    void on_MEDIA_BUTTON_MUSIC_SETTINGS_clicked();

    void on_pushButton_38_clicked();

    void on_MUSIC_DB_ADD_clicked();

    void on_MUSIC_DB_REMOVE_FOLDER_clicked();

    void on_pushButton_34_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_33_clicked();

    void on_MUSIC_PLAYER_DATABASE_clicked();

    void on_MM_BUTTON_TIMER_pressed();

    void on_MM_BUTTON_SYSTEM_pressed();

    void on_MM_BUTTON_SEARCH_pressed();

    void on_MM_BUTTON_RECIPE_pressed();

    void on_MM_BUTTON_MEDIA_pressed();

    void on_MC_BUTTON_M_PLAYER_pressed();

    void on_MC_BUTTON_HIDE_pressed();

    void on_MC_BUTTON_V_PLAYER_pressed();

    void on_MC_BUTTON_NEXT_pressed();

    void on_MC_BUTTON_STOP_pressed();

    void on_MC_BUTTON_PAUSE_pressed();

    void on_MC_BUTTON_PAUSE_2_pressed();

    void on_TIMER_PAUSE_pressed();

    void on_TIMER_STOP_pressed();

    void on_pushButton_3_pressed();

    void on_TIMER_DELETE_pressed();

    void on_TIMER_NEW_pressed();

    void on_TIMER_SAVE_pressed();

    void on_TIMER_EDIT_pressed();

    void on_TIMER_START_pressed();

    void on_TIMER_RESET_pressed();

    void on_ALARM_SPIN_H_Plus_pressed();

    void on_ALARM_SPIN_M_Plus_pressed();

    void on_ALARM_SPIN_S_Plus_pressed();

    void on_ALARM_SPIN_H_Minus_pressed();

    void on_ALARM_SPIN_M_Minus_pressed();

    void on_ALARM_SPIN_S_Minus_pressed();

    void on_TIMER_LIST_PREV_pressed();

    void on_TIMER_LIST_NEXT_pressed();

    void on_RECIPE_MARK_pressed();

    void on_RECIPE_READ_pressed();

    void on_MEDIA_BUTTON_MUSIC_DATABASE_pressed();

    void on_MEDIA_BUTTON_MUSIC_FILES_pressed();

    void on_MEDIA_BUTTON_MUSIC_SETTINGS_pressed();

    void on_MEDIA_BUTTON_MUSIC_DEACTIVATED_pressed();

    void on_MEDIA_BUTTON_MUSIC_PLAYER_pressed();

    void on_MEDIA_BUTTON_VIDEO_PLAYER_pressed();

    void on_MEDIA_BUTTON_VIDEO_FILES_pressed();

    void on_MEDIA_BUTTON_VIDEOS_DATABASE_pressed();

    void on_MEDIA_BUTTON_VIDEOS_SETTINGS_pressed();

    void on_MEDIA_BUTTON_VIDEOS_DEACTIVATED_pressed();


    void on_MUSIC_PLAYER_FILES_pressed();

    void on_MUSIC_PLAYER_DATABASE_pressed();

    void on_MUSIC_PLAYER_PLAY_PAUSE_pressed();

    void on_MUSIC_PLAYER_STOP_pressed();

    void on_MUSIC_PLAYER_NEXT_pressed();

    void on_MUSIC_PLAYER_PREV_pressed();


    void on_MUSIC_FILES_OKAYER_3_pressed();

    void on_MUSIC_PLAYLIST_PLAY_pressed();

    void on_pushButton_35_pressed();

    void on_pushButton_36_pressed();

    void on_MUSIC_DB_REMOVE_FOLDER_pressed();

    void on_pushButton_38_pressed();

    void on_pushButton_41_pressed();

    void on_pushButton_42_pressed();

    void on_MUSIC_DB_ADD_pressed();

    void on_pushButton_29_pressed();

    void on_pushButton_30_pressed();

    void on_pushButton_33_pressed();

    void on_MUSIC_DATABASE_LIST_FOLDERS_itemSelectionChanged();

    void on_pushButton_34_pressed();

    void on_MUSIC_DB_BACK_ARTISTS_pressed();

    void on_pushButton_24_pressed();

    void on_pushButton_27_pressed();

    void on_pushButton_28_pressed();

    void on_pushButton_18_pressed();

    void on_MUSIC_DB_BACK_ALBUMS_pressed();

    void on_pushButton_32_pressed();

    void on_pushButton_22_pressed();

    void on_pushButton_23_pressed();

    void on_pushButton_12_pressed();

    void on_MUSIC_DB_BACK_TITLES_pressed();

    void on_pushButton_31_pressed();

    void on_MUSIC_DB_ADD_TITLES_pressed();

    void on_MUSIC_DB_PLAY_TITLE_pressed();

    void on_SR_SEARCH_pressed();

    void on_SR_DIS_RETRY_pressed();

    void on_pushButton_45_pressed();

    void on_pushButton_44_pressed();

    void on_SYSTEM_SETTINGS_BUTTON_pressed();

    void on_SYSTEM_SERVER_STATUS_pressed();

    void on_SYSTEM_BUTTON_NOTES_pressed();

    void on_pushButton_pressed();

    void on_LCARS_PushButton_8_pressed();

    void on_LCARS_PushButton_7_pressed();

    void on_pushButton_47_pressed();

    void on_pushButton_48_pressed();

    void on_LCARS_PushButton_pressed();

    void on_LCARS_PushButton_9_pressed();

    void on_LCARS_PushButton_6_pressed();

    void on_LCARS_PushButton_11_pressed();

    void on_SET_AVAIL_LANG_SAVE_pressed();

    void on_SET_AVAIL_LANG_RIGHT_SLIDER_pressed();

    void on_SET_AVAIL_LANG_LEFT_SLIDER_pressed();

    void on_SET_AVAIL_LANG_LIST_itemPressed(QListWidgetItem *item);

    void on_pushButton_4_pressed();

    void on_SYSTEM_BUTTON_EQUALIZER_BACK_2_pressed();

    void on_pushButton_46_pressed();

    void on_pushButton_69_pressed();

    void on_SETTINGS_BUTTON_DISPLAY_OFF_pressed();

    void on_SETTINGS_BUTTON_NETWORK_pressed();

    void on_SETTINGS_BUTTON_ADVANCED_pressed();

    void on_SETTINGS_BUTTON_LANGUAGE_pressed();

    void on_SETTINGS_BUTTON_AUDIO_pressed();

    void on_SETTINGS_BUTTON_BACK_pressed();

    void on_pushButton_16_pressed();

    void on_RECIPE_Ingredients_back_pressed();

    void on_RECIPE_Ingredients_next_pressed();

    void on_MM_BUTTON_TIMER_7_clicked();

    void on_MM_BUTTON_TIMER_7_pressed();

    void on_MUSIC_PLAYER_LOOP_clicked();

    void on_MUSIC_PLAYER_DETAILS_clicked();

    void on_DISPLAY_BRIGHTNESS_SLIDER_valueChanged(int value);


    void on_pushButton_5_clicked();

    void on_pushButton_45_clicked();

    void searchOK(QString section,QString searchTerm);


    void on_LCARS_PushButton_12_clicked();

    void on_MUSIC_PLAYER_RANDOM_clicked();

    void on_SETTINGS_BUTTON_ADVANCED_2_clicked();

    void on_SETTINGS_BUTTON_DISPLAY_OFF_2_clicked();

    void on_SETTINGS_BUTTON_ADVANCED_3_clicked();

    void on_SETTINGS_BUTTON_BACK_2_clicked();

    void on_DISPLAY_BRIGHTNESS_SLIDER_2_valueChanged(int value);

    void on_LCARS_PushButton_11_clicked();

    void on_LCARS_PushButton_6_clicked();

    void on_SET_AVAIL_LANG_BACK_clicked();

    void on_SETTINGS_BUTTON_EXTENDED_clicked();

    void on_SET_START_SERVER_2_clicked();

    void on_TIMER_LIST_PREV_clicked();

    void on_TIMER_SOUND_REPEAT_clicked();

    void on_TIMER_SOUND_REPEAT_toggled(bool checked);

    void on_SYSTEM_BUTTON_NOTES_clicked();

    void on_REMOVE_MARKED_RECIPE_clicked();

    void on_RECIPE_MARK_toggled(bool checked);

    void on_SHOW_RECIPE_clicked();

    void on_SET_SERVER_BACK_3_clicked();

    void on_VIDEO_MAXIMIZE_clicked();

    void on_KEYBOARD_GLYPH_LEFT_clicked();

    void on_KEYBOARD_GLYPH_RIGHT_clicked();

    void on_SETTINGS_SERVER_BACK_clicked();

    void on_MEDIA_BUTTON_MUSIC_GENERAL_SETTINGS_clicked();

    void on_MEDIA_BUTTON_VIDEOS_DATABASE_clicked();

    void on_MEDIA_BUTTON_VIDEOS_GENERAL_SETTINGS_clicked();

    void on_SETTINGS_BUTTON_BACK_clicked();

    void on_MM_BUTTON_TIMER_2_clicked();

    void on_SET_SERVER_BACK_clicked();

    void on_SET_AVAIL_LANG_LIST_itemClicked(QListWidgetItem *item);

    void on_TIMER_LIST_ACTIVE_itemClicked(QListWidgetItem *item);

    void on_RECIPE_read_clicked();

    void on_RECIPE_speak_clicked();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
