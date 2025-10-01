// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "apprepairmodule.h"
#include "apprepairmodel.h"
#include "apprepairwidget.h"

#include <QLayout>

#include <DApplication>
#include <DConfig>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

AppRepairModule::AppRepairModule()
    : QObject()
    , ToolBoxInterface()
    , m_appRepairModel(nullptr)
    , m_appRepairWidget(nullptr)
{
    QTranslator *translator = new QTranslator(this);
    translator->load(QString("/usr/share/deepin-apprepair/translations/deepin-apprepair_%1.qm").arg(QLocale::system().name()));
    QCoreApplication::installTranslator(translator);
}

AppRepairModule::~AppRepairModule()
{
}

void AppRepairModule::active()
{
    if (!m_appRepairModel) {
        m_appRepairModel = new AppRepairModel;
    }
    if (!m_appRepairWidget) {
        m_appRepairWidget = new AppRepairWidget(m_appRepairModel);
    }

    m_frameProxy->pushWidget(this, m_appRepairWidget);
    m_appRepairWidget->setVisible(true);
}

const QString AppRepairModule::name() const
{
    return "DEEPINAPPREPAIR";
}
const QString AppRepairModule::apptitle() const
{
    return tr("Application Repair");
}
const QString AppRepairModule::apptip() const
{
    return tr("Apply anomaly quick fix, update continuously");
}
const QString AppRepairModule::appsupporter() const
{
    return tr("Supported by UOS");
}
const QString AppRepairModule::icon() const
{
    return "deepin-app-repair";
}

int AppRepairModule::apptype() const
{
    return ToolBoxProxyInterface::Repair;
}

bool AppRepairModule::enable() const
{
    return false;
}

bool AppRepairModule::checkauth() const
{
    return false;
}
