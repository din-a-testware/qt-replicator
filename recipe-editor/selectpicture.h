#ifndef SELECTPICTURE_H
#define SELECTPICTURE_H

#include <QDialog>

namespace Ui {
class SelectPicture;
}

class SelectPicture : public QDialog
{
    Q_OBJECT

public:
    explicit SelectPicture(QWidget *parent = 0);
    ~SelectPicture();
    QString fileName;

    QString getPicturePath() const;

private slots:
    void on_choosePicture_clicked();

private:
    Ui::SelectPicture *ui;
};

#endif // SELECTPICTURE_H
