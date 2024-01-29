/*
 * Copyright (C) 2019 ~ 2022 Deepin Technology Co., Ltd.
 *
 * Author:     xuxin <xuxina@uniontech.com>
 *
 * Maintainer: xuxin <xuxina@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "systemmessagehelper.h"

#include <QDBusMessage>
#include <QDBusConnection>

SystemMessageHelper::SystemMessageHelper(QObject *parent)
    : QObject(parent)
{
}

void SystemMessageHelper::createMessageForAVEngineExpiredInDays(const QString &engineName, int days)
{
    // 提示时间为0-6天
    // 第0天是否过期在调用处判断
    if (engineName.isEmpty() || days < 0 || days >= 7) {
        return;
    }

    QString notifyInfo = tr("The %1 antivirus engine service will expire in %2 days. After the expiration, files in the quarantine list cannot be restored. Please restore them in time if you want.");
    notifyInfo = notifyInfo.arg(engineName).arg(days);

    // 跳转到安全中心的dbus调用
    QString jumpToAVWidget = QString("qdbus,com.deepin.pc.manager,"
                                     "/com/deepin/pc/manager,"
                                     "com.deepin.pc.manager.ShowModule,"
                                     "virusscan");

    QDBusMessage m = QDBusMessage::createMethodCall("com.deepin.dde.Notification",
                                                    "/com/deepin/dde/Notification",
                                                    "com.deepin.dde.Notification",
                                                    "Notify");
    QStringList action;
    action << "_open1" << tr("View", "button"); //添加按钮
    QVariantMap inform; //按钮的点击操作
    inform.insert(QString("x-deepin-action-_open1"), jumpToAVWidget);

    m << QString("deepin-defender")
      << uint(0)
      << QString("deepin-defender")
      << QString("") << notifyInfo
      << action
      << inform
      << int(10 * 1000); // 停留10秒
    QDBusMessage respone = QDBusConnection::sessionBus().call(m);
}
