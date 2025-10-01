// Copyright (C) 2022 ~ 2022 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "toolboxkithelper.h"

ToolBoxKitHelper::ToolBoxKitHelper()
{
}

ToolBoxKitHelper::~ToolBoxKitHelper()
{
}

// 确认文件审计总开关状态
void ToolBoxKitHelper::confirmCheckShowAudit(QString property)
{
    bool result = false;
    // 权限验证
    result = checkAuthorization("com.deepin.deepin-pc-manager.checkAuthentication", getpid());
    Q_EMIT notifyCheckShowAudit(property, result);
}

bool ToolBoxKitHelper::checkAuthorization(const QString &actionId, qint64 applicationPid)
{
    bool ret;
    Authority::Result result;
    result = Authority::instance()->checkAuthorizationSync(actionId, UnixProcessSubject(applicationPid), /// 第一个参数是需要验证的action，和规则文件写的保持一致
                                                           Authority::AllowUserInteraction);
    if (result == Authority::Yes) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}
