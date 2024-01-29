/*
 * Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
 *
 * Author:     caitao <caitao@uniontech.com>
 *
 * Maintainer: caitao <caitao@uniontech.com>
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

#pragma once

#include "window/namespace.h"
#include "../src/window/modules/common/common.h"
#include "com_deepin_lastore_jobmanager.h"
#include "com_deepin_lastore_updater.h"
#include "com_deepin_lastore_job.h"

#include <QDBusInterface>

class DBusInvokerInterface;

DEF_NAMESPACE_BEGIN

class SysUpdaterModel : public QObject
{
    Q_OBJECT
public:
    explicit SysUpdaterModel(QObject *parent = nullptr);
    ~SysUpdaterModel();

    // 检查是否有更新
    void CheckForUpdates();

Q_SIGNALS:
    // 发送是否存在系统更新结果
    void SendHaveUpdates(bool have);

public Q_SLOTS:
    // 设置检查系统更新任务
    void SetCheckUpdatesJob(const QString &jobPath);
    // 停止检查系统版本
    void stopCheckingSysVer();
    // 检查更新任务状态
    void onCheckJobStatusChanged(const QString &status);

private:
    // 系统更新管理器dbus服务
    DBusInvokerInterface *m_managerInvokerInter;
    // 系统更新dbus服务
    DBusInvokerInterface *m_updaterInvokerInter;
    // 检查更新任务
    DBusInvokerInterface *m_checkUpdateJobInvokerInter;
    QTimer *m_checkUpdateJobTimer;
};

DEF_NAMESPACE_END