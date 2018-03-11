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
  #include <QtCore/QCoreApplication>
  #include "replicator_database_server.h"
  #include "db_query.h"
  #include "globalvars.h"
  #include <QDebug>
  #include <iostream>
  #include "console.h"

  int main(int argc, char *argv[])
  {
      QCoreApplication a(argc, argv);

      QStringList args = a.arguments();
      bool inputConsole = false;
      foreach (QString arg, args) {

          QStringList argument = arg.split("-");
          if (argument.last()=="c" || argument.last()=="C") {
              inputConsole=true;
          } else if (argument.last()=="?" || argument.last()=="help") {
              std::cout << "\e[1m" << "Available commandline arguments are:" << "\e[0m" << std::endl
                        << std::endl
                        << "\e[1m" << "-c" << "\e[0m" << "," << "\e[1m" << " -C" << "\e[0m" << "     Enables a console for user-input."
                        << std::endl << std::endl;

            return 1;
          }

      }

      if (inputConsole == true) {
          Replicator_Database_Server server(1701);
          QString compilationTime = QString("%1T%2").arg(__DATE__).arg(__TIME__);
          qDebug() << "Compilation timestamp: " << compilationTime << "\e[1m" <<"\n" ;

          Console console;
          console.run();
          QObject::connect(&console, SIGNAL(quit()), &a, SLOT(quit()));
          return a.exec();
      } else {
          Replicator_Database_Server server(1701);
          QString compilationTime = QString("%1T%2").arg(__DATE__).arg(__TIME__);
          qDebug() << "Compilation timestamp: " << compilationTime << "\e[1m" <<"\n" ;

          qDebug() << "Welcome to the replicator database server." << "\e[0m" << "\n";
          qDebug() << "Console input is disabled.";
          return a.exec();
      }

  }
