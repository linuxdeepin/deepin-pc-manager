// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

class FileIconModel : public QObject
{
    Q_OBJECT
public:
    explicit FileIconModel(QObject *parent = nullptr);
    ~FileIconModel();
    QString resolveIconIssue();

private:
    void initData();

private:
};
