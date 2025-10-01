// Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "keycleanwidget.h"
#include "keycleanmodel.h"

#include <DFontSizeManager>
#include <DDialog>

#include <QVBoxLayout>

KeyCleanWidget::KeyCleanWidget(KeyCleanModel *model, QWidget *parent)
    : DFrame(parent)
    , m_model(model)
{
    initUI();
    connect(m_execButton, &DSuggestButton::clicked, this, &KeyCleanWidget::cleanKeyLock);
}

KeyCleanWidget::~KeyCleanWidget()
{
}

void KeyCleanWidget::initUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(20, 20, 20, 20);
    this->setLayout(layout);

    QLabel *title = new QLabel(tr("Keyring cleaning"), this);
    QFont font1 = title->font();
    font1.setBold(true);
    font1.setWeight(QFont::DemiBold);
    DFontSizeManager::instance()->bind(title, DFontSizeManager::T5);
    title->setContentsMargins(10, 0, 0, 0);

    QLabel *detailInfo = new QLabel(tr("Resolve repeated keyring unlock requests"), this);
    DFontSizeManager::instance()->bind(detailInfo, DFontSizeManager::T8);
    detailInfo->setContentsMargins(10, 0, 0, 0);

    QLabel *descriptionInfo = new QLabel(tr("Tool Description"), this);
    DFontSizeManager::instance()->bind(descriptionInfo, DFontSizeManager::T8);
    descriptionInfo->setContentsMargins(10, 0, 0, 0);

    DTipLabel *tipLabel = new DTipLabel(tr("Resolve the issue of repeatedly prompting for 'Unlock Login Keyring' dialog and being unable to prevent the pop-up through operations."), this);
    DFontSizeManager::instance()->bind(tipLabel, DFontSizeManager::T8);
    tipLabel->setContentsMargins(10, 0, 0, 0);
    tipLabel->setAlignment(Qt::AlignLeft);

    DFrame *textFrame = new DFrame(this);
    QVBoxLayout *textLayout = new QVBoxLayout(textFrame);
    textLayout->setContentsMargins(2, 2, 2, 2);
    textLayout->setSpacing(0);
    textFrame->setLayout(textLayout);

    m_textEdit = new QTextEdit(textFrame);
    m_textEdit->setFixedSize(QSize(900, 400));
    m_textEdit->setLineWidth(0);
    textLayout->addWidget(m_textEdit, 0, Qt::AlignCenter);

    m_execButton = new DSuggestButton(this);
    m_execButton->setFixedWidth(180);
    m_execButton->setText(tr("Execute immediately"));

    layout->addWidget(title);
    layout->addWidget(detailInfo);
    layout->addSpacing(10);
    layout->addWidget(descriptionInfo);
    layout->addWidget(tipLabel);
    layout->addWidget(textFrame, 0, Qt::AlignCenter);
    layout->addWidget(m_execButton, 0, Qt::AlignRight);
    layout->addStretch();
}

void KeyCleanWidget::cleanKeyLock()
{
    DDialog *dlg = new DDialog(this);
    dlg->setOnButtonClickedClose(true);
    // 设置属性当关闭时候删除自己
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setIcon(QIcon::fromTheme("deepin-key-clean"));
    dlg->setMessage(tr("The operation cannot be undone. Please confirm if you want to proceed?"));
    dlg->addButton(tr("Cancel", "button"), false, DDialog::ButtonType::ButtonNormal);
    dlg->addButton(tr("Continue"), false, DDialog::ButtonType::ButtonRecommend);

    connect(dlg, &DDialog::buttonClicked, this, [&](int index, const QString &text) {
        Q_UNUSED(text);
        if (1 == index) {
            m_textEdit->setText(m_model->startCleanKeyLock());
        }
    });
    dlg->exec();
}
