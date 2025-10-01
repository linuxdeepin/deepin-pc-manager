// Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "toolboxwidget.h"
#include "toolboxmodel.h"
#include "toolboxdialog.h"
#include "include/interface/toolboxinterface.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

#define APPTYPE Qt::UserRole + 1
using namespace PCC_NAMESPACE;
ToolBoxWidget::ToolBoxWidget(ToolBoxModel *model, DWidget *parent)
    : DWidget(parent)
    , m_model(model)
    , m_pToolButtonBox(nullptr)
    , m_toolDialog(nullptr)
{
    // 初始化UI
    initUI();
    initData();
    connect(m_model, &ToolBoxModel::sendCheckShowAuditResult, this, &ToolBoxWidget::acceptCheckShowAuditResult);
}

ToolBoxWidget::~ToolBoxWidget()
{
}

// 初始化界面
void ToolBoxWidget::initUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(20, 20, 20, 20);
    this->setLayout(mainLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setContentsMargins(18, 0, 0, 0);
    mainLayout->addLayout(btnLayout);

    m_pToolButtonBox = new DButtonBox(this);
    m_pToolButtonBox->setOrientation(Qt::Horizontal);
    m_pToolButtonBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    QList<DButtonBoxButton *> btnlist;
    // 全部
    DButtonBoxButton *pBtn = createBoxButton(tr("All"), m_pToolButtonBox);
    btnlist.append(pBtn);
    // 系统
    pBtn = createBoxButton(tr("System"), m_pToolButtonBox);
    btnlist.append(pBtn);
    // 网络
    pBtn = createBoxButton(tr("Network"), m_pToolButtonBox);
    btnlist.append(pBtn);
    // 修复
    pBtn = createBoxButton(tr("Repair"), m_pToolButtonBox);
    btnlist.append(pBtn);
    // 应用
    pBtn = createBoxButton(tr("Apps"), m_pToolButtonBox);
    btnlist.append(pBtn);
    // 硬件
    pBtn = createBoxButton(tr("Devices"), m_pToolButtonBox);
    btnlist.append(pBtn);
    m_pToolButtonBox->setButtonList(btnlist, true);
    btnlist.at(0)->setChecked(true);

    for (int i = 0; i < btnlist.size(); i++) {
        m_pToolButtonBox->setId(btnlist.at(i), ToolBoxProxyInterface::AppType(i));
    }

    btnLayout->addWidget(m_pToolButtonBox, 0, Qt::AlignLeft);
    m_pToolView = new ToolBoxListView(this);
    m_pToolView->setFixedHeight(500);
    m_pToolView->setSpacing(10);
    mainLayout->addWidget(m_pToolView);
    mainLayout->addStretch();

    connect(m_pToolButtonBox, &DButtonBox::buttonClicked, this, [&](QAbstractButton *pBtn) {
        int iID = m_pToolButtonBox->id(pBtn);
        updateToolsMsgByClassify(iID);
    });
}

DButtonBoxButton *ToolBoxWidget::createBoxButton(const QString &strText, QWidget *pParent)
{
    QFont font;
    font.setFamily("SourceHanSansSC");
    font.setWeight(QFont::Medium);
    font.setPixelSize(14);
    DButtonBoxButton *pBtn = new DButtonBoxButton(strText, pParent);
    pBtn->setFixedSize(QSize(72, 30));
    pBtn->setFont(font);
    return pBtn;
}

void ToolBoxWidget::initData()
{
    //updateToolsMsgByClassify(ToolBoxProxyInterface::AppType::All);
    QList<QPair<AppInfo, QObject *>> modules = m_model->getAllModulesInfo();

    for (auto &pluginInfo : modules) {
        int apptype = pluginInfo.first.apptype;
        QString name = pluginInfo.first.name;
        QString apptitle = pluginInfo.first.apptitle;
        QString apptip = pluginInfo.first.apptip;
        QString appsupporter = pluginInfo.first.appsupporter;
        QString icon = pluginInfo.first.icon;
        bool enable = pluginInfo.first.enable;
        bool checkauth = pluginInfo.first.checkauth;
        if (!enable) {
            continue;
        }

        ToolBoxItemWidget *pToolItem = new ToolBoxItemWidget(m_pToolView);
        connect(pToolItem, &ToolBoxItemWidget::sendOpenAppDialog, this, &ToolBoxWidget::openAppDialog);
        pToolItem->setIcon(icon);
        pToolItem->setAppTitle(apptitle);
        pToolItem->setAppTip(apptip);
        pToolItem->setSupporter(appsupporter);
        pToolItem->setBtnProperty(name);
        pToolItem->setBtnCheckAuth(checkauth);

        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setSizeHint(QSize(UNITWIDTH, UNITHEIGHT));
        pItem->setData(APPTYPE, apptype);
        m_pToolView->addItem(pItem);
        m_pToolView->setItemWidget(pItem, pToolItem);
    }
}

void ToolBoxWidget::openAppDialog(QString property, bool checkauth)
{
    if (checkauth) {
        m_model->checkShowAudit(property);
    } else {
        showAppWidget(property);
    }
}

// 发送入口点击权限判断结果
void ToolBoxWidget::acceptCheckShowAuditResult(QString property, bool result)
{
    if (result) {
        showAppWidget(property);
    }
}

void ToolBoxWidget::showAppWidget(QString property)
{
    QList<QPair<AppInfo, QObject *>> modules = m_model->getAllModulesInfo();
    for (auto &pluginInfo : modules) {
        if (property == pluginInfo.first.name) {
            QObject *instance = pluginInfo.second;
            auto *module = qobject_cast<ToolBoxInterface *>(instance);
            if (!module) {
                qWarning() << "module object is nullptr:";
                return;
            }

            m_toolDialog = new ToolBoxDialog(this);
            m_toolDialog->addWidget(module);
            m_toolDialog->raise();
            m_toolDialog->show();
            break;
        }
    }
}

void ToolBoxWidget::updateToolsMsgByClassify(int type)
{
    int count = m_pToolView->count();
    for (int i = 0; i < count; ++i) {
        QListWidgetItem *pItem = m_pToolView->item(i);
        if (ToolBoxProxyInterface::AppType::All == type) {
            m_pToolView->setItemHidden(m_pToolView->item(i), false);
        } else {
            if (pItem->data(APPTYPE).toInt() == type) {
                m_pToolView->setItemHidden(m_pToolView->item(i), false);
            } else {
                m_pToolView->setItemHidden(m_pToolView->item(i), true);
            }
        }
    }
}
