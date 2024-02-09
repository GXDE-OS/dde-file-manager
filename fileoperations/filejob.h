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

#ifndef FILEJOB_H
#define FILEJOB_H

#include <QObject>
#include <QMap>
#include <QElapsedTimer>
#include <QUrl>
#include "durl.h"
#include <QStorageInfo>

#define TRANSFER_RATE 5
#define MSEC_FOR_DISPLAY 1000
#define DATA_BLOCK_SIZE 65536
#define ONE_MB_SIZE 1048576
#define ONE_KB_SIZE 1024

#undef signals
extern "C" {
    #include <gio/gio.h>
}
#define signals public

namespace DISOMasterNS {
    class DISOMaster;
}

class FileJob : public QObject
{
    Q_OBJECT
public:
    enum Status
    {
        Started,
        Paused,
        Cancelled,
        Run,
        Conflicted
    };

    enum JobType{
        Copy,
        Move,
        Trash,
        Delete,
        Restore,
        OpticalBurn,
        OpticalBlank,
        OpticalImageBurn
    };
    Q_ENUM(JobType)

    static int FileJobCount;
    static DUrlList CopyingFiles;

    static qint64 Msec_For_Display;
    static qint64 Data_Block_Size;
    static qint64 Data_Flush_Size;
    static bool setDirPermissions(const QString& scrPath, const QString& tarDirPath);

    explicit FileJob(JobType jobType, QObject *parent = 0);
    ~FileJob();
    void setStatus(Status status);
    void setJobId(const QString &id);
    QString getJobId();
    QString checkDuplicateName(const QString &name);
    void setApplyToAll(bool v);
    void setReplace(bool v);

    QString getTargetDir();

    inline QMap<QString, QString> jobDetail(){ return m_jobDetail; }
    inline qint64 currentMsec() { return m_timer.elapsed(); }
    inline qint64 lastMsec() { return m_lastMsec; }
    inline bool isJobAdded() { return m_isJobAdded; }

    void adjustSymlinkPath(QString& scrPath, QString& tarDirPath);

    bool isAborted() const;
    void setIsAborted(bool isAborted);

    bool getIsSkip() const;
    void setIsSkip(bool isSkip);

    bool getIsCoExisted() const;
    void setIsCoExisted(bool isCoExisted);

    int getWindowId();
    void setWindowId(int windowId);

    bool getIsGvfsFileOperationUsed() const;

    bool getIsFinished() const;

signals:

    /*add copy/move/delete job to taskdialog when copy/move/delete job created*/
    void requestJobAdded(const QMap<QString, QString>& jobDetail);

    /*remove copy/move/delete job to taskdialog when copy/move/delete job finished*/
    void requestJobRemoved(const QMap<QString, QString>& jobDetail);

    /*remove copy/move/delelte job to taskdialog immediately*/
    void requestJobRemovedImmediately(const QMap<QString, QString>& jobDetail);

    /*update copy/move/delete job taskdialog ui*/
    void requestJobDataUpdated(const QMap<QString, QString>& jobDetail,
                           const QMap<QString, QString>& data);

    /*abort copy/move/delete job taskdialog from ui*/
    void requestAbortTask(const QMap<QString, QString>& jobDetail);

    /*copy/move job conflict dialog show */
    void requestConflictDialogShowed(const QMap<QString, QString>& jobDetail);

    /*copy/move job tip dialog shows when src and target are same  */
    void requestCopyMoveToSelfDialogShowed(const QMap<QString, QString>& jobDetail);

    /*tip dialog show when target disk has no enough storage space for job needs*/
    void requestNoEnoughSpaceDialogShowed();

    /*tip dialog show when move file which size is large than of 1GB to trash*/
    void requestCanNotMoveToTrashDialogShowed(const DUrlList& urls);

    void requestOpticalJobFailureDialog(JobType type, const QString& err, const QStringList& details);
    void requestOpticalJobCompletionDialog(const QString& msg, const QString& icon);

    void progressPercent(int value);
    void error(QString content);
    void result(QString content);
    void finished();

public slots:
    DUrlList doCopy(const DUrlList &files, const DUrl &destination);
    DUrlList doMove(const DUrlList &files, const DUrl &destination);

    DUrlList doMoveCopyJob(const DUrlList &files, const DUrl &destination);

    void doDelete(const DUrlList &files);
    DUrlList doMoveToTrash(const DUrlList &files);

    bool doTrashRestore(const QString &srcFilePath, const QString& tarFilePath);

    void doOpticalBurn(const DUrl &device, QString volname, int speed, int flag);
    void doOpticalBlank(const DUrl &device);
    void doOpticalImageBurn(const DUrl &device, const DUrl &image, int speed, int flag);
    void opticalJobUpdated(DISOMasterNS::DISOMaster *jobisom, int status, int progress);

    void paused();
    void started();
    void cancelled();
    void handleJobFinished();

    void jobUpdated();
    void jobAdded();
    void jobRemoved();
    void jobAborted();
    void jobPrepared();
    void jobConflicted();

private:
    Status m_status;
    QString m_trashLoc;
    QString m_id;
    QMap<QString, QString> m_jobDetail;
    QMap<QString, QString> m_checkDiskJobDataDetail;
    bool m_isCheckingDisk = false;

    bool m_isGvfsFileOperationUsed = false;
    bool m_needGhostFileCreateSignal = false;

    qint64 m_bytesCopied = 0;
    qint64 m_totalSize = 1;
    qint64 m_bytesPerSec = 0;
    qint64 m_last_current_num_bytes = 0;

    QString m_progress;
    float m_factor;
    bool m_isJobAdded = false;
    QString m_srcFileName;
    QString m_tarDirName;
    QString m_srcPath;
    QString m_tarPath;
    QElapsedTimer m_timer;
    qint64 m_lastMsec;
    bool m_applyToAll  = false;
    bool m_isReplaced = false;
    bool m_isSkip = false;
    bool m_isCoExisted = true;
    bool m_isAborted = false;
    JobType m_jobType;
    int m_windowId = -1;
    bool m_skipandApplyToAll = false;
    int m_opticalJobStatus;
    int m_opticalJobProgress;
    int m_opticalJobPhase;
    QString m_opticalOpSpeed;

    int m_filedes[2] = {0, 0};
    bool m_isInSameDisk = true;
    bool m_isFinished = false;

    GCancellable* m_abortGCancellable = NULL;

    DUrlList m_noPermissonUrls;

    char* m_bufferAlign = nullptr;
    char* m_buffer = nullptr;

    bool copyFile(const QString &srcFile, const QString &tarDir, bool isMoved=false, QString *targetPath = 0);
    static void showProgress(goffset current_num_bytes, goffset total_num_bytes, gpointer user_data);
    bool copyFileByGio(const QString &srcFile, const QString &tarDir, bool isMoved=false, QString *targetPath = 0);
    bool copyDir(const QString &srcDir, const QString &tarDir, bool isMoved=false, QString *targetPath = 0);
    bool moveFile(const QString &srcFile, const QString &tarDir, QString *targetPath = 0);
    bool moveFileByGio(const QString &srcFile, const QString &tarDir, QString *targetPath = 0);
    bool moveDir(const QString &srcDir, const QString &tarDir, QString *targetPath = 0);
    bool handleMoveJob(const QString &srcPath, const QString &tarDir, QString *targetPath = 0);
    bool handleSymlinkFile(const QString &srcFile, const QString &tarDir, QString *targetPath = 0);

    bool restoreTrashFile(const QString &srcFile, const QString &tarFile);
    bool deleteFile(const QString &file);
    bool deleteFileByGio(const QString &srcFile);
    bool deleteDir(const QString &dir);
    void deleteEmptyDir(const QString &srcPath);
    bool moveDirToTrash(const QString &dir, QString *targetPath = 0);
    bool moveFileToTrash(const QString &file, QString *targetPath = 0);
    bool writeTrashInfo(const QString &fileBaseName, const QString &path, const QString &time);

    //check disk space available before do copy/move job
    bool checkDiskSpaceAvailable(const DUrlList& files, const DUrl& destination);

    //check if is moving to trash file out of size range of 1GB;
    bool checkTrashFileOutOf1GB(const DUrl& url);

    bool checkFat32FileOutof4G(const QString &srcFile, const QString &tarDir);

    QString getNotExistsTrashFileName(const QString &fileName);
    bool checkUseGvfsFileOperation(const DUrlList &files, const DUrl &destination);
    bool checkUseGvfsFileOperation(const QString& path);

    static QStorageInfo getStorageInfo(const QString &file);
    static bool canMove(const QString &filePath);
    static QString getXorrisoErrorMsg(const QStringList& msg);

#ifdef SW_LABEL
public:
    static bool isLabelFile(const QString &srcFileName);
    static int checkCopyJobPrivilege(const QString &srcFileName, const QString &dstFileName);
    static int checkMoveJobPrivilege(const QString &srcFileName, const QString &dstFileName);
    static int checkStoreInRemovableDiskPrivilege(const QString &srcFileName, const QString &dstFileName);
    static int checkDeleteJobPrivilege(const QString &srcFileName);
    static int checkRenamePrivilege(const QString &srcFileName);
    static int checkReadPrivilege(const QString &srcFileName);
#endif
};

#endif // FILEJOB_H
