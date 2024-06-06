#ifndef PROTREE_H
#define PROTREE_H

#include <QDialog>
#include "protreewidget.h"

namespace Ui {
class ProTree;
}

class ProTree : public QDialog
{
    Q_OBJECT

public:
    explicit ProTree(QWidget *parent = nullptr);
    ~ProTree();
    QTreeWidget* GetTreeWidget();
public slots:
    void AddProToTree( QString name,QString path);

private:
    Ui::ProTree *ui;
};

#endif // PROTREE_H
