#include "music_db_control.h"
#include "ExifTool.h"
#include <QDebug>
#include <QDir>
#include <QXmlStreamWriter>
#include <QFileDialog>
#include <QDirIterator>
#include <QFileInfo>
#include <QtConcurrent/QtConcurrent>

Music_DB_Control::Music_DB_Control()
{

}
void Music_DB_Control::createDB() {


    ExifTool *et = new ExifTool();

    QStringList MusicDBDirectories;
    MusicDBDirectories.append(QDir::homePath()+"/Laufwerke/Hoerspiele/");

    QVector<QStringList> DBDirs;
    QStringList eins;
    eins.append("Hörspiele");
    eins.append(QDir::homePath()+"/Laufwerke/Hoerspiele/");
    QStringList zwei;
    zwei.append("Musik");
    zwei.append(QDir::homePath()+"/Laufwerke/Musik/");
    DBDirs.append(eins);
    DBDirs.append(zwei);



    QString filename;
    /*filename = QFileDialog::getSaveFileName(this,
                                           tr("Save Xml"), ".",
                                           tr("Xml files (*.xml)"));*/
    filename=QDir::homePath()+"/Dokumente/temp_db.xml";
    QFile file(filename);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("MUSIC_DATABASE");

    foreach (QStringList q, DBDirs) {

    //QDirIterator it(QDir::homePath()+"/Laufwerke/Hoerspiele/Sherlock Holmes/", QStringList() << "*.mp3" << "*.wav" << "*.ogg" << "*.aiff" << "*.flac", QDir::NoFilter, QDirIterator::Subdirectories);
        QDirIterator it(q[1], QStringList() << "*.mp3" << "*.wav" << "*.ogg" << "*.aiff" << "*.flac", QDir::NoFilter, QDirIterator::Subdirectories);



      xmlWriter.writeStartElement("FOLDER");
      xmlWriter.writeAttribute("NAME",QString(q[0]).toUpper()); //foreach bla bla
      xmlWriter.writeAttribute("PATH",q[1]);
    int i=0;
    while (it.hasNext()) {
        it.next();
        {

         xmlWriter.writeStartElement("ENTRY");
         xmlWriter.writeAttribute("ID",QString::number(i));


         QString filen=it.filePath();




         xmlWriter.writeStartElement("DATEN");

         TagInfo *info = et->ImageInfo(filen.toStdString().c_str(),"-artist\n-title\n-album\n-track\n--a\n-e");

         if (info) {
             xmlWriter.writeStartElement("FILE_NAME");
             xmlWriter.writeCDATA(it.fileName());
             xmlWriter.writeEndElement();

             xmlWriter.writeStartElement("FILE_PATH");
             xmlWriter.writeCDATA("/"+it.filePath().remove(it.fileName()).remove(q[1]));
             xmlWriter.writeEndElement();
                 // print returned information
                 for (TagInfo *i=info->next; i; i=i->next) {
                     xmlWriter.writeStartElement(QString(i->name).toUpper());

                     xmlWriter.writeCDATA(QString(i->value));
                     xmlWriter.writeEndElement();
                 }

                 // we are responsible for deleting the information when done

                 delete info;
             } else if (et->LastComplete() <= 0) {
                 qDebug() << "Error executing exiftool!";
             }
         xmlWriter.writeEndElement();

        }



         xmlWriter.writeEndElement();
         qDebug() << i;
         ++i;
    }


    xmlWriter.writeEndElement();
    }


    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    file.close();
}
