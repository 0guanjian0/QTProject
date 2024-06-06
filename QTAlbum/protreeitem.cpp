#include "protreeitem.h"

ProTreeItem::ProTreeItem(QTreeWidget *view, const QString &name, const QString &path, int type)
    :QTreeWidgetItem(view,type),_path(path),_name(name),_root(this),_pre_item(nullptr),_next_item(nullptr)
{

}

ProTreeItem::ProTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path, QTreeWidgetItem *root, int type)
    :QTreeWidgetItem(parent,type),_root(root),_name(name),_path(path),_pre_item(nullptr),_next_item(nullptr)
{

}

const QString &ProTreeItem::GetPath() const
{
    return _path;
}

ProTreeItem *ProTreeItem::GetRoot() const
{
    return dynamic_cast<ProTreeItem*>(_root);
}

void ProTreeItem::SetPreItem(QTreeWidgetItem *item)
{
    _pre_item = item;
}

void ProTreeItem::SetNextItem(QTreeWidgetItem *item)
{
    _next_item = item;
}

ProTreeItem *ProTreeItem::GetPreItem() const
{
    return dynamic_cast<ProTreeItem*>(_pre_item) ;
}

ProTreeItem *ProTreeItem::GetNextItem() const
{
    return dynamic_cast<ProTreeItem*>(_next_item);
}

ProTreeItem *ProTreeItem::GetLastPicChild() const
{

}

ProTreeItem *ProTreeItem::GetFirstPicChild() const
{

}




