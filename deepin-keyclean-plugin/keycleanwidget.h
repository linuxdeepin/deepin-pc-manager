// Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <DFrame>
#include <DTableView>
#include <DTipLabel>
#include <DSuggestButton>

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QLabel>
#include <QTextEdit>

DWIDGET_USE_NAMESPACE
class KeyCleanModel;
class KeyCleanWidget : public DFrame
{
    Q_OBJECT
public:
    explicit KeyCleanWidget(KeyCleanModel *model, QWidget *parent = nullptr);
    ~KeyCleanWidget();

private:
    // 初始化界面
    void initUI();

public Q_SLOTS:
    void cleanKeyLock();

private:
    KeyCleanModel *m_model;
    QTextEdit *m_textEdit;
    DSuggestButton *m_execButton;
};
