// Copyright (C) 2019 ~ 2022 UnionTech Software Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SYSCHECKRESULTHEADERWIDGET_H
#define SYSCHECKRESULTHEADERWIDGET_H

#include <DFrame>
#include <DLabel>
#include <DTipLabel>

#include <QWidget>

DWIDGET_USE_NAMESPACE
class SysCheckResultHeaderWidget : public Dtk::Widget::DFrame
{
    Q_OBJECT
public:
    explicit SysCheckResultHeaderWidget(QWidget *parent = nullptr);
    ~SysCheckResultHeaderWidget() {}

public Q_SLOTS:
    void onPointChanged(int, int);
    void onFastFixFinished();
    void onFastFixStarted();

private:
    void initUI();
    void hideAll();

    QString getColoredText(int point, const QString &input);

private:
    DLabel *m_pointValue;
    DLabel *m_pointText;
    DLabel *m_title;
    DLabel *m_logo;
    DTipLabel *m_tips;
    DLabel *m_fixingTitle;
    DTipLabel *m_fixingTips;
};

#endif // SYSCHECKRESULTHEADERWIDGET_H
