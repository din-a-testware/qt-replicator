#include "selectpicture.h"
#include "ui_selectpicture.h"
#include <QDir>
#include <QFileDialog>
#include <QDebug>

SelectPicture::SelectPicture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectPicture)
{
    ui->setupUi(this);
}

SelectPicture::~SelectPicture()
{
    delete ui;
}

void SelectPicture::on_choosePicture_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,
         tr("Open Image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));

    if (fileName!="") {
        ui->pictureLabel->setPixmap(QPixmap(fileName));
    }

}

QString SelectPicture::getPicturePath() const {
    return fileName;
}
