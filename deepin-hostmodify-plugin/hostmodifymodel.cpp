// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "hostmodifymodel.h"
#include "invokers/invokerfactory.h"

#include <QProcess>

HostModifyModel::HostModifyModel(QObject *parent)
    : QObject(parent)
    , m_sysetemDaemonInvokerInter(nullptr)
{
    initData();
}

HostModifyModel::~HostModifyModel()
{
}

// 初始化数据
void HostModifyModel::initData()
{
    m_sysetemDaemonInvokerInter = InvokerFactory::GetInstance().CreateInvoker("com.deepin.pc.manager.system.daemon",
                                                                              "/com/deepin/pc/manager/system/daemon",
                                                                              "com.deepin.pc.manager.system.daemon",
                                                                              ConnectType::SYSTEM,
                                                                              this);
}

// 获取hosts文件信息
QString HostModifyModel::getHostsFileInfo()
{
    QProcess proc;
    proc.start("cat /etc/hosts");
    proc.waitForStarted(-1);
    proc.waitForFinished(-1);
    m_hostInfo = proc.readAllStandardOutput();
    return m_hostInfo;
}

void HostModifyModel::saveHostFile(QString host)
{
    m_hostInfo = host;
    DBUS_NOBLOCK_INVOKE(m_sysetemDaemonInvokerInter, "ReplaceHostFile", host);
}
