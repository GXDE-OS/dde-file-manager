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

#ifndef DTOOLBAR_H
#define DTOOLBAR_H

#include <QFrame>
#include <QStackedWidget>
#include "dfileview.h"
#include "dstatebutton.h"

class DFMEvent;
class HistoryStack;

QT_BEGIN_NAMESPACE
class QHBoxLayout;
QT_END_NAMESPACE

DWIDGET_BEGIN_NAMESPACE
class DGraphicsClipEffect;
DWIDGET_END_NAMESPACE

DFM_BEGIN_NAMESPACE
class DFMCrumbBar;
class DFMCrumbItem;
class DFMAddressBar;
DFM_END_NAMESPACE

DFM_USE_NAMESPACE

class DToolBar : public QFrame
{
    Q_OBJECT
public:
    explicit DToolBar(QWidget *parent = nullptr);
    ~DToolBar();
    static const int ButtonWidth;
    static const int ButtonHeight;
    void initData();
    void initUI();
    void initAddressToolBar();
    void initContollerToolBar();
    void initConnect();
    DFMCrumbBar *getCrumbWidget();
    QPushButton* getSettingsButton();
    void addHistoryStack();

    int navStackCount() const;
    void updateBackForwardButtonsState();

    void setCustomActionList(const QList<QAction*> &list);
    void triggerActionByIndex(int index);

signals:
    void refreshButtonClicked();
    void toolbarUrlChanged(const DUrl& url);

public slots:
    void searchBarActivated();
    void searchBarDeactivated();
    void searchBarTextEntered(const QString textEntered);
    void onSearchButtonClicked();
    void crumbSelected(const DFMCrumbItem* item);
    void currentUrlChanged(const DFMEvent &event);

    void back();
    void forward();

    void handleHotkeyCtrlF(quint64 winId);
    void handleHotkeyCtrlL(quint64 winId);

    void moveNavStacks(int from, int to);
    void removeNavStackAt(int index);
    void switchHistoryStack(const int index );

private:
    void toggleSearchButtonState(bool asb = true);
    void pushUrlToHistoryStack(DUrl url);
    void onBackButtonClicked();
    void onForwardButtonClicked();

    bool m_searchState = false;
    QFrame* m_addressToolBar;
    QPushButton* m_backButton = nullptr;
    QPushButton* m_forwardButton = nullptr;
    QPushButton* m_searchButton = nullptr;
    QPushButton* m_settingsButton = nullptr;
    QFrame* m_contollerToolBar;
    DGraphicsClipEffect *m_contollerToolBarClipMask;
    QHBoxLayout *m_contollerToolBarContentLayout;
    QList<QAction*> m_actionList;

    bool m_switchState = false;
    bool m_searchButtonAsbState = false;
    DFMCrumbBar * m_crumbWidget = nullptr;
    HistoryStack * m_navStack = nullptr;
    QList<HistoryStack*> m_navStacks;
};

#endif // DTOOLBAR_H
