// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "systemcheckmodel.h"
#include "window/common/gsettingkey.h"
#include "window/common/invokers/invokerfactory.h"

#include <QVariant>

SystemCheckModel::SystemCheckModel(QObject *parent)
    : QObject(parent)
    , m_dconfig(nullptr)
{
    initData();
}

SystemCheckModel::~SystemCheckModel()
{
}

void SystemCheckModel::initData()
{
    m_dconfig = DConfig::create("org.deepin.dde.deepin-pc-manager", "org.deepin.dde.deepin-pc-manager");
    m_autoStartCheckModel = new AutoStartCheckModel(this);
    m_sysUpdateModel = new SysUpdaterModel(this);
    m_diskCheckModel = new DiskCheckModel(this);

    // 检查系统更新完成
    connect(m_sysUpdateModel, &SysUpdaterModel::sendHaveUpdates, this, &SystemCheckModel::checkSysUpdateFinished);

    // 同步助手dbus服务，用于查询是否处于开发者模式
    m_helperInvokerInter = InvokerFactory::GetInstance().CreateInvoker("com.deepin.sync.Helper",
                                                                       "/com/deepin/sync/Helper",
                                                                       "com.deepin.sync.Helper",
                                                                       ConnectType::SYSTEM,
                                                                       this);
    // 控制中心dbus服务对象
    m_controlCenterInvokerInter =
        InvokerFactory::GetInstance().CreateInvoker("com.deepin.dde.ControlCenter",
                                                    "/com/deepin/dde/ControlCenter",
                                                    "com.deepin.dde.ControlCenter",
                                                    ConnectType::SESSION,
                                                    this);

    m_sysetemDaemonInvokerInter = InvokerFactory::GetInstance().CreateInvoker("com.deepin.pc.manager.system.daemon",
                                                                              "/com/deepin/pc/manager/system/daemon",
                                                                              "com.deepin.pc.manager.system.daemon",
                                                                              ConnectType::SYSTEM,
                                                                              this);
}

// 设置上次体检分数
void SystemCheckModel::setSafetyScore(int score)
{
    if (m_dconfig->isValid()) {
        m_dconfig->setValue(SAFETY_SCORE, score);
    }
}
// 获取上次体检分数
int SystemCheckModel::getSafetyScore()
{
    int score = -1;
    if (m_dconfig->isValid()) {
        score = m_dconfig->value(SAFETY_SCORE).toInt();
    }
    return score;
}

// 设置上次体检时间
void SystemCheckModel::setLastCheckTime(QDateTime dateTime)
{
    if (m_dconfig->isValid()) {
        m_dconfig->setValue(LAST_CHECK_TIME, dateTime.toString("yyyy-MM-dd-hh-mm-ss"));
    }
}
// 获取上次体检时间
QDateTime SystemCheckModel::getLastCheckTime()
{
    QString timeStr;
    if (m_dconfig->isValid()) {
        timeStr = m_dconfig->value(LAST_CHECK_TIME).toString();
    }
    return QDateTime::fromString(timeStr, "yyyy-MM-dd-hh-mm-ss");
}

bool SystemCheckModel::ignoreDevModeChecking() const
{
    bool status = false;
    if (m_dconfig->isValid()) {
        status = m_dconfig->value(IGNORE_DEV_MODE_CHECKING).toBool();
    }
    return status;
}

void SystemCheckModel::setIgnoreDevModeChecking(bool ignore)
{
    if (m_dconfig->isValid()) {
        m_dconfig->setValue(IGNORE_DEV_MODE_CHECKING, ignore);
    }
}

bool SystemCheckModel::ignoreAutoStartChecking() const
{
    bool status = false;
    if (m_dconfig->isValid()) {
        status = m_dconfig->value(IGNORE_AUTO_START_CHECKING).toBool();
    }
    return status;
}

void SystemCheckModel::setIgnoreAutoStartChecking(bool ignore)
{
    if (m_dconfig->isValid()) {
        m_dconfig->setValue(IGNORE_AUTO_START_CHECKING, ignore);
    }
}

// 开始体检
void SystemCheckModel::startExam()
{
    m_issuePoint = 0;
    m_issueCount = 0;
    m_virusCount = 0;
    m_isAVOutDate = false;
    m_autoStartCount = 0;
    m_isDevMode = false;
    m_isDiskNormal = true;
    m_isSysOutDate = false;
    m_totalTrashSize = 0.0;

    m_logProblemItemsInfo = "";
    m_fixingItems.clear();

    resetResultItems();
    startAutoStartExam();
}
// 开始自启动检测
void SystemCheckModel::startAutoStartExam()
{
    m_processingIndex = SysCheckItemID::AutoStartApp;
    Q_EMIT stageChanged(SysCheckItemID::AutoStartApp);

    m_autoStartCount = m_autoStartCheckModel->getAutoStartAppCount();
    QStandardItem *autoStartItem = m_resultItems[SysCheckItemID::AutoStartApp];
    autoStartItem->setData(SysCheckItemID::AutoStartApp, CheckResultModelRole::ID);
    autoStartItem->setData(CheckProgressStatus::Success,
                           CheckResultModelRole::CheckProgressFlag);
    autoStartItem->setData(m_autoStartCount > 0, CheckResultModelRole::IsIssueHappen);
    autoStartItem->setData(ignoreAutoStartChecking(), CheckResultModelRole::IsIgnored);
    autoStartItem->setData(m_autoStartCount, CheckResultModelRole::Details);

    startSysVersionExam();
}
// 开始系统版本检测
void SystemCheckModel::startSysVersionExam()
{
    if (m_processingIndex != SysCheckItemID::AutoStartApp) {
        return;
    }

    m_processingIndex = SysCheckItemID::SystemUpdate;
    Q_EMIT stageChanged(SysCheckItemID::SystemUpdate);
    m_sysUpdateModel->checkForUpdates();
}

// 检查系统更新完成
void SystemCheckModel::checkSysUpdateFinished(bool canUpdate)
{
    m_isSysOutDate = canUpdate;
    QStandardItem *sysVersionItem = m_resultItems[SysCheckItemID::SystemUpdate];
    sysVersionItem->setData(SysCheckItemID::SystemUpdate, CheckResultModelRole::ID);
    sysVersionItem->setData(CheckProgressStatus::Success,
                            CheckResultModelRole::CheckProgressFlag);
    sysVersionItem->setData(m_isSysOutDate, CheckResultModelRole::IsIssueHappen);
    sysVersionItem->setData(m_isSysOutDate, CheckResultModelRole::Details);

    startTrashExam();
}

// 开始垃圾清理检测
void SystemCheckModel::startTrashExam()
{
    if (m_processingIndex != SysCheckItemID::SystemUpdate) {
        return;
    }
    m_processingIndex = SysCheckItemID::Trash;
    Q_EMIT stageChanged(SysCheckItemID::Trash);

    m_totalTrashSize = getTrashCheckCount();
    m_isTrashFiles = m_totalTrashSize >= (100 * MB_COUNT);
    QStandardItem *trashItem = m_resultItems[SysCheckItemID::Trash];
    trashItem->setData(SysCheckItemID::Trash, CheckResultModelRole::ID);
    trashItem->setData(CheckProgressStatus::Success, CheckResultModelRole::CheckProgressFlag);
    trashItem->setData(m_totalTrashSize >= (100 * MB_COUNT),
                       CheckResultModelRole::IsIssueHappen);
    trashItem->setData(m_totalTrashSize, CheckResultModelRole::Details);

    startDiskExam();
}
// 开始磁盘检测
void SystemCheckModel::startDiskExam()
{
    if (m_processingIndex != SysCheckItemID::Trash) {
        return;
    }

    m_processingIndex = SysCheckItemID::Disk;
    Q_EMIT stageChanged(SysCheckItemID::Disk);

    QStandardItem *diskItem = m_resultItems[SysCheckItemID::Disk];
    diskItem->setData(SysCheckItemID::Disk, CheckResultModelRole::ID);
    if (m_diskCheckModel->isDiskManagerInterValid()) {
        m_isDiskNormal = m_diskCheckModel->getDiskStatus();
        diskItem->setData(CheckProgressStatus::Success, CheckResultModelRole::CheckProgressFlag);
        diskItem->setData(m_isDiskNormal != true, CheckResultModelRole::IsIssueHappen);
        diskItem->setData(m_isDiskNormal, CheckResultModelRole::Details);
        diskItem->setData(false, CheckResultModelRole::IsIgnored);
    } else {
        diskItem->setData(CheckProgressStatus::Masked, CheckResultModelRole::CheckProgressFlag);
    }

    startDevModeExam();
}
// 开始开发者模式检测
void SystemCheckModel::startDevModeExam()
{
    if (m_processingIndex != SysCheckItemID::Disk) {
        return;
    }

    m_processingIndex = SysCheckItemID::DevMode;
    Q_EMIT stageChanged(SysCheckItemID::DevMode);

    m_isDevMode = IsDeveloperMode();

    QStandardItem *devItem = m_resultItems[SysCheckItemID::DevMode];
    devItem->setData(SysCheckItemID::DevMode, CheckResultModelRole::ID);
    devItem->setData(CheckProgressStatus::Success, CheckResultModelRole::CheckProgressFlag);
    devItem->setData(m_isDevMode, CheckResultModelRole::IsIssueHappen);
    devItem->setData(m_isDevMode, CheckResultModelRole::Details);
    devItem->setData(ignoreDevModeChecking(), CheckResultModelRole::IsIgnored);
    m_processingIndex = SysCheckItemID::DevMode;

    onCheckDone();
}
// 取消体检
void SystemCheckModel::onCheckCanceled()
{
    m_processingIndex = SysCheckItemID::MaxLimit;
}
void SystemCheckModel::onCheckDone()
{
    // 统计分数
    updateIssuePoint();
    // 记录检查完成时间
    setLastCheckTime(QDateTime::currentDateTime());
    Q_EMIT checkFinished();
}

void SystemCheckModel::resetResultItems()
{
    qDeleteAll(m_resultItems);
    m_resultItems.clear();
    for (int i = SysCheckItemID::AutoStartApp; i <= SysCheckItemID::DevMode; i++) {
        m_resultItems.push_back(new QStandardItem);
    }
}

const QStandardItemModel &SystemCheckModel::getResultModel()
{
    m_resultModel.clear();
    m_resultModel.appendRow(m_resultItems);
    return m_resultModel;
}

// 更新检查结果
void SystemCheckModel::updateIssuePoint()
{
    m_issuePoint = 0;
    m_issueCount = 0;

    // autostart

    if (!ignoreAutoStartChecking()) {
        m_issuePoint += m_autoStartCount ? kIssuePointInfoList[SysCheckItemID::AutoStartApp] : 0;
        m_issueCount += m_autoStartCount ? 1 : 0;
        if (m_autoStartCount) {
            if (!m_logProblemItemsInfo.isEmpty()) {
                m_logProblemItemsInfo.append(SEPERATOR);
            }
            m_logProblemItemsInfo.append(tr("Startup programs"));
        }
    }

    // sys update
    m_issuePoint += m_isSysOutDate ? kIssuePointInfoList[SysCheckItemID::SystemUpdate] : 0;
    m_issueCount += m_isSysOutDate ? 1 : 0;
    if (m_isSysOutDate) {
        if (!m_logProblemItemsInfo.isEmpty()) {
            m_logProblemItemsInfo.append(SEPERATOR);
        }
        m_logProblemItemsInfo.append(tr("System version"));
    }

    // trash
    m_issuePoint +=
        m_totalTrashSize >= (100 * MB_COUNT) ? kIssuePointInfoList[SysCheckItemID::Trash] : 0;
    m_issueCount += m_totalTrashSize >= (100 * MB_COUNT) ? 1 : 0;
    // trash总量大于1G时，有修正值
    m_issuePoint += m_totalTrashSize > (1 * GB_COUNT) ? kTrashSizeTooLargeAdjustPoint : 0;
    if (m_totalTrashSize >= (100 * MB_COUNT)) {
        if (!m_logProblemItemsInfo.isEmpty()) {
            m_logProblemItemsInfo.append(SEPERATOR);
        }
        m_logProblemItemsInfo.append(tr("System cleanup"));
    }

    // disk
    m_issuePoint += !m_isDiskNormal ? kIssuePointInfoList[SysCheckItemID::Disk] : 0;
    m_issueCount += !m_isDiskNormal ? 1 : 0;
    if (!m_isDiskNormal) {
        if (!m_logProblemItemsInfo.isEmpty()) {
            m_logProblemItemsInfo.append(SEPERATOR);
        }
        m_logProblemItemsInfo.append(tr("Disk checking"));
    }

    // devmod
    if (!ignoreDevModeChecking()) {
        m_issuePoint += m_isDevMode ? kIssuePointInfoList[SysCheckItemID::DevMode] : 0;
        m_issueCount += m_isDevMode ? 1 : 0;
        if (m_isDevMode) {
            if (!m_logProblemItemsInfo.isEmpty()) {
                m_logProblemItemsInfo.append(SEPERATOR);
            }
            m_logProblemItemsInfo.append(tr("Developer mode"));
        }
    }

    setSafetyScore(100 > m_issuePoint ? 100 - m_issuePoint : 0);
}

// 是否处于开发者模式
bool SystemCheckModel::IsDeveloperMode()
{
    QDBusMessage msg = DBUS_BLOCK_INVOKE(m_helperInvokerInter, "IsDeveloperMode");
    GET_MESSAGE_VALUE(bool, rst, msg);
    qInfo() << "[HomePageModel] [IsDeveloperMode] " << rst;

    return rst;
}

double SystemCheckModel::getTrashCheckCount()
{
    QDBusMessage msg;
    msg = DBUS_BLOCK_INVOKE(m_sysetemDaemonInvokerInter, "GetTrashFileCount", QDir::homePath());
    GET_MESSAGE_VALUE(double, trashFileCount, msg);
    msg = DBUS_BLOCK_INVOKE(m_sysetemDaemonInvokerInter, "GetCacheFileCount");
    GET_MESSAGE_VALUE(double, cacheFileCount, msg);
    msg = DBUS_BLOCK_INVOKE(m_sysetemDaemonInvokerInter, "GetLogFileCount");
    GET_MESSAGE_VALUE(double, logFileCount, msg);
    msg = DBUS_BLOCK_INVOKE(m_sysetemDaemonInvokerInter, "GetHistoryFileCount", QDir::homePath());
    GET_MESSAGE_VALUE(double, historyFileCount, msg);

    return trashFileCount + cacheFileCount + logFileCount + historyFileCount;
}

void SystemCheckModel::requestFixItem(SysCheckItemID itemId)
{
    switch (itemId) {
    case SysCheckItemID::AutoStartApp:
        fixAutoStart();
        break;
    case SysCheckItemID::SystemUpdate:
        fixSysUpdate();
        break;
    case SysCheckItemID::Trash:
        fixTrash();
        break;
    case SysCheckItemID::Disk:
        fixDisk();
        break;
    default:
        break;
    }
}

void SystemCheckModel::fixAutoStart()
{
    // 跳转到自启动界面
}

void SystemCheckModel::fixSysUpdate()
{
    DBUS_NOBLOCK_INVOKE(m_controlCenterInvokerInter, "Show");
    DBUS_NOBLOCK_INVOKE(m_controlCenterInvokerInter, "ShowPage", "update", "");
}

void SystemCheckModel::fixTrash()
{
    QStringList itemList;
    itemList.append(TRASH_CLEAN_ITEM_TYPE_CACHE);
    itemList.append(TRASH_CLEAN_ITEM_TYPE_LOG);
    itemList.append(TRASH_CLEAN_ITEM_TYPE_HISTORY);
    // 清理系统垃圾文件
    DTrashManager::instance()->cleanTrash();
    DBUS_NOBLOCK_INVOKE(m_sysetemDaemonInvokerInter, "CleanTrashFiles", itemList);

    m_totalTrashSize = 0.0;
    m_isTrashFiles = false;
    m_resultItems[SysCheckItemID::Trash]->setData(false, CheckResultModelRole::IsIssueHappen);
    m_resultItems[SysCheckItemID::Trash]->setData(m_totalTrashSize,
                                                  CheckResultModelRole::Details);
    m_fixingItems.removeAll(SysCheckItemID::Trash);
    Q_EMIT sendFixItemFinished(SysCheckItemID::Trash);
}

void SystemCheckModel::fixDisk()
{
    QProcess::startDetached("deepin-diskmanager", {});
}

void SystemCheckModel::setItemIgnore(SysCheckItemID id, bool status)
{
    if (SysCheckItemID::AutoStartApp == id) {
        setIgnoreAutoStartChecking(status);
    }
    if (SysCheckItemID::DevMode == id) {
        setIgnoreDevModeChecking(status);
    }
}
