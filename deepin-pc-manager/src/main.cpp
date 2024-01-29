// Copyright (C) 2017 ~ 2019 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "window/mainwindow.h"
#include "defenderdbusservice.h"
#include "src/window/modules/common/common.h"
#include "src/accessible/appaccessible.h"

#include <DApplication>
#include <DWidgetUtil>
#include <DApplicationSettings>
#include <DLog>

#include <QObject>
#include <QDBusConnection>
#include <QDBusInterface>
#include <polkit-qt5-1/PolkitQt1/Authority>

DEF_USING_NAMESPACE
DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE
using namespace PolkitQt1;

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    a.setAttribute(Qt::AA_UseHighDpiPixmaps); // 高清图
    a.setOrganizationName("deepin");
    a.setApplicationName("deepin-pc-manager");
    a.setApplicationVersion(DApplication::buildVersion((QMAKE_VERSION))); // pro 文件中的版本
    a.loadTranslator();
    a.setApplicationDisplayName(QObject::tr("PC Manager"));
    a.setStyle("chameleon");
    a.setWindowIcon(QIcon::fromTheme("deepin-defender"));
    a.setProductIcon(QIcon::fromTheme("deepin-defender"));
    a.setApplicationDescription(QApplication::translate("main", "Deepin PC Manager is an application tool designed to help users quickly manage, maintain, and optimize computer systems."));
    a.setQuitOnLastWindowClosed(false);

    // 设置异步线程退出时间，默认为30秒
    QThreadPool::globalInstance()->setExpiryTimeout(100);

    // 提供自动化测试所需要的实现
    QAccessible::installFactory([](const QString &classname, QObject *object) {
        QAccessibleInterface *interface = nullptr;
        if (object && object->isWidgetType()) {
            // 实测,QPushButton的access不能通过QAbstractButton实现
            if ("QPushButton" == classname) {
                interface = new AppAccessibleButton(static_cast<QPushButton *>(object));
            } else if ("QLabel" == classname) {
                interface = new AppAccessibleLabel(static_cast<QLabel *>(object));
            } else if ("QAbstractButton" == classname) {
                interface = new AppAccessibleQAbstractButton(static_cast<QAbstractButton *>(object));
            }
        }

        return interface;
    });

    DApplicationSettings settings;

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    MainWindow w;
    Dtk::Widget::moveToCenter(&w);

    // regist dbus service
    DefenderDBusService dbus(&w);

    // 若是服务器版本，添加授权弹窗
    if (DSysInfo::UosType::UosServer == SystemType) {
        // 打开之前，进行授权确认
        Authority::Result result;
        result = Authority::instance()->checkAuthorizationSync("com.deepin.deepin-pc-manager.runByAuthentication",
                                                               UnixProcessSubject(getpid()), /// 第一个参数是需要验证的action，和规则文件写的保持一致
                                                               Authority::AllowUserInteraction);
        if (result != Authority::Yes) {
            exit(0);
        }
    }

    w.show();
    return a.exec();
}
