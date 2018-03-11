#ifndef EXTRA_FUNCTIONS_H
#define EXTRA_FUNCTIONS_H
#include <QtGui>
#include <QLabel>
#include <QVector>

class extra_functions
{
public:
    extra_functions();
    static void delay(int millisecondsToWait);
    static QVector<QVariant> checkCode(QStringList code, QStringList code_temp, QString code_append, QObject *sender, QVector<QObject*> *codeButtons);
    static QImage createImageWithOverlay(QImage& baseImage, QImage& overlayImage,
                                         int referenceImage=0,
                                         int width={}, int height={},
                                         int baseWidth={}, int baseHeight={},
                                         int overlayWidth={},int overlayHeight={},
                                         int baseX={}, int baseY={},
                                         int overlayX={}, int overlayY={},
                                         bool centerBaseX=false,bool centerBaseY=false,
                                         bool centerOverlayX=false,bool centerOverlayY=false);
    static QVector<QVector<QWidget *>> WidgetReturnVector(QList<QWidget *> widgetList, QString id_type);


};

#endif // EXTRA_FUNCTIONS_H
