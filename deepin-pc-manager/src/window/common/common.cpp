// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "common.h"
#include <QDBusVariant>

// 检查并转换QDbusVariant为QVariant
QVariant Utils::checkAndTransQDbusVarIntoQVar(const QVariant &var)
{
    QDBusVariant qdbusVar = var.value<QDBusVariant>();
    QVariant retVar = qdbusVar.variant();
    if (!retVar.isNull()) {
        return retVar;
    }
    return var;
}
