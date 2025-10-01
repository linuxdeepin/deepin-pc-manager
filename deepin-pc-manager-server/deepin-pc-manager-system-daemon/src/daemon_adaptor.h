// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DAEMON_ADAPTOR_H
#define DAEMON_ADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface com.deepin.pc.manager.system.daemon
 */
class DaemonAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.pc.manager.system.daemon")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.deepin.pc.manager.system.daemon\">\n"
"    <method name=\"StartApp\"/>\n"
"    <method name=\"ExitApp\"/>\n"
"    <method name=\"GetTrashFileCount\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"homepath\"/>\n"
"      <arg direction=\"out\" type=\"d\" name=\"count\"/>\n"
"    </method>\n"
"    <method name=\"GetCacheFileCount\">\n"
"      <arg direction=\"out\" type=\"d\" name=\"count\"/>\n"
"    </method>\n"
"    <method name=\"GetLogFileCount\">\n"
"      <arg direction=\"out\" type=\"d\" name=\"count\"/>\n"
"    </method>\n"
"    <method name=\"GetHistoryFileCount\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"homepath\"/>\n"
"      <arg direction=\"out\" type=\"d\" name=\"count\"/>\n"
"    </method>\n"
"    <method name=\"CleanTrashFiles\">\n"
"      <arg direction=\"in\" type=\"as\" name=\"path\"/>\n"
"      <annotation value=\"QStringList\" name=\"org.qtproject.QtDBus.QtTypeName.Out0\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    DaemonAdaptor(QObject *parent);
    virtual ~DaemonAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void CleanTrashFiles(const QStringList &path);
    void ExitApp();
    double GetCacheFileCount();
    double GetHistoryFileCount(const QString &homepath);
    double GetLogFileCount();
    double GetTrashFileCount(const QString &homepath);
    void StartApp();
Q_SIGNALS: // SIGNALS
};

#endif
