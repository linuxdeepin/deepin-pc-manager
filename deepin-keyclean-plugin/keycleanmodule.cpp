// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "keycleanmodule.h"
#include "keycleanmodel.h"
#include "keycleanwidget.h"

#include <QLayout>

#include <DApplication>
#include <DConfig>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

KeyCLeanModule::KeyCLeanModule()
    : QObject()
    , ToolBoxInterface()
    , m_keyCleanModel(nullptr)
    , m_keyCleanWidget(nullptr)
{
    QTranslator *translator = new QTranslator(this);
    translator->load(QString("/usr/share/deepin-keyclean/translations/deepin-keyclean_%1.qm").arg(QLocale::system().name()));
    QCoreApplication::installTranslator(translator);
}

KeyCLeanModule::~KeyCLeanModule()
{
}

void KeyCLeanModule::active()
{
    if (!m_keyCleanModel) {
        m_keyCleanModel = new KeyCleanModel;
    }
    if (!m_keyCleanWidget) {
        m_keyCleanWidget = new KeyCleanWidget(m_keyCleanModel);
    }

    m_frameProxy->pushWidget(this, m_keyCleanWidget);
    m_keyCleanWidget->setVisible(true);
}

const QString KeyCLeanModule::name() const
{
    return "DEEPINKEYCLEAN";
}
const QString KeyCLeanModule::apptitle() const
{
    return tr("Keyring cleaning");
}
const QString KeyCLeanModule::apptip() const
{
    return tr("Resolve repeated keyring unlock requests");
}
const QString KeyCLeanModule::appsupporter() const
{
    return tr("Supported by UOS");
}
const QString KeyCLeanModule::icon() const
{
    return "deepin-key-clean";
}

int KeyCLeanModule::apptype() const
{
    return ToolBoxProxyInterface::System;
}

bool KeyCLeanModule::enable() const
{
    return true;
}

bool KeyCLeanModule::checkauth() const
{
    return false;
}
