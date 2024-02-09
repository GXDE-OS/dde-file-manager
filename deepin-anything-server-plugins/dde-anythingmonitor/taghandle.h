/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     shihua <tangtong@deepin.com>
 *
 * Maintainer: shihua <tangtong@deepin.com>
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
#ifndef TAGHANDLE_H
#define TAGHANDLE_H

#include <dasinterface.h>


using namespace DAS_NAMESPACE;

class TagHandle : public DASInterface
{
    Q_OBJECT

public:
    explicit TagHandle(QObject *const parent = nullptr)
        : DASInterface{ parent } {}
    virtual ~TagHandle() = default;

    TagHandle(const TagHandle &other) = delete;
    TagHandle &operator=(const TagHandle &other) = delete;


    virtual void onFileCreate(const QByteArrayList &files) override;
    virtual void onFileDelete(const QByteArrayList &files) override;
    virtual void onFileRename(const QList<QPair<QByteArray, QByteArray>> &files) override;
};

#endif // TAGHANDLE_H
