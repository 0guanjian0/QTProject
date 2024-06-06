#include "picshow.h"
#include "ui_picshow.h"
#include <QSize>


PicShow::PicShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PicShow)
{
    ui->setupUi(this);
    ui->btnPre->SetIcons(":/icon/previous.png",
                         ":/icon/previous_hover.png",
                         ":/icon/previous_press.png");
    ui->btnNext->SetIcons(":/icon/next.png",
                          ":/icon/next_hover.png",
                          ":/icon/next_press.png");
    QGraphicsOpacityEffect * opacity_pre = new QGraphicsOpacityEffect(this);
    QGraphicsOpacityEffect * opacity_next = new QGraphicsOpacityEffect(this);
    opacity_pre->setOpacity(0);
    opacity_next->setOpacity(0);;
    ui->btnPre->setGraphicsEffect(opacity_pre);
    ui->btnNext->setGraphicsEffect(opacity_next);

    _animation_show_pre = new QPropertyAnimation(opacity_pre,"opacity",this);
    _animation_show_pre->setEasingCurve(QEasingCurve::Linear);
    _animation_show_pre->setDuration(500);

    _animation_show_next = new QPropertyAnimation(opacity_next,"opacity",this);
    _animation_show_next->setEasingCurve(QEasingCurve::Linear);
    _animation_show_next ->setDuration(500);

    connect(ui->btnPre,&PicButton::clicked,this,&PicShow::SigPreClicked);
    connect(ui->btnNext,&PicButton::clicked,this,&PicShow::SigNextClicked);

}

PicShow::~PicShow()
{
    delete ui;
}

void PicShow::SlotUpdateSelectedPic(const QString &path)
{
    _selected_path = path;
    _pix_map.load(path);
    auto width = this->width() - 20;
    auto heigh = this->height()-20;
    _pix_map = _pix_map.scaled(width,heigh,Qt::KeepAspectRatio);
    ui->label->setPixmap(_pix_map);

}

bool PicShow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        showProNextBtns(true);
        break;
    case QEvent::Leave:
        showProNextBtns(false);
    default:
        break;
    }
    return QDialog::event(event);
}



void PicShow::showProNextBtns(bool b_show)
{
    if(!b_show && _b_btnvisible)
    {
        _animation_show_pre->stop();
        _animation_show_pre->setStartValue(1);
        _animation_show_pre->setEndValue(0);
        _animation_show_pre->start();

        _animation_show_next->stop();
        _animation_show_next->setStartValue(1);
        _animation_show_next->setEndValue(0);
        _animation_show_next->start();

        _b_btnvisible = false;
        return ;
    }
    else if(b_show && !_b_btnvisible)
    {
        _animation_show_pre->stop();
        _animation_show_pre->setStartValue(0);
        _animation_show_pre->setEndValue(1);
        _animation_show_pre->start();

        _animation_show_next->stop();
        _animation_show_next->setStartValue(0);
        _animation_show_next->setEndValue(1);
        _animation_show_next->start();

        _b_btnvisible = true;
        return ;
    }
}

void PicShow::RoloadPic()
{
    if(_selected_path != "")
    {
        const auto &width = ui->gridLayout->geometry().width();
        const auto &heigh = ui->gridLayout->geometry().height();
        _pix_map.load(_selected_path);
        _pix_map = _pix_map.scaled(width,heigh,Qt::KeepAspectRatio);
        ui->label->setPixmap(_pix_map);
    }
}
