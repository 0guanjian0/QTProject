#include "picbutton.h"
#include<QDebug>

PicButton::PicButton(QWidget *parent):QPushButton(parent) {}

void PicButton::SetIcons(const QString &normal,const  QString &hover,const  QString &pressed)
{
    _normal = normal;
    _hover = hover;
    _pressed = pressed;

    QPixmap tempPixmap;
    tempPixmap.load(normal);
    // qDebug()<<"button "<<this->size();
    // qDebug()<<"pixmap "<<tempPixmap.size();
    this->resize(tempPixmap.size());
    //qDebug()<<"button"<<this->size();
    this->setIcon(tempPixmap);
    this->setIconSize(tempPixmap.size());



}

bool PicButton::event(QEvent *e)
{
    //qDebug()<<"event :"<<e->type();
    switch (e->type()) {
    case QEvent::Enter:
        SetHoverIcon();

        break;
    case QEvent::Leave:
        SetNormalIcon();
        break;
    case QEvent::MouseButtonPress:
        SetPressedIcon();
        break;
    case QEvent::MouseButtonRelease:
        SetHoverIcon();
    default:

        break;
    }
    return QPushButton::event(e);
}

void PicButton::SetNormalIcon()
{
    //qDebug()<<"setNormalIcon";
    QPixmap tempPixmap;
    tempPixmap.load(_normal);

    this->setIcon(tempPixmap);

}

void PicButton::SetHoverIcon()
{
   // qDebug()<<"setHoverIcon";
    QPixmap tempPixmap;
    tempPixmap.load(_hover);
    this->setIcon(tempPixmap);

}

void PicButton::SetPressedIcon()
{
    //qDebug()<<"seTpressedIcon";
    QPixmap tempPixmap;
    tempPixmap.load(_pressed);
    this->setIcon(tempPixmap);

}
