// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "interface/frameproxyinterface.h"
#include "interface/moduleinterface.h"
#include "window/common/common.h"

#include <DGuiApplicationHelper>
#include <DMainWindow>
#include <DDialog>
#include <DFrame>
#include <DConfig>
#include <DToolButton>
#include <DLabel>

#include <QDBusContext>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QStandardItemModel>
#include <QPair>
#include <QStack>
#include <QStackedWidget>
#include <QMetaEnum>
#include <QToolButton>

DWIDGET_BEGIN_NAMESPACE
class DListView;
class DBackgroundGroup;
class DSettingsDialog;
DWIDGET_END_NAMESPACE

QT_BEGIN_NAMESPACE
class QHBoxLayout;
QT_END_NAMESPACE

class MultiSelectListView;

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE
class DBusInvokerInterface;
class MainWindow : public DTK_WIDGET_NAMESPACE::DMainWindow
    , public FrameProxyInterface
    , protected QDBusContext
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    // 模板函数,供各模块调用
    void popWidget(ModuleInterface *const inter) override; // 弹栈
    void popAndDelWidget(ModuleInterface *const inter) override;
    void pushWidget(ModuleInterface *const inter, QWidget *const w) override; // 压栈
    void setModuleVisible(ModuleInterface *const inter, const bool visible) override; // 模块可视化
    void setCurrentModule(int iModuleIdx, int iPageIdx = 0) override; // 设置当前模块
    ModuleInterface *getCurrentModule() override;
    int getModuleIndex(const QString &name) override; // 获得模块下标
    void setBackForwardButtonStatus(bool status) override; // 设置后退按钮可用状态

private:
    // 初始化
    void initUI();
    void initData();
    void initTray();

    // 出栈/入栈
    void popAndDelWidget();
    void popAllWidgets(int place = 0);
    void popWidget();
    void pushNormalWidget(ModuleInterface *const inter, QWidget *const w);

    // 主界面关闭方式配置项 获取/设置
    int getCloseType();
    void setCloseType(int value);
    // 主界面退出询问弹框配置项 获取/设置
    int getAskType();
    void setAskType(int value);

    // 程序退出
    void closeWindow();
    void initCloseDialog(QCloseEvent *event);

    // 显示用户反馈弹框
    void showUserReplyDialog();

public Q_SLOTS:
    int getModuleCount();
    void showModulePage(const QString &module, const QString &page);
    // 寻找子页面索引
    int getPageIndex(QString pageName);

    void initAllModule();
    void modulePreInitialize();
    void onFirstItemClick(const QModelIndex &index);

    void onTrayActivated(QSystemTrayIcon::ActivationReason state);
    void showDefaultSettingDialog();

    // 主题类型改变
    void themeTypeChange(ColorType themeType);

Q_SIGNALS:
    void moduleVisibleChanged(const QString &module, bool visible);

protected:
    void closeEvent(QCloseEvent *event) override;
    // 事件过滤
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    DConfig *m_dconfig;
    DBusInvokerInterface *m_serSupportInvokerInter;

    QHBoxLayout *m_contentLayout;
    DFrame *m_splitLine;
    MultiSelectListView *m_navView;
    QHBoxLayout *m_rightContentLayout;
    DTK_WIDGET_NAMESPACE::DBackgroundGroup *m_rightView;
    QStandardItemModel *m_navModel;
    DLabel *m_supportLabel;

    QList<QPair<ModuleInterface *, QString>> m_modules;
    QStack<QPair<ModuleInterface *, QWidget *>> m_contentStack;
    DGuiApplicationHelper *m_guiHelper; // 方便得到系统主题
    QStringList m_modulesDarakIconList; // 深色主题图标列表
    QStringList m_modulesLightIconList; // 浅色主题图标列表

    QSystemTrayIcon *m_pSystemTray; // 系统托盘图标
    QMenu *m_trayMenu; // 托盘目录
    DDialog *m_trayDialag;

    DSettingsDialog *m_dsd; // 设置弹框
};

#endif // MAINWINDOW_H
