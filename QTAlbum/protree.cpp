#include "protree.h"
#include "ui_protree.h"

ProTree::ProTree(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProTree)
{
    ui->setupUi(this);
    this->setMinimumWidth(378);
    this->setMaximumWidth(378);
}

ProTree::~ProTree()
{
    delete ui;
}

QTreeWidget *ProTree::GetTreeWidget()
{
    return ui->treeWidget;
}

void ProTree::AddProToTree(QString name, QString path)
{
    //添加文件到项目树
    ui->treeWidget->AddProToTree(name,path);
}
