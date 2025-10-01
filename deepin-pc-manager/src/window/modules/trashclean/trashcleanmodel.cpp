// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "trashcleanmodel.h"
#include "window/common/invokers/invokerfactory.h"

#include <QDir>

TrashCleanModel::TrashCleanModel(QObject *parent)
    : QObject(parent)
{
    initData();
}

TrashCleanModel::~TrashCleanModel()
{
}

void TrashCleanModel::initData()
{
    m_sysetemDaemonInvokerInter = InvokerFactory::GetInstance().CreateInvoker("com.deepin.pc.manager.system.daemon",
                                                                              "/com/deepin/pc/manager/system/daemon",
                                                                              "com.deepin.pc.manager.system.daemon",
                                                                              ConnectType::SYSTEM,
                                                                              this);
}

double TrashCleanModel::getTrashFileCount()
{
    QDBusMessage msg;
    msg = DBUS_BLOCK_INVOKE(m_sysetemDaemonInvokerInter, "GetTrashFileCount", QDir::homePath());
    GET_MESSAGE_VALUE(double, trashFileCount, msg);
    return trashFileCount;
}
double TrashCleanModel::getCacheFileCount()
{
    QDBusMessage msg;
    msg = DBUS_BLOCK_INVOKE(m_sysetemDaemonInvokerInter, "GetCacheFileCount");
    GET_MESSAGE_VALUE(double, cacheFileCount, msg);
    return cacheFileCount;
}
double TrashCleanModel::getLogFileCount()
{
    QDBusMessage msg;
    msg = DBUS_BLOCK_INVOKE(m_sysetemDaemonInvokerInter, "GetLogFileCount");
    GET_MESSAGE_VALUE(double, logFileCount, msg);
    return logFileCount;
}
double TrashCleanModel::getHistoryFileCount()
{
    QDBusMessage msg;
    msg = DBUS_BLOCK_INVOKE(m_sysetemDaemonInvokerInter, "GetHistoryFileCount", QDir::homePath());
    GET_MESSAGE_VALUE(double, historyFileCount, msg);
    return historyFileCount;
}

void TrashCleanModel::cleanTrashFile(QStringList itemlist)
{
    if (itemlist.contains(TRASH_CLEAN_ITEM_TYPE_TRASH)) {
        DTrashManager::instance()->cleanTrash();
    }
    DBUS_NOBLOCK_INVOKE(m_sysetemDaemonInvokerInter, "CleanTrashFiles", itemlist);
}
