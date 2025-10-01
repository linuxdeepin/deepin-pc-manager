// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "window/common/common.h"
#include "syscheckresultheaderwidget.h"
#include "syscheckresultitemwidget.h"

#include <DWidget>
#include <DFrame>
#include <DIconButton>
#include <DGuiApplicationHelper>
#include <DLabel>
#include <DTipLabel>
#include <DSuggestButton>
#include <DProgressBar>

#include <QObject>
#include <QTimer>
#include <QTime>

//// 安全级别界限分数
// 非常安全分数界限
#define PERFECTLY_SAFE_SCORE_ABOVE 90
// 比较安全分数界限
#define SAFE_SCORE_ABOVE 75
// 比较危险分数界限
#define DANGEROUS_SCORE_ABOVE 60

//// 分数安全级别文字颜色
// 非常安全分数颜色
#define PERFECTLY_SAFE_SCORE_COLOR QColor("#00C134")
// 非常安全分数 HTML格式颜色
#define PERFECTLY_SAFE_SCORE_COLOR_HTML "#00C134"

// 比较安全分数 颜色
#define SAFE_SCORE_COLOR QColor("#8CD400")
// 比较安全分数 HTML格式颜色
#define SAFE_SCORE_COLOR_HTML "#8CD400"

// 比较危险分数 颜色
#define DANGEROUS_SCORE_COLOR QColor("#FF5C00")
// 比较危险分数 HTML格式颜色
#define DANGEROUS_SCORE_COLOR_HTML "#FF5C00"

// 非常危险分数 颜色
#define EXTREMELY_DANGEROUS_SCORE_COLOR QColor("#D80000")
// 非常危险分数 HTML格式颜色
#define EXTREMELY_DANGEROUS_SCORE_COLOR_HTML "#D80000"

// 未体检时，要求进行体检 或 第一次体检
#define REQUEST_PROCESS_CHECK_COLOR QColor("#FF5C00")
// 要求体检时HTML格式颜色
#define REQUEST_PROCESS_CHECK_COLOR_HTML "#FF5C00"

//// 分数安全级别阴影颜色
// 非常安全阴影颜色
#define PERFECTLY_SAFE_SCORE_SHADOW_COLOR QColor(35, 196, 0, 127)
// 比较安全阴影颜色
#define SAFE_SCORE_SHADOW_COLOR QColor(158, 202, 0, 127)
// 比较危险阴影颜色
#define DANGEROUS_SCORE_SHADOW_COLOR QColor(255, 93, 0, 127)
// 非常危险阴影颜色
#define EXTREMELY_DANGEROUS_SCORE_SHADOW_COLOR QColor(216, 49, 108, 127)

// 评分规则
#define SCORE_ANTIVIRUS_ABNORMAL 30 // 病毒扫描结果异常扣除的分数
#define SCORE_VIRUS_LIB_VER_ABNORMAL 10 // 病毒库版本可更新扣除的分数
#define SCORE_AUTOSTART_ABNORMAL 5 // 自启动应用数异常扣除的分数
#define SCORE_SSH_ABNORMAL 5 // SSH异常扣除的分数
#define SCORE_SYS_VER_ABNORMAL 5 // 系统本可更新异常扣除的分数
#define SCORE_CLEANER_ABNORMAL_FIRST_STAGE 5 // 垃圾清理第一阶段异常扣除的分数
#define SCORE_CLEANER_ABNORMAL_SEC_STAGE 10 // 垃圾清理第二阶段异常扣除的分数
#define SCORE_DISCK_CHECK_ABNORMAL 15 // 磁盘检测异常扣除的分数
#define SCORE_ROOT_MODEL_ABNORMAL 5 // 处于开发者模式扣除的分数

class SystemCheckModel;
DWIDGET_USE_NAMESPACE
class SystemCheckWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SystemCheckWidget(SystemCheckModel *model, DWidget *parent = nullptr);
    ~SystemCheckWidget();

private:
    // 初始化界面
    void initUI();
    // 更新界面
    void updateUI();

    // 系统体检首页界面
    QWidget *getSystemCheckHomePage();
    // 系统体检检查界面
    QWidget *getSystemCheckProgressPage();
    // 系统体检结果界面
    QWidget *getSystemCheckResultPage();

    void createItemWidgets();
    void setResultModel(const QStandardItemModel &);
    void setHeaderContent(int point, int issueCount);
    void resetItemsBackGround();

public Q_SLOTS:
    void showSystemCheckProgressPage();
    void showSystemCheckHomePage();
    void showSystemCheckResultPage();

    void updateCheckingUI();
    void onCheckMissionStarted(SysCheckItemID);
    void requestFixItem(SysCheckItemID id);
    void requestSetIgnore(SysCheckItemID, bool);
    void requestFixAll();
    void requestCheckDone();
    void requestCheckAgain();

Q_SIGNALS:
    void onFixItemFinished(SysCheckItemID id);

private:
    SystemCheckModel *m_model;
    QTimer *m_timer;
    QTime m_startTime;
    int m_progressValue;

    QWidget *m_systemCheckHomePageWidget;
    QLabel *m_scorePic;
    DLabel *m_lbScoreTip;
    DLabel *m_greetingInfo;
    DTipLabel *m_examTip;
    DSuggestButton *m_examBtn;

    QWidget *m_systemCheckProgressWidget;
    DLabel *m_movieLable;
    DLabel *m_stageLable;
    DLabel *m_infoLable;
    DProgressBar *m_progressBar;
    DTipLabel *m_timeLable;
    DPushButton *m_cancelButton;

    QWidget *m_systemCheckResultWidget;
    SysCheckResultHeaderWidget *m_header;
    DSuggestButton *m_fixAllButton;
    DSuggestButton *m_doneButton;
    DPushButton *m_recheckButton;
    QList<SysCheckResultItemWidget *> m_itemWidgets;
    QList<SysCheckItemID> m_currentFixingItems;
};
