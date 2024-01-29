// Copyright (C) 2017 ~ 2019 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "csingleteon.h"

CMutex::CMutex(QObject *parent)
    : QObject(parent)
    , m_mutex(QMutex::Recursive)
{
}
