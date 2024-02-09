/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include "dfmactionbutton.h"

#include <QAction>
#include <QDebug>
#include <QStyleOptionButton>
#include <QStylePainter>
#include <QApplication>

DFMActionButton::DFMActionButton(QWidget *parent)
    : QAbstractButton(parent)
{
    setMouseTracking(true);
}

void DFMActionButton::setAction(QAction *action)
{
    auto onActionChanged = [this, action] {
        setVisible(action->isVisible());
        setEnabled(action->isEnabled());
        setCheckable(action->isCheckable());
        setChecked(action->isChecked());
    };

    onActionChanged();
    connect(action, &QAction::changed, this, onActionChanged);
    connect(this, &DFMActionButton::clicked, action, &QAction::trigger);
    connect(action, &QAction::triggered, this, &DFMActionButton::setChecked);

    addAction(action);
}

QAction *DFMActionButton::action() const
{
    return actions().value(0);
}

void DFMActionButton::paintEvent(QPaintEvent *)
{
    QStylePainter p(this);
    QStyleOption option;
    option.rect = rect();
    option.initFrom(this);

    if (isChecked()) {
        option.state |= QStyle::State_On;
    }

    p.drawPrimitive(QStyle::PE_Widget, option);

    if (action()){
        const QIcon &icon = action()->icon();
        QSize pixmapSize(16, 16);
        QPixmap pixmap = icon.pixmap(pixmapSize, QIcon::Normal, option.state.testFlag(QStyle::State_On) ? QIcon::On : QIcon::Off);

        if (option.state.testFlag(QStyle::State_MouseOver)) {
            pixmap = icon.pixmap(pixmapSize, QIcon::Active, option.state.testFlag(QStyle::State_On) ? QIcon::On : QIcon::Off);
        }
        pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
        p.drawItemPixmap(option.rect, Qt::AlignCenter, pixmap);
    }
}
