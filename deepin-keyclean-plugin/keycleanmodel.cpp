// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "keycleanmodel.h"

#include <QProcess>

#define LOOG_KEY_LOCK_DIR "/usr/share/deepin-pc-manager/"
#define LOOG_KEY_LOCK_PATH "/usr/share/deepin-pc-manager/keyclean.sh"
KeyCleanModel::KeyCleanModel(QObject *parent)
    : QObject(parent)
{
    initData();
}

KeyCleanModel::~KeyCleanModel()
{
}

// 初始化数据
void KeyCleanModel::initData()
{
}

QString KeyCleanModel::startCleanKeyLock()
{
    QProcess builder;
    builder.setWorkingDirectory(LOOG_KEY_LOCK_DIR);
    builder.setProcessChannelMode(QProcess::MergedChannels);
    builder.start(QString("/bin/bash %1 %2").arg(LOOG_KEY_LOCK_PATH));
    if (!builder.waitForFinished()) {
        builder.close();
        return "秘钥环清理失败";
    }
    return QString::fromLocal8Bit(builder.readAllStandardOutput()).trimmed();
}
