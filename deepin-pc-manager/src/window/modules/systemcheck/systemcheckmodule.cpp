// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "systemcheckmodule.h"
#include "systemcheckmodel.h"
#include "systemcheckwidget.h"
#include "window/common/common.h"

SystemCheckModule::SystemCheckModule(FrameProxyInterface *frame, QObject *parent)
    : QObject(parent)
    , ModuleInterface(frame)
    , m_systemCheckModel(nullptr)
    , m_systemCheckWidget(nullptr)
{
}

SystemCheckModule::~SystemCheckModule()
{
}

// 初始化
void SystemCheckModule::initialize()
{
}

// 预初始化
void SystemCheckModule::preInitialize()
{
    m_systemCheckModel = new SystemCheckModel(this);
}

// 模块名称
const QString SystemCheckModule::name() const
{
    return MODULE_SYSTEM_CHECK_NAME;
}

// 激活
void SystemCheckModule::active(int index)
{
    Q_UNUSED(index);

    if (!m_systemCheckWidget) {
        m_systemCheckWidget = new SystemCheckWidget(m_systemCheckModel);
    }
    m_systemCheckWidget->show();
    m_frameProxy->pushWidget(this, m_systemCheckWidget);
}

// 反激活
void SystemCheckModule::deactive()
{
    m_systemCheckWidget->hide();
}
