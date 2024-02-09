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

#ifndef DFILEMANAGERWINDOW_H
#define DFILEMANAGERWINDOW_H

#include "durl.h"
#include "dfmglobal.h"
#include "dfmabstracteventhandler.h"
#include "shutil/filebatchprocess.h"


#include <DMainWindow>

#include <dfmbaseview.h>

#include <atomic>
#include <tuple>
#include <array>

#define DEFAULT_WINDOWS_WIDTH 960
#define DEFAULT_WINDOWS_HEIGHT 540
#define TITLE_FIXED_HEIGHT 40

class DTitleBar;
class DToolBar;
class DDetailView;
class QStatusBar;
class QFrame;
class QHBoxLayout;
class QVBoxLayout;
class QResizeEvent;

class ExtendView;
class QStackedLayout;
class QPushButton;

class DStatusBar;
class DFMEvent;
class DFMUrlBaseEvent;
class TabBar;
class Tab;
class RecordRenameBarState;


class DFMUrlListBaseEvent;

DFM_BEGIN_NAMESPACE
class DFMSideBar;
class DFMAddressBar;
DFM_END_NAMESPACE

DWIDGET_USE_NAMESPACE
DFM_USE_NAMESPACE

class DFileManagerWindowPrivate;
class DFileManagerWindow : public DMainWindow, public DFMAbstractEventHandler
{
    Q_OBJECT
public:
    explicit DFileManagerWindow(QWidget *parent = 0);
    explicit DFileManagerWindow(const DUrl &fileUrl, QWidget *parent = 0);
    virtual ~DFileManagerWindow();

    DUrl currentUrl() const;
    DFMBaseView::ViewState currentViewState() const;
    bool isCurrentUrlSupportSearch(const DUrl &currentUrl);

    DToolBar *getToolBar() const;
    DFMBaseView *getFileView() const;
    DFMSideBar *getLeftSideBar() const;
    int getSplitterPosition() const;

    void setSplitterPosition(int pos);

    quint64 windowId();

    bool tabAddable() const;
    void hideRenameBar() noexcept;
    void requestToSelectUrls();
    bool isAdvanceSearchBarVisible();
    void toggleAdvanceSearchBar(bool visible = true, bool resetForm = true);

signals:
    void aboutToClose();
    void positionChanged(const QPoint &pos);
    void currentUrlChanged();
    void currentViewStateChanged();

public slots:
    void moveCenter(const QPoint &cp);
    void moveTopRight();
    void moveCenterByRect(QRect rect);
    void moveTopRightByRect(QRect rect);

    bool cd(const DUrl &fileUrl);
    bool cdForTab(int tabIndex, const DUrl &fileUrl);
    bool cdForTabByView(DFMBaseView *view , const DUrl &fileUrl);

    bool openNewTab(DUrl fileUrl);
    void switchToView(DFMBaseView *view);
    void onTabAddableChanged(bool addable);
    void onCurrentTabChanged(int tabIndex);
    void onRequestCloseTab(const int index, const bool &remainState);
    void closeCurrentTab(quint64 winId);
    void showNewTabButton();
    void hideNewTabButton();
    void showEmptyTrashButton();
    void hideEmptyTrashButton();
    void onNewTabButtonClicked();
    void requestEmptyTrashFiles();
    void onTrashStateChanged();

    void onShowRenameBar(const DFMUrlListBaseEvent &event)noexcept;
    void onTabBarCurrentIndexChange(const int &index)noexcept;
    void onReuqestCacheRenameBarState() const;

    void setTheme(const QString &theme);
    void onThemeChanged();

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void moveEvent(QMoveEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    bool fmEvent(const QSharedPointer<DFMEvent> &event, QVariant *resultData = 0) Q_DECL_OVERRIDE;
    QObject *object() const Q_DECL_OVERRIDE;

    virtual void handleNewView(DFMBaseView *view);

    void initData();
    void initUI();

    void initTitleFrame();
    void initTitleBar();
    void initSplitter();

    void initLeftSideBar();

    void initRightView();

    void initRenameBarState();

    void initToolBar();
    void initTabBar();
    void initViewLayout();

    void initCentralWidget();
    void initConnect();

private:
    Tab *m_currentTab{ nullptr };
    std::atomic<bool> m_tabBarIndexChangeFlag{ false };//###: when the index of tabbar changed hide RenameBar through the value.

    QScopedPointer<DFileManagerWindowPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), DFileManagerWindow)

public:
    static std::unique_ptr<RecordRenameBarState>  renameBarState;//###: record pattern of RenameBar and the string of QLineEdit's content.
    static std::atomic<bool> flagForNewWindowFromTab;           //###: open a new window form a already has tab, this will be true.
    //and after opening new window this will be back to false.
};

#endif // DFILEMANAGERWINDOW_H
