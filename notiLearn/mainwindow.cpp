#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addnewdialog.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>
#include <memory>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>
#include <memory>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QCloseEvent>
#include <QtWidgets>

static int slidervalue;
static int sliderdelay = 10000;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);

    trayIcon = new QSystemTrayIcon(this);
    auto menu = this->createMenu();
    trayIcon->setContextMenu(menu);
    trayIcon->setIcon(QIcon::fromTheme("edit-paste"));
    trayIcon->show();
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    settings = new QSettings("settings.txt",QSettings::IniFormat,this);


    connect(ui->clearButton, SIGNAL(clicked(bool)), this, SLOT(clearList()));
    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addTodoClicked()));
    connect(ui->wordList, SIGNAL(itemSelectionChanged()), this, SLOT(selectedItemChanged()));
    connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteClicked()));
    connect(ui->actionOpenFromHere, SIGNAL(triggered(bool)), this, SLOT(openFromHereClicked()));
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openClicked()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(saveClicked()));
    connect(ui->actionSaveAs, SIGNAL(triggered(bool)), this, SLOT(saveAsClicked()));

    connect(ui->clearButton, SIGNAL(clicked(bool)), this, SLOT(selectedItemChanged()));
    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(selectedItemChanged()));
    connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(selectedItemChanged()));
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(selectedItemChanged()));

    connect(timer, SIGNAL(timeout()),this, SLOT(TimerSlot()));

    slidervalue = ui->horizontalSliderMinute->value();
    int valslidmin = slidervalue * 1000 * 60;
    timer->start(valslidmin);

    int intvalue = ui->horizontalSliderMinute->value();
    QString stringvalue = QString::number(intvalue);
    QString textmin = "Minutes: " + QString::number( intvalue );
    ui->labelMinutes->setText(textmin.toStdString().c_str());

    int intdelay = ui->horizontalSliderDelay->value();
    QString stringdelay = QString::number(intdelay);
    QString textdelay = "Milliseconds: " + QString::number( intdelay );
    ui->labelDelay->setText(textdelay.toStdString().c_str());


    QString fileName = "wordlist.txt";
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {

        QSystemTrayIcon* ic = new QSystemTrayIcon(this);
        ic->setVisible(true);
        ic->showMessage(tr("notilearn - open file"), tr(file.errorString().toStdString().c_str()), QSystemTrayIcon::Information, sliderdelay);
        ic->setVisible(false);
        qApp->processEvents();

        return;
    }
    QTextStream stream(&file);
    QString buffer;

    while (!stream.atEnd())
    {
        buffer = stream.readLine(256);
        ui->wordList->addItem(buffer);
    }
    file.close();

    readSettings();
}

QMenu* MainWindow::createMenu()
{
  // App can exit via Quit menu
  auto quitAction = new QAction("&Quit", this);
  connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

  auto menu = new QMenu(this);
  menu->addAction(quitAction);

  return menu;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason_)
{
  switch (reason_) {
  case QSystemTrayIcon::Trigger:
    this->setVisible(!isVisible());
    break;
  default:
    ;
  }
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    QString fileName = "wordlist.txt";
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {

        QSystemTrayIcon* ic = new QSystemTrayIcon(this);
        ic->setVisible(true);
        ic->showMessage(tr("notilearn - save file"), tr(file.errorString().toStdString().c_str()), QSystemTrayIcon::Information, sliderdelay);
        ic->setVisible(false);
        qApp->processEvents();

        return;
    }
    currentFile = fileName;
    QTextStream out(&file);
    int listLength = ui->wordList->count();
    for (int i = 0; i < listLength; ++i)
    {
        out << ui->wordList->item(i)->text() << '\n';
    }
    file.close();

    timer->stop();
    timer->destroyed();
    QApplication::quit();

    writeSettings();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeSettings()
{
    settings->setValue( "geometry", saveGeometry() );
    settings->setValue( "savestate", saveState() );
    settings->setValue( "maximized", isMaximized() );
    settings->setValue( "minimized", isMinimized());
    settings->setValue( "trayIcon", trayIcon->isVisible());
    settings->setValue("checkboxautonext",ui->checkBoxautonext->isChecked());
    settings->setValue("checkboxtrayIcon",ui->checkBoxtrayIcon->isChecked());
    settings->setValue("checkboxrandom",ui->checkBoxrandom->isChecked());
    settings->setValue("minutes",ui->horizontalSliderMinute->value());
    settings->setValue("delay",ui->horizontalSliderDelay->value());
}

void MainWindow::readSettings()
{
    restoreGeometry(settings->value( "geometry", saveGeometry() ).toByteArray());
    restoreState(settings->value( "savestate", saveState() ).toByteArray());
    move(settings->value( "pos", pos() ).toPoint());
    resize(settings->value( "size", size() ).toSize());
    if ( settings->value( "maximized", isMaximized() ).toBool() )
        showMaximized();
    if ( settings->value( "minimized", isMinimized() ).toBool() )
        showMinimized();
    ui->checkBoxtrayIcon->setChecked(settings->value("checkboxtrayIcon").toBool());
    ui->checkBoxautonext->setChecked(settings->value("checkboxautonext").toBool());
    ui->checkBoxrandom->setChecked(settings->value("checkboxrandom").toBool());
    ui->horizontalSliderMinute->setValue(settings->value("minutes").toInt());
    ui->horizontalSliderDelay->setValue(settings->value("delay").toInt());
}

void MainWindow::on_addButton_clicked()
{
    QString todoDesc;
    auto dialog = std::make_unique<AddNewDialog>(todoDesc);
    dialog->show();
    dialog->exec();
    ui->wordList->addItem(todoDesc);
}

void MainWindow::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {

        QSystemTrayIcon* ic = new QSystemTrayIcon(this);
        ic->setVisible(true);
        ic->showMessage(tr("notilearn - open file"), tr(file.errorString().toStdString().c_str()), QSystemTrayIcon::Information, sliderdelay);
        ic->setVisible(false);
        qApp->processEvents();

        return;
    }
    QTextStream stream(&file);
    QString buffer;

    while (!stream.atEnd())
    {
        buffer = stream.readLine(256);
        ui->wordList->addItem(buffer);
    }
    file.close();

}

void MainWindow::on_saveasButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {


        QSystemTrayIcon* ic = new QSystemTrayIcon(this);
        ic->setVisible(true);
        ic->showMessage(tr("notilearn - save file"), tr(file.errorString().toStdString().c_str()), QSystemTrayIcon::Information, sliderdelay);
        ic->setVisible(false);
        qApp->processEvents();

        return;
    }
    currentFile = fileName;
    QTextStream out(&file);
    int listLength = ui->wordList->count();
    for (int i = 0; i < listLength; ++i)
    {
        out << ui->wordList->item(i)->text() << '\n';
    }
    file.close();
}

void MainWindow::on_deleteButton_clicked()
{
    if (ui->wordList->currentItem() == nullptr)
    {
        qDebug() << "Tried to delete with no item selected, bug!";
        return;
    }

    auto takenItem = ui->wordList->takeItem(ui->wordList->currentRow());
    delete takenItem;
}

void MainWindow::on_clearButton_clicked()
{
    ui->wordList->clear();
}

void MainWindow::on_notifyButton_clicked()
{
    int currentIndex = ui->wordList->currentRow();
    ui->wordList->setCurrentRow(currentIndex+1);

    if(ui->wordList->currentItem()){
        QString str = ui->wordList->currentItem()->text();
        QSystemTrayIcon* ic = new QSystemTrayIcon(this);
        ic->setVisible(true);
        ic->showMessage(tr("notilearn"), tr(str.toStdString().c_str()), QSystemTrayIcon::Information, sliderdelay);
        ic->setVisible(false);
        qApp->processEvents();
    }
}

void MainWindow::on_horizontalSliderMinute_valueChanged(int value)
{
    int intvalue = value;
    QString stringvalue = QString::number(intvalue);
    QString text = "Minutes: " + QString::number( intvalue );
    ui->labelMinutes->setText(text.toStdString().c_str());

    slidervalue = ui->horizontalSliderMinute->value();
    int valslidmin = slidervalue * 1000 * 60;
    timer->setInterval(valslidmin);
    slidervalue = value;

}


void MainWindow::on_horizontalSliderDelay_valueChanged(int value)
{
    int intdelay = ui->horizontalSliderDelay->value();
    QString stringdelay = QString::number(intdelay);
    QString textdelay = "Milliseconds: " + QString::number( intdelay );
    ui->labelDelay->setText(textdelay.toStdString().c_str());

    sliderdelay = ui->horizontalSliderMinute->value();
    sliderdelay = value;
}

void MainWindow::TimerSlot(){

    slidervalue = ui->horizontalSliderMinute->value();
    int valslidmin = slidervalue * 1000 * 60;
    timer->setInterval(valslidmin);
    if(ui->checkBoxautonext->isChecked())
    {
        if(ui->wordList->currentItem()){
            if(ui->checkBoxrandom->isChecked()){
                int wordlistcount = ui->wordList->count();
                int rand = QRandomGenerator::global()->bounded(wordlistcount);
                ui->wordList->setCurrentRow(rand);
                QString str = ui->wordList->currentItem()->text();
                QSystemTrayIcon* ic = new QSystemTrayIcon(this);
                ic->setVisible(true);
                ic->showMessage(tr("notilearn"), tr(str.toStdString().c_str()), QSystemTrayIcon::Information, sliderdelay);
                ic->setVisible(false);
                qApp->processEvents();
            }
            else{
                int currentIndex = ui->wordList->currentRow();
                ui->wordList->setCurrentRow(currentIndex+1);
                QString str = ui->wordList->currentItem()->text();
                QSystemTrayIcon* ic = new QSystemTrayIcon(this);
                ic->setVisible(true);
                ic->showMessage(tr("notilearn"), tr(str.toStdString().c_str()), QSystemTrayIcon::Information, sliderdelay);
                ic->setVisible(false);
                qApp->processEvents();
            }
        }
    }
}


void MainWindow::on_wordList_doubleClicked(const QModelIndex &index)
{
    QString str = ui->wordList->currentItem()->text();
    QSystemTrayIcon* ic = new QSystemTrayIcon(this);
    ic->setVisible(true);
    ic->showMessage(tr("notilearn"), tr(str.toStdString().c_str()), QSystemTrayIcon::Information, sliderdelay);
    ic->setVisible(false);
    qApp->processEvents();
}

void MainWindow::on_checkBoxtrayIcon_stateChanged(int arg1)
{
    if(ui->checkBoxtrayIcon->isChecked()){
        trayIcon->show();
    }
    else{
        trayIcon->hide();

    }

}
