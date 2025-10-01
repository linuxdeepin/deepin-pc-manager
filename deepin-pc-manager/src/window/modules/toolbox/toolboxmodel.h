// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "toolboxkithelper.h"

#include <QObject>
#include <QWidget>
#include <QIcon>

struct AppInfo {
    int apptype;
    QString name;
    QString apptitle;
    QString apptip;
    QString appsupporter;
    QString icon;
    bool enable;
    bool checkauth;
};

class ToolBoxModel : public QObject
{
    Q_OBJECT
public:
    explicit ToolBoxModel(QObject *parent = nullptr);
    ~ToolBoxModel();

    QList<QPair<AppInfo, QObject *>> getAllModulesInfo() { return m_allModules; }
    // 入口点击权限判断
    void checkShowAudit(QString property);

private:
    // 初始化数据
    void initData();
    void loadPlugin(const QString &pluginPath);

public Q_SLOTS:
    void acceptCheckShowAudit(QString property, bool result);

Q_SIGNALS:
    // 发送入口点击提权
    void nofityCheckAudit(QString property);
    // 发送入口点击权限判断结果
    void sendCheckShowAuditResult(QString property, bool result);

private:
    QList<QPair<AppInfo, QObject *>> m_allModules;

    ToolBoxKitHelper *m_policyKitHelper;
    QThread *m_polKitHelperThread;
};
