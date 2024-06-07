#include "protreethread.h"
#include<QDir>
#include<QDebug>
#include "protreeitem.h"
#include "const.h"


ProTreeThread::ProTreeThread(QObject *parent)
{

}

ProTreeThread::ProTreeThread(const QString &src_path,
                             const QString &dist_path,
                             QTreeWidgetItem *parent_item,
                             int file_count, QTreeWidget *self,
                             QTreeWidgetItem *root,
                             QObject *parent)
    :QThread(parent),_src_path(src_path),_dist_path(dist_path),_file_count(file_count),
    _parent_item(parent_item),_self(self),_root(root),_bstop(false)
{


}

ProTreeThread::~ProTreeThread()
{

}

void ProTreeThread::SlotCancelProcess()
{
    this->_bstop = true;
}

void ProTreeThread::run()
{
    CreateProTree(_src_path,_dist_path,_parent_item,_file_count,_self,_root);
    if(_bstop)
    {
        //程序结束
        auto path = dynamic_cast<ProTreeItem*>(_root)->GetPath();
        auto index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();

        return ;
    }
    emit SigFinishProcess(0);
}

void ProTreeThread::CreateProTree(const QString &src_path, const QString &dist_path, QTreeWidgetItem *parent_item, int &file_count, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *pre_item)
{
    qDebug()<<"ProTreeThread::CrateProTree()";
    if(_bstop)
    {
        return;
    }
    bool needcpoy = true;
    if(src_path == dist_path)
    {
        needcpoy = false;
    }
    //导入文件的位置
    QDir import_dir(src_path);
    QStringList namefilters;
    import_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    import_dir.setSorting(QDir::Name);
    QFileInfoList list = import_dir.entryInfoList();
    for(int i = 0;i < list.size();++i)
    {
        if(_bstop)
        {
            return;
        }

        QFileInfo fileinfo = list.at(i);
        if(fileinfo.isDir())
        {
            //碰到子目录
            qDebug()<<"DIR";
            if(_bstop)
            {
                return;
            }
            ++file_count;
            emit SigUpdateProcess(file_count);

            QDir dist_dir(_dist_path);
            QString sub_dist_path = dist_dir.absoluteFilePath(fileinfo.fileName());
            QDir sub_dist_dir(sub_dist_path);
            if(!sub_dist_dir.exists())
            {
                bool ok = sub_dist_dir.mkpath(sub_dist_path);
                if(!ok)
                {
                    continue;
                }
            }

            auto * item = new ProTreeItem(parent_item,fileinfo.fileName(),sub_dist_path,root,TreeItemDir);

            item->setData(0,Qt::DisplayRole,fileinfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
            item->setData(0,Qt::ToolTipRole,sub_dist_path);
            CreateProTree(fileinfo.absoluteFilePath(),sub_dist_path,item,file_count,self,root,pre_item);
        }
        else
        {
            if(_bstop)
            {
                return;
            }
            const QString suffix = fileinfo.completeSuffix();
            if(suffix != "png" && suffix != "jpeng" && suffix != "jpg")
            {
                continue;
            }
            ++file_count;
            emit SigUpdateProcess(file_count);
            if(!needcpoy)
            {
                continue;
            }

            QDir dist_dir(dist_path);
            QString dist_file_path = dist_dir.absoluteFilePath(fileinfo.fileName());
            if(!QFile::copy(fileinfo.absoluteFilePath(),dist_file_path))
            {
                continue;
            }

            auto * item = new ProTreeItem(parent_item,fileinfo.fileName(),dist_file_path,root,TreeItemPic);
            item->setData(0,Qt::DisplayRole,fileinfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/pic.png"));
            item->setData(0,Qt::ToolTipRole,dist_file_path);

            if(pre_item)
            {
                auto * pre_proitem =  dynamic_cast<ProTreeItem*>(pre_item);
                pre_proitem->SetNextItem(item);
            }
            item->SetPreItem(pre_item);
            pre_item = item;



        }
    }
    qDebug()<<"finish preocess++++++++++++++++++++++++++++++++++++++++++++++++";
    //emit SigFinishProcess(0);
}


