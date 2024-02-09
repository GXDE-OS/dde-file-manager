/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *               2016 ~ 2018 dragondjf
 *
 * Author:     dragondjf<dingjiangfeng@deepin.com>
 *
 * Maintainer: dragondjf<dingjiangfeng@deepin.com>
 *             zccrs<zhangjide@deepin.com>
 *             Tangtong<tangtong@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dmoveablewidget.h"
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QResizeEvent>
#include <QPainter>
#include <QWidget>
#include <QDebug>


DMoveableWidget::DMoveableWidget(QWidget *parent):QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

void DMoveableWidget::setMovableHeight(int height){
    m_movableHeight = height;
}

void DMoveableWidget::moveCenter(){
    QRect qr = frameGeometry();
    QPoint cp;
    if (parent()){
        cp = static_cast<QWidget*>(parent())->geometry().center();
    }else{
        cp = qApp->desktop()->availableGeometry().center();
    }
    qr.moveCenter(cp);
    move(qr.topLeft());
}

void DMoveableWidget::moveTopRight(){
    QRect pRect;
    if (parent()){
        pRect = static_cast<QWidget*>(parent())->geometry();
    }else{
        pRect = qApp->desktop()->availableGeometry();
    }
    int x = pRect.width() - width();
    move(QPoint(x, 0));
}

void DMoveableWidget::moveTopRightByRect(QRect rect){
    int x = rect.x() + rect.width() - width();
    move(QPoint(x, 0));
}

void DMoveableWidget::moveCenterByRect(QRect rect){
    QRect qr = frameGeometry();
    qr.moveCenter(rect.center());
    move(qr.topLeft());
}

void DMoveableWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton)
    {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
    }
    QWidget::mousePressEvent(event);
}

void DMoveableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}

void DMoveableWidget::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos() - m_dragPosition);
    QWidget::mouseMoveEvent(event);
}

void DMoveableWidget::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);
}

void DMoveableWidget::paintEvent(QPaintEvent *event){
//    QPainter painter(this);
//    painter.setPen(QPen(QColor(255, 255, 255, 51), 2));
//    painter.setBrush(QColor(0, 0 , 0, 204));
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    QRect r(1, 1, width() - 2, height() - 2);
//    painter.drawRoundedRect(r, 2, 2, Qt::RelativeSize);
    QWidget::paintEvent(event);
}

DMoveableWidget::~DMoveableWidget()
{

}

