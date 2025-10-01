// Copyright (C) 2019 ~ 2022 UnionTech Software Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SYSCHECKREUSLTITEMWIDGET_H
#define SYSCHECKREUSLTITEMWIDGET_H

#include "window/common/common.h"

#include <DFrame>
#include <DLabel>
#include <DTipLabel>
#include <DSpinner>
#include <DPushButton>
#include <DCommandLinkButton>

#include <QWidget>
#include <QStandardItemModel>

const QList<QString> kSystemCheckTypeIconList = {
    "dcc_syscheck_item_autostart",
    "dcc_syscheck_item_sysupdate",
    "dcc_syscheck_item_garbage",
    "dcc_syscheck_item_disk",
    "dcc_syscheck_item_dev",
};

DWIDGET_USE_NAMESPACE
class SysCheckResultItemWidget : public Dtk::Widget::DFrame
{
    Q_OBJECT
public:
    explicit SysCheckResultItemWidget(SysCheckItemID id, QWidget *parent = nullptr);
    ~SysCheckResultItemWidget() {}

public Q_SLOTS:
    void onNotifyComplete(SysCheckItemID index, bool isAbnormal);
    void onNotifyFixStarted(SysCheckItemID index);
    void onNotifyFixFinished(SysCheckItemID index);
    void onNotifyIgnored(bool isIgnored);
    void setResult(const QStandardItemModel &);
    void onAutoStartAppCountChanged(int);

Q_SIGNALS:
    void requestFixItem(SysCheckItemID index);
    void requestSetIgnore(SysCheckItemID index, bool isNeedIgnore);
    void widgetHidden(); // 隐藏时需要刷新其它项的背景

private:
    void initUI();
    void initConnection();
    void hideAll();
    void setResult(QStandardItem *);

    QString getColoredText(int point, const QString &input);

private:
    SysCheckItemID m_id;
    DLabel *m_typeIconLabel;
    DLabel *m_typeNameLabel;
    DLabel *m_statusIconLabel;
    DLabel *m_statusInfoLabel;
    DTipLabel *m_fixTipLabel;
    DCommandLinkButton *m_jumpLabel;
    DSpinner *m_fixSpinner;
    DPushButton *m_ignoreBtn;
    DPushButton *m_disIgnoreBtn;
    QStandardItem *m_resultItem;
};

#endif // SYSCHECKREUSLTITEMWIDGET_H
