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
#ifndef DSTORAGEINFO_H
#define DSTORAGEINFO_H

#include <dfmglobal.h>

#include <QStorageInfo>

DFM_BEGIN_NAMESPACE

class DStorageInfoPrivate;
class DStorageInfo : public QStorageInfo
{
    Q_DECLARE_PRIVATE(DStorageInfo)

public:
    enum PathHint {
        FollowSymlink = 0x01,
    };

    Q_DECLARE_FLAGS(PathHints, PathHint)

    DStorageInfo();
    explicit DStorageInfo(const QString &path, PathHints hints = 0);
    explicit DStorageInfo(const QDir &dir, PathHints hints = 0);
    DStorageInfo(const DStorageInfo &other);
    ~DStorageInfo();

    DStorageInfo &operator=(const DStorageInfo &other);
#ifdef Q_COMPILER_RVALUE_REFS
    DStorageInfo &operator=(DStorageInfo &&other) Q_DECL_NOTHROW { swap(other); return *this; }
#endif

    inline void swap(DStorageInfo &other) Q_DECL_NOTHROW
    { qSwap(d_ptr, other.d_ptr); }

    void setPath(const QString &path, PathHints hints = 0);

    QString rootPath() const;
    QByteArray device() const;
    QByteArray fileSystemType() const;

    qint64 bytesTotal() const;
    qint64 bytesFree() const;
    qint64 bytesAvailable() const;

    bool isReadOnly() const;
    bool isLocalDevice() const;
    bool isLowSpeedDevice() const;

    bool isValid() const;
    void refresh();

    static bool inSameDevice(QString path1, QString path2, PathHints hints = 0);
    static bool inSameDevice(const DUrl &url1, const DUrl &url2, PathHints hints = 0);
    static bool isLocalDevice(const QString &path);
    static bool isLowSpeedDevice(const QString &path);

private:
    friend bool operator==(const DStorageInfo &first, const DStorageInfo &second);

    QExplicitlySharedDataPointer<DStorageInfoPrivate> d_ptr;
};

inline bool operator==(const DStorageInfo &first, const DStorageInfo &second)
{
    if (first.d_ptr == second.d_ptr)
        return true;

    return first.device() == second.device() && first.rootPath() == second.rootPath();
}

inline bool operator!=(const DStorageInfo &first, const DStorageInfo &second)
{
    return !(first == second);
}

DFM_END_NAMESPACE

QT_BEGIN_NAMESPACE
#ifndef QT_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug debug, const DFM_NAMESPACE::DStorageInfo &info);
#endif
QT_END_NAMESPACE

Q_DECLARE_SHARED(DFM_NAMESPACE::DStorageInfo)
Q_DECLARE_METATYPE(DFM_NAMESPACE::DStorageInfo*)

#endif // DSTORAGEINFO_H
