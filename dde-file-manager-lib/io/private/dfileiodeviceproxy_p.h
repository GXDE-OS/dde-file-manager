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
#ifndef DFILEIODEVICEPROXY_P_H
#define DFILEIODEVICEPROXY_P_H

#include "dfileiodeviceproxy.h"
#include "dfiledevice_p.h"

#include <QPointer>

DFM_BEGIN_NAMESPACE
class DFileIODeviceProxyPrivate : public DFileDevicePrivate
{
public:
    DFileIODeviceProxyPrivate(DFileIODeviceProxy *qq);

    QPointer<QIODevice> device;
};
DFM_END_NAMESPACE

#endif // DFILEIODEVICEPROXY_P_H
