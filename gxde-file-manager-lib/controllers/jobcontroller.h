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

#ifndef JOBCONTROLLER_H
#define JOBCONTROLLER_H

#include "ddiriterator.h"

#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QQueue>
#include <QDir>

QT_BEGIN_NAMESPACE
class QElapsedTimer;
QT_END_NAMESPACE

class JobController : public QThread
{
    Q_OBJECT

    Q_PROPERTY(int timeCeiling READ timeCeiling WRITE setTimeCeiling)
    Q_PROPERTY(int countCeiling READ countCeiling WRITE setCountCeiling)

public:
    enum State {
        Started,
        Paused,
        Stoped
    };

    explicit JobController(const DUrl &fileUrl, const DDirIteratorPointer &iterator, bool silent = false, QObject *parent = 0);
    explicit JobController(const DUrl &fileUrl, const QStringList &nameFilters,
                           QDir::Filters filters, bool silent = false, QObject *parent = 0);

    ~JobController();

    State state() const;

    int timeCeiling() const;
    int countCeiling() const;

public slots:
    void start();
    void pause();
    void stop();
    void stopAndDeleteLater();

    void setTimeCeiling(int timeCeiling);
    void setCountCeiling(int countCeiling);

signals:
    void stateChanged(State state);
    void addChildren(const DAbstractFileInfoPointer &info);
    void addChildrenList(const QList<DAbstractFileInfoPointer> &infoList);
    void childrenUpdated(const QList<DAbstractFileInfoPointer> &list);

private:
    bool m_silent;
    DDirIteratorPointer m_iterator;
    DUrl m_fileUrl;
    QStringList m_nameFilters;
    QDir::Filters m_filters;

    State m_state = Stoped;
    QWaitCondition waitCondition;
    QMutex mutex;

    QElapsedTimer *timer = Q_NULLPTR;

    int m_timeCeiling = 2000;
    int m_countCeiling = 9999999;

    void run() Q_DECL_OVERRIDE;
    void setState(State state);
};

#endif // JOBCONTROLLER_H
