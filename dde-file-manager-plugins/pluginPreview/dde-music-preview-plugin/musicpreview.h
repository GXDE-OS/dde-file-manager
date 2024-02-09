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

#ifndef MUSICPREVIEWPLUGIN_H
#define MUSICPREVIEWPLUGIN_H

#include "dfmfilepreview.h"
#include "durl.h"

#include <QPointer>

class MusicMessageView;
class ToolBarFrame;

DFM_BEGIN_NAMESPACE

class MusicPreview : public DFMFilePreview
{
    Q_OBJECT

public:
    MusicPreview(QObject *parent = 0);
    ~MusicPreview();

    bool setFileUrl(const DUrl &url) Q_DECL_OVERRIDE;
    DUrl fileUrl() const override;

    QWidget *contentWidget() const Q_DECL_OVERRIDE;
    QWidget *statusBarWidget() const Q_DECL_OVERRIDE;
    Qt::Alignment statusBarWidgetAlignment() const Q_DECL_OVERRIDE;

    void play() Q_DECL_OVERRIDE;
    void pause() Q_DECL_OVERRIDE;
    void stop() Q_DECL_OVERRIDE;

    bool canPreview(const DUrl &url) const;

private:
    DUrl m_url;

    QPointer<MusicMessageView> m_musicView;
    QPointer<ToolBarFrame> m_statusBarFrame;
};

DFM_END_NAMESPACE

#endif // MUSICPREVIEWPLUGIN_H
