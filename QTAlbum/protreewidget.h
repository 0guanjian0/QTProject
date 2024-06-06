#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QObject>
#include <QTreeWidget>
#include <QWidget>
#include <QSet>
#include <QAction>
#include <QProgressDialog>
#include <QThread>
#include <QSharedPointer>

#include "protreeitem.h"
#include "protreethread.h"
#include "opentreethread.h"


class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget *parent = nullptr);

    void AddProToTree(const QString name,const QString path);
private:
    ProTreeItem *_right_btn_item = nullptr;
    ProTreeItem *_active_item = nullptr;
    ProTreeItem *_selected_item = nullptr;
    QSet<QString> _set_path;
    QAction *_action_import = nullptr;
    QAction *_action_setstart = nullptr;
    QAction *_action_close = nullptr;
    QAction *_action_slideshow =nullptr;

    QProgressDialog * _dialog_progress = nullptr;

    QSharedPointer<ProTreeThread> _thread_create_pro = nullptr;
    QSharedPointer<OpenTreeThread> _thread_open_pro = nullptr;
public slots:
    void SlotOpenPro(const QString& path);
    void SlotPreClicked();
    void SlotNextClicked();

private slots:
    void SlotItemPressed(QTreeWidgetItem *pressedItem, int column);
    void SlotImport();
    void SlotUpdateProcess(int count);  //用于更新processWidget
    void SlotFinishProcess(int count);  //完成添加文件
    void SlotCancelProcess();
    void SlotSetActive();
    void SlotColsePro();
    void SlotDoubleClickItem (QTreeWidgetItem *doubleItem, int column);


signals:
    void SigCancelProcess();
    void SigUpdateSelectedPic(const QString& path);
};

#endif // PROTREEWIDGET_H
