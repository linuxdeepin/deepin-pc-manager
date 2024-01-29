// Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "parted/parted.h"
#include <fcntl.h>
#include <unistd.h>

#include <QObject>

// 获取磁盘设备路径列表
QList<QString> getDiskDevicePathList();
