// Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "hostmodifywidget.h"
#include "hostmodifymodel.h"

#include <DFloatingMessage>
#include <DMessageManager>
#include <DFontSizeManager>
#include <DPalette>
#include <QVBoxLayout>

HostModifyWidget::HostModifyWidget(HostModifyModel *model, QWidget *parent)
    : DFrame(parent)
    , m_model(model)
    , m_textEdit(nullptr)
    , m_resetButton(nullptr)
    , m_saveButton(nullptr)
{
    initUI();

    m_textEdit->setText(m_model->getHostsFileInfo());
    connect(m_textEdit, &QTextEdit::textChanged, this, &HostModifyWidget::acceptTextChanged);
    connect(m_resetButton, &QPushButton::clicked, this, &HostModifyWidget::resetHostFile);
    connect(m_saveButton, &DSuggestButton::clicked, this, &HostModifyWidget::saveHostFile);
}

HostModifyWidget::~HostModifyWidget()
{
}

void HostModifyWidget::initUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(20, 20, 20, 20);
    this->setLayout(layout);

    QLabel *title = new QLabel(tr("Hosts configuration modification"), this);
    QFont font1 = title->font();
    font1.setBold(true);
    font1.setWeight(QFont::DemiBold);
    title->setFont(font1);
    DFontSizeManager::instance()->bind(title, DFontSizeManager::T5);
    title->setContentsMargins(0, 0, 0, 0);
    title->setContentsMargins(10, 0, 0, 0);

    QLabel *detailInfo = new QLabel(tr("Manually modify hosts file"), this);
    DFontSizeManager::instance()->bind(detailInfo, DFontSizeManager::T8);
    detailInfo->setContentsMargins(0, 0, 0, 0);
    detailInfo->setContentsMargins(10, 0, 0, 0);

    DFrame *textFrame = new DFrame(this);
    QVBoxLayout *textLayout = new QVBoxLayout(textFrame);
    textLayout->setContentsMargins(2, 2, 2, 2);
    textLayout->setSpacing(0);
    textFrame->setLayout(textLayout);

    m_textEdit = new QTextEdit(textFrame);
    m_textEdit->setFixedSize(QSize(900, 400));
    m_textEdit->setLineWidth(0);
    textLayout->addWidget(m_textEdit, 0, Qt::AlignCenter);

    layout->addWidget(title);
    layout->addWidget(detailInfo);
    layout->addWidget(textFrame, 0, Qt::AlignCenter);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(10);
    m_resetButton = new DPushButton(this);
    m_resetButton->setFixedWidth(150);
    m_resetButton->setText(tr("Reset"));
    m_saveButton = new DSuggestButton(this);
    m_saveButton->setFixedWidth(150);
    m_saveButton->setText(tr("Save"));

    m_resetButton->setEnabled(false);
    m_saveButton->setEnabled(false);
    btnLayout->addStretch();
    btnLayout->addWidget(m_resetButton, 0, Qt::AlignRight);
    btnLayout->addWidget(m_saveButton, 0, Qt::AlignRight);
    layout->addLayout(btnLayout);
}

void HostModifyWidget::acceptTextChanged()
{
    QString sDefaultInfo = m_model->getDefaultHostInfo();
    QString sCurrentInfo = m_textEdit->toPlainText();

    if (sDefaultInfo != sCurrentInfo) {
        m_resetButton->setEnabled(true);
        m_saveButton->setEnabled(true);
    } else {
        m_resetButton->setEnabled(false);
        m_saveButton->setEnabled(false);
    }
}

void HostModifyWidget::resetHostFile()
{
    m_textEdit->setText(m_model->getDefaultHostInfo());
}
void HostModifyWidget::saveHostFile()
{
    m_model->saveHostFile(m_textEdit->toPlainText());

    m_resetButton->setEnabled(false);
    m_saveButton->setEnabled(false);

    DFloatingMessage *floMsgSwitchOff = new DFloatingMessage(DFloatingMessage::TransientType);
    floMsgSwitchOff->setDuration(2000);
    floMsgSwitchOff->setIcon(QIcon::fromTheme("dcc_ok_tip"));
    floMsgSwitchOff->setMessage(tr("Save successful"));
    DMessageManager::instance()->sendMessage(this, floMsgSwitchOff);
    DMessageManager::instance()->setContentMargens(this, QMargins(0, 0, 0, 20));
}
