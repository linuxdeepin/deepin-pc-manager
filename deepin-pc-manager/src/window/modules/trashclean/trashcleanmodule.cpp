// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "trashcleanmodule.h"
#include "trashcleanmodel.h"
#include "trashcleanwidget.h"
#include "window/common/common.h"

TrashCleanModule::TrashCleanModule(FrameProxyInterface *frame, QObject *parent)
    : QObject(parent)
    , ModuleInterface(frame)
    , m_trashCleanModel(nullptr)
    , m_trashCleanWidget(nullptr)
{
}

TrashCleanModule::~TrashCleanModule()
{
}

// 初始化
void TrashCleanModule::initialize()
{
}

// 预初始化
void TrashCleanModule::preInitialize()
{
    m_trashCleanModel = new TrashCleanModel(this);
}

// 模块名称
const QString TrashCleanModule::name() const
{
    return MODULE_TRASH_CLEAN_NAME;
}

// 激活
void TrashCleanModule::active(int index)
{
    Q_UNUSED(index);

    if (!m_trashCleanWidget) {
        m_trashCleanWidget = new TrashCleanWidget(m_trashCleanModel);
    }
    m_trashCleanWidget->show();
    m_frameProxy->pushWidget(this, m_trashCleanWidget);
}

// 反激活
void TrashCleanModule::deactive()
{
}
