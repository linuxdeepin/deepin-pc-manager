// Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <DFrame>
#include <DTableView>
#include <DSuggestButton>

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QLabel>
#include <QTextEdit>

DWIDGET_USE_NAMESPACE
class HostModifyModel;
class HostModifyWidget : public DFrame
{
    Q_OBJECT
public:
    explicit HostModifyWidget(HostModifyModel *model, QWidget *parent = nullptr);
    ~HostModifyWidget();

private:
    // 初始化界面
    void initUI();

public Q_SLOTS:
    void acceptTextChanged();
    void resetHostFile();
    void saveHostFile();

private:
    HostModifyModel *m_model;
    QTextEdit *m_textEdit;

    DPushButton *m_resetButton;
    DSuggestButton *m_saveButton;
};
