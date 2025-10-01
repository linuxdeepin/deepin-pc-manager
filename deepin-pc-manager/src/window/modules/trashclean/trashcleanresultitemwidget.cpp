// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "trashcleanresultitemwidget.h"

#include <DCheckBox>
#include <DFontSizeManager>
#include <DSpinner>
#include <DTipLabel>

#include <QEvent>
#include <QHBoxLayout>
#include <QThread>

#define ROOT_NODE_WIDTH 734
#define ITEM_INDENTATION 16
#define FATHER_NODE_WIDTH (ROOT_NODE_WIDTH - ITEM_INDENTATION)
#define CHILD_NODE_WIDTH (FATHER_NODE_WIDTH)

TrashCleanResultItemWidget::TrashCleanResultItemWidget(QWidget *parent)
    : DFrame(parent)
    , m_isRootItem(false)
    , m_checkBox(nullptr)
    , m_iconLable(nullptr)
    , m_headLabel(nullptr)
    , m_headTipLabel(nullptr)
    , m_tailLabel(nullptr)
    , m_usageLabel(nullptr)
    , m_contentFrame(nullptr)
    , m_root(nullptr)
    , m_isWorking(false)
{
    this->setFixedHeight(46); // 单个项目高度固定
    this->setFixedWidth(FATHER_NODE_WIDTH); // 单个项目宽度固定,避免在不同位置产生变化
    this->setBackgroundRole(DPalette::NoType);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(0, 0, 0, 0); // 上下需要留白，模拟窗口内空隙
    setLayout(mainLayout);

    m_contentFrame = new DFrame(this);
    m_contentFrame->setObjectName("contentFrame");
    m_contentFrame->setLineWidth(0);
    m_contentFrame->setFixedHeight(36);
    // 背景，假如是根检查项，将背景重设为NoType
    m_contentFrame->setBackgroundRole(DPalette::ItemBackground);
    mainLayout->addWidget(m_contentFrame);

    QHBoxLayout *firstLayerLayout = new QHBoxLayout;
    firstLayerLayout->setContentsMargins(5, 0, 0, 0);
    m_contentFrame->setLayout(firstLayerLayout);

    m_checkBox = new DCheckBox(this);
    // 避免得到焦点
    m_checkBox->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_checkBox->setFixedSize(QSize(20, 20));
    m_checkBox->setContentsMargins(0, 5, 0, 5);
    // 所有检查项第一次默认选中
    m_checkBox->setChecked(true);
    firstLayerLayout->addWidget(m_checkBox);

    // 用于清理阶段显示清理结果的提示图标
    m_iconLable = new DTipLabel("", this);
    // 根据UI整改,图标设置为16*16
    m_iconLable->setFixedSize(20, 20);
    // 仅在清理后显示
    m_iconLable->hide();
    m_iconLable->setAlignment(Qt::AlignRight);
    firstLayerLayout->addWidget(m_iconLable);

    // 检查项主标题
    m_headLabel = new DLabel(this);
    m_headLabel->setObjectName("headLable");
    m_headLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_headLabel->setContentsMargins(5, 0, 0, 0);
    m_headLabel->adjustSize();
    firstLayerLayout->addWidget(m_headLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);

    //    QPalette headPal;
    //    headPal.setColor(QPalette::Text, QColor("#001a2e"));
    //    m_headLabel->setPalette(headPal);
    QFont headFont = m_headLabel->font();
    headFont.setWeight(QFont::Medium);
    headFont.setPixelSize(13);
    m_headLabel->setFont(headFont);

    // 通过stretch调整m_headLabel水平位置
    firstLayerLayout->addStretch(1);

    // 检查项副标题
    m_headTipLabel = new DTipLabel("", this);
    m_headTipLabel->setObjectName("headTipLable");
    m_headTipLabel->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
    m_headTipLabel->setContentsMargins(0, 0, 0, 0);
    m_headTipLabel->adjustSize();
    firstLayerLayout->addWidget(m_headTipLabel);

    QPalette tipPal;
    tipPal.setColor(QPalette::Text, QColor("#526A7F"));
    m_headTipLabel->setPalette(tipPal);
    QFont tipFont = m_headTipLabel->font();
    tipFont.setWeight(QFont::Normal);
    tipFont.setPixelSize(12);
    m_headTipLabel->setFont(tipFont);
    DFontSizeManager::instance()->unbind(m_headTipLabel);

    // 通过stretch调整m_tailLabel水平位置
    firstLayerLayout->addStretch(15);

    // 右侧说明部分，记录状态或文件数量信息
    m_tailLabel = new DTipLabel("", this);
    DFontSizeManager::instance()->unbind(m_tailLabel);

    m_tailLabel->setObjectName("tailLabel");
    m_tailLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    firstLayerLayout->addWidget(m_tailLabel);
    // 仅在根检查项上显示说明
    m_tailLabel->hide();

    // 最右侧，记录本项检查总计文件空间大小
    m_usageLabel = new DLabel(this);

    m_usageLabel->setObjectName("usageLabel");
    m_usageLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_usageLabel->setContentsMargins(0, 0, 5, 0);
    firstLayerLayout->addWidget(m_usageLabel);

    // 设置边框的宽度
    setLineWidth(0);

    // 项目选中时，触发上级和下级检查项状态变更
    connect(m_checkBox, &DCheckBox::clicked, this, &TrashCleanResultItemWidget::checkBoxSelected);
}

TrashCleanResultItemWidget::~TrashCleanResultItemWidget()
{
}

// 设置标题
void TrashCleanResultItemWidget::setHeadText(const QString &text)
{
    m_headLabel->setText(text);
    m_headStr = text;
    resetElidedTextByFontChange();
}

// 本项目内容说明
void TrashCleanResultItemWidget::setHeadTipText(const QString &text)
{
    m_headTipLabel->setText(text);
    m_headTipStr = text;
}

// 项目右侧提示信息
// 注意tailtext仅对根项目起作用
// 在子项上需要隐藏
void TrashCleanResultItemWidget::setTailText(const QString &text)
{
    if (m_isRootItem) {
        // 扫描完成，显示内容
        m_tailLabel->setVisible(true);
        m_tailLabel->setText(text);
        m_tailStr = text;
        resetElidedTextByFontChange();
    }
}

// 由上一级或根结点遍历，在根项被勾选或
void TrashCleanResultItemWidget::setCheckBoxStatus(bool isChecked)
{
    m_checkBox->setChecked(isChecked);
}

// 清理完成时隐藏勾选框
void TrashCleanResultItemWidget::setCheckBoxVisable(bool isVisable)
{
    m_checkBox->setVisible(isVisable);
}

// 清理完成时显示完成图标
void TrashCleanResultItemWidget::setIconVisable(bool isVisable)
{
    m_iconLable->setVisible(isVisable);
}

// 清理成功
void TrashCleanResultItemWidget::setCleanDone()
{
    // 修改勾选状态，通知根项重新计算大小
    m_checkBox->setVisible(false);

    setIconVisable(true);
    m_usageLabel->setText("");
    m_usageLabel->setVisible(true);

    // 清理成功显示成功图标
    m_iconLable->setPixmap(QIcon::fromTheme("dcc_ok_tip").pixmap(16, 16));
    m_iconLable->setVisible(true);

    m_tailLabel->setText(tr("Removed"));
    m_tailLabel->show();
}

// 扫描完成时显示占用空间大小
// 所有检查项在扫描完成时均需要
void TrashCleanResultItemWidget::setUsedSpace(const QString &text)
{
    m_usageLabel->setVisible(true);
    m_usageLabel->setText(text);
    m_usageStr = text;
    resetElidedTextByFontChange();
}

bool TrashCleanResultItemWidget::getCheckBoxStatus() const
{
    return m_checkBox->isChecked();
}

// 本项作为检查的根项目,修改页面效果
void TrashCleanResultItemWidget::setAsRoot()
{
    m_isRootItem = true;
    // 根项不需要显示tip
    m_headTipLabel->setVisible(false);

    // root项固定高度减小,以从视觉上减少与其它项目的距离
    this->setFixedHeight(38);
    m_contentFrame->setFixedHeight(38);
    // 根项的frame不需要背景色
    setBackgroundRole(DPalette::NoType);
    // 根项的内容不需要背景色
    m_contentFrame->setBackgroundRole(DPalette::NoType);

    // 与次级项目宽度保持一致性
    this->setFixedWidth(ROOT_NODE_WIDTH);
}

void TrashCleanResultItemWidget::setAsChild()
{
    // 缩小长度,与父级对齐右侧
    this->setFixedWidth(CHILD_NODE_WIDTH);
}

// 对于根项目，显示扫描总文件数与选中的文件大小
// 对于子项目，只显示文件总大小
void TrashCleanResultItemWidget::setScanResult(const QString &filesize)
{
    setTailText(tr("total %1, selected:").arg(filesize));
    setUsedSpace(filesize);
}

void TrashCleanResultItemWidget::setUncheckable()
{
    if (m_isRootItem) {
        m_checkBox->hide();
    } else {
        m_checkBox->setDisabled(true);
    }

    m_iconLable->hide();
}

bool TrashCleanResultItemWidget::getCheckBoxEnable()
{
    return m_checkBox->isEnabled();
}

void TrashCleanResultItemWidget::checkBoxSelected()
{
    Q_EMIT sendBoxClicked(m_checkBox->checkState());
}

void TrashCleanResultItemWidget::paintEvent(QPaintEvent *event)
{
    resetElidedTextByFontChange();
    DFrame::paintEvent(event);
}

// 在label被写入字符串或字体变化时，将超出label的文字部分写作为省略号
void TrashCleanResultItemWidget::resetElidedTextByFontChange()
{
    if (m_isRootItem) {
        // 根项标题部分内容固定
        // 保留左侧标题和右侧数值
        // 中间部分的右侧超出部分省略
        QFontMetrics fontmet(m_usageLabel->font());
        int usageWidth = fontmet.horizontalAdvance(m_usageStr, -1);
        int headerWidth = fontmet.horizontalAdvance(m_headStr, -1);
        int elidedWidth = ROOT_NODE_WIDTH - usageWidth - headerWidth - 40;
        m_tailLabel->setText(fontmet.elidedText(m_tailStr, Qt::ElideRight, elidedWidth));
    } else {
        if (m_headTipLabel->text().isEmpty()) {
            // 主要是针对父项，保持右侧的完全显示
            // 当前字体宽度
            // 标题右侧超出部分省略
            QFontMetrics fontmet(m_usageLabel->font());
            int usageWidth = fontmet.horizontalAdvance(m_usageStr, -1);
            // 扣除20的预留空白
            int elidedWidth = CHILD_NODE_WIDTH - usageWidth - 20;
            m_headLabel->setText(fontmet.elidedText(m_headStr, Qt::ElideRight, elidedWidth));
        } else {
            QFontMetrics usagefontmet(m_usageLabel->font());
            int usageWidth = usagefontmet.horizontalAdvance(m_usageStr, -1);

            QFontMetrics headfontmet(m_headLabel->font());
            int headWidth = headfontmet.horizontalAdvance(m_headStr, -1);

            // 扣除40的空白
            int elidedWidth = CHILD_NODE_WIDTH - headWidth - usageWidth - 40;
            QFontMetrics fontmet(m_headTipLabel->font());
            m_headTipLabel->setText(fontmet.elidedText(m_headTipStr, Qt::ElideRight, elidedWidth));
        }
    }
}
