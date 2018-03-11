#pragma once

#include <QObject>
#include <QSocketNotifier>
#include <iostream>
//#include "db_search.h"
#include "replicator_database_server.h"

class Replicator_Database_Server;

class Console : public QObject
{
    Q_OBJECT

public:
    Console();
    Replicator_Database_Server *repliDB = new Replicator_Database_Server(1701);
    void run();

signals:
    void quit();

private:
    QSocketNotifier *m_notifier;

private slots:
    void readCommand();
};

inline Console::Console()
{
    m_notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
}
