#!/bin/sh

# SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

echo '操作开始'
sed -i '/^inode/d'  ~/.config/mimeapps.list
echo 'mimeapps.list字符替换成功'
echo '操作完成'