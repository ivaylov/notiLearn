#ifndef ADDNEWDIALOG_H
#define ADDNEWDIALOG_H


#include <QDialog>
#include <QString>

namespace Ui {
class AddNewDialog;
}

class AddNewDialog : public QDialog
{
    Q_OBJECT

    QString& text;
public:
    explicit AddNewDialog(QString& todoDesc, QWidget *parent = 0);

    ~AddNewDialog();

public slots:
    void textEdited();
    void addClicked();

private:
    Ui::AddNewDialog *ui;
};

#endif // ADDNEWDIALOG_H
