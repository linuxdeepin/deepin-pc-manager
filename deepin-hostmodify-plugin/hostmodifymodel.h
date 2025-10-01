// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

class DBusInvokerInterface;
class HostModifyModel : public QObject
{
    Q_OBJECT
public:
    explicit HostModifyModel(QObject *parent = nullptr);
    ~HostModifyModel();

    QString getDefaultHostInfo() { return m_hostInfo; }
    // 获取hosts文件信息
    QString getHostsFileInfo();

    void saveHostFile(QString host);

private:
    void initData();

private:
    DBusInvokerInterface *m_sysetemDaemonInvokerInter;

    QString m_hostInfo;
};
