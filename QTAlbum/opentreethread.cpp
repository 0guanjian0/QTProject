#include "opentreethread.h"

#include "protreeitem.h"
#include "const.h"
#include <QDir>
#include <QIcon>
#include <QDebug>





OpenTreeThread::OpenTreeThread(const QString &src_path, int file_count, QTreeWidget *self, QObject *parent)
    :QThread(parent),_bstop(false),_src_path(src_path),_file_count(file_count),
    _self(self),_root(nullptr)
{

}

OpenTreeThread::~OpenTreeThread()
{

}

void OpenTreeThread::OpenProTree(const QString &src_path, int &file_count, QTreeWidget *self)
{
    qDebug()<<"OpenProThread::OpenProTree()";
    QDir src_dir(src_path);
    auto name = src_dir.dirName();
    auto * item = new ProTreeItem(self,name,src_path,TreeItemPro);
    item->setData(0,Qt::DisplayRole,name);
    item->setData(0,Qt::DecorationRole,QIcon(tr(":/icon/dir.png")));
    item->setData(0,Qt::ToolTipRole,src_path);
    _root = item;
    RecursiveProTree(src_path,file_count,self,_root,item,nullptr);

}

void OpenTreeThread::run()
{
    qDebug()<<"OpenTreeThread::run()";
    OpenProTree(_src_path,_file_count,_self);
    emit SigFinishProcess(0);
}

void OpenTreeThread::RecursiveProTree(const QString &src_path, int file_count, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *parent, QTreeWidgetItem *preItem)
{
    qDebug()<<"OpenTreeTread::RecursiveProTree()";
    if(_bstop)
    {
        return;
    }
    //打开的源文件夹
    QDir src_dir(src_path);
    src_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    src_dir.setSorting(QDir::Name);
    QFileInfoList list = src_dir.entryInfoList();
    qDebug()<<"list size :"<<list.size();
    for(int i = 0; i < list.size();++i)
    {
        if(_bstop)
        {
            return;
        }
        QFileInfo fileInfo = list.at(i);
        bool bIsDir = fileInfo.isDir();
        if(bIsDir)
        {
            //是目录
            if(_bstop)
            {

                return;
            }
            qDebug()<<"Is Dir";
            ++file_count;
            emit SigUpdateProcess(file_count);
            auto * item = new ProTreeItem(parent,fileInfo.fileName(),fileInfo.absoluteFilePath(),root,TreeItemDir);
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(tr(":/icon/dir.png")));
            item->setData(0,Qt::ToolTipRole,fileInfo.absoluteFilePath());
            RecursiveProTree(fileInfo.absoluteFilePath(),file_count,self,root,item,preItem);


        }else
        {
            qDebug()<<"Is Pic";
            //是文件
            if(_bstop)
            {
                return;
            }
            const QString & suffix = fileInfo.completeSuffix();
            if(suffix != "png" && suffix != "jpg" && suffix != "jpeg")
            {
                continue;
            }
            ++file_count;
            emit SigUpdateProcess(file_count);
            auto * item = new ProTreeItem(parent,fileInfo.fileName(),fileInfo.absoluteFilePath(),root,TreeItemPic);
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(tr(":/icon/pic.png")));
            item->setData(0,Qt::ToolTipRole,fileInfo.absoluteFilePath());
            if(preItem)
            {
                auto * pre_proitem = dynamic_cast<ProTreeItem*>(preItem);
                pre_proitem->SetNextItem(item);

            }
            item->SetPreItem(preItem);
            preItem = item;
        }
    }
   // emit SigFinishProcess(file_count);
}

void OpenTreeThread::SlotFinishProcess()
{
    qDebug()<<"SlotFinishProcess 01";
    _bstop = false;
}
