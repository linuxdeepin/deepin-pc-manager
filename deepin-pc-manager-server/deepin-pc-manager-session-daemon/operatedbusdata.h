/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     wubowen <wubowen_cm@deepin.com>
 *
 * Maintainer: wubowen <wubowen_cm@deepin.com>
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

#include "daemon_adaptor.h"
#include "window/modules/common/defenderlauncherinfo.h"

#include <DGuiApplicationHelper>

#include <QObject>

class StartupMonitor;
class DBusInvokerInterface;
class DefenderAuthorityWrapper;

// 系统配置
namespace def {
class SystemSettings;
}

class QGSettings;
class QDBusContext;

using namespace def;

DCORE_USE_NAMESPACE
class AppsStartToEnd;
class OperateDBusData : public QObject
    , protected QDBusContext
{
    Q_OBJECT

public:
    explicit OperateDBusData(QObject *parent = nullptr);

    ~OperateDBusData();

public Q_SLOTS:
    // 接口
    void StartApp();
    void ExitApp();
    void preInitialize();

    // 初始化
    void initialize();

    // 开机自启动管理
    bool startLauncherManage();
    QString getAppsInfoEnable();
    QString getAppsInfoDisable();
    bool isAutostart(QString sPath);
    bool exeAutostart(int nStatus, QString sPath);
    void refreshData(bool bAdd, QString sID);

    // 垃圾清理 回收站/日志/缓存/痕迹
    QStringList GetTrashInfoList();
    QStringList GetLogInfoList();
    QStringList GetCacheInfoList();
    QStringList GetHistoryInfoList();
    QString GetBrowserCookiesInfoList();
    QString GetAppTrashInfoList();
    void DeleteSpecifiedFiles(QStringList);
    void OnLauncherItemChanged(const QString &, const LauncherItemInfo &, qint64);

    // 递归函数-获取目录下所有文件
    void scanFile(const QString &path);
    // 首页体检垃圾清理项大小
    double GetCleanerSum();

    // 首页体检清理所有垃圾
    void DeleteAllCleaner();

    // 请求开始垃圾文件扫描
    void RequestStartTrashScan();
    // 请求清理选中的垃圾文件
    void RequestCleanSelectTrash();

    // 初始化获取系统架构
    void initSystemArchitecture();
    QString GetSystemArchitecture();

    void RequestAuthWithID(const QString &, int);
    void ModulesRequestAuthorityActived();

private:
    // 校验调用者
    bool isValidInvoker();

Q_SIGNALS:
    // 开机自启动管理
    void AccessRefreshData(bool bAdd, QString sID);

    void NotifyAuthStarted();
    void NotifyAuthFinished();
    void NotifyAuthResult(const QString &, int, bool);

private:
    QGSettings *m_gsetting; // gsetting配置对象
    SystemSettings *m_sysSettings; // 系统配置
    DaemonAdaptor *m_adaptpr;

    // 开机自启动管理
    QMap<QString, QString> m_mapEnable; // 不允许自启动数据容器
    QMap<QString, QString> m_mapDisable; // 允许自启动数据容器
    QStringList m_enableData;
    QStringList m_disableData;
    QString m_enableAppsInfo;
    QString m_disableAppsInfo;
    DBusInvokerInterface *m_launcherInter; // 取应用数据接口
    DBusInvokerInterface *m_startManagerInterface; // 自启动接口

    // 垃圾清理
    DBusInvokerInterface *m_managerInter; // 检查/var/cache/apt/archives下，可安全删除的deb包
    QStringList m_fileList;
    QStringList m_fileListSum;
    double m_dbCleanerSum;
    /// 首页垃圾清理
    // 首页垃圾清理扫描结果可清理文件路径列表
    QStringList m_homepageCleanerResultFileList;
    // 首页垃圾清理扫描结果可清理文件总大小，byte
    double m_homepageCleanerResultSize;

    // 系统服务
    DBusInvokerInterface *m_rootMonitorDBusInter;

    // 系统架构
    QString m_sSystemArchitecture;

    // 提权管理
    DefenderAuthorityWrapper *m_authWrapper;
};