#ifndef GLOBALVARS_H
#define GLOBALVARS_H
#include <QStringList>
#include <QDir>

static bool devmode = true;

// 0 = local Desktop
// 1 = raspi
// 2 = android
static int systemSpeed = 0;

static bool redActive;
static QString deploySystem;
static bool redAlertKillMe;
/*static QString musicDB_file = QDir::homePath()+"/replicator-data/databases/music_db.db";*/
static QString musicDB_file = QDir::homePath()+"/replicator-data/databases/music_db.db";
static QString settingsDbFile = QDir::homePath()+"/replicator-data/databases/replicator_settings.db";
extern bool StopTheRedAlert;

static QString searchSection;

static QString compilationTime = QString("%1T%2").arg(__DATE__).arg(__TIME__);

static int menuVolumeGlobal;

extern QString VideoFile;
extern qint64 VideoPosition;

#endif // GLOBALVARS_H


