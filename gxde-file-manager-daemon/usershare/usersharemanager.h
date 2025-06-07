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

#ifndef USERSHAREDAEMONMANAGER_H
#define USERSHAREDAEMONMANAGER_H

#include <QObject>

class UserShareAdaptor;

class UserShareManager : public QObject
{
    Q_OBJECT
public:
    explicit UserShareManager(QObject *parent = 0);
    ~UserShareManager();

    static QString ObjectPath;

signals:

public slots:
    bool addGroup(const QString &groupName);
    bool addUserToGroup(const QString &userName, const QString &groupName);
    bool setUserSharePassword(const QString &username, const QString &passward);
    bool restartSambaService();
private:
    UserShareAdaptor* m_userShareAdaptor = NULL;
};

#endif // USERSHAREDAEMONMANAGER_H
