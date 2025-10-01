// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "filenamemodel.h"

#include <QProcess>

#define LOOG_FILE_NAME_DIR "/usr/share/deepin-pc-manager/"
#define LOOG_FILE_NAME_PATH "/usr/share/deepin-pc-manager/loogfilename.sh"
FileNameModel::FileNameModel(QObject *parent)
    : QObject(parent)
{
    initData();
}

FileNameModel::~FileNameModel()
{
}

// 初始化数据
void FileNameModel::initData()
{
}

QString FileNameModel::startLoogFileNameSupport()
{
    QProcess builder;
    builder.setWorkingDirectory(LOOG_FILE_NAME_DIR);
    builder.setProcessChannelMode(QProcess::MergedChannels);
    builder.start(QString("/bin/bash %1 %2").arg(LOOG_FILE_NAME_PATH));
    if (!builder.waitForFinished()) {
        builder.close();
        return "长文件名支持失败";
    }

    return QString::fromLocal8Bit(builder.readAllStandardOutput()).trimmed();
}
