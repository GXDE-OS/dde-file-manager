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
#include "dfiledevice.h"
#include "private/dfiledevice_p.h"

DFM_BEGIN_NAMESPACE

DFileDevice::~DFileDevice()
{

}

DUrl DFileDevice::fileUrl() const
{
    Q_D(const DFileDevice);

    return d->url;
}

bool DFileDevice::setFileUrl(const DUrl &url)
{
    Q_D(DFileDevice);

    d->url = url;

    return true;
}

int DFileDevice::handle() const
{
    return -1;
}

bool DFileDevice::resize(qint64 size)
{
    Q_UNUSED(size)

    return false;
}

bool DFileDevice::flush()
{
    return false;
}

bool DFileDevice::syncToDisk()
{
    return false;
}

DFileDevice::DFileDevice(QObject *parent)
    : DFileDevice(*new DFileDevicePrivate(this), parent)
{

}

DFileDevice::DFileDevice(DFileDevicePrivate &dd, QObject *parent)
    : QIODevice(parent)
    , d_ptr(&dd)
{

}

DFileDevicePrivate::DFileDevicePrivate(DFileDevice *qq)
    : q_ptr(qq)
{

}

DFM_END_NAMESPACE
