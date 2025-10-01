// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "defenderlauncherinfo.h"

#include <QObject>

#define Action_Flag_Disable 0 // 状态标志 - 不允许
#define Action_Flag_Enable 1 // 状态标志 - 允许
class DBusInvokerInterface;
class AutoStartModel : public QObject
{
    Q_OBJECT
public:
    explicit AutoStartModel(QObject *parent = nullptr);
    ~AutoStartModel();

    LauncherItemInfoList getAppInfoEnableList() { return m_appInfoEnableList; }
    LauncherItemInfoList getAppInfoDisableList() { return m_appInfoDisableList; }

    bool isAutostart(QString sPath);
    bool exeAutostart(int status, QString sPath);
    QString getRealName(QString name);

private:
    void initData();
    void initLauncherData();

public Q_SLOTS:
    void doRefreshPackageTable();

Q_SIGNALS:
    void sendRefreshTable();

private:
    DBusInvokerInterface *m_launcherInter; // 取应用数据接口
    DBusInvokerInterface *m_startManagerInterface; // 自启动接口

    LauncherItemInfoList m_appInfoEnableList;
    LauncherItemInfoList m_appInfoDisableList;
};
