/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     Gary Wang <wzc782970009@gmail.com>
 *
 * Maintainer: Gary Wang <wangzichong@deepin.com>
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
#include "dfmfilecrumbcontroller.h"
#include "dfmcrumbitem.h"

#include "controllers/pathmanager.h"
#include "deviceinfo/udiskdeviceinfo.h"
#include "deviceinfo/udisklistener.h"

#include "dfileservices.h"
#include "dfileinfo.h"
#include "app/define.h"
#include "singleton.h"

#include <QStandardPaths>
#include <QStorageInfo>

#include <QDebug>

DFM_BEGIN_NAMESPACE

DFMFileCrumbController::DFMFileCrumbController(QObject *parent)
    : DFMCrumbInterface(parent)
{
    homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).last();
}

DFMFileCrumbController::~DFMFileCrumbController()
{

}

bool DFMFileCrumbController::supportedUrl(DUrl url)
{
    return (url.scheme() == FILE_SCHEME);
}

QList<CrumbData> DFMFileCrumbController::seprateUrl(const DUrl &url)
{
    QList<CrumbData> list;
    QString prefixPath = "/";
    DAbstractFileInfoPointer info = DFileService::instance()->createFileInfo(nullptr, url);
    const QString &path = info->toLocalFile();

    if (path.isEmpty()) {
        return list;
    }

    if (path.startsWith(homePath)) {
        prefixPath = homePath;
        CrumbData data(DUrl::fromLocalFile(homePath), getDisplayName("Home"), "CrumbIconButton.Home");
        list.append(data);
    } else {
        QStorageInfo storageInfo(path);
        UDiskDeviceInfoPointer deviceInfo = deviceListener->getDeviceByPath(path);
        QString iconName = QStringLiteral("CrumbIconButton.Disk");
        prefixPath = storageInfo.rootPath();

        if (deviceInfo) {
            switch (deviceInfo->getMediaType()) {
            case UDiskDeviceInfo::MediaType::removable:
                iconName = QStringLiteral("CrumbIconButton.Usb");
                break;
            case UDiskDeviceInfo::MediaType::dvd:
                iconName = QStringLiteral("CrumbIconButton.Dvd");
                break;
            default:
                break;
            }
        }

        if (prefixPath == "/") {
            CrumbData data(DUrl(FILE_ROOT), getDisplayName("System Disk"), "CrumbIconButton.Disk");
            list.append(data);
        } else {
            CrumbData data(DUrl::fromLocalFile(prefixPath), QString(), iconName);
            list.append(data);
        }
    }

    DUrlList urlList = info->parentUrlList();
    urlList.insert(0, url);

    DAbstractFileInfoPointer infoPointer;
    // Push urls into crumb list (without prefix url)
    DUrlList::const_reverse_iterator iter = urlList.crbegin();
    while (iter != urlList.crend()) {
        const DUrl & oneUrl = *iter;
        if (!prefixPath.startsWith(oneUrl.toLocalFile())) {
            QString displayText = oneUrl.fileName();
            // Check for possible display text.
            infoPointer = DFileService::instance()->createFileInfo(nullptr, oneUrl);
            if (infoPointer) {
                displayText = infoPointer->fileDisplayName();
            }
            CrumbData data(oneUrl, displayText);
            list.append(data);
        }
        iter++;
    }

    return list;
}

// blumia: avoid using this, recommand using the following way:
//         DAbstractFileInfoPointer info = DFileService::instance()->createFileInfo(nullptr, oneUrl);
//         then you can get a proper display name via `info->fileDisplayName()`.
//         We are using this since not all path can got a proper display name yet.
// TODO: When backend is ready, switch to DAbstractFileInfo::fileDisplayName().
QString DFMFileCrumbController::getDisplayName(const QString &name) const
{
    QString text = Singleton<PathManager>::instance()->getSystemPathDisplayName(name);
    return text.isEmpty() ? name : text;
}

DFM_END_NAMESPACE
