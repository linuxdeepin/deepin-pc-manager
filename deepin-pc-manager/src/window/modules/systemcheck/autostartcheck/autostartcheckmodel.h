// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "defenderlauncherinfo.h"

#include <QObject>

class DBusInvokerInterface;
class AutoStartCheckModel : public QObject
{
    Q_OBJECT
public:
    explicit AutoStartCheckModel(QObject *parent = nullptr);
    ~AutoStartCheckModel();

    // 获取允许自启动应用个数
    int getAutoStartAppCount();

private:
    // 初始化数据
    void initData();
    bool isAutostart(QString sPath);

private:
    DBusInvokerInterface *m_launcherInter; // 取应用数据接口
    DBusInvokerInterface *m_startManagerInterface; // 自启动接口
};
