#include "loadingscreen.h"
#include "ui_loadingscreen.h"

LoadingScreen::LoadingScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadingScreen)
{
    ui->setupUi(this);
}

LoadingScreen::~LoadingScreen()
{
    delete ui;
}
