// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "netcheckmodule.h"
#include "netcheckwidget.h"

#include <QLayout>

#include <DApplication>
#include <DConfig>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE
NetCheckModule::NetCheckModule()
    : QObject()
    , ToolBoxInterface()
    , m_netCheckModel(nullptr)
    , m_moduleWidget(nullptr)
{
    QTranslator *translator = new QTranslator(this);
    translator->load(QString("/usr/share/deepin-netcheck/translations/deepin-netcheck_%1.qm").arg(QLocale::system().name()));
    QCoreApplication::installTranslator(translator);
}

NetCheckModule::~NetCheckModule()
{
}

void NetCheckModule::active()
{
    if (!m_netCheckModel) {
        m_netCheckModel = new NetCheckModel;
    }
    if (!m_moduleWidget) {
        m_moduleWidget = new NetCheckWidget(m_netCheckModel);
    }

    m_frameProxy->pushWidget(this, m_moduleWidget);
    m_moduleWidget->setVisible(true);
}

const QString NetCheckModule::name() const
{
    return "DEEPINNETCHECK";
}
const QString NetCheckModule::apptitle() const
{
    return tr("Network Detection");
}
const QString NetCheckModule::apptip() const
{
    return tr("Quickly detect network anomaly causes");
}
const QString NetCheckModule::appsupporter() const
{
    return tr("Supported by UOS");
}
const QString NetCheckModule::icon() const
{
    return "deepin-net-check";
}

int NetCheckModule::apptype() const
{
    return ToolBoxProxyInterface::Network;
}

bool NetCheckModule::enable() const
{
    return true;
}

bool NetCheckModule::checkauth() const
{
    return false;
}
