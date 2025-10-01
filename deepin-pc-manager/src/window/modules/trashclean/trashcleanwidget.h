// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "trashcleanconfigitem.h"
#include "trashcleanresultheaderitem.h"
#include "trashcleanresultitemwidget.h"

#include <DWidget>
#include <DFrame>
#include <DIconButton>
#include <DGuiApplicationHelper>
#include <DLabel>
#include <DSuggestButton>
#include <DTreeWidget>

#include <QItemDelegate>

#define SYS_CONFIG_INDEX 0
#define HISTORY_CONFIG_INDEX 1

DWIDGET_USE_NAMESPACE
class TrashCleanModel;

/* 添加树表代理 */
class TCTableDelegateTree : public QItemDelegate
{
    Q_OBJECT
public:
    explicit TCTableDelegateTree(QObject *parent = nullptr);

    virtual ~TCTableDelegateTree() {}

    // 重绘
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
};

class TrashCleanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrashCleanWidget(TrashCleanModel *model, DWidget *parent = nullptr);
    ~TrashCleanWidget();

private:
    // 初始化界面
    void initUI();
    QWidget *getTrashCleanHomePage();
    QWidget *getTrashCleanResultPage();

    // 将文件大小由BYTE转换为KB MB GB，最多保留两位小数
    static QString fileSizeToString(quint64 filesize);

public Q_SLOTS:
    void onItemStageChanged(int, int);
    void showResultWidget();
    void showHomeWidget();
    void cleanTrashFiles();
    void trashCleanAgianScan();
    void trashCleanSystemItemChecked(bool status);
    void trashCleanSystemTrashItemChecked(bool status);
    void trashCleanSystemCacheItemChecked(bool status);
    void trashCleanSystemLogItemChecked(bool status);
    void trashCleanHistoryItemChecked(bool status);
    void trashCleanHistoryInfoItemChecked(bool status);
    bool trashCleanItemAllStatus();

protected:
    void paintEvent(QPaintEvent *event);

private:
    TrashCleanModel *m_model;
    double m_trashFileCount;
    double m_cacheFileCount;
    double m_logFileCount;
    double m_historyFileCount;

    QWidget *m_trashCleanHomePageWidget;
    DLabel *m_logo;
    DLabel *m_title;
    DLabel *m_greeting;
    TrashCleanConfigItem *m_sysItem;
    TrashCleanConfigItem *m_hisItem;
    DSuggestButton *m_scanBtn;
    QList<int> m_scanConfigList;

    QWidget *m_trashCleanResultPageWidget;
    TrashCleanResultHeaderItem *m_headerItem;
    DTreeWidget *m_treeWidget;
    TrashCleanResultItemWidget *m_systemItem;
    QTreeWidgetItem *m_treeSystemItem;
    TrashCleanResultItemWidget *m_systemTrashItem;
    TrashCleanResultItemWidget *m_systemCacheItem;
    TrashCleanResultItemWidget *m_systemLogItem;
    TrashCleanResultItemWidget *m_hisInfoItem;
    QTreeWidgetItem *m_treeHisInfoItem;
    TrashCleanResultItemWidget *m_hisInfoAppItem;
    DPushButton *m_scanDoneButton;
    DSuggestButton *m_nowCleanButton;
    DSuggestButton *m_scanAgainButton;

    // 表格代理
    TCTableDelegateTree *m_delegate;
};
