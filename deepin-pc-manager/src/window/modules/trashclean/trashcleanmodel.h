// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "window/common/common.h"

#include <DTrashManager>

#include <QObject>

DCORE_USE_NAMESPACE
class DBusInvokerInterface;
class TrashCleanModel : public QObject
{
    Q_OBJECT
public:
    explicit TrashCleanModel(QObject *parent = nullptr);
    ~TrashCleanModel();

    double getTrashFileCount();
    double getCacheFileCount();
    double getLogFileCount();
    double getHistoryFileCount();
    void cleanTrashFile(QStringList itemlist);

private:
    // 初始化数据
    void initData();

private:
    DBusInvokerInterface *m_sysetemDaemonInvokerInter;
};
