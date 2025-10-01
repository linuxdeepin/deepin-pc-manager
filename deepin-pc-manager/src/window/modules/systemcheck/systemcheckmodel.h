// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "window/common/common.h"
#include "autostartcheck/autostartcheckmodel.h"
#include "systemupdatecheck/sysupdatermodel.h"
#include "diskcheck/diskcheckmodel.h"

#include <DConfig>
#include <DTrashManager>

#include <QObject>
#include <QDateTime>
#include <QStandardItem>

#define SEPERATOR ", "
#define MB_COUNT (1 << 20)
#define GB_COUNT (1 << 30)
// 垃圾清理总量大于1G时，额外扣分
const int kTrashSizeTooLargeAdjustPoint = 5;
// 检查项异常扣分规则
const QList<int> kIssuePointInfoList {
    5, // autostart
    5, // system update
    5, // trash
    15, // disk
    5}; // dev mode

DCORE_USE_NAMESPACE
class DBusInvokerInterface;
class SystemCheckModel : public QObject
{
    Q_OBJECT
public:
    explicit SystemCheckModel(QObject *parent = nullptr);
    ~SystemCheckModel();

    inline int getIssueCount() { return m_issueCount; }

    inline int getIssuePoint()
    {
        return 100 > m_issuePoint ? 100 - m_issuePoint : 0;
    }

    inline bool isTrashFile() { return m_isTrashFiles; }

    // 设置上次体检分数
    void setSafetyScore(int score);
    // 获取上次体检分数
    int getSafetyScore();
    // 设置上次体检时间
    void setLastCheckTime(QDateTime dateTime);
    // 获取上次体检时间
    QDateTime getLastCheckTime();
    // 获取是否忽略开发者模式
    bool ignoreDevModeChecking() const;
    // 设置是否忽略检测开发者模式
    void setIgnoreDevModeChecking(bool ignore);
    // 获取是否忽略自启动项
    bool ignoreAutoStartChecking() const;
    // 设置是否忽略检测自启动项
    void setIgnoreAutoStartChecking(bool ignore);

    // 是否处于开发者模式
    bool IsDeveloperMode();

    void startExam();
    void startAutoStartExam();
    void startSysVersionExam();
    void startTrashExam();
    void startDiskExam();
    void startDevModeExam();
    void onCheckCanceled();
    void onCheckDone();
    void resetResultItems();
    void requestFixItem(SysCheckItemID);
    void setItemIgnore(SysCheckItemID, bool);

    const QStandardItemModel &getResultModel();

private:
    // 初始化数据
    void initData();
    void updateIssuePoint();
    double getTrashCheckCount();

    void fixAutoStart();
    void fixSysUpdate();
    void fixTrash();
    void fixDisk();

public Q_SLOTS:
    // 检查系统更新完成
    void checkSysUpdateFinished(bool canUpdate);

Q_SIGNALS:
    void stageChanged(SysCheckItemID);
    void checkFinished();

    void sendFixItemFinished(SysCheckItemID id);

private:
    DConfig *m_dconfig;
    AutoStartCheckModel *m_autoStartCheckModel;
    SysUpdaterModel *m_sysUpdateModel;
    DiskCheckModel *m_diskCheckModel;

    DBusInvokerInterface *m_helperInvokerInter; // 同步助手dbus服务，用于查询是否处于开发者模式
    DBusInvokerInterface *m_controlCenterInvokerInter; // 控制中心dbus服务
    DBusInvokerInterface *m_sysetemDaemonInvokerInter;

    int m_processingIndex;
    int m_issuePoint; // 检查项累积扣分
    int m_issueCount;
    int m_virusCount;
    bool m_isAVOutDate;
    int m_autoStartCount;
    bool m_isDevMode;
    bool m_isDiskNormal;
    bool m_isSysOutDate;
    double m_totalTrashSize;
    bool m_isTrashFiles;
    QString m_logProblemItemsInfo;
    QList<SysCheckItemID> m_fixingItems;
    QList<QStandardItem *> m_resultItems;
    QStandardItemModel m_resultModel;
};
