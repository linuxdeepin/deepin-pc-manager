// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

class FileNameModel : public QObject
{
    Q_OBJECT
public:
    explicit FileNameModel(QObject *parent = nullptr);
    ~FileNameModel();

    QString startLoogFileNameSupport();

private:
    void initData();

private:
};
