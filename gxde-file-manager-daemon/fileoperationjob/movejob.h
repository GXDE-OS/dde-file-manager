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

#ifndef MOVEJOB_H
#define MOVEJOB_H

#include <QObject>
#include "basejob.h"

class MoveJobAdaptor;

class MoveJob : public BaseJob
{
    Q_OBJECT
public:
    explicit MoveJob(const QStringList &filelist, const QString &targetDir, QObject *parent = 0);
    ~MoveJob();

    static QString BaseObjectPath;
    static QString PolicyKitActionId;
    static int JobId;

signals:
    void Done(const QString& message);

public slots:
    void Execute();

private:
    QStringList m_filelist;
    QString m_targetDir;
    int m_jobId = 0;

    MoveJobAdaptor* m_adaptor;
};

#endif // MOVEJOB_H
