#ifndef OPENTREETHREAD_H
#define OPENTREETHREAD_H

#include <QObject>
#include <QThread>
#include "qtreewidget.h"

class OpenTreeThread : public QThread
{
    Q_OBJECT
public:
    explicit OpenTreeThread(const QString& src_path,int file_count,QTreeWidget* self, QObject *parent = nullptr);

    ~OpenTreeThread();
    void OpenProTree(const QString& src_path,int &file_count,QTreeWidget* self);
protected:
    virtual void run();
private:
    void RecursiveProTree(const QString& src_path,int file_count,QTreeWidget* self,
                          QTreeWidgetItem* root,QTreeWidgetItem* parent,QTreeWidgetItem* preItem  );

private:
    QString _src_path;
    int _file_count = 0;
    QTreeWidget *_self = 0;
    bool _bstop = false;
    QTreeWidgetItem * _root = nullptr;
signals:
    void SigFinishProcess(int);

    void SigUpdateProcess(int);
public slots:
    void SlotFinishProcess();

};

#endif // OPENTREETHREAD_H
