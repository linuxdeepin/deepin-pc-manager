// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fileiconmodule.h"
#include "fileiconmodel.h"
#include "fileiconwidget.h"

#include <QLayout>

#include <DApplication>
#include <DConfig>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

FileIconModule::FileIconModule()
    : QObject()
    , ToolBoxInterface()
    , m_fileIconModel(nullptr)
    , m_fileIconWidget(nullptr)
{
    QTranslator *translator = new QTranslator(this);
    translator->load(QString("/usr/share/deepin-fileicon/translations/deepin-fileicon_%1.qm").arg(QLocale::system().name()));
    QCoreApplication::installTranslator(translator);
}

FileIconModule::~FileIconModule()
{
}

void FileIconModule::active()
{
    if (!m_fileIconModel) {
        m_fileIconModel = new FileIconModel;
    }
    if (!m_fileIconWidget) {
        m_fileIconWidget = new FileIconWidget(m_fileIconModel);
    }

    m_frameProxy->pushWidget(this, m_fileIconWidget);
    m_fileIconWidget->setVisible(true);
}

const QString FileIconModule::name() const
{
    return "DEEPINFILEICON";
}
const QString FileIconModule::apptitle() const
{
    return tr("File icon anomaly repair");
}
const QString FileIconModule::apptip() const
{
    return tr("Resolves the issue of abnormal file icon display");
}
const QString FileIconModule::appsupporter() const
{
    return tr("Supported by UOS");
}
const QString FileIconModule::icon() const
{
    return "deepin-file-icon";
}

int FileIconModule::apptype() const
{
    return ToolBoxProxyInterface::Repair;
}

bool FileIconModule::enable() const
{
    return true;
}

bool FileIconModule::checkauth() const
{
    return false;
}
