#include <iostream>
#include "console.h"
//#include "db_search.h"
#include "replicator_database_server.h"

void Console::run()
{
    std::cout << std::endl << "\e[1m" << "Welcome to the replicator database server." << "\e[0m"
              << std::endl << std::endl
              << "Console input is enabled." << std::endl
              << "Type \"" << "\e[1m" << "help" << "\e[0m" <<"\" or \"" << "\e[1m" << "?" << "\e[0m" << "\" for a list of commands." << std::endl;
    std::cout << std::endl << "> " << std::flush;
    connect(m_notifier, SIGNAL(activated(int)), this, SLOT(readCommand()));


}

void Console::readCommand()
{
    std::string line;


    std::getline(std::cin, line);
    if (std::cin.eof() || line == "shutdown" || line =="quit" || line =="exit") {
        std::cout << "Shutting down server. Closing all connections." << std::endl << "Goodbye! Live long and prosper!" << std::endl;
        emit quit();
    }
    else if (std::cin.eof() || line == "help" || line== "?") {
        std::cout << "\e[1m" << std::endl << "LIST OF AVAILABLE COMMANDS:" << "\e[0m" << std::endl << std::endl;
        std::cout << "\e[1m" << "shutdown" << "\e[0m" << std::endl
                  << "         As the name suggests, it will shutdown the server." << std::endl << std::endl;
        std::cout << "\e[1m" << "quit" << "\e[0m" << std::endl
                  << "         Same as shutdown." << std::endl << std::endl;
        std::cout << "\e[1m" << "exit"<<"\e[0m"<< std::endl
                  << "         Also same as shutdown." << std::endl << std::endl;
        std::cout << "\e[1m" << "get \e[0m\e[4msettingname"<<"\e[0m"<< std::endl
                  << "         Show value of the desired setting. Available variables are:" << std::endl
                  << "              \e[1mport\e[0m     It's the current port the server listenes on." << std::endl
                  << "              \e[1mip\e[0m       The current ip on which the server is connected to." << std::endl
                  << "              \e[1mdb\e[0m       The path of the database file." << std::endl
                  << "              \e[1mpid\e[0m      The pid of the server bin." << std::endl
                  << "              \e[1mclients\e[0m  Shows a list of connected clients." << std::endl
                  << std::endl;
        std::cout << "\e[1m" << "set \e[0m\e[4msettingname=value"<<"\e[0m"<< std::endl
                  << "         Sets the value of the desired setting. Available variables are:" << std::endl
                  << "              \e[1mport\e[0m It's the current port the server listenes on." << std::endl
                  << "              \e[1mdb\e[0m   The path of the database file." << std::endl
                  << std::endl;
        std::cout << "\e[1m" << "sql \e[0m\e[4mstatement"<<"\e[0m"<< std::endl
                  << "         Executes an SQLite query statement on the recipe database and returns a QVector." << std::endl
                  << "         Just type the statement without any quotes etc. around it." << std::endl
                  << "         Example:" << std::endl
                  << "              \e[1msql SELECT `id`,`title` FROM recipes\e[0m" << std::endl
                  << std::endl;
        std::cout << "\e[1m" << "clear" << "\e[0m" << std::endl
                  << "         Clears the console screen." << std::endl << std::endl;
        std::cout << "> " << std::flush;
    } // (QString(argv[1]).contains(QRegExp("^(?:http://|www)")))
    else if (std::cin.eof() || (QString(line.c_str()).contains(QRegExp("^(?:get)"))) ){
        QString variable = QString(line.c_str()).remove(0,4);
        qDebug() << "getting" << variable;
        std::cout << "> " << std::flush;
    }
    else if (std::cin.eof() || (QString(line.c_str()).contains(QRegExp("^(?:set)"))) ){
        QString variable = QString(line.c_str()).remove(0,4).section('=',0,0);
        QString value = QString(line.c_str()).remove(0,4).section('=',1);
        qDebug() << "setting" << variable <<  value;
        std::cout << "> " << std::flush;
    }
    else if (std::cin.eof() || (QString(line.c_str()).contains(QRegExp("^(?:sql)"))) ){

        std::cout << "executing query: \e[1m" << QString(line.c_str()).remove(0,4).toStdString() << "\e[0m" << std::endl << std::endl;
        //qDebug() << db_Search->get_Data(QDir::homePath()+"/replicator-data/databases/replicator_recipes.db",QString(line.c_str()).remove(0,4));
        qDebug() << repliDB->localQuery(QString(line.c_str()).remove(0,4));
        std::cout << std::endl << "> " << std::flush;
    }
    else if (std::cin.eof() || line == "clear") {
        if (system("CLS")) system("clear");
        std::cout << "> " << std::flush;
    }
    else {
        std::cout << "No such command: \"" << line << "\"" << std::endl;
        QString input = line.c_str();
        //db_Search->console_exec(input);
        std::cout << "> " << std::flush;
    }
}
