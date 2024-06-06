#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>
#include "picbutton.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

namespace Ui {
class PicShow;
}

class PicShow : public QDialog
{
    Q_OBJECT

public:
    explicit PicShow( QWidget *parent = nullptr);
    ~PicShow();
    QString _selected_path;
    QPixmap _pix_map;
    void RoloadPic();
public slots:
    void SlotUpdateSelectedPic(const QString& path);

protected:
    virtual bool event(QEvent *event) override;

private:
    Ui::PicShow *ui;
    QPropertyAnimation *_animation_show_pre = nullptr;
    QPropertyAnimation *_animation_show_next = nullptr;
    bool _b_btnvisible = true;
private:
    void showProNextBtns(bool b_show);
signals:
    void SigPreClicked();
    void SigNextClicked();

};

#endif // PICSHOW_H
