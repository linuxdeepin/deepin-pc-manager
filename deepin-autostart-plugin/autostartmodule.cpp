// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "autostartmodule.h"
#include "autostartmodel.h"
#include "autostartwidget.h"

#include <QLayout>

#include <DApplication>
#include <DConfig>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

AutoStartModule::AutoStartModule()
    : QObject()
    , ToolBoxInterface()
    , m_autoStartModel(nullptr)
    , m_autoStartWidget(nullptr)
{
    QTranslator *translator = new QTranslator(this);
    translator->load(QString("/usr/share/deepin-autostart/translations/deepin-autostart_%1.qm").arg(QLocale::system().name()));
    QCoreApplication::installTranslator(translator);
}

AutoStartModule::~AutoStartModule()
{
}

void AutoStartModule::active()
{
    if (!m_autoStartModel) {
        m_autoStartModel = new AutoStartModel;
    }
    if (!m_autoStartWidget) {
        m_autoStartWidget = new AutoStartWidget(m_autoStartModel);
    }

    m_frameProxy->pushWidget(this, m_autoStartWidget);
    m_autoStartWidget->setVisible(true);
}

const QString AutoStartModule::name() const
{
    return "DEEPINAUTOSTART";
}
const QString AutoStartModule::apptitle() const
{
    return tr("Startup Programs");
}
const QString AutoStartModule::apptip() const
{
    return tr("Manage auto startup programs");
}
const QString AutoStartModule::appsupporter() const
{
    return tr("Supported by UOS");
}
const QString AutoStartModule::icon() const
{
    return "deepin-auto-start";
}

int AutoStartModule::apptype() const
{
    return ToolBoxProxyInterface::Apps;
}

bool AutoStartModule::enable() const
{
    return true;
}

bool AutoStartModule::checkauth() const
{
    return false;
}
