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

#ifndef WALLPAPERLIST_H
#define WALLPAPERLIST_H

#include <QScrollArea>
#include <QPropertyAnimation>

#include <dtkwidget_global.h>
#include <danchors.h>

#include <com_deepin_wm.h>

QT_BEGIN_NAMESPACE
class QHBoxLayout;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE
class DImageButton;
DWIDGET_END_NAMESPACE

DWIDGET_USE_NAMESPACE

class WallpaperItem;
class AppearanceDaemonInterface;
class QGSettings;
class WallpaperList : public QScrollArea
{
    Q_OBJECT

public:
    explicit WallpaperList(QWidget * parent = 0);
    ~WallpaperList();

    WallpaperItem * addWallpaper(const QString &path);
    void removeWallpaper(const QString &path);

    void scrollList(int step, int duration = 100);

    void prevPage();
    void nextPage();

    QSize gridSize() const;
    void setGridSize(const QSize &size);

    void addItem(WallpaperItem *item);
    QWidget *item(int index) const;
    QWidget *itemAt(const QPoint &pos) const;
    QWidget *itemAt(int x, int y) const;
    int count() const;

    void clear();

    void updateItemThumb();

signals:
    void itemPressed(QString data) const;
    void mouseOverItemChanged(QString path, QPoint pos) const;

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    QWidget *m_contentWidget;
    QHBoxLayout *m_contentLayout;

    com::deepin::wm *m_wmInter;

    //It was handpicked item, Used for wallpaper page
    WallpaperItem *prevItem = Q_NULLPTR;
    WallpaperItem *nextItem = Q_NULLPTR;

    DAnchors<DImageButton> prevButton;
    DAnchors<DImageButton> nextButton;

    QPropertyAnimation scrollAnimation;

    QSize m_gridSize;

    QList<WallpaperItem*> m_items;

    QTimer *m_updateTimer;

    void updateBothEndsItem();
    void showDeleteButtonForItem(const WallpaperItem *item) const;

    friend class Frame;

private slots:
    void wallpaperItemPressed();
    void wallpaperItemHoverIn();
    void wallpaperItemHoverOut();
};

#endif // WALLPAPERLIST_H
