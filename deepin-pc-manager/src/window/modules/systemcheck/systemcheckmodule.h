// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "window/interface/frameproxyinterface.h"
#include "window/interface/moduleinterface.h"

class FrameProxyInterface;
class ModuleInterface;
class SystemCheckModel;
class SystemCheckWidget;

class SystemCheckModule : public QObject
    , public ModuleInterface
{
    Q_OBJECT
public:
    explicit SystemCheckModule(FrameProxyInterface *frame, QObject *parent = nullptr);
    ~SystemCheckModule() override;

public:
    // 初始化
    void initialize() override;
    // 预初始化
    void preInitialize() override;
    // 模块名称
    const QString name() const override;
    // 激活
    void active(int index) override;
    // 反激活
    void deactive() override;

private:
    SystemCheckModel *m_systemCheckModel;
    SystemCheckWidget *m_systemCheckWidget;
};
