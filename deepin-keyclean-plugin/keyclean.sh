#!/bin/sh

# SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

echo '操作开始'
echo '移动keyrings目录到回收站：'`mv  ~/.local/share/keyrings/*    ~/.local/share/Trash/files/`
echo 'deepin-keyrings-wb：'`mv  ~/.local/share/deepin-keyrings-wb/*    ~/.local/share/Trash/files/`
echo '操作完成'