// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

class KeyCleanModel : public QObject
{
    Q_OBJECT
public:
    explicit KeyCleanModel(QObject *parent = nullptr);
    ~KeyCleanModel();

    QString startCleanKeyLock();

private:
    void initData();

private:
};
