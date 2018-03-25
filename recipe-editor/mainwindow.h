#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "selectpicture.h"
#include <QWebSocket>
#include <QSettings>
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    SelectPicture *selectPicDialog;
    QWebSocket *m_webSocket;
    QVector<QStringList> replicationData;
    QString fileName;
    QSettings settings;
    QString serverPath;
    QString serverPort;
    QString serverIP;
    QString local_DB;
    QString serverURL;
    QString server_PID;
    bool useLocalDb;

     QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","recipeDB");

private slots:
    void on_pushButton_6_clicked();

    void on_add_ingredient_clicked();

    void on_remove_ingredient_clicked();

    void on_connectServer_clicked();

    void onConnected();

    void onDisconnected();

    void onTextMessageReceived(QString message);

    void onBinaryMessageRecieved(QByteArray message);

    void processData(QString searchString);

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

        void populateRecipeList(QVector<QStringList> result);

        void on_pushButton_clicked();

        void loadRecipe(QVector<QStringList> result);

        void on_pushButton_7_clicked();

        void on_pushButton_2_clicked();

        void on_pushButton_8_clicked();

        void on_pushButton_3_clicked();

        void on_recipe_search_returnPressed();

        void on_pushButton_10_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
