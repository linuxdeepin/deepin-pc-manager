#!/bin/sh

# SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

echo '操作开始'
cd /home/${USER}/.config/
echo '正在清理输入法缓存目录'
rm -rf com.sogou.sogoupinyin-uos/ fcitx/ iflytek/ iflyime-qimpanel/ SogouPY/ sogoupinyin/ .com.sogou.sogoupinyin-uos-zyb/ /fcitx-huayupy
echo '操作完成'
echo '请重启系统后打开输入法配置进行配置'