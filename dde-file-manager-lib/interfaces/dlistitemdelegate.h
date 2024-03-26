/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DLISTITEMDELEGATE_H
#define DLISTITEMDELEGATE_H

#include "dstyleditemdelegate.h"

class DListItemDelegatePrivate;
class DListItemDelegate : public DStyledItemDelegate
{
public:
    explicit DListItemDelegate(DFileViewHelper *parent);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    void drawNotStringData(const QStyleOptionViewItem &opt, int lineHeight, const QRect &rect, const QVariant &data,
                           bool drawBackground, QPainter *painter, const int &column) const;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex &index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;

    QList<QRect> paintGeomertys(const QStyleOptionViewItem &option, const QModelIndex &index, bool sizeHintMode = false) const;

    void updateItemSizeHint();

protected:
    bool eventFilter(QObject *object, QEvent *event);
    bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    Q_DECLARE_PRIVATE(DListItemDelegate)
};

#endif // DLISTITEMDELEGATE_H
