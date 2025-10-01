// Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <DFrame>
#include <DTableView>

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QLabel>

DWIDGET_USE_NAMESPACE
class AppRepairModel;
class AppRepairWidget : public DFrame
{
    Q_OBJECT
public:
    explicit AppRepairWidget(AppRepairModel *model, QWidget *parent = nullptr);
    ~AppRepairWidget();

private:
    // 初始化界面
    void initUI();

private:
    AppRepairModel *m_model;
};
