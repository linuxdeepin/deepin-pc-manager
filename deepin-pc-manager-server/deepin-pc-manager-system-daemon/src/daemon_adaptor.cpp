// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "daemon_adaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class DaemonAdaptor
 */

DaemonAdaptor::DaemonAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

DaemonAdaptor::~DaemonAdaptor()
{
    // destructor
}

void DaemonAdaptor::CleanTrashFiles(const QStringList &path)
{
    // handle method call com.deepin.pc.manager.system.daemon.CleanTrashFiles
    QMetaObject::invokeMethod(parent(), "CleanTrashFiles", Q_ARG(QStringList, path));
}

void DaemonAdaptor::ExitApp()
{
    // handle method call com.deepin.pc.manager.system.daemon.ExitApp
    QMetaObject::invokeMethod(parent(), "ExitApp");
}

double DaemonAdaptor::GetCacheFileCount()
{
    // handle method call com.deepin.pc.manager.system.daemon.GetCacheFileCount
    double count;
    QMetaObject::invokeMethod(parent(), "GetCacheFileCount", Q_RETURN_ARG(double, count));
    return count;
}

double DaemonAdaptor::GetHistoryFileCount(const QString &homepath)
{
    // handle method call com.deepin.pc.manager.system.daemon.GetHistoryFileCount
    double count;
    QMetaObject::invokeMethod(parent(), "GetHistoryFileCount", Q_RETURN_ARG(double, count), Q_ARG(QString, homepath));
    return count;
}

double DaemonAdaptor::GetLogFileCount()
{
    // handle method call com.deepin.pc.manager.system.daemon.GetLogFileCount
    double count;
    QMetaObject::invokeMethod(parent(), "GetLogFileCount", Q_RETURN_ARG(double, count));
    return count;
}

double DaemonAdaptor::GetTrashFileCount(const QString &homepath)
{
    // handle method call com.deepin.pc.manager.system.daemon.GetTrashFileCount
    double count;
    QMetaObject::invokeMethod(parent(), "GetTrashFileCount", Q_RETURN_ARG(double, count), Q_ARG(QString, homepath));
    return count;
}

void DaemonAdaptor::StartApp()
{
    // handle method call com.deepin.pc.manager.system.daemon.StartApp
    QMetaObject::invokeMethod(parent(), "StartApp");
}

