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

#include "pdfpreview.h"

#include <QMimeType>
#include <QMimeDatabase>
#include <QUrl>
#include <QFileInfo>

#include "pdfwidget.h"

DFM_BEGIN_NAMESPACE

PDFPreview::PDFPreview(QObject *parent)
    : DFMFilePreview(parent)
{

}

PDFPreview::~PDFPreview()
{
    if (m_pdfWidget)
        m_pdfWidget->deleteLater();
}

bool PDFPreview::setFileUrl(const DUrl &url)
{
    if (m_url == url)
        return true;

    if (!url.isLocalFile())
        return false;

    if (m_pdfWidget)
        return false;

    m_pdfWidget = new PdfWidget(url.toLocalFile());
    m_pdfWidget->setFixedSize(800, 500);

    m_title = QFileInfo(url.toLocalFile()).fileName();

    Q_EMIT titleChanged();

    return true;
}

DUrl PDFPreview::fileUrl() const
{
    return m_url;
}

QWidget *PDFPreview::contentWidget() const
{
    return m_pdfWidget;
}

QString PDFPreview::title() const
{
    return m_title;
}

bool PDFPreview::showStatusBarSeparator() const
{
    return true;
}

DFM_END_NAMESPACE
