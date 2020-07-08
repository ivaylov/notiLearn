#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <memory>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    QTimer *timer;

private slots:
    void on_addButton_clicked();

    void on_openButton_clicked();

    void on_saveasButton_clicked();

    void on_deleteButton_clicked();

    void on_clearButton_clicked();

    void on_notifyButton_clicked();

    void on_horizontalSliderMinute_valueChanged(int value);

    void on_horizontalSliderDelay_valueChanged(int value);

    void on_wordList_doubleClicked(const QModelIndex &index);

    void on_checkBoxtrayIcon_stateChanged(int arg1);

    void on_exitButton_clicked();

    void on_minimizeButton_clicked();

private:
    Ui::MainWindow *ui;

private:
    QString currentFile;
    QSystemTrayIcon* trayIcon;
    QMenu* trayIconMenu;
    QMenu* createMenu();
    QSettings *settings;


public slots:
    void TimerSlot();
    void iconActivated(QSystemTrayIcon::ActivationReason);
    void writeSettings();
    void readSettings();

protected:
     void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
