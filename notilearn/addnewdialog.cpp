#include "addnewdialog.h"
#include "ui_addnewdialog.h"

AddNewDialog::AddNewDialog(QString& todoDesc, QWidget *parent) :
    QDialog(parent),
    text(todoDesc),
    ui(new Ui::AddNewDialog)
{
    ui->setupUi(this);
    ui->pushButton->setDisabled(true);

    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(textEdited()));
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(addClicked()));

    text = "";
}

void AddNewDialog::textEdited()
{
    if (ui->lineEdit->text().length() == 0)
    {
        ui->pushButton->setDisabled(true);
        return;
    }

    ui->pushButton->setDisabled(false);
}

void AddNewDialog::addClicked()
{
    text = ui->lineEdit->text();
    close();
}

AddNewDialog::~AddNewDialog()
{
    delete ui;
}
