// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

class AppRepairModel : public QObject
{
    Q_OBJECT
public:
    explicit AppRepairModel(QObject *parent = nullptr);
    ~AppRepairModel();

private:
    void initData();

private:
};
