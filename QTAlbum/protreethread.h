#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QObject>
#include <QThread>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class ProTreeThread : public QThread
{
    Q_OBJECT
public:
    explicit ProTreeThread(QObject *parent = nullptr);

    explicit ProTreeThread(const QString &src_path,const QString &dist_path,QTreeWidgetItem* parent_item,
                           int file_count,QTreeWidget *self,QTreeWidgetItem* root,QObject* parent = nullptr);
    ~ProTreeThread();
signals:
    void SigUpdateProcess(int);
    void SigFinishProcess(int);
public slots:
    void SlotCancelProcess();
protected:
    virtual void run();
private:
    void CreateProTree(const QString& src_path,const QString& dist_path,QTreeWidgetItem* parent_item,
                        int &file_count,QTreeWidget* self,QTreeWidgetItem* root,QTreeWidgetItem* pre_item = nullptr);
private:
    QString _src_path;
    QString _dist_path;
    int _file_count;
    QTreeWidgetItem * _parent_item;
    QTreeWidget* _self;
    QTreeWidgetItem * _root;
    bool _bstop;
};

#endif // PROTREETHREAD_H
