#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QWidget>
#include <QResizeEvent>
#include "protree.h"
#include "picshow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    virtual void resizeEvent(QResizeEvent *event)override;

private slots:
        void SlotCreatePro();
        void SlotOpenPro();

private:
    Ui::MainWindow *ui;
    QWidget *_protree;//左侧的窗口
    QDialog *_picshow;//右侧显示的窗口
signals:
    void SigOpenPro(QString path);
};
#endif // MAINWINDOW_H
