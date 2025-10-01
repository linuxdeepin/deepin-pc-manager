// Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "systemcheckwidget.h"
#include "systemcheckmodel.h"

#include <QHBoxLayout>

#define FIXED_COL(column, weight) \
    { \
        mainLayout->setColumnStretch(column, weight); \
        mainLayout->setColumnMinimumWidth(column, weight); \
    }

#define FIXED_ROW(row, height) \
    { \
        mainLayout->setRowStretch(row, height); \
        mainLayout->setRowMinimumHeight(row, height); \
    }

#define RICH_TEXT_COLOR_FORMAT "<font color=%1>%2</font>"
const QString k90_100Color = "#00C134";
const QString k75_90Color = "#8CD400";
const QString k60_75Color = "#FF5C00";
const QString k0_60Color = "#D80000";
const QString kOutOfDate = "#FF5C00";

SystemCheckWidget::SystemCheckWidget(SystemCheckModel *model, DWidget *parent)
    : DWidget(parent)
    , m_model(model)
    , m_systemCheckHomePageWidget(nullptr)
    , m_systemCheckProgressWidget(nullptr)
    , m_systemCheckResultWidget(nullptr)
{
    // 初始化UI
    initUI();

    // 正在检测定时器
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SystemCheckWidget::updateCheckingUI);
    m_timer->start(1000);
}

SystemCheckWidget::~SystemCheckWidget()
{
}

// 初始化界面
void SystemCheckWidget::initUI()
{
    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 加载系统体检首页
    mainLayout->addWidget(getSystemCheckHomePage());
    // 加载系统体检检测界面
    mainLayout->addWidget(getSystemCheckProgressPage());
    // 加载系统体检结果页
    mainLayout->addWidget(getSystemCheckResultPage());

    // 初始化
    m_systemCheckHomePageWidget->setVisible(true);
    m_systemCheckProgressWidget->setVisible(false);
    m_systemCheckResultWidget->setVisible(false);

    // 更新界面
    updateUI();

    // 连接信号
    connect(m_examBtn, &DSuggestButton::clicked, this, &SystemCheckWidget::showSystemCheckProgressPage);
    connect(m_cancelButton, &DPushButton::clicked, this, &SystemCheckWidget::showSystemCheckHomePage);
    connect(m_doneButton, &QPushButton::clicked, this, &SystemCheckWidget::requestCheckDone);
    connect(m_fixAllButton, &QPushButton::clicked, this, &SystemCheckWidget::requestFixAll);
    connect(m_recheckButton, &QPushButton::clicked, this, &SystemCheckWidget::requestCheckAgain);

    connect(m_model, &SystemCheckModel::checkFinished, this, &SystemCheckWidget::showSystemCheckResultPage);
    connect(m_model, &SystemCheckModel::stageChanged, this, &SystemCheckWidget::onCheckMissionStarted);
    connect(m_model, &SystemCheckModel::sendFixItemFinished, this, &SystemCheckWidget::onFixItemFinished);
}

// 系统体检首页界面
QWidget *SystemCheckWidget::getSystemCheckHomePage()
{
    m_systemCheckHomePageWidget = new QWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(m_systemCheckHomePageWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setAlignment(Qt::AlignHCenter);
    m_systemCheckHomePageWidget->setLayout(mainLayout);

    // 首页展现的图片
    m_scorePic = new QLabel(this);
    // 水平居中对齐
    m_scorePic->setAlignment(Qt::AlignCenter);
    m_scorePic->setFixedHeight(320);
    m_scorePic->setPixmap(QIcon::fromTheme(SYSTEM_CHECK_HOME_PAGE_LOGO).pixmap(320, 320));
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_scorePic, 0, Qt::AlignHCenter);

    // 分数提示项
    m_lbScoreTip = new DLabel(this);
    m_lbScoreTip->setFixedHeight(24);
    m_lbScoreTip->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    m_lbScoreTip->setElideMode(Qt::ElideMiddle);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_lbScoreTip, 0, Qt::AlignHCenter);

    // 欢迎语
    m_greetingInfo = new DLabel(this);
    m_greetingInfo->setFixedHeight(44);
    m_greetingInfo->setText("########");
    m_greetingInfo->setAlignment(Qt::AlignHCenter);
    m_greetingInfo->setElideMode(Qt::ElideMiddle);
    // 设置分数提示字体样式
    QFont ft = m_greetingInfo->font();
    ft.setBold(true);
    m_greetingInfo->setFont(ft);
    mainLayout->addWidget(m_greetingInfo, 0, Qt::AlignHCenter);

    // 体检提示标签
    m_examTip = new DTipLabel("", this);
    m_examTip->setFixedHeight(20);
    m_examTip->setAlignment(Qt::AlignBottom);
    m_examTip->setElideMode(Qt::ElideMiddle);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_examTip, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(25);

    // 体检按钮
    m_examBtn = new DSuggestButton(this);
    m_examBtn->setFixedSize(170, 36);
    mainLayout->addWidget(m_examBtn, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(60);

    return m_systemCheckHomePageWidget;
}

// 系统体检检查界面
QWidget *SystemCheckWidget::getSystemCheckProgressPage()
{
    m_systemCheckProgressWidget = new QWidget(this);
    m_systemCheckProgressWidget->setContentsMargins(0, 10, 0, 10);

    m_movieLable = new DLabel(this);
    m_movieLable->setFixedSize(340, 340);
    m_movieLable->setAlignment(Qt::AlignCenter);
    m_movieLable->setPixmap(QIcon::fromTheme(SYSTEM_CHECK_HOME_PAGE_LOGO).pixmap(320, 320));

    m_stageLable = new DLabel(this);
    m_infoLable = new DLabel(this);

    m_progressBar = new DProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(50);
    m_progressBar->setFixedSize(573, 12);
    m_progressBar->setTextVisible(false);
    m_cancelButton = new DPushButton(this);
    m_cancelButton->setFixedSize(22, 22);
    m_cancelButton->setFlat(true);
    m_cancelButton->setIcon(QIcon::fromTheme(SYSTEM_CHECK_CANCEL_LOGO));
    m_cancelButton->setIconSize(QSize(22, 22));

    m_timeLable = new DTipLabel("", this);
    m_timeLable->setText(QTime(0, 0, 0, 0).toString("hh:mm:ss"));

    QGridLayout *mainLayout = new QGridLayout(m_systemCheckProgressWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    m_systemCheckProgressWidget->setLayout(mainLayout);

    FIXED_COL(0, 87);
    FIXED_COL(1, 573);
    FIXED_COL(2, 0);
    FIXED_COL(3, 78);

    FIXED_ROW(0, 340); // logo
    mainLayout->addWidget(m_movieLable, 0, 0, 1, -1, Qt::AlignBottom | Qt::AlignHCenter);
    FIXED_ROW(1, 24); // stage
    mainLayout->addWidget(m_stageLable, 1, 0, 1, -1, Qt::AlignHCenter | Qt::AlignBottom);
    FIXED_ROW(2, 54); // info
    mainLayout->addWidget(m_infoLable, 2, 0, 1, -1, Qt::AlignHCenter | Qt::AlignBottom);
    FIXED_ROW(3, 20); // space
    FIXED_ROW(4, 22); // progressbar
    mainLayout->addWidget(m_progressBar, 4, 1, 1, 1, Qt::AlignVCenter);
    mainLayout->addWidget(m_cancelButton, 4, 3, 1, 1, Qt::AlignVCenter | Qt::AlignLeft);
    FIXED_ROW(5, 27); // time
    mainLayout->addWidget(m_timeLable, 5, 0, 1, -1, Qt::AlignCenter);
    FIXED_ROW(6, 78); // space

    return m_systemCheckProgressWidget;
}

// 系统体检结果界面
QWidget *SystemCheckWidget::getSystemCheckResultPage()
{
    m_systemCheckResultWidget = new QWidget(this);
    m_systemCheckResultWidget->setContentsMargins(0, 10, 0, 10);

    QGridLayout *mainLayout = new QGridLayout(m_systemCheckResultWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    m_systemCheckResultWidget->setLayout(mainLayout);

    m_header = new SysCheckResultHeaderWidget(this);
    m_fixAllButton = new DSuggestButton(this);
    m_fixAllButton->setFocusPolicy(Qt::NoFocus);
    m_fixAllButton->setFixedWidth(150);
    m_fixAllButton->setText(tr("Fix All"));

    m_doneButton = new DSuggestButton(this);
    m_doneButton->setFocusPolicy(Qt::NoFocus);
    m_doneButton->setFixedWidth(150);
    m_doneButton->setText(tr("Done", "button"));

    m_recheckButton = new DPushButton(this);
    m_recheckButton->setFocusPolicy(Qt::NoFocus);
    m_recheckButton->setFixedWidth(150);
    m_recheckButton->setText(tr("Check Again"));

    createItemWidgets();
    QVBoxLayout *boxLayout = new QVBoxLayout;
    boxLayout->setSpacing(4);

    // 调整检查项的显示顺序
    boxLayout->addWidget(m_itemWidgets[SysCheckItemID::AutoStartApp], 0, Qt::AlignTop | Qt::AlignLeft);
    boxLayout->addWidget(m_itemWidgets[SysCheckItemID::SystemUpdate], 0, Qt::AlignTop | Qt::AlignLeft);
    boxLayout->addWidget(m_itemWidgets[SysCheckItemID::Trash], 0, Qt::AlignTop | Qt::AlignLeft);
    boxLayout->addWidget(m_itemWidgets[SysCheckItemID::Disk], 0, Qt::AlignTop | Qt::AlignLeft);
    boxLayout->addWidget(m_itemWidgets[SysCheckItemID::DevMode], 0, Qt::AlignTop | Qt::AlignLeft);

    boxLayout->addStretch(10);

    FIXED_ROW(0, 146);
    FIXED_ROW(1, 348);
    FIXED_ROW(2, 56);

    FIXED_COL(0, 385);
    FIXED_COL(1, 385);

    mainLayout->addWidget(m_header, 0, 0, 1, -1, Qt::AlignCenter);
    mainLayout->addLayout(boxLayout, 1, 0, 1, -1, Qt::AlignCenter);
    mainLayout->addWidget(m_recheckButton, 2, 0, Qt::AlignRight | Qt::AlignVCenter);
    mainLayout->addWidget(m_fixAllButton, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(m_doneButton, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);

    return m_systemCheckResultWidget;
}

// 更新界面
void SystemCheckWidget::updateUI()
{
    // 根据体检分数，改变显示
    int score = m_model->getSafetyScore();
    // 若分数为“-1”，则表示没有体检过
    if (-1 == score) {
        // 分数提示项
        QString coloredTip = tr("Your computer has not been checked yet");
        coloredTip = QString(RICH_TEXT_COLOR_FORMAT).arg(kOutOfDate).arg(coloredTip);
        m_lbScoreTip->setText(coloredTip);
        m_examTip->setText(tr("Try a full check."));
        m_examBtn->setText(tr("Check Now"));
        m_greetingInfo->setText(tr("Your computer has not been checked yet"));
        return;
    }
    // 非常安全分数阶段;
    else if (PERFECTLY_SAFE_SCORE_ABOVE <= score) {
        // 分数提示项
        m_lbScoreTip->setText(tr("Last Result: <font color=%1>%2</font> points")
                                  .arg(k90_100Color)
                                  .arg(score));
        m_examBtn->setText(tr("Check Now"));
        m_greetingInfo->setText(tr("Your computer is healthy"));
        m_examTip->setText(tr("Great! Keep staying healthy please."));
    }
    // 安全分数阶段
    else if (SAFE_SCORE_ABOVE <= score) {
        // 分数提示项
        m_lbScoreTip->setText(tr("Last Result: <font color=%1>%2</font> points")
                                  .arg(k75_90Color)
                                  .arg(score));
        m_examTip->setText(tr("Start a check and perform some improvements."));
        m_examBtn->setText(tr("Check Now"));
        m_greetingInfo->setText(tr("Your computer is in good condition"));
    }
    // 危险分数阶段
    else if (DANGEROUS_SCORE_ABOVE <= score) {
        // 分数提示项
        m_lbScoreTip->setText(tr("Last Result: <font color=%1>%2</font> points")
                                  .arg(k60_75Color)
                                  .arg(score));
        m_examTip->setText(tr("Please check your system now."));
        m_examBtn->setText(tr("Check Now"));

        QString coloredGreeting = tr("Your computer is ") + QString(RICH_TEXT_COLOR_FORMAT).arg(k60_75Color).arg(tr("at risk"));
        m_greetingInfo->setText(coloredGreeting);
    }
    // 非常危险分数阶段
    else {
        // 分数提示项
        m_lbScoreTip->setText(tr("Last Result: <font color=%1>%2</font> points")
                                  .arg(k0_60Color)
                                  .arg(score));
        m_examTip->setText(tr("It is strongly recommended to check and fix your system now."));
        m_examBtn->setText(tr("Check Now"));

        QString coloredGreeting = tr("Your computer is ") + QString(RICH_TEXT_COLOR_FORMAT).arg(k0_60Color).arg(tr("at high risk"));
        m_greetingInfo->setText(coloredGreeting);
    }

    //根据上次体检时间，调整体检提示标签文字
    QDateTime lastCheckTime = m_model->getLastCheckTime();
    qint64 days = lastCheckTime.daysTo(QDateTime::currentDateTime());
    if (days > 5) {
        QString coloredTip = tr("Checked %1 days ago").arg(int(days));
        coloredTip = QString(RICH_TEXT_COLOR_FORMAT).arg(kOutOfDate).arg(coloredTip);
        m_lbScoreTip->setText(coloredTip);

        m_examTip->setText(tr("Have a new check right now."));
        m_greetingInfo->setText(tr("Your computer has not been checked for a long time"));
    }
}

// 显示系统体检检测界面
void SystemCheckWidget::showSystemCheckProgressPage()
{
    m_systemCheckHomePageWidget->setVisible(false);
    m_systemCheckProgressWidget->setVisible(true);
    m_systemCheckResultWidget->setVisible(false);

    m_startTime = QTime::currentTime();
    m_progressBar->setValue(0);
    m_stageLable->setText("");
    m_timeLable->setText(QTime(0, 0, 0, 0).toString("hh:mm:ss"));
    m_infoLable->setText(tr("Checking"));

    m_timer->start();
    m_progressValue = 0;

    // 开始体检
    m_model->startExam();
}

// 显示系统体检首页界面
void SystemCheckWidget::showSystemCheckHomePage()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }
    m_systemCheckHomePageWidget->setVisible(true);
    m_systemCheckProgressWidget->setVisible(false);
    m_systemCheckResultWidget->setVisible(false);
}

void SystemCheckWidget::showSystemCheckResultPage()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }
    m_systemCheckHomePageWidget->setVisible(false);
    m_systemCheckProgressWidget->setVisible(false);
    m_systemCheckResultWidget->setVisible(true);

    setResultModel(m_model->getResultModel());
    setHeaderContent(m_model->getIssuePoint(), m_model->getIssueCount());
    if (m_model->isTrashFile()) {
        m_fixAllButton->show();
        m_doneButton->hide();
    } else {
        m_fixAllButton->hide();
        m_doneButton->show();
    }
}

// 定时器更新检测界面
void SystemCheckWidget::updateCheckingUI()
{
    QTime now = QTime::currentTime();
    QTime info(0, 0, 0, 0);
    info = info.addSecs(m_startTime.secsTo(now));
    m_timeLable->setText(info.toString("hh:mm:ss"));

    QString infoText = m_infoLable->text();
    infoText.append(".");
    infoText.remove("....");
    m_infoLable->setText(infoText);
}
// 根据检测项更新检测过程
void SystemCheckWidget::onCheckMissionStarted(SysCheckItemID id)
{
    if (SysCheckItemID::AutoStartApp == id) {
        m_stageLable->setText(tr("Checking startup programs"));
    } else if (SysCheckItemID::SystemUpdate == id) {
        m_stageLable->setText(tr("Checking if your system is up to date"));
    } else if (SysCheckItemID::Trash == id) {
        m_stageLable->setText(tr("Checking system junk"));
    } else if (SysCheckItemID::Disk == id) {
        m_stageLable->setText(tr("Checking disks"));
    } else if (SysCheckItemID::DevMode == id) {
        m_stageLable->setText(tr("Checking if the developer mode is enabled"));
    } else if (SysCheckItemID::MaxLimit == id) {
        m_stageLable->setText(tr("Checking startup programs"));
    }
    m_progressBar->setValue(m_progressValue);
    m_progressValue += 100 / (int(SysCheckItemID::MaxLimit) - 2);
}

void SystemCheckWidget::createItemWidgets()
{
    QStringList accNames = {"antiVirusCheckItem", "avDatabaseCheckItem", "sshCheckItem", "diskCheckItem",
                            "trashCheckItem", "sysUpdateCheckItem", "devModeCheckItem", "autoStartCheckItem"};
    m_itemWidgets.clear();
    for (int id = SysCheckItemID::AutoStartApp; id <= SysCheckItemID::DevMode; id++) {
        SysCheckResultItemWidget *item = new SysCheckResultItemWidget(SysCheckItemID(id), this);

        item->setAccessibleName(accNames[id]);

        connect(item, &SysCheckResultItemWidget::requestFixItem, this, &SystemCheckWidget::requestFixItem);
        connect(item, &SysCheckResultItemWidget::requestSetIgnore, this, &SystemCheckWidget::requestSetIgnore);
        connect(this, &SystemCheckWidget::onFixItemFinished, item, &SysCheckResultItemWidget::onNotifyFixFinished);
        //        connect(item, &SysCheckResultItemWidget::widgetHidden, this, &SysCheckResultWidget::resetItemsBackGround);

        //        connect(this, &SysCheckResultWidget::onFixItemStarted, item, &SysCheckResultItemWidget::onNotifyFixStarted);
        //        connect(this, &SysCheckResultWidget::autoStartAppCountChanged, item, &SysCheckResultItemWidget::onAutoStartAppCountChanged);

        m_itemWidgets.push_back(item);
    }

    resetItemsBackGround();
}

void SystemCheckWidget::setResultModel(const QStandardItemModel &result)
{
    // 对各项设置结果
    for (auto wid : m_itemWidgets) {
        wid->setResult(result);
    }
    resetItemsBackGround();
}

void SystemCheckWidget::setHeaderContent(int point, int issueCount)
{
    m_header->onPointChanged(point, issueCount);
}

void SystemCheckWidget::resetItemsBackGround()
{
    bool isNeedColored = false;
    for (int index = 0; index < m_itemWidgets.size(); index++) {
        auto wid = m_itemWidgets[index];
        if (!wid->isHidden()) {
            DPalette::ColorType role = isNeedColored ? DPalette::ItemBackground : DPalette::NoType;
            wid->setBackgroundRole(role);
            isNeedColored = !isNeedColored;
        }
    }
}

void SystemCheckWidget::requestFixItem(SysCheckItemID id)
{
    m_model->requestFixItem(id);
    if (SysCheckItemID::Trash == id) {
        if (m_model->isTrashFile()) {
            m_fixAllButton->show();
            m_doneButton->hide();
        } else {
            m_fixAllButton->hide();
            m_doneButton->show();
        }
    }
}

void SystemCheckWidget::requestSetIgnore(SysCheckItemID id, bool status)
{
    m_model->setItemIgnore(id, status);
}

void SystemCheckWidget::requestFixAll()
{
    requestFixItem(SysCheckItemID::Trash);
}
void SystemCheckWidget::requestCheckDone()
{
    m_systemCheckHomePageWidget->setVisible(true);
    m_systemCheckProgressWidget->setVisible(false);
    m_systemCheckResultWidget->setVisible(false);

    updateUI();
}

void SystemCheckWidget::requestCheckAgain()
{
    showSystemCheckProgressPage();
}
