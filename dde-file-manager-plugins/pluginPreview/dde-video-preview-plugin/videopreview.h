/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#ifndef VIDEOPREVIEW_H
#define VIDEOPREVIEW_H

#include "dfmfilepreview.h"
#include "durl.h"

#include <playlist_model.h>

#include <QFileInfo>
#include <QPointer>

DFM_BEGIN_NAMESPACE

class VideoWidget;
class VideoStatusBar;

class VideoPreview : public DFMFilePreview
{
    Q_OBJECT

public:
    explicit VideoPreview(QObject* parent = 0);
    ~VideoPreview();

    bool setFileUrl(const DUrl &url) Q_DECL_OVERRIDE;
    DUrl fileUrl() const override;

    QWidget *contentWidget() const Q_DECL_OVERRIDE;
    QWidget *statusBarWidget() const override;

    bool showStatusBarSeparator() const Q_DECL_OVERRIDE;
    Qt::Alignment statusBarWidgetAlignment() const override;

    void play() override;
    void pause() override;
    void stop() override;

private:
    QUrl videoUrl;
    QPointer<VideoWidget> playerWidget;
    dmr::MovieInfo info;
    QPointer<VideoStatusBar> statusBar;

    friend class VideoWidget;
    friend class VideoStatusBar;
};

DFM_END_NAMESPACE

#endif // VIDEOPREVIEW_H
