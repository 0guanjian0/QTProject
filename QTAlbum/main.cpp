 #include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //导入设置qss
    QFile qss_file(QString::fromStdString(":/style/style.qss"));

    if(qss_file.open(QFile::ReadOnly))
    {
        //打开qss文件成功
        QString style = QLatin1String(qss_file.readAll());
        if(style.isEmpty())
        {
            qDebug()<<"qss failed";
            return 0;
        }
        //qDebug()<<"style :"<<style;
        a.setStyleSheet(style);
        qss_file.close();
    }else
    {
        qDebug()<<"open qss file failed";
        qDebug()<<"fail error code:"<<qss_file.error()<<" message:"<<qss_file.errorString();
        qss_file.close();
        return 0;
    }
    //开始设置启动主窗口主窗口
    MainWindow w;
    w.setWindowTitle(QString::fromStdString("GJ图片查看器"));
    w.showMaximized();


    return a.exec();
}
