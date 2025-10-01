// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "namespace.h"
#include <QObject>

class QWidget;
class QString;

namespace PCC_NAMESPACE {
class ToolBoxInterface;
class ToolBoxProxyInterface
{
public:
    enum AppType {
        All,
        System,
        Network,
        Repair,
        Apps,
        Devices
    };

public:
    // Module request to into next page
    virtual void pushWidget(ToolBoxInterface *const inter, QWidget *const w) = 0;
    virtual void popWidget(ToolBoxInterface *const inter) = 0;
};
} // namespace PCC_NAMESPACE
