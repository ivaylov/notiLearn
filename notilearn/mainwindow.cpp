#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addnewdialog.h"
#include "placeholder.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>
#include <memory>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QCloseEvent>


static int slidervalue;
static int sliderdelay = 10000;


MainWindow::MainWindow(const QString& filename, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);


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

    slidervalue = ui->horizontalSlider->value();
    int valslidmin = slidervalue * 1000 * 60;
    timer->start(valslidmin);

    int intvalue = ui->horizontalSlider->value();
    QString stringvalue = QString::number(intvalue);
    QString textmin = "Minutes: " + QString::number( intvalue );
    ui->labelMinutes->setText(textmin.toStdString().c_str());

    int intdelay = ui->horizontalSlider_2->value();
    QString stringdelay = QString::number(intdelay);
    QString textdelay = "Milliseconds: " + QString::number( intdelay );
    ui->labelDelay->setText(textdelay.toStdString().c_str());

    auto file = openFile(filename);
    if (file)
    {
        parseFile(*file);
    }

}

void MainWindow::closeEvent(QCloseEvent *event)
{

    timer->stop();
    timer->destroyed();
    QApplication::quit();

}


void MainWindow::parseFile(QFile& file)
{
    if (ui->wordList->count() != 0)
    {
        auto buttonState = QMessageBox::question(this, "Question",
                                                 "Current list contains some items, do you wish to discard it?",
                                                 QMessageBox::StandardButton::Yes| QMessageBox::StandardButton::No,
                                                 QMessageBox::StandardButton::No);
        if (buttonState == QMessageBox::StandardButton::Yes)
        {
            ui->wordList->clear();
        }
    }

    QTextStream stream(&file);
    QString buffer;

    while (!stream.atEnd())
    {
        buffer = stream.readLine(256);
        ui->wordList->addItem(buffer);
    }
}

std::unique_ptr<QFile> MainWindow::openFile(const QString &filename)
{
    if (filename.isEmpty())
    {
        return nullptr;
    }

    if (!filename.endsWith(".txt", Qt::CaseInsensitive))
    {
        QMessageBox::information(this, "Soft error", "The specified file doesn't end with \".txt\", refusing to open.");
        return {};
    }

    auto file = std::make_unique<QFile>(filename);
    file->open(QIODevice::ReadWrite);
    if (file->isOpen())
    {
        return file;
    }

    QMessageBox::information(this, "Soft error", "Couldn't open the specified file. Is the path correct?");
    return nullptr;
}

void MainWindow::writeIntoFile(QFile &file)
{
    QTextStream stream(&file);

    int listLength = ui->wordList->count();
    for (int i = 0; i < listLength; ++i)
    {
        stream << ui->wordList->item(i)->text() << '\n';
    }

    QMessageBox::information(this, "Notification", "Saved the list successfully");
}

MainWindow::~MainWindow()
{
    delete ui;
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
    auto filename = QFileDialog::getOpenFileName(this, "Open", {}, "*.txt");
    if (filename == "")
    {
        return;
    }

    auto file = openFile(filename);
    if (file)
    {
        parseFile(*file);
        return;
    }

    QMessageBox::information(this, "Soft Error",
                             "Couldn't open the file chosen. Does the directory/folder have strict permissions?");
}

void MainWindow::on_saveasButton_clicked()
{
    auto filename = QFileDialog::getSaveFileName(this, "Save as", "*.txt", "*.txt");

    if (filename == "")
    {
        return;
    }

    auto file = openFile(filename);
    if (file)
    {
        writeIntoFile(*file);
        return;
    }

    QMessageBox::information(this, "Soft error", "Couldn't create/open file chosen. "
                                   "Does the directory/file have strict permissions?");
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

void MainWindow::on_notify_clicked()
{
    int currentIndex = ui->wordList->currentRow();
    ui->wordList->setCurrentRow(currentIndex+1);

    if(ui->wordList->currentItem()){
        QString str = ui->wordList->currentItem()->text();
        QSystemTrayIcon* ic = new QSystemTrayIcon(this);
        ic->setVisible(true);
        ic->showMessage(tr("notilearn"), tr(str.toStdString().c_str()), QSystemTrayIcon::Information, sliderdelay);
        qApp->processEvents();
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    int intvalue = value;
    QString stringvalue = QString::number(intvalue);
    QString text = "Minutes: " + QString::number( intvalue );
    ui->labelMinutes->setText(text.toStdString().c_str());

    slidervalue = ui->horizontalSlider->value();
    int valslidmin = slidervalue * 1000 * 60;
    timer->setInterval(valslidmin);
    slidervalue = value;

}


void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    int intdelay = ui->horizontalSlider_2->value();
    QString stringdelay = QString::number(intdelay);
    QString textdelay = "Milliseconds: " + QString::number( intdelay );
    ui->labelDelay->setText(textdelay.toStdString().c_str());

    sliderdelay = ui->horizontalSlider->value();
    sliderdelay = value;
}

void MainWindow::TimerSlot(){

    slidervalue = ui->horizontalSlider->value();
    int valslidmin = slidervalue * 1000 * 60;
    timer->setInterval(valslidmin);
    if(ui->checkBox->isChecked())
    {
        int currentIndex = ui->wordList->currentRow();
        ui->wordList->setCurrentRow(currentIndex+1);

        if(ui->wordList->currentItem()){
            QString str = ui->wordList->currentItem()->text();
            QSystemTrayIcon* ic = new QSystemTrayIcon(this);
            ic->setVisible(true);
            ic->showMessage(tr("notilearn"), tr(str.toStdString().c_str()), QSystemTrayIcon::Information, sliderdelay);
            qApp->processEvents();
        }
    }
}


void MainWindow::on_wordList_doubleClicked(const QModelIndex &index)
{
    QString str = ui->wordList->currentItem()->text();
    QSystemTrayIcon* ic = new QSystemTrayIcon(this);
    ic->setVisible(true);
    ic->showMessage(tr("notilearn"), tr(str.toStdString().c_str()), QSystemTrayIcon::Information, sliderdelay);
    qApp->processEvents();
}
