// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fcitxrepairmodel.h"

#include <QProcess>
#define LOOG_FCITX_REPAIR_DIR "/usr/share/deepin-pc-manager/"
#define LOOG_FCITX_REPAIR_PATH "/usr/share/deepin-pc-manager/fcitxrepair.sh"
FcitxRepairModel::FcitxRepairModel(QObject *parent)
    : QObject(parent)
{
    initData();
}

FcitxRepairModel::~FcitxRepairModel()
{
}

// 初始化数据
void FcitxRepairModel::initData()
{
}

QString FcitxRepairModel::repairFcitxIssue()
{
    QProcess builder;
    builder.setWorkingDirectory(LOOG_FCITX_REPAIR_DIR);
    builder.setProcessChannelMode(QProcess::MergedChannels);
    builder.start(QString("/bin/bash %1 %2").arg(LOOG_FCITX_REPAIR_PATH));
    if (!builder.waitForFinished()) {
        builder.close();
        return "修复输入法失败";
    }
    return QString::fromLocal8Bit(builder.readAllStandardOutput()).trimmed();
}
