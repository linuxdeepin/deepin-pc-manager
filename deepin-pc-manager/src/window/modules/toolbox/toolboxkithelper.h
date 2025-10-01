// Copyright (C) 2022 ~ 2022 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

#include <polkit-qt5-1/PolkitQt1/Authority>

using namespace PolkitQt1;

class ToolBoxKitHelper : public QObject
{
    Q_OBJECT
public:
    explicit ToolBoxKitHelper();
    ~ToolBoxKitHelper();

Q_SIGNALS:
    // 通知检测显示结果
    void notifyCheckShowAudit(QString property, bool result);

public Q_SLOTS:
    // 确认检测显示结果
    void confirmCheckShowAudit(QString property);

public:
    bool checkAuthorization(const QString &actionId, qint64 applicationPid);
};
