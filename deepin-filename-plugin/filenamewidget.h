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
class FileNameModel;
class FileNameWidget : public DFrame
{
    Q_OBJECT
public:
    explicit FileNameWidget(FileNameModel *model, QWidget *parent = nullptr);
    ~FileNameWidget();

private:
    // 初始化界面
    void initUI();

public Q_SLOTS:
    void supportLoogFileName();

private:
    FileNameModel *m_model;
    QTextEdit *m_textEdit;
    DSuggestButton *m_execButton;
};
