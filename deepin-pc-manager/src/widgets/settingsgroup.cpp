// Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "settingsgroup.h"

#include "settingsheaderitem.h"
#include "settingsitem.h"

#include <DBackgroundGroup>

#include <QDebug>
#include <QEvent>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE

namespace def {
namespace widgets {

SettingsGroup::SettingsGroup(QFrame *parent, BackgroundStyle bgStyle)
    : TranslucentFrame(parent)
    , m_layout(new QVBoxLayout)
    , m_headerItem(nullptr)
{
    m_layout->setMargin(0);
    m_layout->setSpacing(10);

    QVBoxLayout *vLayout = m_layout;
    if (GroupBackground == bgStyle) {
        m_bggroup = new DBackgroundGroup(m_layout, this);
        m_bggroup->setBackgroundRole(QPalette::Window);
        m_bggroup->setItemSpacing(1);
        vLayout = new QVBoxLayout;
        vLayout->addWidget(m_bggroup);
        m_bggroup->setUseWidgetBackground(false);
    }

    m_bgStyle = bgStyle;
    setLayout(vLayout);
}

SettingsGroup::SettingsGroup(const QString &title, QFrame *parent)
    : SettingsGroup(parent)
{
    setHeaderVisible(!title.isEmpty());
    setAccessibleName(title);

    m_headerItem->setTitle(title);
}

SettingsGroup::~SettingsGroup()
{
    if (m_headerItem)
        m_headerItem->deleteLater();
}

void SettingsGroup::setHeaderVisible(const bool visible)
{
    if (visible) {
        if (!m_headerItem)
            m_headerItem = new SettingsHeaderItem(this);
        insertItem(0, m_headerItem);
    } else {
        if (m_headerItem) {
            m_headerItem->deleteLater();
            m_headerItem = nullptr;
        }
    }
}

void SettingsGroup::insertItem(const int index, SettingsItem *item)
{
    if (ItemBackground == m_bgStyle) {
        // 当SettingsItem 被加入　SettingsGroup　时，为其加入背景
        item->addBackground();
    }

    m_layout->insertWidget(index, item);
    item->installEventFilter(this);
}

void SettingsGroup::appendItem(SettingsItem *item)
{
    insertItem(m_layout->count(), item);
}

void SettingsGroup::appendItem(SettingsItem *item, BackgroundStyle bgStyle)
{
    if ((ItemBackground == bgStyle) && (m_bgStyle == ItemBackground)) {
        // 当SettingsItem 被加入　SettingsGroup　时，为其加入背景
        item->addBackground();
    }

    m_layout->insertWidget(m_layout->count(), item);
    item->installEventFilter(this);
}

void SettingsGroup::removeItem(SettingsItem *item)
{
    m_layout->removeWidget(item);
    item->removeEventFilter(this);
}

void SettingsGroup::moveItem(SettingsItem *item, const int index)
{
    const int oldIndex = m_layout->indexOf(item);
    if (oldIndex == index)
        return;

    m_layout->removeWidget(item);
    m_layout->insertWidget(index, item);
}

void SettingsGroup::setSpacing(const int spaceing)
{
    m_layout->setSpacing(spaceing);
    if (m_bggroup)
        m_bggroup->setItemSpacing(spaceing);
}

int SettingsGroup::itemCount() const
{
    return m_layout->count();
}

void SettingsGroup::clear()
{
    const int index = m_headerItem ? 1 : 0;
    const int count = m_layout->count();

    for (int i(index); i != count; ++i) {
        QLayoutItem *item = m_layout->takeAt(index);
        QWidget *w = item->widget();
        w->removeEventFilter(this);
        w->setParent(nullptr);
        delete item;
        w->deleteLater();
    }
}

SettingsItem *SettingsGroup::getItem(int index)
{
    if (index < 0)
        return nullptr;

    if (index < itemCount()) {
        return qobject_cast<SettingsItem *>(m_layout->itemAt(index)->widget());
    }

    return nullptr;
}
} // namespace widgets
} // namespace def
