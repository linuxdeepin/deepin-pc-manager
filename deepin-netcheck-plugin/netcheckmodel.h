// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <DConfig>

#include <QObject>

// 硬件检测结果
enum ServiceResult {
    ServiceBlock = 0,
    NoFound,
    CheckSucessed
};

// 网络设置检测结果
enum NetSettingResult {
    NoNetConn = 0,
    NetSettingSkip,
    NetSettingCheckFailed,
    NetSettingCheckSucessed
};

// DHCP检测结果
enum DHCPResult {
    DHCPCheckFailed = 0,
    DHCPCheckSucessed
};

// DNS检测结果
enum DNSResult {
    DNSCheckError = 0,
    DNSCheckAbnormal,
    DNSCheckSucessed
};

// host检测结果
enum HostResult {
    HostCheckFailed = 0,
    HostCheckSucessed
};

// 网络连接访问结果
enum NetConnResult {
    NetConnFailed = 0,
    NetConnSucessed
};

DCORE_USE_NAMESPACE
class DBusInvokerInterface;
class NetCheckModel : public QObject
{
    Q_OBJECT
public:
    explicit NetCheckModel(QObject *parent = nullptr);
    ~NetCheckModel();

    // 获取/设置网络检测类型和内网域名/ip
    int getNetCheckType();
    void setNetCheckType(int type);
    QString getNetCheckIntranet();
    void setNetCheckIntranet(QString domain);

    // 初始化数据
    void initData();
    void startNetCheck(QString domainText);

    // 开始检测网络设备
    void startCheckNetDevice();
    // 开始检测网络设置
    void startCheckNetSetting();
    // 开始检测DHCP
    void startCheckNetDHCP();
    // 开始检测DNS
    void startCheckNetDNS();
    // 开始检测host文件
    void startCheckNetHostFile();
    // 开始检测网络连接访问
    void startCheckNetConn();

    bool checkVPNOrProxyOpen();
    bool getNetCheckStatus();
    void setNetCheckStatus(bool status);

public Q_SLOTS:
    // 接收网络硬件检测结果
    void acceptCheckNetDevice(int result);
    // 接收网络连接配置检测结果
    void acceptCheckNetConnSetting(int result);
    // 接收DHCP检测结果
    void acceptCheckNetDHCP(int result);
    // 接收DNS检测结果
    void acceptCheckNetDNS(int result);
    // 接收host检测结果
    void acceptCheckNetHost(int result);
    // 接收网络连接访问检测结果
    void acceptCheckNetConn(int result);

Q_SIGNALS:
    void notifyNetCheckReset();
    void notifyNetDeviceCheck(int result);
    void notifyNetSettingCheck(int result);
    void notifyNetDHCPCheck(int result);
    void notifyNetDNSCheck(int result);
    void notifyNetHostCheck(int result);
    void notifyNetConnCheck(int result);

    void sendNetCheckStatus(bool status);

private:
    DConfig *m_dconfig;
    DBusInvokerInterface *m_netCheckDBusInvokerInter;
    DBusInvokerInterface *m_netWorkDBusInvokerInter;
    QString m_domainText;
};
