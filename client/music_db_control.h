#ifndef MUSIC_DB_CONTROL_H
#define MUSIC_DB_CONTROL_H
#include <QDir>

class Music_DB_Control: public QObject
{
    Q_OBJECT
public:
    Music_DB_Control();

    void createDB();
};

#endif // MUSIC_DB_CONTROL_H
