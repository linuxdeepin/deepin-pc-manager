// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <dtkcore_global.h>
#include <QObject>

#include "interface/toolboxinterface.h"
#include "interface/toolboxproxyinterface.h"

namespace PCC_NAMESPACE {
class ToolBoxInterface;
class ToolBoxProxyInterface;
} // namespace PCC_NAMESPACE

using namespace PCC_NAMESPACE;

class AppRepairWidget;
class AppRepairModel;
class AppRepairModule : public QObject
    , public ToolBoxInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID ToolBoxInterface_iid FILE "apprepair_settings.json")
    Q_INTERFACES(PCC_NAMESPACE::ToolBoxInterface)

public:
    explicit AppRepairModule();

    ~AppRepairModule() Q_DECL_OVERRIDE;

    const QString name() const Q_DECL_OVERRIDE;
    const QString apptitle() const Q_DECL_OVERRIDE;
    const QString apptip() const Q_DECL_OVERRIDE;
    const QString appsupporter() const Q_DECL_OVERRIDE;
    const QString icon() const Q_DECL_OVERRIDE;
    int apptype() const Q_DECL_OVERRIDE;
    bool enable() const Q_DECL_OVERRIDE;
    bool checkauth() const Q_DECL_OVERRIDE;

private:
    void onStatusChanged();

public Q_SLOTS:
    void active() Q_DECL_OVERRIDE;

private:
    AppRepairModel *m_appRepairModel;
    AppRepairWidget *m_appRepairWidget;
};
