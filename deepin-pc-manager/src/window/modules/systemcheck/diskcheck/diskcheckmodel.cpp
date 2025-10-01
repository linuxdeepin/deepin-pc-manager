// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "diskcheckmodel.h"
#include "window/common/invokers/invokerfactory.h"
#include "parted/parted.h"

#include <QProcess>
#include <QDebug>

// 检查是否为可用设备
bool useableDevice(const PedDevice *lpDevice)
{
    Q_ASSERT(nullptr != lpDevice);
    char *buf = static_cast<char *>(malloc(size_t(lpDevice->sector_size)));
    if (!buf)
        return false;

    // Must be able to read from the first sector before the disk device is considered
    // useable in GParted.
    bool success = false;
    int fd = open(lpDevice->path, O_RDONLY | O_NONBLOCK);
    if (fd >= 0) {
        ssize_t bytesRead = read(fd, buf, size_t(lpDevice->sector_size));
        success = (bytesRead == lpDevice->sector_size);
        close(fd);
    }
    free(buf);
    return success;
}

DiskCheckModel::DiskCheckModel(QObject *parent)
    : QObject(parent)
    , m_diskManagerInvokerInter(nullptr)
{
    initData();
}

DiskCheckModel::~DiskCheckModel()
{
}

void DiskCheckModel::initData()
{
    // 磁盘管理dbus服务
    m_diskManagerInvokerInter = InvokerFactory::GetInstance().CreateInvoker("com.deepin.diskmanager",
                                                                            "/com/deepin/diskmanager",
                                                                            "com.deepin.diskmanager",
                                                                            ConnectType::SYSTEM,
                                                                            this);
}

// 磁盘管理器 是否已安装
bool DiskCheckModel::isDiskManagerInterValid()
{
    QProcess proc;
    // 查询是否已经安装 磁盘管理器
    proc.start("dpkg", {"-s", "deepin-diskmanager"});
    proc.waitForFinished();
    QString status = proc.readAllStandardOutput();

    return status.startsWith("Package: deepin-diskmanager\nStatus: install");
}

// 获取磁盘状态
bool DiskCheckModel::getDiskStatus()
{
    // 遍历所有磁盘设备路径
    QDBusMessage msg;
    QStringList devPaths = getDiskDevicePathList();
    for (QString devPath : devPaths) {
        msg = DBUS_BLOCK_INVOKE(m_diskManagerInvokerInter, "onGetDeviceHardStatus", devPath);
        GET_MESSAGE_VALUE(QString, data, msg);

        // 状态结果转小写
        QString status = data.toLower();
        // 若状态为异常，返回false
        if ("passed" != status) {
            qWarning() << "[HomePageModel] [getDiskStatus] no passed";
            return false;
        }
    }

    // 所有磁盘正常
    return true;
}

// 获取磁盘设备路径列表
QList<QString> DiskCheckModel::getDiskDevicePathList()
{
    QList<QString> devicepaths {};

    ped_device_probe_all();
    PedDevice *lpDevice = ped_device_get_next(nullptr);
    while (lpDevice) {
        if (useableDevice(lpDevice)) {
            devicepaths.append(lpDevice->path);
        }
        lpDevice = ped_device_get_next(lpDevice);
    }

    return devicepaths;
}
