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

#include "desktopfileinfo.h"
#include "private/dfileinfo_p.h"
#include "app/define.h"
#include "controllers/trashmanager.h"

#include "shutil/properties.h"
#include "shutil/fileutils.h"
#include "interfaces/dfmstandardpaths.h"
#include "interfaces/dfileservices.h"
#include "interfaces/dfmstandardpaths.h"

#include "singleton.h"

#include <QSettings>
#include <QLocale>

class DesktopFileInfoPrivate : public DFileInfoPrivate
{
public:
    DesktopFileInfoPrivate(const DUrl &url, DesktopFileInfo *qq)
        : DFileInfoPrivate(url, qq)
    {
        updateInfo(url);
    }

    QString name;
    QString genericName;
    QString exec;
    QString iconName;
    QString type;
    QStringList categories;
    QStringList mimeType;
    QString deepinID;
    QString deepinVendor;

    void updateInfo(const DUrl &fileUrl);
};

DesktopFileInfo::DesktopFileInfo(const DUrl &fileUrl)
    : DFileInfo(*new DesktopFileInfoPrivate(fileUrl, this))
{

}

DesktopFileInfo::DesktopFileInfo(const QFileInfo &fileInfo)
    : DesktopFileInfo(DUrl::fromLocalFile(fileInfo.absoluteFilePath()))
{
    d_func()->fileInfo = fileInfo;
}

DesktopFileInfo::~DesktopFileInfo()
{

}

QString DesktopFileInfo::getName() const
{
    Q_D(const DesktopFileInfo);

    if (d->deepinVendor == QStringLiteral("deepin") && !(d->genericName.isEmpty())) {
        return d->genericName;
    }

    return d->name;
}

QString DesktopFileInfo::getExec() const
{
    Q_D(const DesktopFileInfo);

    return d->exec;
}

QString DesktopFileInfo::getIconName() const
{
    Q_D(const DesktopFileInfo);

     //special handling for trash desktop file which has tash datas
    if (d->iconName == "user-trash") {
        if (!TrashManager::isEmpty())
            return "user-trash-full";
    }

    return d->iconName;
}

QString DesktopFileInfo::getType() const
{
    Q_D(const DesktopFileInfo);

    return d->type;
}

QStringList DesktopFileInfo::getCategories() const
{
    Q_D(const DesktopFileInfo);

    return d->categories;
}

QIcon DesktopFileInfo::fileIcon() const
{
    Q_D(const DesktopFileInfo);

    if (Q_LIKELY(!d->icon.isNull())) {
        if (Q_LIKELY(!d->icon.availableSizes().isEmpty()))
            return d->icon;

        d->icon = QIcon();
    }

    const QString &iconName = this->iconName();

    if (iconName.startsWith("data:image/")) {
        int first_semicolon  = iconName.indexOf(';', 11);

        if (first_semicolon > 11) {
            // iconPath is a string representing an inline image.
//            const QString &format = iconName.mid(11, first_semicolon - 11);

            int base64str_pos = iconName.indexOf("base64,", first_semicolon);

            if (base64str_pos > 0) {
                QPixmap pixmap;

                bool ok = pixmap.loadFromData(QByteArray::fromBase64(iconName.mid(base64str_pos + 7).toLatin1())/*, format.toLatin1().constData()*/);

                if (ok) {
                    d->icon = QIcon(pixmap);
                } else {
                    d->icon = QIcon::fromTheme("application-default-icon");
                }
            }
        }
    } else {
        const QString &currentDir = QDir::currentPath();

        QDir::setCurrent(absolutePath());

        QFileInfo fileInfo(iconName.startsWith("~") ? (QDir::homePath() + iconName.mid(1)) : iconName);

        if (!fileInfo.exists())
            fileInfo.setFile(QUrl::fromUserInput(iconName).toLocalFile());

        if (fileInfo.exists()) {
            d->icon = QIcon(fileInfo.absoluteFilePath());
        }

        QDir::setCurrent(currentDir);

        if (!d->icon.isNull() && QPixmap(fileInfo.absoluteFilePath()).isNull())
            d->icon = QIcon();
    }

    if (d->icon.isNull())
        return DFileInfo::fileIcon();

    return d->icon;
}

void DesktopFileInfo::refresh()
{
    Q_D(DesktopFileInfo);

    DFileInfo::refresh();
    d->updateInfo(fileUrl());
}

QString DesktopFileInfo::iconName() const
{
    return getIconName();
}

QString DesktopFileInfo::genericIconName() const
{
    return QStringLiteral("application-default-icon");
}

QString DesktopFileInfo::fileDisplayName() const
{
    return getName();
}

QString DesktopFileInfo::fileNameOfRename() const
{
    return fileDisplayName();
}

QString DesktopFileInfo::baseNameOfRename() const
{
    return fileDisplayName();
}

QString DesktopFileInfo::suffixOfRename() const
{
    return QString();
}

QMap<QString, QVariant> DesktopFileInfo::getDesktopFileInfo(const DUrl &fileUrl)
{
    QMap<QString, QVariant> map;
    QSettings settings(fileUrl.path(), QSettings::IniFormat);

    settings.setIniCodec("utf-8");
    settings.beginGroup("Desktop Entry");
    // Loads .desktop file (read from 'Desktop Entry' group)
    Properties desktop(fileUrl.path(), "Desktop Entry");

    const QString &name = desktop.value("Name[" + QLocale::system().name() + "]", settings.value("Name[" + QLocale::system().name() + "]")).toString();
    const QString &genericName = desktop.value("GenericName[" + QLocale::system().name() + "]", settings.value("GenericName[" + QLocale::system().name() + "]")).toString();

    if (name.isEmpty()) {
        map["Name"] = desktop.value("Name", settings.value("Name"));
    } else {
        map["Name"] = name;
    }

    if (genericName.isEmpty()) {
        map["GenericName"] = desktop.value("GenericName", settings.value("GenericName"));
    } else {
        map["GenericName"] = genericName;
    }

    map["Exec"] = desktop.value("Exec", settings.value("Exec"));
    map["Icon"] = desktop.value("Icon", settings.value("Icon"));
    map["Type"] = desktop.value("Type", settings.value("Type", "Application"));
    map["Categories"] = desktop.value("Categories", settings.value("Categories")).toString().remove(" ").split(";");
    map["MimeType"] = desktop.value("MimeType", settings.value("MimeType")).toString().remove(" ").split(";");
    map["DeepinID"] = desktop.value("X-Deepin-AppID", settings.value("X-Deepin-AppID")).toString();
    map["DeepinVendor"] = desktop.value("X-Deepin-Vendor", settings.value("X-Deepin-Vendor")).toString();

    return map;
}

QVector<MenuAction> DesktopFileInfo::menuActionList(DAbstractFileInfo::MenuType type) const
{
    Q_D(const DesktopFileInfo);
    if(d->deepinID == "dde-trash" || d->deepinID == "dde-computer"){
        QVector<MenuAction> actions;
        actions << MenuAction::Open
        << MenuAction::Separator;

        if(d->deepinID == "dde-trash"){
            actions << MenuAction::ClearTrash
                    << MenuAction::Separator;
        }

        if(type == SingleFile)
            actions << MenuAction::CreateSymlink;

        actions << MenuAction::Property;

        return actions;
    } else{
        return DFileInfo::menuActionList(type);
    }
}

QSet<MenuAction> DesktopFileInfo::disableMenuActionList() const
{
    Q_D(const DesktopFileInfo);
    if(d->deepinID == "dde-trash"){
        QSet<MenuAction> actions;
        if(FileUtils::filesCount(DFMStandardPaths::location(DFMStandardPaths::TrashFilesPath)) <= 0)
            actions << MenuAction::ClearTrash;
        return actions;
    }
    return DFileInfo::disableMenuActionList();
}

QList<QIcon> DesktopFileInfo::additionalIcon() const
{
    return QList<QIcon>();
}

Qt::DropActions DesktopFileInfo::supportedDragActions() const
{
    Q_D(const DesktopFileInfo);
    if(d->deepinID == "dde-trash" || d->deepinID == "dde-computer"){
        return Qt::IgnoreAction;
    }

    return DFileInfo::supportedDragActions();
}

bool DesktopFileInfo::canDrop() const
{
    //ignore drop event for computer desktop file
    Q_D(const DesktopFileInfo);
    if(d->deepinID == "dde-computer")
        return false;

    return DFileInfo::canDrop();
}

DUrl DesktopFileInfo::trashDesktopFileUrl()
{
    return DUrl::fromLocalFile(DFMStandardPaths::location(DFMStandardPaths::DesktopPath) + "/dde-trash.desktop");
}

DUrl DesktopFileInfo::computerDesktopFileUrl()
{
    return DUrl::fromLocalFile(DFMStandardPaths::location(DFMStandardPaths::DesktopPath) + "/dde-computer.desktop");
}

void DesktopFileInfoPrivate::updateInfo(const DUrl &fileUrl)
{
    const QMap<QString, QVariant> &map = DesktopFileInfo::getDesktopFileInfo(fileUrl);

    name = map.value("Name").toString();
    genericName = map.value("GenericName").toString();
    exec = map.value("Exec").toString();
    iconName = map.value("Icon").toString();
    type = map.value("Type").toString();
    categories = map.value("Categories").toStringList();
    mimeType = map.value("MimeType").toStringList();
    deepinID = map.value("DeepinID").toString();
    deepinVendor = map.value("DeepinVendor").toString();
    // Fix categories
    if (categories.first().compare("") == 0) {
      categories.removeFirst();
    }
}
