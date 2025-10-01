// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <fcntl.h>
#include <unistd.h>

#include <QObject>

class DBusInvokerInterface;
class DiskCheckModel : public QObject
{
    Q_OBJECT
public:
    explicit DiskCheckModel(QObject *parent = nullptr);
    ~DiskCheckModel();

    // 磁盘管理器 是否已安装
    bool isDiskManagerInterValid();
    // 获取磁盘状态
    bool getDiskStatus();

private:
    // 初始化数据
    void initData();

    // 获取磁盘设备路径列表
    QList<QString> getDiskDevicePathList();

private:
    DBusInvokerInterface *m_diskManagerInvokerInter; // 磁盘管理dbus服务
};
