#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "protreewidget.h"
#include<QMenu>
#include<QAction>
#include<QDebug>
#include<QIcon>
#include<QKeySequence>
#include<QFileDialog>
#include "wizard.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setMinimumSize(1629,869);
    ui->setupUi(this);

    //初始化 左侧protree
    _protree = new ProTree();
    ui->prolayout->addWidget(_protree);

    //初始化 右侧picshow
    _picshow = new PicShow();
    ui->piclayout->addWidget(_picshow);

    //创建菜单
    QMenu * menu_file = menuBar()->addMenu(tr("文件(&F)"));
    //创建项目的动作
    QAction * act_creator_pro = new QAction(QIcon(":/icon/createpro.png"),tr("创建项目"));
    act_creator_pro->setShortcut(QKeySequence(Qt::CTRL,Qt::Key_N));
    menu_file->addAction(act_creator_pro);

    //创建打开项目的动作
    QAction * act_open_pro = new QAction(QIcon(":/icon/openpro.png"),tr("打开项目"));
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL,Qt::Key_O));
    menu_file->addAction(act_open_pro);


    //创建设置菜单
    QMenu* mene_set = menuBar()->addMenu("设置(&S");
    //设置背景音乐
    QAction* act_set_music = new QAction(QIcon(":/icon/music.png"),tr("设置背景音乐"));
    act_set_music->setShortcut(QKeySequence(Qt::CTRL,Qt::Key_M));
    mene_set->addAction(act_set_music);

    //得到protree 里面的ProTreeWideget指针
    QTreeWidget *tree_widget = dynamic_cast<ProTree*>(_protree)->GetTreeWidget();
    ProTreeWidget *pro_tree_widget = dynamic_cast<ProTreeWidget*>(tree_widget);

    //得到得到picshow的原始指针
    PicShow *pic_show = dynamic_cast<PicShow*>(_picshow);

    connect(act_creator_pro,&QAction::triggered,this,&MainWindow::SlotCreatePro);
    connect(act_open_pro,&QAction::triggered,this,&MainWindow::SlotOpenPro);

    connect(this,&MainWindow::SigOpenPro,pro_tree_widget,&ProTreeWidget::SlotOpenPro);

    //连接图片更新
    connect(pro_tree_widget,&ProTreeWidget::SigUpdateSelectedPic,pic_show,&PicShow::SlotUpdateSelectedPic);

    //连接图片前后按钮
    connect(pic_show,&PicShow::SigPreClicked,pro_tree_widget,&ProTreeWidget::SlotPreClicked);
    connect(pic_show,&PicShow::SigNextClicked,pro_tree_widget,&ProTreeWidget::SlotNextClicked);
    connect(pro_tree_widget,&ProTreeWidget::SigUpdateSelectedPic,pic_show,&PicShow::SlotUpdateSelectedPic);






}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    auto *pic_show = dynamic_cast<PicShow*>(_picshow);
    if(!pic_show)
    {
        qDebug()<<"_picshow dynamic_cast failed";
        return;
    }
    pic_show->RoloadPic();
    QMainWindow::resizeEvent(event);
}



//创建文件
void MainWindow::SlotCreatePro()
{
    //qDebug()<<"创建项目";
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));
    auto * page = wizard.page(0);
    page->setTitle(tr("准备创建项目"));

    //连接信号和槽,将项目的配置传回来tode...
    connect(&wizard,&Wizard::SigProSettings,dynamic_cast<ProTree*> (_protree),&ProTree::AddProToTree);

    wizard.show();
    wizard.exec();

    //断开所有信号 todo...
    disconnect(&wizard,&Wizard::SigProSettings,dynamic_cast<ProTree*> (_protree),&ProTree::AddProToTree);
}

void MainWindow::SlotOpenPro()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("选择导入的文件夹"));
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileName;
    if(file_dialog.exec())
    {
        fileName = file_dialog.selectedFiles();
    }

    if(fileName.length() <= 0)
    {
        //没有导入任何文件或错误
        return ;
    }
    qDebug()<<"select open file is:"<<fileName.at(0);
    emit SigOpenPro(fileName.at(0));


}
