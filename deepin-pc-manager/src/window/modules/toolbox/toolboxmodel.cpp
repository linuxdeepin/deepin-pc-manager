// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "toolboxmodel.h"
#include "include/interface/toolboxinterface.h"
#include "window/common/common.h"

const QString ModuleDirectory = "/usr/lib/deepin-pc-manager/modules/";

using namespace PCC_NAMESPACE;
ToolBoxModel::ToolBoxModel(QObject *parent)
    : QObject(parent)
{
    initData();

    // thread
    m_policyKitHelper = new ToolBoxKitHelper;
    m_polKitHelperThread = new QThread(this);
    m_policyKitHelper->moveToThread(m_polKitHelperThread);
    // 通知去改变usb限制模式
    connect(m_policyKitHelper, &ToolBoxKitHelper::notifyCheckShowAudit, this, &ToolBoxModel::acceptCheckShowAudit);

    m_polKitHelperThread->start();
    connect(this, &ToolBoxModel::nofityCheckAudit, m_policyKitHelper, &ToolBoxKitHelper::confirmCheckShowAudit);
}

ToolBoxModel::~ToolBoxModel()
{
}

void ToolBoxModel::initData()
{
    QDir moduleDir(ModuleDirectory);
    if (!moduleDir.exists()) {
        qInfo() << "module directory not exists";
        return;
    }

    moduleDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    auto moduleList = moduleDir.entryInfoList();
    for (const auto &module : moduleList) {
        loadPlugin(module.absoluteFilePath());
    }
}

void ToolBoxModel::loadPlugin(const QString &path)
{
    if (!QLibrary::isLibrary(path))
        return;
    qDebug() << "loading module: " << path << " threadId:" << QThread::currentThreadId();

    QPluginLoader loader(path);
    const QJsonObject &meta = loader.metaData().value("MetaData").toObject();
    qDebug() << "api: " << meta.value("api").toString();
    if (!Utils::compareVersion(meta.value("api").toString(), "1.0.0")) {
        qDebug() << "plugin's version is too low";
        return;
    }

    QObject *instance = loader.instance();
    if (!instance) {
        qWarning() << "Load plugin error:" << loader.errorString();
        return;
    }

    auto *module = qobject_cast<ToolBoxInterface *>(instance);
    if (!module) {
        qWarning() << "module object is nullptr:";
        return;
    }
    AppInfo appInfo;
    appInfo.apptype = module->apptype();
    appInfo.name = module->name();
    appInfo.apptitle = module->apptitle();
    appInfo.apptip = module->apptip();
    appInfo.appsupporter = module->appsupporter();
    appInfo.icon = module->icon();
    appInfo.enable = module->enable();
    appInfo.checkauth = module->checkauth();

    m_allModules.push_back({appInfo, instance});
}

// 入口点击权限判断
void ToolBoxModel::checkShowAudit(QString property)
{
    Q_EMIT nofityCheckAudit(property);
}

void ToolBoxModel::acceptCheckShowAudit(QString property, bool result)
{
    Q_EMIT sendCheckShowAuditResult(property, result);
}
