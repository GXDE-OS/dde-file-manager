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

#include "commandlinemanager.h"
#include "dfmeventdispatcher.h"
#include "filemanagerapp.h"
#include "dabstractfileinfo.h"
#include "dfileservices.h"
#include "dfmapplication.h"

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QCoreApplication>
#include <QDebug>

DFM_USE_NAMESPACE

CommandLineManager::CommandLineManager()
    : m_commandParser(new QCommandLineParser)
{
    m_commandParser->setApplicationDescription(QString("%1 helper").arg(QCoreApplication::applicationName()));
    initOptions();
    m_commandParser->addHelpOption();
    m_commandParser->addVersionOption();
}

void CommandLineManager::process()
{
    return process(qApp->arguments());
}

void CommandLineManager::process(const QStringList &arguments)
{
    m_commandParser->process(arguments);
}

void CommandLineManager::initOptions()
{
    QCommandLineOption newWindowOption(QStringList() << "n" << "new-window", "show new window");
    QCommandLineOption backendOption(QStringList() << "d" << "none-window-process", "start dde-file-manager in no window mode");
    QCommandLineOption openPropertyDialogOption(QStringList() << "p" << "property", "show property dialog");
    QCommandLineOption rootOption(QStringList() << "r" << "root", "exec dde-file-manager in root mode");
    QCommandLineOption showFileItem(QStringList() << "show-item", "Show a file item in a new window");
    QCommandLineOption event(QStringList() << "e" << "event", "Process the event by json data");

    QCommandLineOption get_monitor_files(QStringList() << "get-monitor-files", "Get all the files that have been monitored");
    // blumia: about -w and -r: -r will exec `dde-file-manager-pkexec` (it use `pkexec` command) which won't pass the currect
    //         working dir, so we need to manually set the working dir via -w. that's why we add a -w arg.
    QCommandLineOption workingDirOption(QStringList() << "w" << "working-dir",
                                        "Set the file manager working directory (won't work with -r argument)",
                                        "directory");

    addOption(newWindowOption);
    addOption(backendOption);
    addOption(openPropertyDialogOption);
    addOption(rootOption);
    addOption(showFileItem);
    addOption(event);
    addOption(get_monitor_files);
    addOption(workingDirOption);
}

void CommandLineManager::addOption(const QCommandLineOption &option)
{
    m_commandParser->addOption(option);
}

void CommandLineManager::addOptions(const QList<QCommandLineOption> &options)
{
    foreach (QCommandLineOption option, options) {
        addOption(option);
    }
}

QStringList CommandLineManager::positionalArguments() const
{
    return m_commandParser->positionalArguments();
}

QStringList CommandLineManager::unknownOptionNames() const
{
    return m_commandParser->unknownOptionNames();
}

void CommandLineManager::processCommand()
{
    if (isSet("e")) {
        // init
        Q_UNUSED(FileManagerApp::instance());
        const QSharedPointer<DFMEvent> &event = DFMEvent::fromJson(QJsonDocument::fromJson(positionalArguments().first().toLocal8Bit().constData()).object());

        if (event)
            DFMEventDispatcher::instance()->processEvent(event);

        return;
    }

    if (isSet("p")) {
        QStringList paths = positionalArguments();
        FileManagerApp::instance()->showPropertyDialog(paths);

        return;
    }

    DUrlList argumentUrls;

    foreach (QString path, positionalArguments()) {
        DUrl url = DUrl::fromUserInput(path);
        if (CommandLineManager::instance()->isSet("show-item")) {
            const DAbstractFileInfoPointer &fileInfo = DFileService::instance()->createFileInfo(Q_NULLPTR, url);
            if (!fileInfo)
                continue;

            DUrl newUrl = fileInfo->parentUrl();

            newUrl.setQuery("selectUrl=" + url.toString());
            url = newUrl;
        }

        argumentUrls.append(url);
    }
    if (argumentUrls.isEmpty())
        argumentUrls.append(DFMApplication::instance()->appUrlAttribute(DFMApplication::AA_UrlOfNewWindow));

    DFMEventDispatcher::instance()->processEvent<DFMOpenNewWindowEvent>(Q_NULLPTR, argumentUrls, isSet("n"));
}

CommandLineManager *CommandLineManager::instance()
{
    static CommandLineManager instance;

    return &instance;
}

bool CommandLineManager::isSet(const QString &name) const
{
    return m_commandParser->isSet(name);
}

QString CommandLineManager::value(const QString &name) const
{
    return m_commandParser->value(name);
}

CommandLineManager::~CommandLineManager()
{

}
