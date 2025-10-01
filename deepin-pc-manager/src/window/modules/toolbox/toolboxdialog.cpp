// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "toolboxdialog.h"

#include <QVBoxLayout>

using namespace PCC_NAMESPACE;
ToolBoxDialog::ToolBoxDialog(QWidget *parent)
    : DFrame(parent)
    , m_mainlayout(nullptr)
    , m_blurEffect(nullptr)
    , m_titleBar(nullptr)
{
    // 大小设置
    this->resize(960, 640);
    this->setFixedHeight(640);
    // 设置为Dialog
    setWindowFlags(Qt::Dialog);
    // 设置为阻态
    setWindowModality(Qt::ApplicationModal);
    // 背景透明化
    setAttribute(Qt::WA_TranslucentBackground);

    // 界面初始化
    initUI();
}

ToolBoxDialog::~ToolBoxDialog()
{
}

// 布局
void ToolBoxDialog::initUI()
{
    // 主布局
    m_mainlayout = new QVBoxLayout();
    m_mainlayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(m_mainlayout);
    // 背景透明化
    m_blurEffect = new DBlurEffectWidget(this);
    m_blurEffect->setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    // 设置模糊大小
    m_blurEffect->resize(size());
    m_blurEffect->lower();

    // 右上角关闭按钮
    m_titleBar = new DTitlebar(this);
    m_titleBar->setBackgroundTransparent(true);
    m_titleBar->setTitle("");
    m_titleBar->setMenuVisible(false);
    m_mainlayout->addWidget(m_titleBar, 0, Qt::AlignRight | Qt::AlignTop);
}

void ToolBoxDialog::addWidget(ToolBoxInterface *inter)
{
    inter->setFrameProxy(this);
    inter->active();
}

void ToolBoxDialog::popWidget(ToolBoxInterface *const inter)
{
    Q_UNUSED(inter);
}
void ToolBoxDialog::pushWidget(ToolBoxInterface *const inter, QWidget *const w)
{
    Q_UNUSED(inter);
    m_mainlayout->addWidget(w);
}
