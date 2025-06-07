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

#ifndef IMAGEPREVIEWPLUGIN_H
#define IMAGEPREVIEWPLUGIN_H

#include <QWidget>
#include <QImage>
#include <QPointer>

#include "dfmfilepreview.h"
#include "durl.h"

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class ImageView;

DFM_BEGIN_NAMESPACE

class ImagePreview : public DFMFilePreview
{
    Q_PLUGIN_METADATA(IID PreviewInterface_iid FILE "dde-image-preview-plugin.json")
    Q_INTERFACES(PreviewInterface)
public:
    explicit ImagePreview(QObject *parent = 0);
    ~ImagePreview();

    bool canPreview(const QUrl &url, QByteArray *format = nullptr) const;

    void initialize(QWidget *window, QWidget *statusBar) Q_DECL_OVERRIDE;

    bool setFileUrl(const DUrl &url) Q_DECL_OVERRIDE;
    DUrl fileUrl() const override;

    QWidget *contentWidget() const Q_DECL_OVERRIDE;

    QString title() const Q_DECL_OVERRIDE;

    void copyFile() const override;

private:
    DUrl m_url;
    QPointer<QLabel> m_messageStatusBar;
    QPointer<ImageView> m_imageView;
    QString m_title;
};

DFM_END_NAMESPACE

#endif // IMAGEPREVIEWPLUGIN_H
