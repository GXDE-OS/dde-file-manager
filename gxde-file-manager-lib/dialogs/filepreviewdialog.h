/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#ifndef FILEPREVIEWDIALOG_H
#define FILEPREVIEWDIALOG_H

#include "dfmglobal.h"
#include "durl.h"
#include "dfmfilepreview.h"
#include "dabstractfileinfo.h"
#include <dabstractdialog.h>
#include <dseparatorhorizontal.h>
#include <QPointer>
QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
QT_END_NAMESPACE

DWIDGET_USE_NAMESPACE

DFM_BEGIN_NAMESPACE

class FilePreviewDialogStatusBar;
class DFileStatisticsJob;

class UnknowFilePreview : public DFMFilePreview
{
    Q_OBJECT
public:
    explicit UnknowFilePreview(QObject *parent = 0);
    ~UnknowFilePreview();

    bool setFileUrl(const DUrl &url) Q_DECL_OVERRIDE;
    DUrl fileUrl() const override;
    void setFileInfo(const DAbstractFileInfoPointer &info);

    QWidget *contentWidget() const Q_DECL_OVERRIDE;

signals:
    void requestStartFolderSize();

public slots:
    void updateFolderSize(qint64 size);

private:
    DUrl m_url;
    QPointer<QWidget> m_contentWidget;
    QLabel *m_iconLabel;
    QLabel *m_nameLabel;
    QLabel *m_sizeLabel;
    QLabel *m_typeLabel;
    DFileStatisticsJob* m_sizeWorker = nullptr;
};


class FilePreviewDialog : public DAbstractDialog
{
    Q_OBJECT
public:
    explicit FilePreviewDialog(const DUrlList &previewUrllist, QWidget *parent = 0);
    ~FilePreviewDialog();

    void updatePreviewList(const DUrlList &previewUrllist);

    void setEntryUrlList(const DUrlList &entryUrlList);

    void done(int r) override;

public slots:
    void playCurrentPreviewFile();

private:
    void childEvent(QChildEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

    void initUI();
    void switchToPage(int index);
    void previousPage();
    void nextPage();

    void updateTitle();

    DUrlList m_fileList;
    DUrlList m_entryUrlList;

    QPushButton *m_closeButton = 0;
    DSeparatorHorizontal *m_separator;
    FilePreviewDialogStatusBar *m_statusBar;

    int m_currentPageIndex = -1;
    DFMFilePreview *m_preview = 0;

};

DFM_END_NAMESPACE

#endif // FILEPREVIEWDIALOG_H
