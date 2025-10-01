// Copyright (C) 2020 ~ 2022 Uniontech Software Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "toolboxitemwidget.h"

#include <DFloatingMessage>
#include <DMessageManager>
#include <DPalette>
#include <DFontSizeManager>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QFont>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QPixmap>
#include <QVBoxLayout>

ToolBoxItemWidget::ToolBoxItemWidget(DWidget *parent)
    : DFrame(parent)
    , m_pImageLabel(nullptr)
    , m_pAppNameLabel(nullptr)
    , m_pDesLabel(nullptr)
    , m_pCompanyNameLabel(nullptr)
    , m_pOpenBtn(nullptr)
{
    this->setFixedSize(UNITWIDTH, UNITHEIGHT);
    this->setFrameRounded(true);
    this->setBackgroundRole(DPalette::ItemBackground);

    initUi();
}

ToolBoxItemWidget::~ToolBoxItemWidget()
{
}

void ToolBoxItemWidget::initUi()
{
    QHBoxLayout *pMainLayout = new QHBoxLayout;
    pMainLayout->setContentsMargins(10, 10, 10, 10);
    pMainLayout->setSpacing(10);

    this->setLayout(pMainLayout);

    // item图标
    m_pImageLabel = new DLabel;

    QVBoxLayout *pVlayout = new QVBoxLayout;
    pVlayout->setSpacing(2);
    // 应用名称
    m_pAppNameLabel = new DLabel(this);
    QFont font1 = m_pAppNameLabel->font();
    font1.setBold(true);
    font1.setWeight(QFont::DemiBold);
    m_pAppNameLabel->setFont(font1);
    DFontSizeManager::instance()->bind(m_pAppNameLabel, DFontSizeManager::T5);

    // 应用说明
    m_pDesLabel = new DLabel(this);
    m_pDesLabel->setElideMode(Qt::ElideMiddle);
    DFontSizeManager::instance()->bind(m_pDesLabel, DFontSizeManager::T8);
    // 应用支持者
    m_pCompanyNameLabel = new DTipLabel("", this);
    m_pCompanyNameLabel->setAlignment(Qt::AlignLeft);
    DFontSizeManager::instance()->bind(m_pCompanyNameLabel, DFontSizeManager::T8);

    pVlayout->addWidget(m_pAppNameLabel);
    pVlayout->addWidget(m_pDesLabel);
    pVlayout->addWidget(m_pCompanyNameLabel);

    // 打开按钮
    m_pOpenBtn = new DSuggestButton(this);
    m_pOpenBtn->setText(tr("Open"));
    connect(m_pOpenBtn, &DSuggestButton::clicked, this, &ToolBoxItemWidget::openAppDialog);

    pMainLayout->addWidget(m_pImageLabel);
    pMainLayout->addLayout(pVlayout);
    pMainLayout->addStretch();
    pMainLayout->addWidget(m_pOpenBtn);
}

void ToolBoxItemWidget::setIcon(QString icon)
{
    m_pImageLabel->setPixmap(QIcon::fromTheme(icon).pixmap(48, 48));
}

void ToolBoxItemWidget::setAppTitle(QString title)
{
    m_pAppNameLabel->setText(title);
}

void ToolBoxItemWidget::setAppTip(QString tip)
{
    m_pDesLabel->setText(tip);
}

void ToolBoxItemWidget::setSupporter(QString supporter)
{
    m_pCompanyNameLabel->setText(supporter);
}

void ToolBoxItemWidget::setBtnProperty(QString property)
{
    m_pOpenBtn->setProperty("name", property);
}

void ToolBoxItemWidget::setBtnCheckAuth(bool checkauth)
{
    m_pOpenBtn->setProperty("checkauth", checkauth);
}

void ToolBoxItemWidget::openAppDialog()
{
    Q_EMIT sendOpenAppDialog(m_pOpenBtn->property("name").toString(), m_pOpenBtn->property("checkauth").toBool());
}
