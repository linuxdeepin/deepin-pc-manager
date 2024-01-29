/*
 * Copyright (C) 2019 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     caimengci <caimengci@uniontech.com>
 *
 * Maintainer: caimengci <caimengci@uniontech.com>
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
#include "securitypkgnamesql.h"

#include <QDebug>
#include <QProcess>
#include <QFileInfo>
SecurityPkgNameSql::SecurityPkgNameSql(QSqlDatabase db, QObject *parent)
    : QObject(parent)
    , m_db(db)
{
}

SecurityPkgNameSql::~SecurityPkgNameSql()
{
}

void SecurityPkgNameSql::quit()
{
}

// 新建(可执行文件->包名)映射数据表
void SecurityPkgNameSql::createPackageLinkTable()
{
    if (!m_db.open()) {
        qDebug() << "pkgName db open error!";
        return;
    }

    // 服务第一次启动启动时，创建包名解析表 package_binary_file
    QSqlQuery query(m_db);
    bool bRet = query.exec("create table if not exists package_binary_file "
                           "(binaryPath VARCHAR NOT NULL, "
                           "packageName VARCHAR NOT NULL)");
    if (!bRet) {
        qDebug() << "pkgName localcachedb exec error" << query.lastError();
    }

    // 查询package_binary_file表中已有的包名
    queryPackageList();
    return;
}

// 查询package_binary_file表中已有的包名
void SecurityPkgNameSql::queryPackageList()
{
    // 清空容器并判断数据库是否打开
    m_sPkgNameList.clear();
    if (!m_db.open()) {
        qDebug() << "pkgName localcachedb open error!";
        return;
    }

    // 查询packageName字段所有数据
    QSqlQuery query(m_db);
    QString sCmd;
    sCmd = QString("select packageName from package_binary_file");
    if (!query.exec(sCmd)) {
        qDebug() << "query localcachedb error : " << query.lastError();
    } else {
        // 将已有的所有包名插入到QStringList容器当中
        while (query.next()) {
            m_sPkgNameList.append(query.value(0).toString());
        }
        // 容器去重
        m_sPkgNameList.toSet().toList();
    }

    // 获取包名及相关联文件
    getPkgNameLinkFile();
}

// 获取包名及相关联文件
void SecurityPkgNameSql::getPkgNameLinkFile()
{
    getDesktopPackage();
}

// 建立desktop file -> package 映射
void SecurityPkgNameSql::getDesktopPackage()
{
    // 清理包名及相关联文件容器
    m_sPkgNameMap.clear();

    // 使用dpkg查询桌面应用包名
    auto shellCmd = "dpkg --search '*.desktop'";

    // 使用管道获取命令执行数据
    FILE *pp = nullptr;
    if ((pp = popen(shellCmd, "r")) == nullptr) {
        perror("popen() error!");
        return;
    }

    // 解析出所有的桌面应用包名
    char buf[1024] = {0};
    std::string szTmp {""}, szResult {""}, szPackageName {""}, szDesktopName {""};
    int nInsertCount = 0;
    while (fgets(buf, sizeof buf, pp)) {
        szTmp = buf;
        szTmp.erase(std::remove(szTmp.begin(), szTmp.end(), '\n'), szTmp.end());
        size_t nIndex = szTmp.find_last_of(':');
        szPackageName = szTmp.substr(0, nIndex);
        if (szPackageName.find(":") != std::string::npos) {
            size_t nIdx = szPackageName.find_last_of(':');
            szPackageName = szTmp.substr(0, nIdx);
        }
        szDesktopName = szTmp.substr(nIndex + 2, szTmp.length() - nIndex - 2);

        // 去重
        QString sPkgName = QString::fromStdString(szPackageName);
        if (!m_sPkgNameList.contains(sPkgName)) {
            nInsertCount += 1;
            m_sPkgNameList.append(sPkgName);
            queryPackageInfo(sPkgName);
        }
        memset(buf, '\0', sizeof buf);
    }

    // 关闭管道
    pclose(pp);

    // 如果没有可插入的项，直接返回完成信号
    if (nInsertCount <= 0) {
        Q_EMIT notifySqlInsertFinish();
        return;
    }

    insertPackageLinkInfo(m_sPkgNameMap);
    return;
}

// 根据已安装的包名来获取可执行文件路径
void SecurityPkgNameSql::queryPackageInfo(const QString szPackageName)
{
    if (szPackageName == "")
        return;

    char shellCmd[260] = {0};
    strcpy(shellCmd, "dpkg -L ");
    strcat(shellCmd, szPackageName.toStdString().c_str());
    strcat(shellCmd, " | grep \"/usr/bin\\|/usr/local/bin\\|/usr/share\\|/usr/sbin\\|/opt/apps\"");

    QProcess *processSSH = new QProcess(this);
    processSSH->start(QString::fromStdString(shellCmd));
    processSSH->waitForStarted();
    processSSH->waitForFinished();
    QString sProcessResultPath = QString::fromLocal8Bit(processSSH->readAllStandardOutput()).trimmed();
    processSSH->close();
    processSSH->deleteLater();

    QStringList sPkgInfoList;
    sPkgInfoList = sProcessResultPath.split('\n');
    // 将包名以及相关联文件插入数据库
    for (int i = 0; i < sPkgInfoList.count(); ++i) {
        QString sPkgInfo = sPkgInfoList.at(i);
        QFileInfo temDir(sPkgInfo);
        QString sSuffix = temDir.completeSuffix();
        // 过滤无关文件
        if (sSuffix == "svg" || sSuffix == "png"
            || sSuffix == "qm" || sSuffix == "htm"
            || sSuffix == "gif" || sSuffix == "jpg"
            || sSuffix == "kuip" || sSuffix == "mo"
            || sSuffix == "qss" || sSuffix == "xml"
            || sSuffix == "js" || sSuffix == "md"
            || sSuffix == "docbook" || sSuffix == "bz2"
            || sSuffix == "gz" || sSuffix == "pak"
            || sSuffix == "so" || sSuffix == "db"
            || sSuffix == "kui" || sSuffix == "data"
            || sSuffix == "css" || sSuffix == "doc"
            || sSuffix == "dps" || sSuffix == "et"
            || sSuffix == "pp" || sSuffix == "woff"
            || sSuffix == "xl" || sSuffix == "cfg"
            || sSuffix == "ico" || sSuffix == "ini"
            || sSuffix == "lex" || sSuffix == "dat"
            || sSuffix == "thmx" || sSuffix == "eftx"
            || sSuffix == "bmp" || sSuffix == "wmf"
            || sSuffix == "wav" || sSuffix == "conf"
            || sSuffix == "qph" || sSuffix == "ent") {
            continue;
        }
        m_sPkgNameMap[sPkgInfo] = szPackageName;
    }

    return;
}

// 插入包名及相关联文件数据
void SecurityPkgNameSql::insertPackageLinkInfo(QMap<QString, QString> sPkgNameMap)
{
    if (!m_db.open()) {
        qDebug() << "pkgName localcachedb open error!";
        return;
    }

    m_db.transaction();
    QSqlQuery query(m_db);

    QMapIterator<QString, QString> i(sPkgNameMap);
    while (i.hasNext()) {
        i.next();
        QString sCmd;
        sCmd = QString("insert into package_binary_file (binaryPath, packageName) values ('%1','%2')").arg(i.key()).arg(i.value());
        if (!query.exec(sCmd)) {
            qDebug() << "insert localcachedb error : " << query.lastError();
        }
    }

    m_db.commit();
    Q_EMIT notifySqlInsertFinish();
}

// 删除数据库某个包名数据 -- 备用DBus
void SecurityPkgNameSql::deletePackageLinkInfo(const QString sPkgName)
{
    if (!m_db.open()) {
        qDebug() << "pkgName localcachedb open error!";
        return;
    }

    QSqlQuery query(m_db);

    QString sCmd;
    sCmd = QString("delete from package_binary_file where packageName='%1'").arg(sPkgName);
    if (!query.exec(sCmd)) {
        qDebug() << "delete localcachedb error : " << query.lastError();
    }
}

// 应用程序安装卸载刷新数据库 --DBus
void SecurityPkgNameSql::refreshPackageLinkTable()
{
    getPkgNameLinkFile();
}
