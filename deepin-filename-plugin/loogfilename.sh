#!/bin/bash

# SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

echo '操作开始'
dde-dconfig --set -a org.deepin.dde.file-manager -r org.deepin.dde.file-manager -k dfm.mount.dlnfs -v true
echo 'dde-dconfig 配置完成'
echo '操作完成，dlnfs状态：'`dde-dconfig --get -a org.deepin.dde.file-manager -r org.deepin.dde.file-manager -k dfm.mount.dlnfs`
echo '已打开长文件名支持，重启电脑后生效'