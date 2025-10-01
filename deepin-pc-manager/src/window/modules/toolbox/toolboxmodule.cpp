// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "toolboxmodule.h"
#include "toolboxmodel.h"
#include "toolboxwidget.h"
#include "window/common/common.h"

ToolBoxModule::ToolBoxModule(FrameProxyInterface *frame, QObject *parent)
    : QObject(parent)
    , ModuleInterface(frame)
    , m_toolBoxModel(nullptr)
    , m_toolBoxWidget(nullptr)
{
}

ToolBoxModule::~ToolBoxModule()
{
}

// 初始化
void ToolBoxModule::initialize()
{
}

// 预初始化
void ToolBoxModule::preInitialize()
{
    m_toolBoxModel = new ToolBoxModel(this);
}

// 模块名称
const QString ToolBoxModule::name() const
{
    return MODULE_TOOL_BOX_NAME;
}

// 激活
void ToolBoxModule::active(int index)
{
    Q_UNUSED(index);

    if (!m_toolBoxWidget) {
        m_toolBoxWidget = new ToolBoxWidget(m_toolBoxModel);
    }
    m_toolBoxWidget->show();
    m_frameProxy->pushWidget(this, m_toolBoxWidget);
}

// 反激活
void ToolBoxModule::deactive()
{
}
