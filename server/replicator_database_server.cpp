/****************************************************************************
 **
 ** Copyright (C) 2016 Kurt Pattyn <pattyn.kurt@gmail.com>.
 ** Contact: https://www.qt.io/licensing/
 **
 ** This file is part of the QtWebSockets module of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** Commercial License Usage
 ** Licensees holding valid commercial Qt licenses may use this file in
 ** accordance with the commercial license agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and The Qt Company. For licensing terms
 ** and conditions see https://www.qt.io/terms-conditions. For further
 ** information use the contact form at https://www.qt.io/contact-us.
 **
 ** BSD License Usage
 ** Alternatively, you may use this file under the terms of the BSD license
 ** as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of The Qt Company Ltd nor the names of its
 **     contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/
 #include "replicator_database_server.h"
 #include "QtWebSockets/QWebSocketServer"
 #include "QtWebSockets/QWebSocket"
 #include <QtCore/QDebug>
 #include <QtCore/QFile>
 #include <QtNetwork/QSslCertificate>
 #include <QtNetwork/QSslKey>
 #include "db_search.h"
 #include <QDataStream>
 #include <QCoreApplication>
 #include <QtSql>
#include <QNetworkInterface>
#include <QTextStream>
#include <iostream>

 QT_USE_NAMESPACE

 Replicator_Database_Server::Replicator_Database_Server(quint16 port, QObject *parent) :
     QObject(parent),
     m_pWebSocketServer(Q_NULLPTR)
 {



     /*foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
                 if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
                      qDebug() << address.toString();

             }*/

     QList<QNetworkInterface> allInterfaces = QNetworkInterface::allInterfaces();
         foreach(QNetworkInterface iFace, allInterfaces) {

             foreach (const QHostAddress &address, iFace.allAddresses()) {
                         if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))

                             //qDebug() << "name:" << iFace.humanReadableName() << "hw:" << iFace.hardwareAddress() << "all: " <<iFace.allAddresses()<< "up:" <<iFace.IsUp;
                              //qDebug() << "add:" <<  address.toString();

                             qDebug() << "name:" << iFace.humanReadableName() << "hw:" << iFace.hardwareAddress() <<"add:" <<  address.toString() << "up:" << iFace.IsUp;

                     }
         }



     quint16 portServer;

     QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

     db.setDatabaseName(QDir::homePath()+"/replicator-data/databases/replicator_server_settings.db");

     if(db.open()){
         QSqlQuery query("SELECT `ip`,`port`,`protocol`,`db_file`,`db_path` FROM `settings` WHERE `id` = '0'");
          if (query.lastError().isValid()) {};
          query.exec();

          while (query.next()) {

              portServer=query.value(1).toInt();
              dbfile=query.value(4).toString()+query.value(3).toString();
              //qDebug() << dbfile;

              }
     }
     else{};


     db.close();

     m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Replicator Server"),
                                               QWebSocketServer::NonSecureMode,
                                               this);

     if (m_pWebSocketServer->listen(QHostAddress::Any, portServer))
     {

         qDebug() << "Replicator database server listening on port" << portServer;
         qDebug() << QCoreApplication::applicationPid();

         connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                 this, &Replicator_Database_Server::onNewConnection);

         connect(m_pWebSocketServer, &QWebSocketServer::sslErrors,
                 this, &Replicator_Database_Server::onSslErrors);
     }




 }

 Replicator_Database_Server::~Replicator_Database_Server()
 {
     m_pWebSocketServer->close();
     qDeleteAll(m_clients.begin(), m_clients.end());
 }

 void Replicator_Database_Server::onNewConnection()
 {

     QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

     qDebug() << "Client connected:" << pSocket->peerName() << pSocket->origin();

     connect(pSocket, &QWebSocket::textMessageReceived, this, &Replicator_Database_Server::processTextMessage);
     connect(pSocket, &QWebSocket::binaryMessageReceived,
             this, &Replicator_Database_Server::processBinaryMessage);
     connect(pSocket, &QWebSocket::disconnected, this, &Replicator_Database_Server::socketDisconnected);
     pSocket->sendTextMessage("Access to Library Computer Acces and Retrieval System granted!");
     m_clients << pSocket;
 }

 QVector<QStringList> Replicator_Database_Server::localQuery(QString queryString) {
     QVector<QStringList> returnSQL;
        db_Search->get_Data(dbfile,queryString);
     return returnSQL;
 }

 void Replicator_Database_Server::processTextMessage(QString message)
 {

     QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
     if (pClient)
     {
         QStringList messageSplitted = message.split("::");
         if (messageSplitted.at(0) == "SYSTEM") {
            //qDebug() << "system message recieved";
            //qDebug() << messageSplitted.at(1);
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
             db.setDatabaseName(QDir::homePath()+"/replicator-data/databases/replicator_server_settings.db");
             if(db.open()){
                 QSqlQuery query(messageSplitted.at(1));
                 if (query.lastError().isValid()) {} else {qDebug() << query.lastError().text();};
                  query.exec();
             } else { qDebug() << "fehler" << db.lastError().text(); }
             db.close();
        }
         else if (messageSplitted.at(0)=="ping") {
             QString appPID = QString::number(QCoreApplication::applicationPid());
             pClient->sendTextMessage("pid:"+appPID);
         } else if (messageSplitted.at(0)=="shutdown") {
             std::cout << "Shutting down server. Closing all connections." << std::endl << "Goodbye! Live long and Prosper!";
             QCoreApplication::quit();
         }
         else if (messageSplitted.at(0)=="MARKED"){
             QVector<QStringList> returnSQL;

             QStringList returnType;
                     returnType.append("MARKED");
             returnSQL.append(returnType);
             returnSQL.append(db_Search->get_Data(dbfile,messageSplitted.at(1)));
             pClient->sendBinaryMessage(serialize(returnSQL));
         }
         else if (messageSplitted.at(0)=="NOTES"){
             QVector<QStringList> returnSQL;

             QStringList returnType;
                     returnType.append("NOTES");
             returnSQL.append(returnType);
             returnSQL.append(db_Search->get_Data(dbfile,messageSplitted.at(1)));
             pClient->sendBinaryMessage(serialize(returnSQL));
         }
         else if (messageSplitted.at(0)=="RECIPE_LIST") {
             QVector<QStringList> returnSQL;

             QStringList returnType;
             returnType.append("RECIPE_LIST");
             returnSQL.append(returnType);
             returnSQL.append(db_Search->get_Data(dbfile,messageSplitted.at(1)));
             pClient->sendBinaryMessage(serialize(returnSQL));
         }

         else if (messageSplitted.at(0)=="RECIPE") {
             QVector<QStringList> returnSQL;

             QStringList returnType;
             returnType.append("RECIPE");
             returnSQL.append(returnType);
             returnSQL.append(db_Search->get_Data(dbfile,messageSplitted.at(1)));
             pClient->sendBinaryMessage(serialize(returnSQL));
         } else if (messageSplitted.at(0)=="UPDATE") {
             db_Search->get_Data(dbfile,messageSplitted.at(1));
         }

     }

 }

 void Replicator_Database_Server::processBinaryMessage(QByteArray message)
 {
 }

 void Replicator_Database_Server::socketDisconnected()
 {
     qDebug() << "Client disconnected";
     QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
     if (pClient)
     {
         m_clients.removeAll(pClient);
         pClient->deleteLater();
     }
 }

 void Replicator_Database_Server::onSslErrors(const QList<QSslError> &)
 {
     qDebug() << "Ssl errors occurred";
 }


 QVector<QStringList> Replicator_Database_Server::deserialize(const QByteArray& byteArray)
 {
     QVector<QStringList> data;
     QDataStream stream(byteArray);
         stream.setVersion(QDataStream::Qt_4_5);

         stream >> data;
         return data;
 }

 QByteArray Replicator_Database_Server::serialize(QVector<QStringList> data)
 {
     QByteArray byteArray;
     QDataStream out(&byteArray, QIODevice::WriteOnly);
     out.setVersion(QDataStream::Qt_4_5);
     out << data;
     return byteArray;
 }
