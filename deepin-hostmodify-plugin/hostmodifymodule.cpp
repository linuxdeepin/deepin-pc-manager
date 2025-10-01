// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "hostmodifymodule.h"
#include "hostmodifymodel.h"
#include "hostmodifywidget.h"

#include <QLayout>

#include <DApplication>
#include <DConfig>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

HostModifyModule::HostModifyModule()
    : QObject()
    , ToolBoxInterface()
    , m_hostModifyModel(nullptr)
    , m_hostModifyWidget(nullptr)
{
    QTranslator *translator = new QTranslator(this);
    translator->load(QString("/usr/share/deepin-hostmodify/translations/deepin-hostmodify_%1.qm").arg(QLocale::system().name()));
    QCoreApplication::installTranslator(translator);
}

HostModifyModule::~HostModifyModule()
{
}

void HostModifyModule::active()
{
    if (!m_hostModifyModel) {
        m_hostModifyModel = new HostModifyModel;
    }
    if (!m_hostModifyWidget) {
        m_hostModifyWidget = new HostModifyWidget(m_hostModifyModel);
    }

    m_frameProxy->pushWidget(this, m_hostModifyWidget);
    m_hostModifyWidget->setVisible(true);
}

const QString HostModifyModule::name() const
{
    return "DEEPINHOSTMODIFY";
}
const QString HostModifyModule::apptitle() const
{
    return tr("Hosts configuration modification");
}
const QString HostModifyModule::apptip() const
{
    return tr("Manually modify hosts file");
}
const QString HostModifyModule::appsupporter() const
{
    return tr("Supported by UOS");
}
const QString HostModifyModule::icon() const
{
    return "deepin-host-modify";
}

int HostModifyModule::apptype() const
{
    return ToolBoxProxyInterface::Network;
}

bool HostModifyModule::enable() const
{
    return true;
}

bool HostModifyModule::checkauth() const
{
    return true;
}
