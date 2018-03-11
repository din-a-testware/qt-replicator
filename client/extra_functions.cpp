
#include "extra_functions.h"
#include <QTimer>
#include <QtGui>
#include <QLabel>
#include "globalvars.h"
#include <QMediaPlayer>
#include <QPushButton>

extra_functions::extra_functions()
{
    Q_UNUSED(systemSpeed);
    Q_UNUSED(devmode);
    Q_UNUSED(redActive);
    Q_UNUSED(redAlertKillMe);
}


void extra_functions::delay(int millisecondsToWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsToWait);
    loop.exec();
}


QVector<QVariant> extra_functions::checkCode(QStringList code, QStringList code_temp, QString code_append, QObject *sender, QVector<QObject*> *codeButtons) {

    QPushButton *senderButton = qobject_cast<QPushButton *>(sender);

    QVector<QVariant> vary;
    //qDebug() << code_temp.size();
    //qDebug() << code_temp;

    if (code_temp.size() < 4) {

        //qDebug() << "NÖ, WEITER";

        //sender->setProperty("styleSheet",sender->property("css_white").toString()); b9a952
        senderButton->setProperty("Center_Color","#ffffff");
        senderButton->repaint();


        vary.append(0);
        vary.append(code_append);

    } else if (code_temp.size() == 4) {
        if (code[0] == code_temp[0] &&
            code[1] == code_temp[1] &&
            code[2] == code_temp[2] &&
            code[3] == code_temp[3]
            )
        {
            for (int i=0;i<codeButtons->size();i++) {
                //codeButtons->at(i)->setProperty("styleSheet",sender->property("css_correct").toString());
                QPushButton *senderButtonTemp = qobject_cast<QPushButton *>(codeButtons->at(i));
                senderButtonTemp->setProperty("Center_Color","#00aa00");
                senderButtonTemp->repaint();
            }

            //qDebug() << codeButtons->size();
            //qDebug() << "YES!";
            code_append.clear();
            vary.append(2);
            vary.append(code_append);
            //sender->setProperty("styleSheet",sender->property("css_correct").toString());
            sender->setProperty("Center_Color","#00aa00");





        } else if (code[0] != code_temp[0] ||
                   code[1] != code_temp[1] ||
                   code[2] != code_temp[2] ||
                   code[3] != code_temp[3]) {

            //qDebug() << "NÖ, RESET";
            for (int i=0;i<codeButtons->size();i++) {
                //codeButtons->at(i)->setProperty("styleSheet",sender->property("css_std").toString());
                QPushButton *senderButtonTemp = qobject_cast<QPushButton *>(codeButtons->at(i));
                senderButtonTemp->setProperty("Center_Color","#c8181c");
                senderButtonTemp->repaint();
            }
            vary.clear();
            vary.append(1);
            code_append.clear();
            vary.append(code_append);
        }
    }

    return vary;
}

QImage extra_functions::createImageWithOverlay(QImage& baseImage, QImage& overlayImage,
                                               int referenceImage,
                                               int width, int height,
                                               int baseWidth, int baseHeight,
                                               int overlayWidth,int overlayHeight,
                                               int baseX, int baseY,
                                               int overlayX, int overlayY,
                                               bool centerBaseX,bool centerBaseY,
                                               bool centerOverlayX,bool centerOverlayY)
{

    /* MAINLY I USE IT TO CREATE THE FLAG IMAGES IN THE LANGUAGE SETTINGS TAB.
     * THEY ARE MADE OF THE UFP OVERLAY AND THE FLAG ON THE BACK.
     * baseImage IS THE FLAG (THE LANG CODE IS USED),
     * overlayImage IS THE UFP OVERLAY.
     * THE SIZES FOR THE FLAGS ARE SPECIFIED IN THE CALL OF THE FUNCTION.
     * THE FLAG IS 130PX IN HEIGHT, THE WIDTH IS CALCULATED.
     * IT'S 16PX FROM THE BOTTOM TO HAVE IT IN THE VERTICAL CENTER OF THE UFP OVERLAY
     * AND IT'S CENTERED HORIZONTALLY.
     * YES IT HAS MANY PARAMETERS, BUT I DID NOT WANT TO HAVE AN OVERKILL OF OVERLOADED FUNCTIONS.
     */

    QImage refImage;

    if (baseImage.isNull() || overlayImage.isNull()) {

        QImage noOverlay = QImage(":/images/UFP-Logo.png");
        return noOverlay;
    } else {

        if (referenceImage==0) {
            refImage = QImage(baseImage);
        } else if (referenceImage==1) {
            refImage = QImage(overlayImage);
        } else {
            refImage = QImage(baseImage);
        }
        //** TODO: GET RID OF NULL
        if (!width) {
            width=refImage.width();
        }

        if (!height) {
            height=refImage.height();
        }
        if (!baseWidth) {
            baseWidth=baseImage.width();
        }
        if (!baseHeight) {
            baseHeight=baseImage.height();
        }
        if (!overlayWidth) {
            overlayWidth=overlayImage.width();
        }
        if (!overlayHeight) {
            overlayHeight = overlayImage.height();
        }
        if (!baseX) {
            baseX = 0;
        }
        if (!baseY) {
            baseY = 0;
        }
        if (!overlayX) {
            overlayX=0;
        }
        if (!overlayY) {
            overlayY=0;
        }

        QImage imageWithOverlay = QImage(refImage.width(),refImage.height(),QImage::Format_ARGB32_Premultiplied);
        QImage baseImageScaled = QImage(baseImage.scaled(baseWidth,baseHeight,Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

        if (centerBaseX==true) {
            baseX = (refImage.width()/2)-(baseImageScaled.width()/2);
        }
        if (centerBaseY==true) {
            baseY = (refImage.height()/2)-(baseImageScaled.height()/2);
        }
        if (centerOverlayX==true) {
            // TODO: overlayX = (refImage.width()/2)-(baseImageScaled.width()/2);
        }
        if (centerOverlayY==true) {
            // TODO: baseX = (refImage.width()/2)-(baseImageScaled.width()/2);
        }


        QPainter painter(&imageWithOverlay);

        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(imageWithOverlay.rect(), Qt::transparent);

        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        //qDebug() << (overlayImage.width()/2)-(baseImageScaled.width()/2);
        painter.drawImage(baseX, baseY, baseImageScaled);

        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.drawImage(overlayX, overlayY, overlayImage);

        painter.end();

        return imageWithOverlay;
    }



/*    if (baseImage.isNull() || overlayImage.isNull()) {

        QImage noOverlay = QImage(":/images/UFP-Logo.png");
        return noOverlay;
    } else {
        QImage imageWithOverlay = QImage(overlayImage.width(),overlayImage.height(), QImage::Format_ARGB32_Premultiplied);
        QImage baseImageScaled = QImage(baseImage.scaled(130,130,Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        QPainter painter(&imageWithOverlay);

        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(imageWithOverlay.rect(), Qt::transparent);

        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        qDebug() << (overlayImage.width()/2)-(baseImageScaled.width()/2);
        painter.drawImage((overlayImage.width()/2)-(baseImageScaled.width()/2), 16, baseImageScaled);

        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.drawImage(0, 0, overlayImage);

        painter.end();

        return imageWithOverlay;
    }*/
}

QVector<QVector<QWidget *>> extra_functions::WidgetReturnVector(QList<QWidget *> widgetList, QString id_type) {


    QList< QWidget *> all_widgets1 = widgetList;
    QList<QWidget *> alarm_id_widgets1;
    QVector<QVector< QWidget *> >  red_alert_widgets1;


    for (int i = 0; i <all_widgets1.count(); i++) {
        if (all_widgets1[i]->property(id_type.toLatin1()).isValid()) {
            alarm_id_widgets1.append(all_widgets1[i]);
        }
    }


    red_alert_widgets1.resize(1);

    int vectorCount1 =0;

    for (int i = 0; i <alarm_id_widgets1.count(); i++) {
        if (vectorCount1 <= alarm_id_widgets1[i]->property(id_type.toLatin1()).toInt()) {
            vectorCount1 = alarm_id_widgets1[i]->property(id_type.toLatin1()).toInt()+1;
        }
    }

    red_alert_widgets1.resize(vectorCount1);

    foreach (QWidget *q, all_widgets1) {

        if (q->property(id_type.toLatin1()).isValid()) {
            red_alert_widgets1[q->property(id_type.toLatin1()).toInt()] += q;
        }
    }
    return red_alert_widgets1;
}
