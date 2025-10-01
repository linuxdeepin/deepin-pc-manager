// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "toolboxproxyinterface.h"
#include "namespace.h"

#include <QtCore>
#include <QIcon>
#include <QWidget>

namespace PCC_NAMESPACE {

// ModuleInterface作为每个规范每个Module的接口，每个Module实现必须实现其所有虚函数。
class ToolBoxInterface
{
public:
    ToolBoxInterface() = default;
    ToolBoxInterface(ToolBoxProxyInterface *frameProxy)
        : m_frameProxy(frameProxy)
    {
    }
    virtual ~ToolBoxInterface() {}

    void setFrameProxy(ToolBoxProxyInterface *frameProxy) { m_frameProxy = frameProxy; }

    virtual const QString name() const = 0;
    virtual const QString apptitle() const = 0;
    virtual const QString apptip() const = 0;
    virtual const QString appsupporter() const = 0;
    virtual const QString icon() const = 0;
    virtual int apptype() const = 0;
    virtual bool enable() const = 0;
    virtual bool checkauth() const = 0;

    /// \brief active
    /// 当模块第一次被点击进入时，active会被调用,如果是插件,重载的时候必须声明为slots,否则加载不了
    virtual void active() {}
    virtual void deactive() {}

protected:
    ToolBoxProxyInterface *m_frameProxy {nullptr};
};

} // namespace PCC_NAMESPACE

#define ToolBoxInterface_iid "com.deepin.dde.pcmanager.module/1.0"
Q_DECLARE_INTERFACE(PCC_NAMESPACE::ToolBoxInterface, ToolBoxInterface_iid)
