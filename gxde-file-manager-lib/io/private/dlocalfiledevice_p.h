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
#ifndef DLOCALFILEDEVICE_P_H
#define DLOCALFILEDEVICE_P_H

#include "dfileiodeviceproxy_p.h"

DFM_BEGIN_NAMESPACE

class DLocalFileDevicePrivate : public DFileIODeviceProxyPrivate
{
public:
    DLocalFileDevicePrivate(dde_file_manager::DLocalFileDevice *qq);

    QFile file;

    Q_DECLARE_PUBLIC(DLocalFileDevice)
};

DFM_END_NAMESPACE

#endif // DLOCALFILEDEVICE_P_H
