// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fcitxrepairmodule.h"
#include "fcitxrepairmodel.h"
#include "fcitxrepairwidget.h"

#include <QLayout>

#include <DApplication>
#include <DConfig>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

FcitxRepairModule::FcitxRepairModule()
    : QObject()
    , ToolBoxInterface()
    , m_fcitxRepairModel(nullptr)
    , m_fcitxRepairWidget(nullptr)
{
    QTranslator *translator = new QTranslator(this);
    translator->load(QString("/usr/share/deepin-fcitxrepair/translations/deepin-fcitxrepair_%1.qm").arg(QLocale::system().name()));
    QCoreApplication::installTranslator(translator);
}

FcitxRepairModule::~FcitxRepairModule()
{
}

void FcitxRepairModule::active()
{
    if (!m_fcitxRepairModel) {
        m_fcitxRepairModel = new FcitxRepairModel;
    }
    if (!m_fcitxRepairWidget) {
        m_fcitxRepairWidget = new FcitxRepairWidget(m_fcitxRepairModel);
    }

    m_frameProxy->pushWidget(this, m_fcitxRepairWidget);
    m_fcitxRepairWidget->setVisible(true);
}

const QString FcitxRepairModule::name() const
{
    return "DEEPINFCITXREPAIR";
}
const QString FcitxRepairModule::apptitle() const
{
    return tr("Input method repair");
}
const QString FcitxRepairModule::apptip() const
{
    return tr("Fix input method anomalies");
}
const QString FcitxRepairModule::appsupporter() const
{
    return tr("Supported by UOS");
}
const QString FcitxRepairModule::icon() const
{
    return "deepin-fcitx-repair";
}

int FcitxRepairModule::apptype() const
{
    return ToolBoxProxyInterface::Repair;
}

bool FcitxRepairModule::enable() const
{
    return true;
}

bool FcitxRepairModule::checkauth() const
{
    return false;
}
