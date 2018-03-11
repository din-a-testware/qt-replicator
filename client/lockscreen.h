#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#include <QWidget>
#include <QMediaPlayer>
#include <QFontDatabase>

namespace Ui {
class LockScreen;
}

class LockScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LockScreen(QWidget *parent = 0);
    ~LockScreen();
    QFontDatabase LCARS_fonts;
    QStringList code;
    QStringList code_temp;
    QMediaPlayer *menuBeeper;
    QStringList codeString;
    QVector<QObject*> *codeButtons;

private slots:
    void on_MovingButton_1_clicked();

    void on_MovingButton_2_clicked();

    void on_MovingButton_3_clicked();

    void on_MovingButton_5_clicked();

    void processCodeResult(int resultValue);

    void on_BUTTON_ALPHA_clicked();

    void on_BUTTON_BETA_clicked();

    void on_BUTTON_GAMMA_clicked();

    void on_BUTTON_DELTA_clicked();

private:
    Ui::LockScreen *ui;
};

#endif // FORM_H
