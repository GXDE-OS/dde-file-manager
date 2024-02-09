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

#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QObject>
#include "debugobejct.h"
#include <QString>
#include <QDateTime>
#include "dfileinfo.h"

class BookMark;
typedef QExplicitlySharedDataPointer<BookMark> BookMarkPointer;

class BookMark : public DAbstractFileInfo
{
public:
    BookMark(const DUrl &url);
    BookMark(const QString &name, const DUrl &sourceUrl);
    ~BookMark();

    DUrl sourceUrl() const;
    QString getName() const;

    bool exists() const override;

    QString fileDisplayName() const Q_DECL_OVERRIDE;

    bool canRedirectionFileUrl() const Q_DECL_OVERRIDE;
    DUrl redirectedFileUrl() const Q_DECL_OVERRIDE;

    DUrl parentUrl() const Q_DECL_OVERRIDE;

    DUrl getUrlByNewFileName(const QString &name) const override;

    QDateTime created() const override;
    QDateTime lastModified() const override;

public:
    QDateTime m_created;
    QDateTime m_lastModified;
    QString mountPoint;
    QString locateUrl;
    // AbstractFileInfo interface

private:
    // always care about these two value when using it.
    // related functions: exists(), canRedirectionFileUrl(), redirectionFileUrl()
    mutable QString udisksDBusPath;
    mutable QString udisksMountPoint;
};

#endif // BOOKMARK_H
