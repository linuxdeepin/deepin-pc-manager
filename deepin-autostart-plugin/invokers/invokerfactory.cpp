// Copyright (C) 2021 ~ 2021 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "invokerfactory.h"

#include "dbusinvoker.h"

InvokerFactory::InvokerFactory()
{
}

InvokerFactory::~InvokerFactory()
{
}

DBusInvokerInterface *InvokerFactory::CreateInvoker(const QString &service,
                                                    const QString &path,
                                                    const QString &interface,
                                                    ConnectType type,
                                                    QObject *parent)
{
    return new DBusInvoker(service, path, interface, type, parent);
}

// 单元测试扩展处理
void InvokerFactory::setInvokerInteface(const QString &interfaceName,
                                        DBusInvokerInterface *interface)
{
#ifdef DEEPIN_DEFENDER_UNITTEST
    // 不关心是否重复
    m_intefaceMap.insert(interfaceName, interface);
#else
    Q_UNUSED(interfaceName);
    Q_UNUSED(interface);
#endif
}
