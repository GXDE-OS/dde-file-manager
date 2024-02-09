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

#ifndef DFILEDIALOG_H
#define DFILEDIALOG_H

#include "dfilemanagerwindow.h"
#include "dfileview.h"
#include "dfmabstracteventhandler.h"

#include <QFileDialog>

DFM_USE_NAMESPACE

class DFileDialogPrivate;
class FileDialogStatusBar;
class DFileDialog : public DFileManagerWindow
{
    Q_OBJECT

public:
    enum CustomWidgetType {
        LineEditType = 0,
        ComboBoxType = 1
    };

    explicit DFileDialog(QWidget *parent = 0);
    ~DFileDialog();

    void setDirectory(const QString & directory);
    void setDirectory(const QDir & directory);
    QDir directory() const;

    void setDirectoryUrl(const DUrl & directory);
    QUrl directoryUrl() const;

    void selectFile(const QString &filename);
    QStringList selectedFiles() const;

    void selectUrl(const QUrl &url);
    QList<QUrl> selectedUrls() const;

    void addDisableUrlScheme(const QString &scheme);

    void setNameFilters(const QStringList &filters);
    QStringList nameFilters() const;
    void selectNameFilter(const QString &filter);
    QString modelCurrentNameFilter() const;
    QString selectedNameFilter() const;
    void selectNameFilterByIndex(int index);
    int selectedNameFilterIndex() const;

    QDir::Filters filter() const;
    void setFilter(QDir::Filters filters);

    void setViewMode(DFileView::ViewMode mode);
    DFileView::ViewMode viewMode() const;

    void setFileMode(QFileDialog::FileMode mode);

    void setAcceptMode(QFileDialog::AcceptMode mode);
    QFileDialog::AcceptMode acceptMode() const;

    void setLabelText(QFileDialog::DialogLabel label, const QString &text);
    QString labelText(QFileDialog::DialogLabel label) const;

    void setOptions(QFileDialog::Options options);
    void setOption(QFileDialog::Option option, bool on = true);
    bool testOption(QFileDialog::Option option) const;
    QFileDialog::Options options() const;

    void setCurrentInputName(const QString &name);
    void addCustomWidget(CustomWidgetType type, const QString &data);
    void beginAddCustomWidget();
    void endAddCustomWidget();
    QVariant getCustomWidgetValue(CustomWidgetType type, const QString &text) const;
    QVariantMap allCustomWidgetsValue(CustomWidgetType type) const;

    void setHideOnAccept(bool enable);
    bool hideOnAccept() const;

    DFileView *getFileView() const;

Q_SIGNALS:
    void finished(int result);
    void accepted();
    void rejected();
    void selectionFilesChanged();
    void currentUrlChanged();
    void selectedNameFilterChanged();

public Q_SLOTS:
    void accept();
    void done(int r);
    int exec();
    void open();
    void reject();

protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void adjustPosition(QWidget *w);

    bool fmEventFilter(const QSharedPointer<DFMEvent> &event, DFMAbstractEventHandler *target = 0, QVariant *resultData = 0) Q_DECL_OVERRIDE;

private:
    void handleNewView(DFMBaseView *view) Q_DECL_OVERRIDE;
    FileDialogStatusBar *statusBar() const;

    void onAcceptButtonClicked();
    void onRejectButtonClicked();
    void onCurrentInputNameChanged();
    void handleEnterPressed();

    QScopedPointer<DFileDialogPrivate> d_ptr;

    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), DFileDialog)
};

#endif // DFILEDIALOG_H
