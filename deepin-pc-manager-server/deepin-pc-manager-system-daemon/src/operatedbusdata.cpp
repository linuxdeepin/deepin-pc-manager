// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "operatedbusdata.h"
#include "common.h"

#include <DTrashManager>

#include <QFileInfo>

// 合法调用进程路径列表
const QStringList ValidInvokerExePathList = {"/usr/bin/deepin-pc-manager",
                                             "/usr/bin/deepin-pc-manager-system-daemon"};

#define HOST_FILE_PATH "/etc/hosts"
OperateDbusData::OperateDbusData(QObject *parent)
    : QObject(parent)
    , m_daemonAdaptor(nullptr)
    , m_trashCleanModel(nullptr)
{
    m_daemonAdaptor = new DaemonAdaptor(this);
    if (!QDBusConnection::systemBus().registerService(service)
        || !QDBusConnection::systemBus().registerObject(path, this)) {
        exit(0);
    }
    m_trashCleanModel = new TrashCleanModel(this);
}

OperateDbusData::~OperateDbusData()
{
}

void OperateDbusData::StartApp()
{
}

void OperateDbusData::ExitApp()
{
    if (!isValidInvoker()) {
        qDebug() << Q_FUNC_INFO << "invalid invoker!";
        return;
    }
    qApp->quit();
}

double OperateDbusData::GetTrashFileCount(QString homepath)
{
    if (!isValidInvoker()) {
        qDebug() << Q_FUNC_INFO << "invalid invoker!";
        return 0.00;
    }
    QFileInfo info(homepath);
    if (!info.isDir()) {
        return 0.00;
    }

    return m_trashCleanModel->getTrashFileCount(homepath);
}
double OperateDbusData::GetCacheFileCount()
{
    if (!isValidInvoker()) {
        qDebug() << Q_FUNC_INFO << "invalid invoker!";
        return 0.00;
    }
    return m_trashCleanModel->getCacheFileCount();
}
double OperateDbusData::GetLogFileCount()
{
    if (!isValidInvoker()) {
        qDebug() << Q_FUNC_INFO << "invalid invoker!";
        return 0.00;
    }
    return m_trashCleanModel->getLogFileCount();
}
double OperateDbusData::GetHistoryFileCount(QString homepath)
{
    if (!isValidInvoker()) {
        qDebug() << Q_FUNC_INFO << "invalid invoker!";
        return 0.00;
    }
    QFileInfo info(homepath);
    if (!info.isDir()) {
        return 0.00;
    }
    return m_trashCleanModel->getHistoryFileCount(homepath);
}

void OperateDbusData::CleanTrashFiles(QStringList items)
{
    if (!isValidInvoker()) {
        qDebug() << Q_FUNC_INFO << "invalid invoker!";
        return;
    }
    for (int i = 0; i < items.count(); ++i) {
        QString itemType = items.at(i);
        if (TRASH_CLEAN_ITEM_TYPE_CACHE == itemType) {
            m_trashCleanModel->cleanCacheFiles();
        } else if (TRASH_CLEAN_ITEM_TYPE_LOG == itemType) {
            m_trashCleanModel->cleanLogFiles();
        } else if (TRASH_CLEAN_ITEM_TYPE_HISTORY == itemType) {
            m_trashCleanModel->cleanHistoryFiles();
        }
    }
}

void OperateDbusData::ReplaceHostFile(QString host)
{
    if (!isValidInvoker()) {
        qDebug() << Q_FUNC_INFO << "invalid invoker!";
        return;
    }
    QFile file(HOST_FILE_PATH);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        file.seek(0);
        stream << host;
        file.close();
    }
}

// 校验调用者
bool OperateDbusData::isValidInvoker()
{
#ifdef QT_DEBUG
    return true;
#else
    // 判断是否位自身调用
    if (!calledFromDBus()) {
        return true;
    }

    bool valid = false;
    QDBusConnection conn = connection();
    QDBusMessage msg = message();

    // 判断是否存在执行路径且是否存在于可调用者名单中
    uint pid = conn.interface()->servicePid(msg.service()).value();
    QFileInfo f(QString("/proc/%1/exe").arg(pid));
    QString invokerPath = f.canonicalFilePath();

    // 防止读取的执行路径为空，为空时取cmdline内容
    if (invokerPath.isEmpty()) {
        QFile cmdlineFile(QString("/proc/%1/cmdline").arg(pid));
        if (cmdlineFile.open(QIODevice::OpenModeFlag::ReadOnly)) {
            QString sCmdline = cmdlineFile.readAll();
            invokerPath = sCmdline.split(" ").first();
        }
        cmdlineFile.close();
    }

    if (ValidInvokerExePathList.contains(invokerPath)) {
        valid = true;
    }

    // 如果是非法调用者，则输出错误
    if (!valid) {
        sendErrorReply(QDBusError::ErrorType::Failed,
                       QString("(pid: %1)[%2] is not allowed to transfer antiav inter")
                           .arg(pid)
                           .arg(invokerPath));
    }

    return valid;
#endif
}
