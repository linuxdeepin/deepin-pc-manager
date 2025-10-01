// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "autostartcheckmodel.h"
#include "window/common/invokers/invokerfactory.h"

AutoStartCheckModel::AutoStartCheckModel(QObject *parent)
    : QObject(parent)
    , m_launcherInter(nullptr)
    , m_startManagerInterface(nullptr)
{
    initData();
}

AutoStartCheckModel::~AutoStartCheckModel()
{
}

void AutoStartCheckModel::initData()
{
    registerLauncherItemInfoMetaType();

    // 取应用数据接口
    m_launcherInter = InvokerFactory::GetInstance().CreateInvoker("com.deepin.dde.daemon.Launcher",
                                                                  "/com/deepin/dde/daemon/Launcher",
                                                                  "com.deepin.dde.daemon.Launcher",
                                                                  ConnectType::SESSION,
                                                                  this);
    m_startManagerInterface = InvokerFactory::GetInstance().CreateInvoker("com.deepin.SessionManager",
                                                                          "/com/deepin/StartManager",
                                                                          "com.deepin.StartManager",
                                                                          ConnectType::SESSION,
                                                                          this);
}

// 获取允许自启动应用个数
int AutoStartCheckModel::getAutoStartAppCount()
{
    int count = 0;

    QDBusMessage msg = DBUS_BLOCK_INVOKE(m_launcherInter, "GetAllItemInfos");
    QDBusArgument arg = msg.arguments().first().value<QDBusArgument>();
    QList<LauncherItemInfo> datas;
    arg >> datas;

    for (const auto &it : datas) {
        // 判断该应用状态（是否是自启动）
        bool bStatus = isAutostart(it.Path);
        if (bStatus) {
            count += 1;
        }
    }
    return count;
}

// 判断是否是开机自启
bool AutoStartCheckModel::isAutostart(QString sPath)
{
    QDBusMessage msg = DBUS_BLOCK_INVOKE(m_startManagerInterface, "IsAutostart", sPath);
    GET_MESSAGE_VALUE(bool, flag, msg);

    return flag;
}
