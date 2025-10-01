// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QTimer>

#define CLEANER_SYSTEM_TRASH_PATH "/.local/share/Trash/files" // 回收站路径
class DBusInvokerInterface;
class TrashCleanModel : public QObject
{
    Q_OBJECT
public:
    explicit TrashCleanModel(QObject *parent = nullptr);
    ~TrashCleanModel();

    double getTrashFileCount(const QString &homepath);
    double getCacheFileCount();
    double getLogFileCount();
    double getHistoryFileCount(const QString &homepath);
    void cleanCacheFiles();
    void cleanLogFiles();
    void cleanHistoryFiles();

private:
    void initData();
    // 递归函数-获取目录下所有文件
    void scanFile(const QString &path);

private:
    DBusInvokerInterface *m_managerInter;

    QStringList m_fileList;
    QStringList m_trashFileList;
    double m_trashFileCount;
    QStringList m_cacheFileList;
    double m_cacheFileCount;
    QStringList m_logFileList;
    double m_logFileCount;
    QStringList m_historyFileList;
    int m_historyFileCount;
};
