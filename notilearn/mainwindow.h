#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <memory>
#include <QTimer>

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

    void on_notify_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_wordList_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

private:
    QString currentFile;


public slots:
    void TimerSlot();

protected:
     void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
