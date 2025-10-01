// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <DFrame>
#include <DLabel>
#include <DTipLabel>
#include <DPushButton>
#include <DSpinner>
#include <DSuggestButton>

#include <QMap>

DWIDGET_USE_NAMESPACE

#define UNITWIDTH 345
#define UNITHEIGHT 80

class ToolBoxItemWidget : public DFrame
{
    Q_OBJECT
public:
    explicit ToolBoxItemWidget(DWidget *parent = nullptr);
    ~ToolBoxItemWidget() override;

    void setIcon(QString icon);
    void setAppTitle(QString title);
    void setAppTip(QString tip);
    void setSupporter(QString supporter);
    void setBtnProperty(QString property);
    void setBtnCheckAuth(bool checkauth);

private:
    void initUi();

public Q_SLOTS:
    void openAppDialog();

Q_SIGNALS:
    void sendOpenAppDialog(QString property, bool checkauth);

private:
    DLabel *m_pImageLabel;
    DLabel *m_pAppNameLabel;
    DLabel *m_pDesLabel;
    DTipLabel *m_pCompanyNameLabel;
    DSuggestButton *m_pOpenBtn;
};
