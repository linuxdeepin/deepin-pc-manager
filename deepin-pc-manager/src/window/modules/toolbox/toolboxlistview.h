// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <DListWidget>

DWIDGET_USE_NAMESPACE
class ToolBoxListView : public DListWidget
{
    Q_OBJECT
public:
    explicit ToolBoxListView(QWidget *parent = nullptr);
    ~ToolBoxListView() override;
};
