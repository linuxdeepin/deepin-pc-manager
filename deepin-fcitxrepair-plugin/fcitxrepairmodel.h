// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

class FcitxRepairModel : public QObject
{
    Q_OBJECT
public:
    explicit FcitxRepairModel(QObject *parent = nullptr);
    ~FcitxRepairModel();

    QString repairFcitxIssue();

private:
    void initData();

private:
};
