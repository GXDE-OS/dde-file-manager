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

#include "mainwindow.h"
#include <QPainter>
#include <QImage>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QImage>
#include <QProgressBar>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextBrowser>
#include <QFile>
#include <QDebug>
#include <QMouseEvent>
#include <QApplication>
#include <QtConcurrent>
#include <DWindowManagerHelper>
#include "../partman/partition.h"
#include "dialogs/messagedialog.h"

using namespace PartMan;
MainWindow::MainWindow(const QString &path, QWidget *parent):
    QDialog(parent)
{
    DPlatformWindowHandle handle(this);
    Q_UNUSED(handle)

    setObjectName("UsbDeviceFormatter");
    m_formatPath = path;
    m_formatType = Partition::getPartitionByDevicePath(path).fs();
    if(m_formatType == "vfat")
        m_formatType = "fat32";
    initUI();
    initStyleSheet();
    initConnect();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initUI()
{
    setFixedSize(WINDOW_SIZE);
    setWindowFlags(Qt::WindowCloseButtonHint |
                   Qt::FramelessWindowHint |
                   Qt::Dialog);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 2, 0, 0);

    QPixmap pixmap(24, 24);
    pixmap.fill(QColor(Qt::transparent));
    QIcon transparentIcon(pixmap);

    QString title = tr("Format");
    m_titleBar = new DTitlebar(this);
    m_titleBar->setIcon(transparentIcon);
    m_titleBar->setTitle(title);
    m_titleBar->setFixedHeight(20);
    m_titleBar->layout()->setContentsMargins(0, 0, 0, 0);
    m_titleBar->setBackgroundTransparent(true);

    m_pageStack = new QStackedWidget(this);
    m_pageStack->setFixedSize(width(), 340);
    m_mainPage = new MainPage(m_formatType, this);
    m_mainPage->setTargetPath(m_formatPath);
    m_warnPage = new WarnPage(this);
    m_formatingPage = new FormatingPage(this);
    m_finishPage = new FinishPage(this);
    m_errorPage = new ErrorPage("", this);

    m_pageStack->addWidget(m_mainPage);
    m_pageStack->addWidget(m_warnPage);
    m_pageStack->addWidget(m_formatingPage);
    m_pageStack->addWidget(m_finishPage);
    m_pageStack->addWidget(m_errorPage);

    m_comfirmButton = new QPushButton(tr("Format"), this);
    m_comfirmButton->setFixedSize(160, 36);
    m_comfirmButton->setObjectName("ComfirmButton");

    mainLayout->addWidget(m_titleBar);
    mainLayout->addWidget(m_pageStack);
    mainLayout->addSpacing(10);
    mainLayout->addStretch(1);
    mainLayout->addWidget(m_comfirmButton, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(34);
    setFixedHeight(m_titleBar->height() + m_pageStack->height() + 10 + m_comfirmButton->height() + 34);
    setLayout(mainLayout);
}

void MainWindow::initStyleSheet()
{
    QFile file(":/light/main-window.qss");
    bool ret = file.open(QIODevice::ReadOnly);
    if(!ret)
        qDebug () << "Cannot load style sheet!";
    QTextStream ts(&file);
    QString styleStr = ts.readAll();
    file.close();
    this->setStyleSheet(styleStr);

}

void MainWindow::initConnect()
{
    connect(m_comfirmButton, &QPushButton::clicked, this, &MainWindow::nextStep);
    connect(m_formatingPage, &FormatingPage::finished, this, &MainWindow::onFormatingFinished);
    connect(this, &MainWindow::taskFinished, this, &MainWindow::preHandleTaskFinished);
}

void MainWindow::formartDevice()
{
    DWindowManagerHelper::setMotifFunctions(windowHandle(), DWindowManagerHelper::FUNC_CLOSE, false);
    m_titleBar->setDisableFlags(Qt::WindowCloseButtonHint);

    QtConcurrent::run([=]{
        bool result = false;
        QString format = m_mainPage->selectedFormat();
        QString first = format.left(1);
        format.remove(0, 1);
        format.insert(0, first.toUpper());

        //First of all to unmount device
        unMountDevice();

        //Format deivice
        PartMan::PartitionManager partitonManager;
        result = partitonManager.mkfs(m_formatPath,
                             m_mainPage->getSelectedFs(),
                             m_mainPage->getLabel());

        //Delay on checking out disk status
        QTimer::singleShot(300,this, [=]{
            if(result){
                bool f = checkBackup();
                emit taskFinished(f);
                return;
            }

            emit taskFinished(result);
        });
    });
}

void MainWindow::unMountDevice()
{
    QStringList args;
    args << m_formatPath;
    QString cmd = "umount";
    QProcess::execute(cmd, args);
}

bool MainWindow::checkBackup()
{
    PartMan::Partition p = PartMan::Partition::getPartitionByDevicePath(m_formatPath);
    if(p.fs().isEmpty())
        return false;
    return true;
}

void MainWindow::nextStep()
{
    switch (m_currentStep) {
    case Normal:
        m_pageStack->setCurrentWidget(m_warnPage);
        m_currentStep = Warn;
        m_comfirmButton->setText(tr("Continue"));
        break;
    case Warn:
        m_pageStack->setCurrentWidget(m_formatingPage);
        m_currentStep = Formating;
        m_comfirmButton->setText(tr("Formatting..."));
        m_comfirmButton->setEnabled(false);
        m_formatingPage->startAnimate();
        formartDevice();
        break;
    case Finished:
        qApp->quit();
        break;
    case FormattError:
        m_pageStack->setCurrentWidget(m_mainPage);
        m_currentStep = Normal;
        break;
    case RemovedWhenFormattingError:
        qApp->quit();
        break;
    default:
        break;
    }
}

void MainWindow::onFormatingFinished(const bool &successful)
{
    DWindowManagerHelper::setMotifFunctions(windowHandle(), DWindowManagerHelper::FUNC_CLOSE, true);
    m_titleBar->setDisableFlags({});

    if (successful) {
        m_currentStep = Finished;
        m_comfirmButton->setText(tr("Done"));
        m_comfirmButton->setEnabled(true);
        m_pageStack->setCurrentWidget(m_finishPage);
    } else {
        if(!QFile::exists(m_formatPath)){
            m_currentStep = RemovedWhenFormattingError;
            m_comfirmButton->setText(tr("Quit"));
            m_errorPage->setErrorMsg(tr("Your disk is removed when formatting"));
        } else{
            m_currentStep = FormattError;
            m_errorPage->setErrorMsg(tr("Failed to format"));
            m_comfirmButton->setText(tr("Reformat"));
        }
        m_comfirmButton->setEnabled(true);
        m_pageStack->setCurrentWidget(m_errorPage);
    }
}

void MainWindow::preHandleTaskFinished(const bool &result)
{
    m_formatingPage->animateToFinish(result);
}
