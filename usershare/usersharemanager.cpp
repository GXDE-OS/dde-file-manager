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

#include "usersharemanager.h"
#include <QProcess>
#include <QStandardPaths>
#include <QFile>
#include <QApplication>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QDir>
#include <QDebug>

#include "shareinfo.h"
#include "../dde-file-manager-daemon/dbusservice/dbusinterface/usershare_interface.h"

#include "dfilewatchermanager.h"
#include "dabstractfileinfo.h"
#include "dfileservices.h"
#include "dabstractfilewatcher.h"
#include "utils/singleton.h"
#include "app/define.h"
#include "app/filesignalmanager.h"
#include "dialogs/dialogmanager.h"


QString UserShareManager::CurrentUser = "";

UserShareManager::UserShareManager(QObject *parent) : QObject(parent)
{
    m_fileMonitor = new DFileWatcherManager(this);
    m_fileMonitor->add(UserSharePath());
    m_shareInfosChangedTimer = new QTimer(this);
    m_shareInfosChangedTimer->setSingleShot(true);
    m_shareInfosChangedTimer->setInterval(300);
//    m_lazyStartSambaServiceTimer = new QTimer(this);
//    m_lazyStartSambaServiceTimer->setSingleShot(true);
//    m_lazyStartSambaServiceTimer->setInterval(3000);
    m_userShareInterface = new UserShareInterface("com.deepin.filemanager.daemon",
                                                                    "/com/deepin/filemanager/daemon/UserShareManager",
                                                                    QDBusConnection::systemBus(),
                                                                    this);
    initConnect();
    updateUserShareInfo();
    initMonitorPath();
//    m_lazyStartSambaServiceTimer->start();

    connect(this, &UserShareManager::userShareAdded, this, &UserShareManager::updateFileAttributeInfo);
    connect(this, &UserShareManager::userShareDeleted, this, &UserShareManager::updateFileAttributeInfo);
}

UserShareManager::~UserShareManager()
{

}

void UserShareManager::initMonitorPath()
{
    const ShareInfoList& infoList = shareInfoList();
    for(auto info : infoList){
        m_fileMonitor->add(info.path());
    }
}

void UserShareManager::initConnect()
{
    connect(m_fileMonitor, &DFileWatcherManager::fileDeleted, this, &UserShareManager::onFileDeleted);
    connect(m_fileMonitor, &DFileWatcherManager::subfileCreated, this, &UserShareManager::handleShareChanged);
    connect(m_fileMonitor, &DFileWatcherManager::fileMoved, this, [this](const QString &from, const QString &to) {
        onFileDeleted(from);
        handleShareChanged(to);
    });
    connect(m_shareInfosChangedTimer, &QTimer::timeout, this, [this](){emit updateUserShareInfo(true);});
//    connect(m_lazyStartSambaServiceTimer, &QTimer::timeout, this, &UserShareManager::initSamaServiceSettings);
}

QString UserShareManager::getCacehPath()
{
    return QString("%1/.cache/%2/usershare.json").arg(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).at(0),
                                                      qApp->applicationName());
}

ShareInfo UserShareManager::getOldShareInfoByNewInfo(const ShareInfo &newInfo) const
{
    QStringList shareNames = m_sharePathToNames.value(newInfo.path());
    shareNames.removeOne(newInfo.shareName());
    if (shareNames.count() > 0)
        return getsShareInfoByShareName(shareNames.last());
    return ShareInfo();
}

ShareInfo UserShareManager::getShareInfoByPath(const QString &path) const
{
    QString shareName = getShareNameByPath(path);
    if (!shareName.isEmpty()){
        if (m_shareInfos.contains(shareName)){
            return m_shareInfos.value(shareName);
        }
    }
    return ShareInfo();
}

ShareInfo UserShareManager::getsShareInfoByShareName(const QString &shareName) const
{
    return m_shareInfos.value(shareName);
}

QString UserShareManager::getShareNameByPath(const QString &path) const
{
    QString shareName;
    if (m_sharePathToNames.contains(path)){
        QStringList shareNames = m_sharePathToNames.value(path);
        if (shareNames.count() > 0){
            shareName = shareNames.last();
        }
    }
    return shareName;
}

void UserShareManager::loadUserShareInfoPathNames()
{
    QString cache = readCacheFromFile(getCacehPath());
    if (!cache.isEmpty()){
        QJsonParseError error;
        QJsonDocument doc=QJsonDocument::fromJson(cache.toLocal8Bit(),&error);
        if (error.error == QJsonParseError::NoError){
            QJsonObject obj = doc.object();
            foreach (QString key, obj.keys()) {
                m_sharePathByFilePath.insert(key, obj.value(key).toString());
            }
        }else{
            qDebug() << "load cache file: " << getCacehPath() << error.errorString();
        }
    }
}

void UserShareManager::saveUserShareInfoPathNames()
{
    QVariantMap cache;
    foreach (const QString& path, m_sharePathByFilePath.keys()) {
        cache.insert(path, m_sharePathByFilePath.value(path));
    }

    QJsonDocument doc(QJsonObject::fromVariantMap(cache));
    writeCacheToFile(getCacehPath(), doc.toJson());
}

void UserShareManager::updateFileAttributeInfo(const QString &filePath) const
{
    const DUrl &fileUrl = DUrl::fromLocalFile(filePath);
    qDebug() << fileUrl;
    const DAbstractFileInfoPointer &fileInfo = DFileService::instance()->createFileInfo(this, fileUrl);

    if (!fileInfo)
        return;
    qDebug() << fileInfo->parentUrl();
    DAbstractFileWatcher::ghostSignal(fileInfo->parentUrl(), &DAbstractFileWatcher::fileAttributeChanged, fileUrl);
}

void UserShareManager::writeCacheToFile(const QString &path, const QString &content)
{
    QFile file(path);
    if (file.open(QFile::WriteOnly)){
        file.write(content.toLocal8Bit());
    }
    file.close();
}

QString UserShareManager::readCacheFromFile(const QString &path)
{
    QFile file(path);
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << path << "isn't exists!";
        return QString();
    }
    QByteArray content = file.readAll();
    file.close();
    return QString(content);
}

QString UserShareManager::getCurrentUserName()
{
    if(CurrentUser.isEmpty()){
        QProcess up;
        up.start("id",QStringList() << "-u" << "-n");
        up.waitForFinished();
        QByteArray data = up.readAll();
        QString userName = data.data();
        // throw out '\n' string
        CurrentUser = userName.trimmed();
    }
    return CurrentUser;
}

void UserShareManager::initSamaServiceSettings()
{
//    addCurrentUserToSambashareGroup();
//    restartSambaService();
}

ShareInfoList UserShareManager::shareInfoList() const
{
    ShareInfoList shareList;
    QList<QString> keys = m_shareInfos.keys();
    foreach (QString key, keys) {
        shareList << m_shareInfos[key];
    }

    return shareList;
}

int UserShareManager::validShareInfoCount() const
{
    int counter = 0;
    for (auto info: shareInfoList()){
        if(info.isValid())
            counter ++;
    }
    return counter;
}

bool UserShareManager::hasValidShareFolders() const
{
    foreach (const ShareInfo& info, shareInfoList()) {
        if(QFile::exists(info.path()))
            return true;
    }
    return false;
}

bool UserShareManager::isShareFile(const QString &filePath) const
{
    return m_sharePathToNames.contains(filePath);
}

void UserShareManager::handleShareChanged(const QString &filePath)
{
    if (filePath.contains(":tmp"))
        return;
    m_shareInfosChangedTimer->start();
}

void UserShareManager::updateUserShareInfo(bool sendSignal)
{
    //cache
    QStringList oldShareInfos = m_shareInfos.keys();
    QMap<QString,ShareInfo> shareInfoCache = m_shareInfos;
    ShareInfoList newInfos;

    m_shareInfos.clear();
    m_sharePathToNames.clear();

    QDir d(UserSharePath());
    QFileInfoList infolist = d.entryInfoList(QDir::Files);
    foreach (const QFileInfo& f, infolist) {
        ShareInfo shareInfo;
        QMap<QString, QString> info;
        QString fpath = f.absoluteFilePath();
        QFile file(fpath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Readonly" << fpath << "failed";
            return;
        }
        QTextStream in(&file);
        while (!in.atEnd()) {
            // Read new line
            QString line = in.readLine();
              // Skip empty line or line with invalid format
            if (line.trimmed().isEmpty()) {
                continue;
            }
            if (line.contains("=")){
                int index = line.indexOf("=");
                QString key = line.mid(0, index);
                QString value = line.mid(index+1);
                info.insert(key, value);
            }
        }
        QString shareName = info.value("sharename");
        QString sharePath = info.value("path");
        QString share_acl = info.value("usershare_acl");
        if (!shareName.isEmpty() &&
                !sharePath.isEmpty() &&
                QFile(sharePath).exists() &&
                !share_acl.isEmpty()){
            shareInfo.setShareName(shareName);
            shareInfo.setPath(sharePath);
            shareInfo.setComment(info.value("comment"));
            shareInfo.setGuest_ok(info.value("guest_ok"));
            shareInfo.setUsershare_acl(info.value("usershare_acl"));
            if (share_acl.contains("r") || share_acl.contains("R")){
                shareInfo.setIsWritable(false);
            }else if (share_acl.contains("f") || share_acl.contains("F")){
                shareInfo.setIsWritable(true);
            }
            m_shareInfos.insert(shareInfo.shareName(), shareInfo);

            if (m_sharePathToNames.contains(shareInfo.path())) {
                QStringList names = m_sharePathToNames.value(shareInfo.path());
                names.append(shareInfo.shareName());
                m_sharePathToNames.insert(shareInfo.path(), names);
            } else {
                QStringList names;
                names.append(shareInfo.shareName());
                m_sharePathToNames.insert(shareInfo.path(), names);
            }
        }
    }

    foreach (ShareInfo info, m_shareInfos.values()) {
        if (info.isValid() && !oldShareInfos.contains(info.shareName())) {
            newInfos << info;
        }else if(info.isValid() && oldShareInfos.contains(info.shareName())){
            oldShareInfos.removeOne(info.shareName());
        }
    }

    // emit deleted usershare
    for (const QString &shareName : oldShareInfos){
        const QString& filePath = shareInfoCache.value(shareName).path();
        emit userShareDeleted(filePath);
        m_fileMonitor->remove(filePath);
    }

    //emit new encoming shared info
    foreach (const ShareInfo& info, newInfos) {
        emit userShareAdded(info.path());
        m_fileMonitor->add(info.path());
    }

    if (!sendSignal) {
        return;
    }

    // send signal.
    if (validShareInfoCount() <= 0) {
        emit userShareDeleted("/");
    }
    usershareCountchanged();
}

void UserShareManager::testUpdateUserShareInfo()
{
    QProcess net_usershare_info;
    net_usershare_info.start("net usershare info");
    if (net_usershare_info.waitForFinished()){
        QString content(net_usershare_info.readAll());
        writeCacheToFile(getCacehPath(), content);
        qDebug() << content;
        QSettings settings(getCacehPath(), QSettings::IniFormat);
        settings.setIniCodec("utf-8");
        qDebug() << settings.childGroups();
        foreach (QString group, settings.childGroups()) {
            settings.beginGroup(group);
            qDebug() << settings.value("path").toString();
            settings.endGroup();
        }
    }
}

void UserShareManager::setSambaPassword(const QString &userName, const QString &password)
{
    QDBusReply<bool> reply = m_userShareInterface->setUserSharePassword(userName, password);
    if(reply.isValid()){
        qDebug() << "set usershare password:" << reply.value();
    }else{
        qDebug() <<"set usershare password:" << reply.error();
    }
}

void UserShareManager::addCurrentUserToSambashareGroup()
{
    QDBusReply<bool> reply = m_userShareInterface->addUserToGroup(getCurrentUserName(), "sambashare");
    if(reply.isValid()){
        qDebug() << "add" << getCurrentUserName() <<  "to sambashare group" << reply.value();
    }else{
        qDebug() << "add" << getCurrentUserName() <<  "to sambashare group" << reply.error();
    }
}

void UserShareManager::restartSambaService()
{
    QDBusReply<bool> reply = m_userShareInterface->restartSambaService();
    if(reply.isValid()){
        qDebug() << "restartSambaService" << reply.value();
    }else{
        qDebug() <<"restartSambaService" << reply.error();
    }
}

bool UserShareManager::addUserShare(const ShareInfo &info)
{
    // check if we got `net` (in `samba-common-bin` package) installed
    QString samba_common_bin_path = QStandardPaths::findExecutable("net");
    if (samba_common_bin_path.isEmpty()) {
        dialogManager->showErrorDialog(tr("Kindly Reminder"), tr("Please firstly install samba to continue"));
        return false;
    }

    // handle old info
    ShareInfo oldInfo = getOldShareInfoByNewInfo(info);
    qDebug() << oldInfo << info;
    if(oldInfo.isValid()){
        deleteUserShareByPath(oldInfo.path());
    }
    if (!info.shareName().isEmpty() && QFile(info.path()).exists()){


        QString cmd = "net";
        QStringList args;
        ShareInfo _info = info;
        if(_info.isWritable()){
            _info.setUsershare_acl("Everyone:f");
        } else {
            _info.setUsershare_acl("Everyone:R");
        }
        args << "usershare" << "add"
             << _info.shareName() << _info.path()
             << _info.comment() << _info.usershare_acl()
             << _info.guest_ok();


        QProcess process;
        process.setProgram(cmd);
        process.setArguments(args);
        process.start();
        // Wait for process to finish without timeout.
        process.waitForFinished(-1);

        QString err = process.readAllStandardError();

        if (err.contains("is already a valid system user name")) {
            emit fileSignalManager->requestShowAddUserShareFailedDialog(_info.path());
            return false;
        }

        if (process.exitCode() != 0) {
            dialogManager->showErrorDialog(QString(), err);
            qWarning() << err;
            return false;
        }

        return true;
    }

    return false;
}


void UserShareManager::deleteUserShareByPath(const QString &path)
{
    QString shareName = getShareNameByPath(path);
    if (!shareName.isEmpty()){
        deleteUserShareByShareName(shareName);
    }
}

void UserShareManager::onFileDeleted(const QString &filePath)
{
    if(filePath.contains(UserSharePath()))
        handleShareChanged(filePath);
    else
        deleteUserShareByPath(filePath);
}

void UserShareManager::usershareCountchanged()
{
    int count = validShareInfoCount();
    emit userShareCountChanged(count);
}

void UserShareManager::deleteUserShareByShareName(const QString &shareName)
{
    QString cmd = "net";
    QStringList args;
    args << "usershare" << "delete"
         << shareName;
    QProcess p;
    p.start(cmd, args);
    p.waitForFinished();
}
