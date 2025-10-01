// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "autostartmodel.h"
#include "invokers/dbusinvoker.h"
#include "invokers/invokerfactory.h"
#include "dpinyin.h"

AutoStartModel::AutoStartModel(QObject *parent)
    : QObject(parent)
    , m_launcherInter(nullptr)
    , m_startManagerInterface(nullptr)
{
    initData();
    initLauncherData();
}

AutoStartModel::~AutoStartModel()
{
}

// 初始化数据
void AutoStartModel::initData()
{
    registerLauncherItemInfoMetaType();

    // 取应用数据接口
    m_launcherInter = InvokerFactory::GetInstance().CreateInvoker("com.deepin.dde.daemon.Launcher",
                                                                  "/com/deepin/dde/daemon/Launcher",
                                                                  "com.deepin.dde.daemon.Launcher",
                                                                  ConnectType::SESSION, this);

    m_startManagerInterface = InvokerFactory::GetInstance().CreateInvoker("com.deepin.SessionManager",
                                                                          "/com/deepin/StartManager",
                                                                          "com.deepin.StartManager",
                                                                          ConnectType::SESSION, this);

    // 启动器与应用商店应用程序更新信号接收
    m_launcherInter->Connect("ItemChanged", this, SLOT(doRefreshPackageTable()));
}

void AutoStartModel::initLauncherData()
{
    m_appInfoEnableList.clear();
    m_appInfoDisableList.clear();

    QDBusMessage msg = DBUS_BLOCK_INVOKE(m_launcherInter, "GetAllItemInfos");
    QDBusArgument arg = msg.arguments().first().value<QDBusArgument>();
    QList<LauncherItemInfo> datas;
    arg >> datas;

    for (const auto &it : datas) {
        QString path = it.Path;
        if (isAutostart(path)) {
            m_appInfoEnableList.append(it);
        } else {
            m_appInfoDisableList.append(it);
        }
    }
}

// 判断是否是开机自启
bool AutoStartModel::isAutostart(QString sPath)
{
    QDBusMessage msg = DBUS_BLOCK_INVOKE(m_startManagerInterface, "IsAutostart", sPath);
    GET_MESSAGE_VALUE(bool, flag, msg);

    return flag;
}

bool AutoStartModel::exeAutostart(int status, QString sPath)
{
    // 执行自启动关闭操作
    if (Action_Flag_Disable == status) {
        QDBusMessage msg = DBUS_BLOCK_INVOKE(m_startManagerInterface, "RemoveAutostart", sPath);
        GET_MESSAGE_VALUE(bool, flag, msg);
        return flag;
    } else {
        // 执行自启动开启操作
        QDBusMessage msg = DBUS_BLOCK_INVOKE(m_startManagerInterface, "AddAutostart", sPath);
        GET_MESSAGE_VALUE(bool, flag, msg);
        return flag;
    }
}

QString AutoStartModel::getRealName(QString name)
{
    return DTK_CORE_NAMESPACE::Chinese2Pinyin(name);
}

void AutoStartModel::doRefreshPackageTable()
{
    initLauncherData();
    Q_EMIT sendRefreshTable();
}
