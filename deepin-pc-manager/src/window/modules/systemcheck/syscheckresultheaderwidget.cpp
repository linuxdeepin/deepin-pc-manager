// Copyright (C) 2019 ~ 2022 UnionTech Software Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "syscheckresultheaderwidget.h"

#include <DPalette>
#include <DPaletteHelper>
#include <DFontSizeManager>

#include <QGridLayout>

DWIDGET_USE_NAMESPACE

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

SysCheckResultHeaderWidget::SysCheckResultHeaderWidget(QWidget *parent)
    : DFrame(parent)
    , m_pointValue(nullptr)
    , m_pointText(nullptr)
    , m_title(nullptr)
    , m_logo(nullptr)
    , m_tips(nullptr)
    , m_fixingTitle(nullptr)
    , m_fixingTips(nullptr)
{
    this->setFixedSize(750, 146);
    this->setLineWidth(0);
    this->setBackgroundRole(DPalette::Background);
    this->setContentsMargins(0, 0, 0, 0);
    DPalette dpa = DPaletteHelper::instance()->palette(this, this->palette());
    dpa.setColor(DPalette::Background, QColor::fromRgb(163, 211, 255, int(0.13 * 255)));
    DPaletteHelper::instance()->setPalette(this, dpa);
    initUI();

    // 唯一实例
    this->setAccessibleName("sysCheckResultHeaderWidget");
}

void SysCheckResultHeaderWidget::onPointChanged(int value, int issueCount)
{
    hideAll();

    m_pointValue->show();
    m_pointText->show();
    m_title->show();
    m_tips->show();
    m_logo->show();

    m_pointValue->setText(getColoredText(value, QString("%1").arg(value)));

    if (100 == value) {
        m_title->setText(tr("Your computer is healthy"));
        m_tips->setText(tr("Great! Keep staying healthy please."));
        m_logo->setPixmap(QIcon::fromTheme("dcc_syscheck_result_100point").pixmap(190, 146));
    } else {
        m_title->setText(QString(tr("%1 issue(s) found")).arg(getColoredText(value, QString("%1").arg(issueCount))));
        m_tips->setText(tr("Fix issues right now to keep your computer healthy."));
        m_logo->setPixmap(QIcon::fromTheme("dcc_syscheck_result_abnormal").pixmap(190, 146));
    }
}

// 修复完成全部换绿色，不管是否真的修完了
void SysCheckResultHeaderWidget::onFastFixFinished()
{
    hideAll();
    m_pointValue->show();
    m_pointText->show();
    m_pointValue->setText(getColoredText(100, QString("%1").arg(100)));
    m_title->show();
    m_title->setText(tr("Your computer is healthy"));
    m_tips->show();
    m_tips->setText(tr("Great! Keep staying healthy please."));

    m_logo->setPixmap(QIcon::fromTheme("dcc_syscheck_result_100point").pixmap(190, 146));
    m_logo->show();
}

void SysCheckResultHeaderWidget::onFastFixStarted()
{
    hideAll();
    m_fixingTips->show();
    m_fixingTitle->show();
    m_fixingTips->setText(tr("Fixing issues"));
    m_tips->setText(tr("It will be completed soon."));
    m_logo->setPixmap(QIcon::fromTheme("dcc_syscheck_result_repair").pixmap(190, 146));
    m_logo->show();
}

void SysCheckResultHeaderWidget::initUI()
{
    m_pointValue = new DLabel(this);
    m_pointValue->setFixedHeight(87);
    QFont pvf = m_pointValue->font();
    pvf.setPixelSize(60);
    m_pointValue->setFont(pvf);

    m_pointText = new DLabel(this);
    m_pointText->setFixedHeight(20);
    m_pointText->setText(tr("points"));

    m_title = new DLabel(this);
    m_title->setFixedHeight(32);
    m_title->setAlignment(Qt::AlignBottom);

    m_tips = new DTipLabel("", this);
    m_tips->setFixedHeight(17);
    DFontSizeManager::instance()->unbind(m_tips);
    m_tips->setAlignment(Qt::AlignBottom);

    m_logo = new DLabel(this);
    m_logo->setFixedSize(190, 146);

    m_fixingTitle = new DLabel(this);
    m_fixingTitle->setFixedHeight(32);

    m_fixingTitle->setAlignment(Qt::AlignBottom);

    m_fixingTips = new DTipLabel("", this);
    m_fixingTips->setFixedHeight(17);
    DFontSizeManager::instance()->unbind(m_fixingTips);

    m_fixingTips->setAlignment(Qt::AlignBottom);

    QGridLayout *mainLayout = new QGridLayout;
    // 7列 1空白*26 2分数*103 3文本*14 4空白*20 5文本*375 6图标*190 7空白*20
    // 3行 1空白*46 2文本*32 3空白*3 4文本*20 5空白*45
    FIXED_COL(0, 20);
    FIXED_COL(1, 38);
    FIXED_COL(2, 14);
    FIXED_COL(3, 20);
    FIXED_COL(4, 375);
    FIXED_COL(5, 190);
    FIXED_COL(6, 20);

    FIXED_ROW(0, 46);
    FIXED_ROW(1, 32);
    FIXED_ROW(2, 3);
    FIXED_ROW(3, 20);
    FIXED_ROW(4, 45);

    mainLayout->addWidget(m_pointValue, 0, 1, -1, 1, Qt::AlignRight | Qt::AlignHCenter);
    mainLayout->addWidget(m_pointText, 3, 2, Qt::AlignCenter);
    mainLayout->addWidget(m_title, 1, 4, Qt::AlignLeft | Qt::AlignHCenter);
    mainLayout->addWidget(m_tips, 3, 4, Qt::AlignLeft | Qt::AlignHCenter);
    mainLayout->addWidget(m_logo, 0, 5, 1, -1, Qt::AlignCenter);
    mainLayout->addWidget(m_fixingTitle, 1, 1, Qt::AlignLeft | Qt::AlignHCenter);
    mainLayout->addWidget(m_fixingTips, 3, 1, Qt::AlignLeft | Qt::AlignHCenter);
    this->setLayout(mainLayout);
}

// 隐藏UI内容，按状态显示
void SysCheckResultHeaderWidget::hideAll()
{
    m_pointValue->hide();
    m_pointText->hide();
    m_title->hide();
    m_tips->hide();
    m_logo->hide();
    m_fixingTips->hide();
    m_fixingTitle->hide();
}

QString SysCheckResultHeaderWidget::getColoredText(int point, const QString &input)
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
