// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fileiconmodel.h"

#include <QProcess>

#define LOOG_FILE_ICON_DIR "/usr/share/deepin-pc-manager/"
#define LOOG_FILE_ICON_PATH "/usr/share/deepin-pc-manager/fileicon.sh"
FileIconModel::FileIconModel(QObject *parent)
    : QObject(parent)
{
    initData();
}

FileIconModel::~FileIconModel()
{
}

// 初始化数据
void FileIconModel::initData()
{
}

QString FileIconModel::resolveIconIssue()
{
    QProcess builder;
    builder.setWorkingDirectory(LOOG_FILE_ICON_DIR);
    builder.setProcessChannelMode(QProcess::MergedChannels);
    builder.start(QString("/bin/bash %1 %2").arg(LOOG_FILE_ICON_PATH));
    if (!builder.waitForFinished()) {
        builder.close();
        return "修复图标异常失败";
    }
    return QString::fromLocal8Bit(builder.readAllStandardOutput()).trimmed();
}
