// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "toolboxitemwidget.h"
#include "toolboxlistview.h"

#include <DWidget>
#include <DButtonBox>

DWIDGET_USE_NAMESPACE
class ToolBoxModel;
class ToolBoxDialog;
class ToolBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ToolBoxWidget(ToolBoxModel *model, DWidget *parent = nullptr);
    ~ToolBoxWidget();

private:
    // 初始化界面
    void initUI();
    DButtonBoxButton *createBoxButton(const QString &strText, QWidget *pParent = nullptr);
    void initData();

    void updateToolsMsgByClassify(int type);
    void showAppWidget(QString property);

public Q_SLOTS:
    void openAppDialog(QString property, bool checkauth);
    // 发送入口点击权限判断结果
    void acceptCheckShowAuditResult(QString property, bool result);

private:
    ToolBoxModel *m_model;
    DButtonBox *m_pToolButtonBox;
    ToolBoxListView *m_pToolView;
    ToolBoxDialog *m_toolDialog;
};
