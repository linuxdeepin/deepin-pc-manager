// Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "apprepairwidget.h"

#include <DFontSizeManager>

#include <QVBoxLayout>

AppRepairWidget::AppRepairWidget(AppRepairModel *model, QWidget *parent)
    : DFrame(parent)
    , m_model(model)
{
    initUI();
}

AppRepairWidget::~AppRepairWidget()
{
}

void AppRepairWidget::initUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    QLabel *title = new QLabel(tr("Application Repair"), this);
    QFont font1 = title->font();
    font1.setBold(true);
    font1.setWeight(QFont::DemiBold);
    DFontSizeManager::instance()->bind(title, DFontSizeManager::T5);
    title->setContentsMargins(20, 0, 0, 0);

    QLabel *detailInfo = new QLabel(tr("Apply anomaly quick fix, update continuously"), this);
    DFontSizeManager::instance()->bind(detailInfo, DFontSizeManager::T8);
    detailInfo->setContentsMargins(20, 0, 0, 0);

    layout->addWidget(title);
    layout->addWidget(detailInfo);
    layout->addStretch();
}
