#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QObject>
#include <QTreeWidgetItem>
#include <QWidget>


class ProTreeItem : public QObject,public QTreeWidgetItem
{
    Q_OBJECT
public:
    ProTreeItem(QTreeWidget* view,const QString &name,const QString &path,int type = Type);
    ProTreeItem(QTreeWidgetItem *parent,const QString& name,const QString &path,
                QTreeWidgetItem *root,int type = Type);
    const QString& GetPath()const;
    ProTreeItem* GetRoot()const;
    void  SetPreItem(QTreeWidgetItem * item);
    void  SetNextItem(QTreeWidgetItem * item);
    ProTreeItem* GetPreItem()const;
    ProTreeItem* GetNextItem()const;
    ProTreeItem* GetLastPicChild()const;
    ProTreeItem* GetFirstPicChild()const;

private:
    QString _path;
    QString _name;
    QTreeWidgetItem *_root;
    QTreeWidgetItem *_pre_item;
    QTreeWidgetItem *_next_item;


};

#endif // PROTREEITEM_H
