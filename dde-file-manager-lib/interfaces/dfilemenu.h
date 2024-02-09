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

#ifndef DFILEMENU_H
#define DFILEMENU_H

#include <QMenu>

#include "dfmglobal.h"
#include "durl.h"

class DFMMenuActionEvent;
class DFileMenu : public QMenu
{
    Q_OBJECT

public:
    explicit DFileMenu(QWidget * parent = 0);

    const QSharedPointer<DFMMenuActionEvent> makeEvent(DFMGlobal::MenuAction action) const;
    void setEventData(const DUrl &currentUrl, const DUrlList &selectedUrls, quint64 eventId = 0, const QObject *sender = 0);

    QAction *actionAt(int index) const;
    QAction *actionAt(const QString &text) const;

    QAction *exec();
    using QMenu::exec;

    quint64 eventId() const;

    QSet<DFMGlobal::MenuAction> ignoreMenuActions() const;
    void setIgnoreMenuActions(const QSet<DFMGlobal::MenuAction> &ignoreMenuActions);

    DUrlList selectedUrls() const;

private:
    DUrl m_currentUrl;
    DUrlList m_selectedUrls;
    quint64 m_eventId = 0;
    const QObject *m_sender = Q_NULLPTR;
    QSet<DFMGlobal::MenuAction> m_ignoreMenuActions; //Don't handle MenuAction list in libdde-file-manager in m_ignoreMenuActions;
};

#endif // DFILEMENU_H
