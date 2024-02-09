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

#ifndef DFM_DFILETHUMBNAILPROVIDER_H
#define DFM_DFILETHUMBNAILPROVIDER_H

#include <QThread>
#include <QFileInfo>

#include "dfmglobal.h"

#include <functional>

QT_BEGIN_NAMESPACE
class QMimeType;
QT_END_NAMESPACE

DFM_BEGIN_NAMESPACE

class DThumbnailProviderPrivate;
class DThumbnailProvider : public QThread
{
    Q_OBJECT

public:
    enum Size {
        Small = 64,
        Normal = 128,
        Large = 256,
    };

    static DThumbnailProvider *instance();

    bool hasThumbnail(const QFileInfo &info) const;
    bool hasThumbnail(const QMimeType &mimeType) const;

    QString thumbnailFilePath(const QFileInfo &info, Size size) const;

    QString createThumbnail(const QFileInfo &info, Size size);
    typedef std::function<void(const QString&)> CallBack;
    void appendToProduceQueue(const QFileInfo &info, Size size, CallBack callback = 0);
    void removeInProduceQueue(const QFileInfo &info, Size size);

    QString errorString() const;

    qint64 defaultSizeLimit() const;
    void setDefaultSizeLimit(qint64 size);

    qint64 sizeLimit(const QMimeType &mimeType) const;
    void setSizeLimit(const QMimeType &mimeType, qint64 size);

signals:
    void thumbnailChanged(const QString &sourceFilePath, const QString &thumbnailPath) const;
    void createThumbnailFinished(const QString &sourceFilePath, const QString &thumbnailPath) const;
    void createThumbnailFailed(const QString &sourceFilePath) const;

protected:
    explicit DThumbnailProvider(QObject *parent = 0);
    ~DThumbnailProvider();

    void run() Q_DECL_OVERRIDE;

private:
    QScopedPointer<DThumbnailProviderPrivate> d_ptr;
    Q_DECLARE_PRIVATE(DThumbnailProvider)
};

DFM_END_NAMESPACE

#endif // DFM_DFILETHUMBNAILPROVIDER_H
