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

#include "windowmanager.h"
#include "dfilemanagerwindow.h"
#include "dabstractfilewatcher.h"
#include "dabstractfileinfo.h"
#include "dfileservices.h"
#include "dfmeventdispatcher.h"
#include "dfmapplication.h"
#include "dfmsettings.h"

#include "app/define.h"
#include "app/filesignalmanager.h"
#include "dfmevent.h"

#include "fileoperations/filejob.h"

#include "dialogs/dialogmanager.h"

#include "qobjecthelper.h"

#include "singleton.h"

#include "shutil/fileutils.h"
#include <DThemeManager>
#include <DApplication>

#include <QThread>
#include <QDebug>
#include <QApplication>
#include <QX11Info>
#include <QScreen>
#include <QWindow>
#include <QTimer>
#include <QProcess>

DTK_USE_NAMESPACE

DFM_USE_NAMESPACE

enum NetWmState {
    NetWmStateAbove = 0x1,
    NetWmStateBelow = 0x2,
    NetWmStateFullScreen = 0x4,
    NetWmStateMaximizedHorz = 0x8,
    NetWmStateMaximizedVert = 0x10,
    NetWmStateModal = 0x20,
    NetWmStateStaysOnTop = 0x40,
    NetWmStateDemandsAttention = 0x80
};

Q_DECLARE_FLAGS(NetWmStates, NetWmState)

QHash<const QWidget*, quint64> WindowManager::m_windows;
int WindowManager::m_count = 0;

WindowManager::WindowManager(QObject *parent) : QObject(parent)
{
#ifdef AUTO_RESTART_DEAMON
    m_restartProcessTimer = new QTimer(this);
    m_restartProcessTimer->setInterval(1000 * 60 * 5);
#endif
    initConnect();
}

class WindowManager_ : public WindowManager {};
Q_GLOBAL_STATIC(WindowManager_, wmGlobal)

WindowManager *WindowManager::instance()
{
    return wmGlobal;
}

WindowManager::~WindowManager()
{

}

void WindowManager::initConnect()
{
    connect(fileSignalManager, &FileSignalManager::requestOpenNewWindowByUrl, this, &WindowManager::showNewWindow);
    connect(fileSignalManager, &FileSignalManager::aboutToCloseLastActivedWindow, this, &WindowManager::onLastActivedWindowClosed);

#ifdef AUTO_RESTART_DEAMON
    connect(m_restartProcessTimer, &QTimer::timeout, this, &WindowManager::reastartAppProcess);
#endif
}

void WindowManager::loadWindowState(DFileManagerWindow *window)
{
    const QVariantMap &state = DFMApplication::appObtuselySetting()->value("WindowManager", "WindowState").toMap();

    int width = state.value("width").toInt();
    int height = state.value("height").toInt();
    int windowState = state.value("state").toInt();
    window->resize(width, height);
    window->setWindowState(static_cast<Qt::WindowState>(windowState));
}


void WindowManager::saveWindowState(DFileManagerWindow *window)
{
//    m_fmStateManager->fmState()->setViewMode(window->getFileViewMode());

    /// The power by dxcb platform plugin
    NetWmStates states = (NetWmStates)window->window()->windowHandle()->property("_d_netWmStates").toInt();

    if ((states & (NetWmStateMaximizedHorz | NetWmStateMaximizedVert)) == 0) {
        QVariantMap state;
        state["width"] = window->size().width();
        state["height"] = window->size().height();
        DFMApplication::appObtuselySetting()->setValue("WindowManager", "WindowState", state);
    }
}

DUrl WindowManager::getUrlByWindowId(quint64 windowId)
{
    if (getWindowById(windowId)){
         DFileManagerWindow* window = qobject_cast<DFileManagerWindow*>(getWindowById(windowId));
         return window->currentUrl();
    }
    return DUrl::fromLocalFile(QDir::homePath());
}

bool WindowManager::tabAddableByWinId(const quint64 &winId)
{
    DFileManagerWindow* window = qobject_cast<DFileManagerWindow*>(getWindowById(winId));
    if(window)
        return window->tabAddable();
    return false;

}

bool WindowManager::enableAutoQuit() const
{
#ifdef AUTO_RESTART_DEAMON
    return m_enableAutoQuit;
#else
    return false;
#endif
}

void WindowManager::showNewWindow(const DUrl &url, const bool& isNewWindow)
{
    if (!isNewWindow){
        for(int i=0; i< m_windows.count(); i++){
            QWidget* window = const_cast<QWidget *>(m_windows.keys().at(i));
            DUrl currentUrl = static_cast<DFileManagerWindow *>(window)->currentUrl();
            if (currentUrl == url){
                qDebug() << currentUrl << static_cast<DFileManagerWindow *>(window);
                qApp->setActiveWindow(static_cast<DFileManagerWindow *>(window));
                return;
            }
        }
    }

    QX11Info::setAppTime(QX11Info::appUserTime());
    DFileManagerWindow *window = new DFileManagerWindow(url.isEmpty() ? DFMApplication::instance()->appUrlAttribute(DFMApplication::AA_UrlOfNewWindow) : url);
    loadWindowState(window);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();

    qDebug() << "new window" << window->winId() << url;

    connect(window, &DFileManagerWindow::aboutToClose,
            this, &WindowManager::onWindowClosed);

    m_windows.insert(window, window->winId());
    window->requestToSelectUrls(); //###: here, when selected files and then drag a tab to create a new window.
                                   //     will select these files again in new window.

    if (m_windows.count() == 1){
        QPoint pos = QCursor::pos();
        QRect currentScreenGeometry;

        for (QScreen *screen : qApp->screens()) {
            if (screen->geometry().contains(pos)) {
                currentScreenGeometry = screen->geometry();
            }
        }

        if (currentScreenGeometry.isEmpty()) {
            currentScreenGeometry = qApp->primaryScreen()->geometry();
        }

        window->moveCenter(currentScreenGeometry.center());
    }
//    window->setFileViewMode(m_fmStateManager->fmState()->viewMode());

    qApp->setActiveWindow(window);
}



quint64 WindowManager::getWindowId(const QWidget *window)
{
    int winId = m_windows.value(window->topLevelWidget(), 0);

    if (winId != 0)
        return winId;

    const QWidget *newW = window;

    while (newW) {
        if (newW->inherits("DFileManagerWindow")) {
            return newW->winId();
        }

        newW = newW->parentWidget();
    }

    return window->window()->internalWinId();
}

QWidget *WindowManager::getWindowById(quint64 winId)
{
    if (winId <= 0)
        return Q_NULLPTR;

    const QWidget *widget = m_windows.key(winId);

    if (widget)
        return const_cast<QWidget*>(widget);

    for (QWidget *widget : qApp->topLevelWidgets()) {
        if (widget->internalWinId() == winId)
            return widget;
    }

    return Q_NULLPTR;
}

void WindowManager::reastartAppProcess()
{
    if (m_windows.count() == 0){
        if (dialogManager->isTaskDialogEmpty()){
            // 当没有顶级窗口时才允许应用自动退出
            if (qApp->topLevelWindows().isEmpty()) {
                qApp->quit();
            }
        }
    }
}

void WindowManager::setEnableAutoQuit(bool enableAutoQuit)
{
#ifdef AUTO_RESTART_DEAMON
    if (m_enableAutoQuit == enableAutoQuit)
        return;

    m_enableAutoQuit = enableAutoQuit;

    if (m_enableAutoQuit) {
        m_restartProcessTimer->start();
    } else {
        m_restartProcessTimer->stop();
    }
#else
    Q_UNUSED(enableAutoQuit)
#endif
}

void WindowManager::onWindowClosed()
{
    if (m_windows.count() == 1){
        DFileManagerWindow* window = static_cast<DFileManagerWindow*>(sender());
        saveWindowState(window);
        dialogManager->closeAllPropertyDialog();
    }
    m_windows.remove(static_cast<const QWidget*>(sender()));
}

void WindowManager::onLastActivedWindowClosed(quint64 winId)
{
    QList<quint64> winIds = m_windows.values();
    foreach (quint64 id, winIds) {
        if (id != winId){
            getWindowById(id)->close();
        }
    }

    if (QWidget *window = getWindowById(winId))
        window->close();

    qApp->quit();
}
