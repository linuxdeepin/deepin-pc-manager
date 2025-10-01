// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QMargins>
#include <QVariant>

Q_DECLARE_METATYPE(QMargins)

// 公共dbus接口类
#define DBUS_PROPERTY_INVOKER_NAME "dbusPropertyInvoker" // 公共dbus接口属性获取识别

// 基础设置
#define SETTING_BASE_CLOSE_WINDOW "base.close_window.close_window_type" // 关闭主窗口设置
#define SETTING_CLOSE_WINDOW_TYPE "close_window_type" // 关闭窗口类型

// 各模块名称
#define MODULE_SYSTEM_CHECK_NAME "systemcheck" // 系统体检
#define MODULE_TRASH_CLEAN_NAME "trashclean" // 垃圾清理
#define MODULE_TOOL_BOX_NAME "toolbox" // 工具箱

/********************导航栏*****************************/
#define SYSTEM_CHECK_MODEL_LIGHT "dcc_system_check" // 系统体检模块--浅色
#define SYSTEM_CHECK_MODEL_DARK "dcc_system_check_dark" // 系统体检模块--深色
#define TRASH_CLEAN_MODEL_LIGHT "dcc_trash_clean" // 垃圾清理模块--浅色
#define TRASH_CLEAN_MODEL_DARK "dcc_trash_clean_dark" // 垃圾清理模块--深色
#define TOOL_BOX_LIGHT "dcc_tool_box" // 工具箱--浅色
#define TOOL_BOX_DARK "dcc_tool_box_dark" // 工具箱--深色

// 图片icon
#define SYSTEM_CHECK_HOME_PAGE_LOGO "dcc_syscheck_main_logo" // 系统体检logo
#define SYSTEM_CHECK_CANCEL_LOGO "dcc_syscheck_progress_cancel" // 系统体检取消

// 垃圾清理项
#define TRASH_CLEAN_ITEM_TYPE_TRASH "SystemTrash" // 垃圾清理item -回收站
#define TRASH_CLEAN_ITEM_TYPE_CACHE "SystemCache" // 垃圾清理item -系统缓存
#define TRASH_CLEAN_ITEM_TYPE_LOG "SystemLog" // 垃圾清理item -系统日志
#define TRASH_CLEAN_ITEM_TYPE_HISTORY "HisInfoApp" // 垃圾清理item -痕迹和应用痕迹

// 枚举 - 程序退出配置
enum closeType { Tray,
                 Exit,
                 Ask,
                 Count };

// 主题类型
enum ColorType { UnknownType,
                 LightType,
                 DarkType };

// 系统体检
enum SysCheckItemID {
    AutoStartApp,
    SystemUpdate,
    Trash,
    Disk,
    DevMode,
    MaxLimit
};
// 错误数据模型角色
enum CheckResultModelRole {
    ID = Qt::UserRole + 1, // 检查项ID
    CheckProgressFlag, // 检查项是完成还是超时还是被屏蔽（异常状态为系统版本检查超时， 病毒查杀冲突， SSH被隐藏）
    IsIssueHappen, // 检查项是否存在问题
    IsIgnored, //  检查项是否被设置为忽略状态
    Details // 检查项问题详细数据(不要记录复杂数据，仅检查项UI需要显示的数据，如 int bool等，具体数据放在本model中处理)
};
enum CheckProgressStatus {
    Success,
    TimeOut,
    Masked
};

// 硬件检测结果
enum ServiceResult {
    ServiceBlock = 0,
    NoFound,
    CheckSucessed
};

// 网络设置检测结果
enum NetSettingResult {
    NoNetConn = 0,
    NetSettingSkip,
    NetSettingCheckFailed,
    NetSettingCheckSucessed
};

// DHCP检测结果
enum DHCPResult {
    DHCPCheckFailed = 0,
    DHCPCheckSucessed
};

// DNS检测结果
enum DNSResult {
    DNSCheckError = 0,
    DNSCheckAbnormal,
    DNSCheckSucessed
};

// host检测结果
enum HostResult {
    HostCheckFailed = 0,
    HostCheckSucessed
};

// 网络连接访问结果
enum NetConnResult {
    NetConnFailed = 0,
    NetConnSucessed
};

namespace Utils {
// 检查并转换QDbusVariant为QVariant
QVariant checkAndTransQDbusVarIntoQVar(const QVariant &var);

inline bool compareVersion(const QString &targetVersion, const QString &baseVersion)
{
    QStringList version1 = baseVersion.split(".");
    QStringList version2 = targetVersion.split(".");

    if (version1.size() != version2.size()) {
        return false;
    }

    for (int i = 0; i < version1.size(); ++i) {
        // 相等判断下一个子版本号
        if (version1[i] == version2[i])
            continue;

        // 转成整形比较
        if (version1[i].toInt() > version2[i].toInt()) {
            return false;
        } else {
            return true;
        }
    }

    return true;
}
} // namespace Utils
