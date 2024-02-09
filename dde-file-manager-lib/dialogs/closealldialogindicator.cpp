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

#include "closealldialogindicator.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QKeyEvent>
#include "shutil/fileutils.h"

CloseAllDialogIndicator::CloseAllDialogIndicator(QWidget *parent) : DAbstractDialog(parent)
{
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setFocusPolicy(Qt::NoFocus);
    initUI();
    initConnect();
}

CloseAllDialogIndicator::~CloseAllDialogIndicator()
{

}

void CloseAllDialogIndicator::initUI()
{
    resize(QSize(400, 50));

    m_messageLabel = new QLabel(this);
    m_closeButton = new QPushButton(tr("Close all"), this);
    m_closeButton->setObjectName("AllCloseButton");

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_messageLabel, Qt::AlignCenter);
    mainLayout->addSpacing(50);
    mainLayout->addWidget(m_closeButton, Qt::AlignRight);
    mainLayout->setContentsMargins(25, 5, 25, 5);
    setLayout(mainLayout);

    setTotalMessage(0, 0);
}

void CloseAllDialogIndicator::initConnect()
{
    connect(m_closeButton, &QPushButton::clicked, this, &CloseAllDialogIndicator::allClosed);
}

void CloseAllDialogIndicator::setTotalMessage(qint64 size, int count)
{
    QString message = tr("Total size is %1, contains %2 files").arg(FileUtils::formatSize(size), QString::number(count));
    m_messageLabel->setText(message);
}

void CloseAllDialogIndicator::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape){
        return;
    }
    QDialog::keyPressEvent(event);
}

void CloseAllDialogIndicator::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    QRect screenGeometry = qApp->desktop()->screenGeometry();

    move((screenGeometry.width() - width()) / 2, screenGeometry.height() - height());

    return DAbstractDialog::showEvent(event);
}

