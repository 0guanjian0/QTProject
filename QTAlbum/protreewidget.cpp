#include "protreewidget.h"
#include "protreeitem.h"
#include "const.h"
#include "removeprodialog.h"

#include<iostream>
#include<functional>
#include<QDir>
#include<QHeaderView>
#include<QGuiApplication>
#include<QMenu>
#include <qfiledialog.h>
#include<QSharedPointer>





ProTreeWidget::ProTreeWidget(QWidget *parent) {

    this->header()->hide();
    connect(this,&ProTreeWidget::itemPressed,this,&ProTreeWidget::SlotItemPressed);
    _action_import = new QAction(QIcon(tr(":/icon/import.png")),tr("导入文件"),this);
    _action_close = new QAction(QIcon(tr(":/icon/close.png")),tr("关闭项目"),this);
    _action_setstart = new QAction(QIcon(tr(":/icon/core.png")),tr("设置为活动项目"),this);
    _action_slideshow = new QAction(QIcon(tr(":/icon/slideshow.png")),tr("幻灯片播放"),this);
    //将动作设置为 可点击
    _action_import->setCheckable(true);
    _action_setstart->setCheckable(true);
    _action_close->setCheckable(true);
    _action_slideshow->setCheckable(true);

    //动作的槽函数
    connect(_action_import,&QAction::toggled,this,&ProTreeWidget::SlotImport);
    connect(_action_setstart,&QAction::toggled,this,&ProTreeWidget::SlotSetActive);
    connect(_action_close,&QAction::toggled,this,&ProTreeWidget::SlotColsePro);
    connect(this,&ProTreeWidget::itemDoubleClicked,this,&ProTreeWidget::SlotDoubleClickItem);




}

void ProTreeWidget::AddProToTree(const QString name, const QString path)
{
    QDir dir(path);
    QString file_path = dir.absoluteFilePath(name);
    if(_set_path.find(file_path) != _set_path.end())
    {
        //找到了path 代表重名
        return;
    }
    QDir pro_dir(file_path);
    if(!pro_dir.exists())
    {
        bool enable = pro_dir.mkpath(file_path);
        if(!enable)
        {
            return;
        }
    }

    _set_path.insert(file_path);
    auto *item = new ProTreeItem(this,name,file_path,TreeItemPro);
    item->setData(0,Qt::DisplayRole,name);
    item->setData(0,Qt::DecorationRole,QIcon(tr(":/icon/dir.png")));
    item->setData(0,Qt::ToolTipRole,file_path);
    this->addTopLevelItem(item);
}

void ProTreeWidget::SlotItemPressed(QTreeWidgetItem *pressedItem, int column)
{

    if(QGuiApplication::mouseButtons() == Qt::RightButton)
    {

        //是右键点击item
        QMenu menu(this);
        int itemtype = pressedItem->type();

        if(itemtype == TreeItemPro)
        {
            //qDebug()<<"Pressed item2";
            //右键点击的是项目
            _right_btn_item = dynamic_cast<ProTreeItem*> (pressedItem);
            //显示action
            menu.addAction(_action_import);
            menu.addAction(_action_setstart);
            menu.addAction(_action_close);
            menu.addAction(_action_slideshow);
            menu.exec(QCursor::pos());

        }


    }
}

void ProTreeWidget::SlotImport()
{
    qDebug()<<"slot import";

    //打开文件夹
    QSharedPointer<QFileDialog>file_dialog( new QFileDialog);
    file_dialog->setFileMode(QFileDialog::Directory);
    file_dialog->setWindowTitle(tr("选择导入的文件夹"));
    QString path = "";


    if(!_right_btn_item)
    {
        qDebug()<<"_right_btn_item is empty";
        return;
    }
    path = dynamic_cast<ProTreeItem*>(_right_btn_item)->GetPath();
    file_dialog->setDirectory(path);
    file_dialog->setModal(QFileDialog::Detail);

    QStringList fileNames;
    if(file_dialog->exec())
    {
        fileNames = file_dialog->selectedFiles();
        if(fileNames.length() <= 0)
        {
            //没有选择任何文件,不导入文件,直接返回
            return;
        }
        qDebug()<<"fileNames "<<fileNames;
        QString import_path = fileNames.at(0);
        int file_count = 0;

        //创建和打开共用一个processdialog,故先判断是否被使用过
        if(_dialog_progress)
        {
            delete _dialog_progress;
            _dialog_progress = nullptr;
        }
        _dialog_progress = new QProgressDialog(this);
        _dialog_progress->setWindowTitle(tr("please wait ..."));
        _dialog_progress->setFixedWidth(PROGRESS_WIDTH);
        _dialog_progress->setRange(0,PROGRESS_WIDTH);

        //const QString &src_path,const QString &dist_path,QTreeWidgetItem* parent_item,
        //                   int file_count,QTreeWidget *self,QTreeWidgetItem* root,QObject* parent = nullptr);
        _thread_create_pro = QSharedPointer<ProTreeThread>::create(std::ref(import_path),std::ref(path),_right_btn_item,file_count,this,_right_btn_item,nullptr);

        //创建项目时 更新 process
        connect(_thread_create_pro.get(),&ProTreeThread::SigUpdateProcess,this,&ProTreeWidget::SlotUpdateProcess);
        connect(_thread_create_pro.get(),&ProTreeThread::SigFinishProcess,this,&ProTreeWidget::SlotFinishProcess);
        connect(_dialog_progress,&QProgressDialog::canceled,this,&ProTreeWidget::SlotCancelProcess);
        connect(this,&ProTreeWidget::SigCancelProcess,_thread_create_pro.get(),&ProTreeThread::SlotCancelProcess);

        _thread_create_pro->start();
        _dialog_progress->exec();
    }



}

void ProTreeWidget::SlotUpdateProcess(int count)
{
    qDebug()<<"SlotUpdateProcess";
    if(! _dialog_progress)
    {
        return;
    }

    if(count >= PROGRESS_MAX)
    {
        _dialog_progress->setValue(count % PROGRESS_MAX);
    }
    else
    {
        _dialog_progress->setValue(count);
    }
}

void ProTreeWidget::SlotFinishProcess(int count)
{
    qDebug()<<"SlotFinishProcess";
    emit SigCancelProcess();
    _dialog_progress->setValue(PROGRESS_MAX);
    _dialog_progress->deleteLater();

    _dialog_progress =nullptr;
}

void ProTreeWidget::SlotCancelProcess()
{
    qDebug()<<"SlotCancelProcess";
    emit SigCancelProcess();
    delete _dialog_progress;
    _dialog_progress = nullptr;
}

void ProTreeWidget::SlotSetActive()
{
    if(!_right_btn_item)
    {
        return ;
    }
    QFont nullFont;
    nullFont.setBold(false);
    if(_active_item)
    {
        _active_item->setFont(0,nullFont);
    }

    _active_item = _right_btn_item;
    nullFont.setBold(true);
    _active_item->setFont(0,nullFont);


}

void ProTreeWidget::SlotColsePro()
{
    QSharedPointer<RemoveProDialog>remove_pro_dialog(new RemoveProDialog());
    auto res = remove_pro_dialog->exec();
    if(res != QDialog::Accepted)
    {
        return;
    }
    bool b_remove = remove_pro_dialog->IsRemoved();
    auto index_right_btn = this->indexOfTopLevelItem(_right_btn_item);
    auto * protreeitem = dynamic_cast<ProTreeItem*>(_right_btn_item);
    auto * selecteditem = dynamic_cast<ProTreeItem*>(_selected_item);
    auto delete_path = protreeitem->GetPath();
    _set_path.remove(delete_path);
    if(b_remove)
    {
        QDir delete_dir(delete_path);
        delete_dir.removeRecursively();
    }
    if(protreeitem == _active_item)
    {
        _active_item = nullptr;
    }
    delete this->takeTopLevelItem(index_right_btn);
    _right_btn_item = nullptr;
}

void ProTreeWidget::SlotDoubleClickItem(QTreeWidgetItem *doubleItem, int column)
{
    if(QGuiApplication::mouseButtons() == Qt::LeftButton)
    {
        ProTreeItem * double_item = dynamic_cast<ProTreeItem*>(doubleItem);
        if(!double_item)
        {
            return;
        }
        int itemtype = double_item->type();
        if(itemtype == TreeItemPic)
        {
            //如果是图片,更新图片
            emit SigUpdateSelectedPic(double_item->GetPath());
            _selected_item = double_item;
        }

    }
}

void ProTreeWidget::SlotOpenPro(const QString &path)
{
    if(_set_path.find(path) != _set_path.end() )
    {
        //说明已经被打开过了,还没有关闭
        return;
    }
    _set_path.insert(path);
    int file_count = 0;
    QDir pro_dir(path);
    QString proname = pro_dir.dirName();

   //创建和打开共用一个processdialog,故先判断是否被使用过
    if(_dialog_progress)
    {
        delete _dialog_progress;
        _dialog_progress = nullptr;
    }

    _dialog_progress = new QProgressDialog(this);

    _dialog_progress->setWindowTitle(tr("please wait ..."));
    _dialog_progress->setFixedWidth(PROGRESS_WIDTH);
    _dialog_progress->setRange(0,PROGRESS_WIDTH);
    _thread_open_pro = QSharedPointer<OpenTreeThread>::create(path,file_count,this,nullptr);

    //打开项目时 更新 process
    connect(_thread_open_pro.get(),&OpenTreeThread::SigUpdateProcess,this,&ProTreeWidget::SlotUpdateProcess);
    connect(_thread_open_pro.get(),&OpenTreeThread::SigFinishProcess,this,&ProTreeWidget::SlotFinishProcess);
    connect(_dialog_progress,&QProgressDialog::canceled,this,&ProTreeWidget:: SlotCancelProcess);
    connect(this,&ProTreeWidget::SigCancelProcess,_thread_open_pro.get(),&OpenTreeThread::SlotFinishProcess);

    _thread_open_pro->start();

    _dialog_progress->exec();







}

void ProTreeWidget::SlotPreClicked()
{
    qDebug()<<"ProTreeWidget::SlotPreClicked()";
    if(!_selected_item)
    {
        return;
    }
    auto *curItem = dynamic_cast<ProTreeItem*>(_selected_item)->GetPreItem();
    if(!curItem)
    {
        return;
    }
    emit SigUpdateSelectedPic(curItem->GetPath());
    this->setCurrentItem(curItem);
    _selected_item = curItem;
}

void ProTreeWidget::SlotNextClicked()
{
    qDebug()<<"ProTreeWidget::SlotNextClicked()";
    if(!_selected_item)
    {
        return;
    }
    auto *curItem = dynamic_cast<ProTreeItem*>(_selected_item)->GetNextItem();
    if(!curItem)
    {
        return;
    }

    emit SigUpdateSelectedPic(curItem->GetPath());
    this->setCurrentItem(curItem);
    _selected_item = curItem;
}
