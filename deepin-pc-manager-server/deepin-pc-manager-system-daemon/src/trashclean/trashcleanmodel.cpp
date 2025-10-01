// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "trashcleanmodel.h"
#include "invokers/invokerfactory.h"

#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QEventLoop>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtConcurrent>

#define CLEANER_SYSTEM_CACHE_ARRAY_NAME "files"
#define CLEANER_SYSTEM_CACHE_APT_PATH "/var/cache/apt/archives" // 系统缓存
#define CLEANER_SYSTEM_CACHE_DEB_NAME "name"
#define CLEANER_SYSTEM_CACHE_LASTORE_PATH "/var/cache/lastore/archives" // 系统缓存
#define CLEANER_SYSTEM_LOG_PATH "/var/log" // 系统日志
#define CLEANER_SYSTEM_HISTORY_PATH "/.bash_history"

#define MB_COUNT (1 << 20)
#define MIN_SYSTEM_LOG_SIZE_TO_CLEAN 100 * MB_COUNT

const QString WhitePathsFile = "/usr/share/deepin-pc-manager/cleanerwhite.json";

TrashCleanModel::TrashCleanModel(QObject *parent)
    : QObject(parent)
    , m_managerInter(nullptr)
{
    initData();
}

TrashCleanModel::~TrashCleanModel()
{
}

void TrashCleanModel::initData()
{
    m_managerInter = InvokerFactory::GetInstance().CreateInvoker("org.deepin.lastore1",
                                                                 "/org/deepin/lastore1",
                                                                 "org.deepin.lastore1.Manager",
                                                                 ConnectType::SYSTEM,
                                                                 this);
}

double TrashCleanModel::getTrashFileCount(const QString &homepath)
{
    m_fileList.clear();
    m_trashFileList.clear();
    m_trashFileCount = 0.00;

    // 获取回收站目录下所有文件
    QEventLoop loop;
    QtConcurrent::run([&] {
        QThread::msleep(50);
        scanFile(homepath + CLEANER_SYSTEM_TRASH_PATH);
        loop.exit();
    });
    loop.exec();

    foreach (const QString &file, m_fileList) {
        QFileInfo info(file);
        m_trashFileCount += info.size();
        m_trashFileList.append(file);
    }

    return m_trashFileCount;
}

double TrashCleanModel::getCacheFileCount()
{
    m_cacheFileList.clear();
    m_cacheFileCount = 0.00;

    QStringList debFileList;
    QString debJsonStr;
    QEventLoop loop;
    QtConcurrent::run([&] {
        QThread::msleep(50);
        QDBusMessage msg = DBUS_BLOCK_INVOKE(m_managerInter, "GetArchivesInfo");
        debJsonStr = QDBusReply<QString>(msg).value();
        loop.exit();
    });
    loop.exec();

    QJsonParseError err;
    QJsonObject object = QJsonDocument::fromJson(debJsonStr.toUtf8(), &err).object();
    if (QJsonParseError::NoError != err.error) {
        qDebug() << "incorrect json format:" << debJsonStr;
    }

    // 获取/var/cache/apt/archives 可清理deb缓存
    QJsonArray debObjArrayApt = object.value(CLEANER_SYSTEM_CACHE_ARRAY_NAME)
                                    .toObject()
                                    .value(CLEANER_SYSTEM_CACHE_APT_PATH)
                                    .toArray();
    foreach (const auto &it, debObjArrayApt) {
        QString debName = it.toObject().value(CLEANER_SYSTEM_CACHE_DEB_NAME).toString();

        if (debName.isEmpty()) {
            continue;
        }

        // 设置文件完整路径
        QString debFilePath = QString("%1%2%3")
                                  .arg(CLEANER_SYSTEM_CACHE_APT_PATH)
                                  .arg(QDir::separator())
                                  .arg(debName);
        QFileInfo info(debFilePath);
        if (info.exists()) {
            debFileList.append(debFilePath);
            m_cacheFileList.append(debFilePath);
            m_cacheFileCount += info.size();
        }
    }

    // 获取/var/cache/lastore/archives 可清理deb缓存
    QJsonArray debObjArray = object.value(CLEANER_SYSTEM_CACHE_ARRAY_NAME)
                                 .toObject()
                                 .value(CLEANER_SYSTEM_CACHE_LASTORE_PATH)
                                 .toArray();
    foreach (const auto &it, debObjArray) {
        QString debName = it.toObject().value(CLEANER_SYSTEM_CACHE_DEB_NAME).toString();

        if (debName.isEmpty()) {
            continue;
        }

        // 设置文件完整路径
        QString debFilePath = QString("%1%2%3")
                                  .arg(CLEANER_SYSTEM_CACHE_LASTORE_PATH)
                                  .arg(QDir::separator())
                                  .arg(debName);
        QFileInfo info(debFilePath);
        if (info.exists()) {
            debFileList.append(debFilePath);
            m_cacheFileList.append(debFilePath);
            m_cacheFileCount += info.size();
        }
    }
    return m_cacheFileCount;
}

double TrashCleanModel::getLogFileCount()
{
    m_fileList.clear();
    m_logFileList.clear();
    m_logFileCount = 0.00;

    QEventLoop loop;
    QtConcurrent::run([&] {
        QThread::msleep(50);
        scanFile(CLEANER_SYSTEM_LOG_PATH);
        loop.exit();
    });
    loop.exec();

    // 系统日志过滤掉白名单
    double sizeCounter = 0.0;
    QStringList sPathList;
    QFile whiteFile(WhitePathsFile);
    if (!whiteFile.open(QFile::ReadOnly)) {
        qDebug() << "can not find white file:" << WhitePathsFile;
    }

    // 获取白名单数据
    QJsonParseError err;
    const QJsonObject &object = QJsonDocument::fromJson(whiteFile.readAll(), &err).object();
    whiteFile.close();

    if (QJsonParseError::NoError != err.error) {
        return 0;
    }
    const QJsonArray &apps = object.value("whitepaths").toArray();

    // 判断是否该目录下文件是否加入到白名单，如果加入，直接跳过
    foreach (const QString &file, m_fileList) {
        if (file.isEmpty()) {
            break;
        }

        bool bAddList = false;
        // 是否是/var/log/journal目录下的文件
        if (file.contains("/var/log/journal")) {
            QFileInfo info(file);
            sizeCounter += info.size();
            sPathList.append(file);
        } else {
            // 黑名单 - 只清除黑名单内带后缀的文件
            for (auto itApp = apps.begin(); itApp != apps.end(); ++itApp) {
                QString sPath = itApp->toObject().value("white_path").toString();
                if (file.contains(sPath) && file != sPath) {
                    bAddList = true;
                }
            }
        }

        // 添加符合规则的文件到清理项
        if (bAddList) {
            QFileInfo info(file);
            sizeCounter += info.size();
            sPathList.append(file);
        }
    }

    // 仅超过100M时对系统日志提供清理
    if (sizeCounter > MIN_SYSTEM_LOG_SIZE_TO_CLEAN) {
        m_logFileList.append(sPathList);
        m_logFileCount += sizeCounter;
    } else {
        sPathList.clear();
    }

    return m_logFileCount;
}

double TrashCleanModel::getHistoryFileCount(const QString &homepath)
{
    m_historyFileList.clear();
    m_historyFileCount = 0.00;
    // 统计清理文件大小和路径
    QFileInfo info(homepath + CLEANER_SYSTEM_HISTORY_PATH);
    m_historyFileCount += info.size();
    m_historyFileList.append(info.absoluteFilePath());

    return m_historyFileCount;
}

// 获取单个文件的大小，同时记录文件总数
void TrashCleanModel::scanFile(const QString &path)
{
    QFileInfo info(path);
    if (info.exists()) {
        if (info.isFile()) {
            // 用户显示在主界面的标题内容，由于文件路径是同检查项层次决定的，所以不需要该文件的路径信息
            QString abpath = info.absoluteFilePath();
            m_fileList.append(abpath);
        } else if (info.isDir()) {
            QDir dir(path);
            for (const QFileInfo &i :
                 dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs | QDir::Hidden
                                   | QDir::NoSymLinks)) {
                // 递归扫描目录内容
                scanFile(i.absoluteFilePath());
            }
        }
    }
}

void TrashCleanModel::cleanCacheFiles()
{
    foreach (const auto &file, m_cacheFileList) {
        QFileInfo fileInfo(file);
        if (fileInfo.isFile() || fileInfo.isSymLink()) {
            if (!QFile::remove(fileInfo.absoluteFilePath())) {
                qWarning() << "remove failed : " << file;
            }
        }
    }
}

void TrashCleanModel::cleanLogFiles()
{
    bool isCleanJournal = false;
    foreach (const auto &file, m_logFileList) {
        if (file.contains("/var/log/journal")) {
            isCleanJournal = true;
            continue;
        }

        QFileInfo fileInfo(file);
        if (fileInfo.isFile() || fileInfo.isSymLink()) {
            if (!QFile::remove(fileInfo.absoluteFilePath())) {
                qWarning() << "remove failed : " << file;
            }
        }
    }

    if (isCleanJournal) {
        QProcess proc;
        proc.start(QString("journalctl --rotate --vacuum-size=1M"));
        proc.waitForFinished();
        proc.close();
        proc.deleteLater();
    }
}

void TrashCleanModel::cleanHistoryFiles()
{
    foreach (const auto &file, m_historyFileList) {
        QFileInfo fileInfo(file);
        if (fileInfo.isFile() || fileInfo.isSymLink()) {
            if (!QFile::remove(fileInfo.absoluteFilePath())) {
                qWarning() << "remove failed : " << file;
            }
        }
    }
}
