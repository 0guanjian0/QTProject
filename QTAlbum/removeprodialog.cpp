#include "removeprodialog.h"
#include "ui_removeprodialog.h"

RemoveProDialog::RemoveProDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RemoveProDialog)
{
    ui->setupUi(this);
}

bool RemoveProDialog::IsRemoved()
{
    bool isremoved = ui->checkBox->isChecked();
    return isremoved;
}

RemoveProDialog::~RemoveProDialog()
{
    delete ui;
}
