// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "include/interface/toolboxinterface.h"
#include "include/interface/toolboxproxyinterface.h"

#include <DFrame>
#include <DDialog>
#include <DTitlebar>
#include <DBlurEffectWidget>

DWIDGET_USE_NAMESPACE

class ToolBoxDialog : public DFrame
    , public PCC_NAMESPACE::ToolBoxProxyInterface
{
    Q_OBJECT

public:
    explicit ToolBoxDialog(QWidget *parent = nullptr);
    ~ToolBoxDialog() override;

    // 添加弹框内容
    void addWidget(PCC_NAMESPACE::ToolBoxInterface *inter);

    void popWidget(PCC_NAMESPACE::ToolBoxInterface *const inter) override;
    void pushWidget(PCC_NAMESPACE::ToolBoxInterface *const inter, QWidget *const w) override;

private:
    // 初始化界面
    void initUI();

private:
    QVBoxLayout *m_mainlayout;
    DBlurEffectWidget *m_blurEffect;
    DTitlebar *m_titleBar;
};
