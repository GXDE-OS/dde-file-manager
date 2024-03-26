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

#ifndef DFILEITEMDELEGATE_H
#define DFILEITEMDELEGATE_H

#include "dstyleditemdelegate.h"

#include <QPointer>

class FileIconItem;
QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class DIconItemDelegatePrivate;
class DIconItemDelegate : public DStyledItemDelegate
{
    Q_OBJECT

    Q_PROPERTY(QColor focusTextBackgroundBorderColor READ focusTextBackgroundBorderColor WRITE setFocusTextBackgroundBorderColor)
    Q_PROPERTY(bool enabledTextShadow READ enabledTextShadow WRITE setEnabledTextShadow)

public:
    explicit DIconItemDelegate(DFileViewHelper *parent);
    ~DIconItemDelegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex &) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;

    QList<QRect> paintGeomertys(const QStyleOptionViewItem &option, const QModelIndex &index, bool sizeHintMode = false) const;

    QModelIndexList hasWidgetIndexs() const;
    void hideNotEditingIndexWidget();

    QModelIndex expandedIndex() const;
    QWidget *expandedIndexWidget() const;

    int iconSizeLevel() const;
    int minimumIconSizeLevel() const;
    int maximumIconSizeLevel() const;

    int increaseIcon();
    int decreaseIcon();
    int setIconSizeByIconSizeLevel(int level);

    void updateItemSizeHint();

    QColor focusTextBackgroundBorderColor() const;
    bool enabledTextShadow() const;

public slots:
    void setFocusTextBackgroundBorderColor(QColor focusTextBackgroundBorderColor);
    void setEnabledTextShadow(bool enabledTextShadow);

protected:
    void initTextLayout(const QModelIndex &index, QTextLayout *layout) const;

    bool eventFilter(QObject *object, QEvent *event);

    QList<QRectF> drawText(const QModelIndex &index, QPainter *painter, QTextLayout *layout,
                           const QRectF &boundingRect, qreal radius, const QBrush &background,
                           QTextOption::WrapMode wordWrap = QTextOption::WrapAtWordBoundaryOrAnywhere,
                           Qt::TextElideMode mode = Qt::ElideMiddle, int flags = Qt::AlignCenter,
                           const QColor &shadowColor = QColor()) const;

    //using DStyledItemDelegate::drawText;

private:
    void onEditWidgetFocusOut();
    void onTriggerEdit(const QModelIndex &index);
    QSize iconSizeByIconSizeLevel() const;

    Q_DECLARE_PRIVATE(DIconItemDelegate)

    friend class ExpandedItem;
};

#endif // DFILEITEMDELEGATE_H
