/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     caitao <caitao@deepin.com>
 *
 * Maintainer: caitao <caitao@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "fixitemwindow.h"

#include <DBackgroundGroup>
#include <DCommandLinkButton>

#include <QVBoxLayout>
#include <QPainter>
#include <QLabel>

DWIDGET_USE_NAMESPACE

FixItemWindow::FixItemWindow(bool flag, QWidget *parent)
    : QFrame(parent)
    , m_bgGroup(nullptr)
    , m_leftIcon(nullptr)
    , m_leftLabel(nullptr)
    , m_rightBtn(nullptr)
    , m_isFlag(flag)
{
    QMargins mg(0, 0, 0, 0);
    //因为需要SettingsItem 整体为一个圆角，所有用一个　widget　当背景
    auto bgWidget = new QWidget(this);

    //将　widget　加入到一个新建的　DBackgroundGroup中
    auto bglayout = new QVBoxLayout;
    bglayout->setContentsMargins(mg);
    m_bgGroup = new DBackgroundGroup(bglayout, this);
    m_bgGroup->setBackgroundRole(QPalette::Window);

    bglayout->addWidget(bgWidget);

    //将 m_bgGroup 沉底
    m_bgGroup->lower();

    //设置m_bgGroup 的大小
    m_bgGroup->setFixedSize(size());

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    m_leftIcon = new QLabel(this);
    mainLayout->addWidget(m_leftIcon, 0, Qt::AlignLeft | Qt::AlignCenter);

    m_leftLabel = new QLabel(this);
    m_leftLabel->setWordWrap(true);
    m_leftLabel->adjustSize();
    // 设置左对齐
    m_leftLabel->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(m_leftLabel, 1);

    m_rightBtn = new DCommandLinkButton("", this);
    mainLayout->addWidget(m_rightBtn, 0, Qt::AlignRight);
}

void FixItemWindow::setIcon(QIcon icon)
{
    m_leftIcon->setPixmap(icon.pixmap(20, 20));
//    m_leftIcon->setAlignment(Qt::AlignLeft);
}

void FixItemWindow::setText(QString text)
{
    m_leftLabel->setText(text);
}

void FixItemWindow::setButtonText(QString text, bool flag)
{
    if(flag) {
        m_rightBtn->setEnabled(false);
        QPalette pal;
       pal.setColor(QPalette::ButtonText, Qt::green);
//        pal.setColor(QPalette::Background, Qt::transparent);
        m_rightBtn->setText(text);
        m_rightBtn->setPalette(pal);
    } else {
        QPalette pal;
        pal.setColor(QPalette::ButtonText, QColor(0, 130, 250, 255));
//        pal.setColor(QPalette::Background, Qt::transparent);
        m_rightBtn->setText(text);
        m_rightBtn->setPalette(pal);
    }
}

DCommandLinkButton *FixItemWindow::getButton()
{
    return m_rightBtn;
}

// 获得按钮的名字
QString FixItemWindow::getButtonText()
{
    return m_rightBtn->text();
}

void FixItemWindow::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    //设置m_bgGroup 的大小
    if (m_bgGroup) {
        m_bgGroup->setFixedSize(size());
    }
}
