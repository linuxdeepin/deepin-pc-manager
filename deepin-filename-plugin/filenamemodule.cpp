// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "filenamemodule.h"
#include "filenamemodel.h"
#include "filenamewidget.h"

#include <QLayout>

#include <DApplication>
#include <DConfig>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

FileNameModule::FileNameModule()
    : QObject()
    , ToolBoxInterface()
    , m_fileNameModel(nullptr)
    , m_fileNameWidget(nullptr)
{
    QTranslator *translator = new QTranslator(this);
    translator->load(QString("/usr/share/deepin-filename/translations/deepin-filename_%1.qm").arg(QLocale::system().name()));
    QCoreApplication::installTranslator(translator);
}

FileNameModule::~FileNameModule()
{
}

void FileNameModule::active()
{
    if (!m_fileNameModel) {
        m_fileNameModel = new FileNameModel;
    }
    if (!m_fileNameWidget) {
        m_fileNameWidget = new FileNameWidget(m_fileNameModel);
    }

    m_frameProxy->pushWidget(this, m_fileNameWidget);
    m_fileNameWidget->setVisible(true);
}

const QString FileNameModule::name() const
{
    return "DEEPINFILENAME";
}
const QString FileNameModule::apptitle() const
{
    return tr("Long filename support");
}
const QString FileNameModule::apptip() const
{
    return tr("Extend file name length to 225 chinese characters");
}
const QString FileNameModule::appsupporter() const
{
    return tr("Supported by UOS");
}
const QString FileNameModule::icon() const
{
    return "deepin-file-name";
}

int FileNameModule::apptype() const
{
    return ToolBoxProxyInterface::System;
}

bool FileNameModule::enable() const
{
    return true;
}

bool FileNameModule::checkauth() const
{
    return false;
}
