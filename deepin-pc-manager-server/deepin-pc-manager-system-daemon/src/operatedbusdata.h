// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "daemon_adaptor.h"
#include "trashclean/trashcleanmodel.h"

#include <QObject>
#include <QDBusContext>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QDBusAbstractAdaptor>
#include <QDBusConnectionInterface>

// 检测ssh登陆端口启动标志
#define REM_SSH_STATUS "running"
const QString service = "com.deepin.pc.manager.system.daemon";
const QString path = "/com/deepin/pc/manager/system/daemon";

class QDBusContext;
class OperateDbusData : public QObject
    , protected QDBusContext
{
    Q_OBJECT
public:
    explicit OperateDbusData(QObject *parent = nullptr);
    ~OperateDbusData();

public Q_SLOTS:
    void StartApp();
    void ExitApp();

    double GetTrashFileCount(QString homepath);
    double GetCacheFileCount();
    double GetLogFileCount();
    double GetHistoryFileCount(QString homepath);
    void CleanTrashFiles(QStringList items);

    void ReplaceHostFile(QString host);

private:
    // 校验调用者
    bool isValidInvoker();

private:
    DaemonAdaptor *m_daemonAdaptor;
    TrashCleanModel *m_trashCleanModel;
};
