// Copyright (C) 2019 ~ 2022 UnionTech Software Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "syscheckresultitemwidget.h"

#include <QGridLayout>

DWIDGET_USE_NAMESPACE

#define FIXED_COL(column, weight) \
    { \
        mainLayout->setColumnStretch(column, weight); \
        mainLayout->setColumnMinimumWidth(column, weight); \
    }

#define RICH_TEXT_COLOR_FORMAT "<font color=%1>%2</font>"
const QString k90_100Color = "#00C134";
const QString k75_90Color = "#8CD400";
const QString k60_75Color = "#FF5C00";
const QString k0_60Color = "#D80000";
const QString kOutOfDate = "#FF5C00";

SysCheckResultItemWidget::SysCheckResultItemWidget(SysCheckItemID id, QWidget *parent)
    : DFrame(parent)
    , m_id(id)
    , m_typeIconLabel(nullptr)
    , m_typeNameLabel(nullptr)
    , m_statusIconLabel(nullptr)
    , m_statusInfoLabel(nullptr)
    , m_fixTipLabel(nullptr)
    , m_jumpLabel(nullptr)
    , m_fixSpinner(nullptr)
    , m_ignoreBtn(nullptr)
    , m_disIgnoreBtn(nullptr)
{
    this->setFixedSize(750, 40);
    this->setContentsMargins(0, 0, 0, 0);
    this->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    this->setMouseTracking(true);
    initUI();
    initConnection();
    hideAll();
    onNotifyComplete(m_id, true);
}

void SysCheckResultItemWidget::onNotifyComplete(SysCheckItemID index, bool isAbnormal)
{
    if (m_id == index) {
        hideAll();
        m_statusIconLabel->show();
        m_statusInfoLabel->show();

        if (isAbnormal) {
            m_jumpLabel->show();

            m_statusIconLabel->setPixmap(QIcon::fromTheme("dcc_syscheck_item_abnormal").pixmap(12, 12));
            if (SysCheckItemID::AutoStartApp == m_id) {
                m_jumpLabel->setText(tr("Check up"));
                m_statusInfoLabel->setText(getColoredText(65, tr("%1 apps")));
            } else if (SysCheckItemID::SystemUpdate == m_id) {
                m_jumpLabel->setText(tr("Check for updates"));
                m_statusInfoLabel->setText(getColoredText(65, tr("Out of date")));
            } else if (SysCheckItemID::Trash == m_id) {
                m_jumpLabel->setText(tr("Clean up"));
                m_statusInfoLabel->setText(getColoredText(65, tr("There are junk files")));
            } else if (SysCheckItemID::Disk == m_id) {
                m_jumpLabel->setText(tr("Go to check"));
                m_statusInfoLabel->setText(getColoredText(65, tr("Found errors")));
            } else if (SysCheckItemID::DevMode == m_id) {
                m_jumpLabel->setText("");
                m_statusInfoLabel->setText(getColoredText(65, tr("Root access allowed, which is risky")));
            } else if (SysCheckItemID::MaxLimit == m_id) {
                m_jumpLabel->setText(tr("Check up"));
                m_statusInfoLabel->setText(getColoredText(65, tr("%1 apps")));
            }
        } else {
            m_statusIconLabel->setPixmap(QIcon::fromTheme("dcc_syscheck_item_normal").pixmap(12, 12));
            if (SysCheckItemID::AutoStartApp == m_id) {
                m_statusInfoLabel->setText(getColoredText(100, tr("%1 apps")));
            } else if (SysCheckItemID::SystemUpdate == m_id) {
                m_statusInfoLabel->setText(getColoredText(100, tr("Up to date")));
            } else if (SysCheckItemID::Trash == m_id) {
                m_statusInfoLabel->setText(getColoredText(100, tr("Clean")));
            } else if (SysCheckItemID::Disk == m_id) {
                m_statusInfoLabel->setText(getColoredText(100, tr("No errors found")));
            } else if (SysCheckItemID::DevMode == m_id) {
                m_statusInfoLabel->setText(getColoredText(100, tr("No root access")));
            } else if (SysCheckItemID::MaxLimit == m_id) {
                m_statusInfoLabel->setText(getColoredText(100, tr("%1 apps")));
            }
        }
    }
}

void SysCheckResultItemWidget::onNotifyFixStarted(SysCheckItemID index)
{
    if (m_id == index) {
        hideAll();
        m_fixTipLabel->show();
        m_fixTipLabel->setText(tr("Fixing"));

        m_fixSpinner->show();
        m_fixSpinner->start();
    }
}

void SysCheckResultItemWidget::onNotifyFixFinished(SysCheckItemID index)
{
    if (m_id == index) {
        hideAll();
        m_statusIconLabel->show();
        m_statusIconLabel->setPixmap(QIcon::fromTheme("dcc_syscheck_item_normal").pixmap(12, 12));

        m_statusInfoLabel->show();
        if (SysCheckItemID::Disk == m_id) {
            m_statusInfoLabel->setText(getColoredText(100, tr("No errors found")));
        } else if (SysCheckItemID::Trash == m_id) {
            m_statusInfoLabel->setText(getColoredText(100, tr("Clean")));
        } else if (SysCheckItemID::SystemUpdate == m_id) {
            m_statusInfoLabel->setText(getColoredText(100, tr("Up to date")));
        } else if (SysCheckItemID::DevMode == m_id) {
            m_statusInfoLabel->setText(getColoredText(100, tr("No root access")));
        } else if (SysCheckItemID::AutoStartApp == m_id) {
            m_statusInfoLabel->setText(getColoredText(100, tr("%1 apps")));
        } else if (SysCheckItemID::MaxLimit == m_id) {
            m_statusInfoLabel->setText(getColoredText(100, tr("%1 apps")));
        }

        m_fixSpinner->stop();
        m_fixSpinner->hide();
    }
}

// 在这时处理忽略按钮
void SysCheckResultItemWidget::onNotifyIgnored(bool isIgnored)
{
    // 仅两个项目需要处理忽略状态
    if (SysCheckItemID::DevMode == m_id) {
        QString info = m_resultItem->data(CheckResultModelRole::IsIssueHappen).toBool()
                           ? tr("Root access allowed, which is risky")
                           : tr("No root access");
        if (isIgnored) {
            // 被忽略时不要隐藏，依照早期需求处理
            // this->hide();
            // 下次检查时被隐藏，不需要处理按钮
            m_ignoreBtn->hide();
            m_disIgnoreBtn->show();
            m_statusIconLabel->setPixmap(QIcon::fromTheme("dcc_syscheck_item_normal").pixmap(12, 12));
            m_statusInfoLabel->setText(getColoredText(100, info));
        } else {
            // 显示忽略按钮
            m_ignoreBtn->show();
            m_disIgnoreBtn->hide();
            if (m_resultItem->data(CheckResultModelRole::IsIssueHappen).toBool()) {
                m_statusIconLabel->setPixmap(QIcon::fromTheme("dcc_syscheck_item_abnormal").pixmap(12, 12));
                m_statusInfoLabel->setText(getColoredText(65, info));
            } else {
                m_statusIconLabel->setPixmap(QIcon::fromTheme("dcc_syscheck_item_normal").pixmap(12, 12));
                QString info = m_statusInfoLabel->text();
                m_statusInfoLabel->setText(getColoredText(100, info));
            }
        }
        return;
    }

    if (SysCheckItemID::AutoStartApp == m_id) {
        if (m_resultItem) {
            m_resultItem->setData(isIgnored, CheckResultModelRole::IsIgnored);
            int count = m_resultItem->data(CheckResultModelRole::Details).toInt();
            QString autoStartDetail = tr("%1 apps");
            autoStartDetail = autoStartDetail.arg(count);
            if (isIgnored) {
                m_ignoreBtn->hide();
                m_disIgnoreBtn->show();
                m_statusIconLabel->setPixmap(QIcon::fromTheme("dcc_syscheck_item_normal").pixmap(12, 12));
                m_statusInfoLabel->setText(getColoredText(100, autoStartDetail));
            } else {
                m_ignoreBtn->show();
                m_disIgnoreBtn->hide();
                m_statusIconLabel->setPixmap(QIcon::fromTheme("dcc_syscheck_item_abnormal").pixmap(12, 12));
                m_statusInfoLabel->setText(getColoredText(65, autoStartDetail));
            }
            return;
        }
    }
}

void SysCheckResultItemWidget::setResult(const QStandardItemModel &resultModel)
{
    QStandardItem *resultItem = nullptr;
    int index = resultModel.columnCount();
    for (int i = 0; i < index; i++) {
        resultItem = resultModel.item(0, i);
        int id = resultItem->data(CheckResultModelRole::ID).toInt();
        if (id == m_id) {
            break;
        }
        resultItem = nullptr;
    }

    // 未匹配到结果,隐藏检查项
    if (!resultItem) {
        this->hide();
        this->setDisabled(true);
        Q_EMIT widgetHidden();
        return;
    }

    setResult(resultItem);
}

void SysCheckResultItemWidget::onAutoStartAppCountChanged(int count)
{
    if (SysCheckItemID::AutoStartApp != m_id) {
        return;
    }

    m_resultItem->setData(count > 0, CheckResultModelRole::IsIssueHappen);
    m_resultItem->setData(count, CheckResultModelRole::Details);
    setResult(m_resultItem);
}

void SysCheckResultItemWidget::initUI()
{
    m_typeIconLabel = new DLabel(this);
    m_typeIconLabel->setFixedSize(16, 16);
    m_typeIconLabel->setPixmap(QIcon::fromTheme(kSystemCheckTypeIconList[m_id]).pixmap(16, 16));

    m_typeNameLabel = new DLabel(this);
    m_typeNameLabel->setFixedHeight(17);
    if (SysCheckItemID::Disk == m_id) {
        m_typeNameLabel->setText(tr("Disk checking"));
    } else if (SysCheckItemID::Trash == m_id) {
        m_typeNameLabel->setText(tr("System cleanup"));
    } else if (SysCheckItemID::SystemUpdate == m_id) {
        m_typeNameLabel->setText(tr("System version"));
    } else if (SysCheckItemID::DevMode == m_id) {
        m_typeNameLabel->setText(tr("Developer mode"));
    } else if (SysCheckItemID::AutoStartApp == m_id) {
        m_typeNameLabel->setText(tr("Startup programs"));
    } else if (SysCheckItemID::MaxLimit == m_id) {
        m_typeNameLabel->setText(tr("Startup programs"));
    }

    m_statusIconLabel = new DLabel(this);
    m_statusIconLabel->setFixedSize(12, 12);
    m_statusIconLabel->setPixmap(QIcon::fromTheme("dcc_syscheck_item_normal").pixmap(12, 12));

    m_statusInfoLabel = new DLabel(this);
    m_statusInfoLabel->setFixedHeight(17);

    m_fixTipLabel = new DTipLabel("", this);

    m_ignoreBtn = new DPushButton(this);
    m_ignoreBtn->setFixedSize(16, 16);
    m_ignoreBtn->setIconSize(QSize(16, 16));
    m_ignoreBtn->setIcon(QIcon::fromTheme("dcc_syscheck_item_noignore").pixmap(16, 16));
    m_ignoreBtn->setFlat(true);
    m_ignoreBtn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_ignoreBtn->setToolTip(tr("Dismiss"));

    m_disIgnoreBtn = new DPushButton(this);
    m_disIgnoreBtn->setFixedSize(16, 16);
    m_disIgnoreBtn->setIconSize(QSize(16, 16));
    m_disIgnoreBtn->setIcon(QIcon::fromTheme("dcc_syscheck_item_ignore").pixmap(16, 16));
    m_disIgnoreBtn->setFlat(true);
    m_disIgnoreBtn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_disIgnoreBtn->setToolTip(tr("No dismission"));

    m_jumpLabel = new DCommandLinkButton("", this);
    m_jumpLabel->setFixedHeight(17);

    m_fixSpinner = new DSpinner(this);
    m_fixSpinner->setFixedSize(16, 16);

    m_fixSpinner->hide();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    FIXED_COL(0, 46); // 靠右放置图标
    FIXED_COL(1, 10);
    FIXED_COL(2, 174); // 项目说明
    FIXED_COL(3, 12); // 状态图标
    FIXED_COL(4, 10);
    FIXED_COL(5, 354); // 状态说明
    FIXED_COL(6, 16); // 忽略图标
    FIXED_COL(7, 20);
    FIXED_COL(8, 108); // 跳转与spinner

    // 布局中插入
    mainLayout->addWidget(m_typeIconLabel, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
    mainLayout->addWidget(m_typeNameLabel, 0, 2, Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(m_statusIconLabel, 0, 3, Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(m_statusInfoLabel, 0, 5, Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(m_fixTipLabel, 0, 5, Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(m_ignoreBtn, 0, 6, Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(m_disIgnoreBtn, 0, 6, Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(m_jumpLabel, 0, 8, Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(m_fixSpinner, 0, 8, Qt::AlignLeft | Qt::AlignVCenter);
    this->setLayout(mainLayout);
}

void SysCheckResultItemWidget::initConnection()
{
    connect(m_ignoreBtn, &DPushButton::clicked, this, [this] {
        Q_EMIT requestSetIgnore(m_id, true);
        m_disIgnoreBtn->show();
        m_ignoreBtn->hide();
        m_jumpLabel->hide();
        onNotifyIgnored(true);
        if (SysCheckItemID::DevMode == m_id) {
            m_disIgnoreBtn->hide();
        }
    });
    connect(m_disIgnoreBtn, &DPushButton::clicked, this, [this] {
        Q_EMIT requestSetIgnore(m_id, false);
        m_disIgnoreBtn->hide();
        m_ignoreBtn->show();
        m_jumpLabel->show();
        onNotifyIgnored(false);
    });

    connect(m_jumpLabel, &DCommandLinkButton::clicked, this, [this] {
        Q_EMIT requestFixItem(m_id);
    });
}

// 隐藏所有次要控件
// 根据各个状态确定是否显示
void SysCheckResultItemWidget::hideAll()
{
    m_statusIconLabel->hide();
    m_statusInfoLabel->hide();
    m_fixTipLabel->hide();
    m_jumpLabel->hide();
    m_fixSpinner->stop();
    m_ignoreBtn->hide();
    m_disIgnoreBtn->hide();
}

void SysCheckResultItemWidget::setResult(QStandardItem *resultItem)
{
    m_resultItem = resultItem;
    // 根据角色设置内容
    // 优先级 ：
    //  1 MASKED 隐藏界面
    int flag = resultItem->data(CheckResultModelRole::CheckProgressFlag).toInt();
    if (flag == CheckProgressStatus::Masked) {
        this->hide();
        this->setDisabled(true);
        Q_EMIT widgetHidden();
    }
    //  2 SUCCESS 成功
    bool isIssueFound = resultItem->data(CheckResultModelRole::IsIssueHappen).toBool();
    onNotifyComplete(m_id, isIssueFound);

    //  3 ERROR 展示问题
    // when erron info changed, put your logic here

    //  4 IGNORE 根据项目判断是否需要隐藏
    bool isIgnored = resultItem->data(CheckResultModelRole::IsIgnored).toBool();
    onNotifyIgnored(isIgnored);

    // 针对开发者模式
    if (m_id == SysCheckItemID::DevMode) {
        this->setVisible(!isIgnored);
    }

    // 针对自启动
    if (m_id == SysCheckItemID::AutoStartApp) {
        int count = resultItem->data(CheckResultModelRole::Details).toInt();
        QString autoStartDetail = tr("%1 apps");
        autoStartDetail = autoStartDetail.arg(count);
        if (isIssueFound && !isIgnored) {
            m_statusInfoLabel->setText(getColoredText(65, autoStartDetail));
            m_statusIconLabel->setPixmap(QIcon::fromTheme("dcc_syscheck_item_abnormal").pixmap(12, 12));
        } else {
            m_statusInfoLabel->setText(getColoredText(100, autoStartDetail));
            m_statusIconLabel->setPixmap(QIcon::fromTheme("dcc_syscheck_item_normal").pixmap(12, 12));
            m_jumpLabel->hide();
        }

        // 数量0时，隐藏忽略按钮
        if (!count) {
            m_ignoreBtn->hide();
            m_disIgnoreBtn->hide();
        }
        return;
    }
}

QString SysCheckResultItemWidget::getColoredText(int point, const QString &input)
{
    if (point >= 90) {
        return QString(RICH_TEXT_COLOR_FORMAT).arg(k90_100Color).arg(input);
    }

    if (point >= 75) {
        return QString(RICH_TEXT_COLOR_FORMAT).arg(k75_90Color).arg(input);
    }

    if (point >= 60) {
        return QString(RICH_TEXT_COLOR_FORMAT).arg(k60_75Color).arg(input);
    }

    if (point >= 0) {
        return QString(RICH_TEXT_COLOR_FORMAT).arg(k0_60Color).arg(input);
    }

    return QString(RICH_TEXT_COLOR_FORMAT).arg(kOutOfDate).arg(input);
}
