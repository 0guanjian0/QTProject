#include "prosetpage.h"
#include "ui_prosetpage.h"
#include<QLineEdit>
#include<QString>
#include<QDir>
#include<QFont>
#include <QFileDialog>
ProSetPage::ProSetPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    registerField("proPath",ui->lineEdit_2);
    registerField("proName",ui->lineEdit);
    connect(ui->lineEdit,&QLineEdit::textEdited,this,&ProSetPage::completeChanged);
    connect(ui->lineEdit_2,&QLineEdit::textEdited,this,&ProSetPage::completeChanged);
    QString curPath = QDir::currentPath();
    ui->lineEdit_2->setText(curPath);
    ui->lineEdit_2->setCursorPosition(ui->lineEdit_2->text().size());
    ui->lineEdit_2->setClearButtonEnabled(true);


    ui->lineEdit->setClearButtonEnabled(true);

}

ProSetPage::~ProSetPage()
{
    delete ui;
}

void ProSetPage::GetProSettings(QString &name, QString &path)
{
    name = ui->lineEdit->text();
    path = ui->lineEdit_2->text();
}

bool ProSetPage::isComplete() const
{
    //将tips 设置为红色


    ui->tips->setStyleSheet("color:red");
    if(ui->lineEdit->text() == "" || ui->lineEdit_2->text() == "")
    {
        return false;
    }
    QDir dir(ui->lineEdit_2->text());
    if(!dir.exists())
    {
        ui->tips->setText(tr("project path is not exists"));
        return false;
    }

    //判断路径
    QString absFilePath = dir.absoluteFilePath(ui->lineEdit->text());
    QDir dist_dir(absFilePath);
    if(dist_dir.exists())
    {

        ui->tips->setText(tr("the project exists,change path or name"));
        return false;
    }
    ui->tips->setStyleSheet("color:green");
    ui->tips->setText("sucess");
    return QWizardPage::isComplete();
}

void ProSetPage::on_pushButton_clicked()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("选择文件夹"));
    auto path = QDir::currentPath();
    file_dialog.setDirectory(path);
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(file_dialog.exec())
    {
        fileNames = file_dialog.selectedFiles();
    }

    if(fileNames.length() <= 0)
    {
        return;
    }
    QString import_path = fileNames.at(0);
    ui->lineEdit_2->setText(import_path);

}

