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
#include "dlocalfiledevice.h"
#include "private/dlocalfiledevice_p.h"

#include <unistd.h>

DFM_BEGIN_NAMESPACE

DLocalFileDevicePrivate::DLocalFileDevicePrivate(DLocalFileDevice *qq)
    : DFileIODeviceProxyPrivate(qq)
{

}

DLocalFileDevice::DLocalFileDevice(QObject *parent)
    : DFileIODeviceProxy(*new DLocalFileDevicePrivate(this), parent)
{
    setDevice(&d_func()->file);
}

bool DLocalFileDevice::setFileUrl(const DUrl &url)
{
    Q_D(DLocalFileDevice);

    if (!url.isLocalFile())
        return false;

    d->file.setFileName(url.toLocalFile());

    return DFileDevice::setFileUrl(url);
}

bool DLocalFileDevice::setFileName(const QString &absoluteFilePath)
{
    return setFileUrl(DUrl::fromLocalFile(absoluteFilePath));
}

int DLocalFileDevice::handle() const
{
    Q_D(const DLocalFileDevice);

    return d->file.handle();
}

bool DLocalFileDevice::resize(qint64 size)
{
    Q_D(DLocalFileDevice);

    return d->file.resize(size);
}

bool DLocalFileDevice::flush()
{
    Q_D(DLocalFileDevice);

    return d->file.flush();
}

bool DLocalFileDevice::syncToDisk()
{
    Q_D(DLocalFileDevice);

    int ret = fdatasync(d->file.handle());

    if (ret != 0) {
        setErrorString(QString::fromLocal8Bit(strerror(errno)));

        return false;
    }

    return true;
}

DFM_END_NAMESPACE
