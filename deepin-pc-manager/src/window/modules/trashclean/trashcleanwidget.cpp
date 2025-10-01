// Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "trashcleanwidget.h"
#include "trashcleanmodel.h"
#include "window/common/common.h"

#include <DFontSizeManager>
#include <QHBoxLayout>

TrashCleanWidget::TrashCleanWidget(TrashCleanModel *model, DWidget *parent)
    : DWidget(parent)
    , m_model(model)
    , m_trashCleanHomePageWidget(nullptr)
    , m_trashCleanResultPageWidget(nullptr)
{
    // 初始化UI
    initUI();
}

TrashCleanWidget::~TrashCleanWidget()
{
}

// 初始化界面
void TrashCleanWidget::initUI()
{
    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 加载垃圾清理首页
    mainLayout->addWidget(getTrashCleanHomePage());
    // 加载垃圾清理结果页
    mainLayout->addWidget(getTrashCleanResultPage());

    connect(m_sysItem, &TrashCleanConfigItem::stageChanged, this, &TrashCleanWidget::onItemStageChanged);
    connect(m_hisItem, &TrashCleanConfigItem::stageChanged, this, &TrashCleanWidget::onItemStageChanged);
    connect(m_scanBtn, &QPushButton::clicked, this, &TrashCleanWidget::showResultWidget);
    connect(m_scanDoneButton, &QPushButton::clicked, this, &TrashCleanWidget::showHomeWidget);
    connect(m_nowCleanButton, &QPushButton::clicked, this, &TrashCleanWidget::cleanTrashFiles);
    connect(m_scanAgainButton, &QPushButton::clicked, this, &TrashCleanWidget::trashCleanAgianScan);
    connect(m_systemItem, &TrashCleanResultItemWidget::sendBoxClicked, this, &TrashCleanWidget::trashCleanSystemItemChecked);
    connect(m_systemTrashItem, &TrashCleanResultItemWidget::sendBoxClicked, this, &TrashCleanWidget::trashCleanSystemTrashItemChecked);
    connect(m_systemCacheItem, &TrashCleanResultItemWidget::sendBoxClicked, this, &TrashCleanWidget::trashCleanSystemCacheItemChecked);
    connect(m_systemLogItem, &TrashCleanResultItemWidget::sendBoxClicked, this, &TrashCleanWidget::trashCleanSystemLogItemChecked);
    connect(m_hisInfoItem, &TrashCleanResultItemWidget::sendBoxClicked, this, &TrashCleanWidget::trashCleanHistoryItemChecked);
    connect(m_hisInfoAppItem, &TrashCleanResultItemWidget::sendBoxClicked, this, &TrashCleanWidget::trashCleanHistoryInfoItemChecked);

    m_scanConfigList = {0, 0};
    m_sysItem->setCheckBoxStatus(true);
    m_hisItem->setCheckBoxStatus(false);
    m_trashCleanHomePageWidget->setVisible(true);
    m_trashCleanResultPageWidget->setVisible(false);
}

QWidget *TrashCleanWidget::getTrashCleanHomePage()
{
    m_trashCleanHomePageWidget = new QWidget(this);

    m_logo = new DLabel(this);
    m_logo->setFixedSize(400, 220);
    m_logo->setPixmap(QIcon::fromTheme("dcc_cleaner_greeting").pixmap(400, 220));

    m_title = new DLabel(this);
    m_title->setText(tr("Clean out junk files and free up disk space"));
    QFont titleFont = m_title->font();
    titleFont.setWeight(QFont::Weight::DemiBold);
    m_title->setFont(titleFont);
    DFontSizeManager::instance()->bind(m_title, DFontSizeManager::T4);

    m_greeting = new DLabel(this);
    m_greeting->setText(tr("Start cleaning now to refresh your computer"));
    QFont greFont = m_greeting->font();
    greFont.setWeight(QFont::Normal);
    m_greeting->setFont(greFont);

    m_sysItem = new TrashCleanConfigItem(this);
    m_sysItem->setIconPath("dcc_cleaner_sys_icon");
    m_sysItem->setTitles(tr("System junk"), tr("Useless files created by the system"));
    m_sysItem->setIndex(SYS_CONFIG_INDEX);

    m_hisItem = new TrashCleanConfigItem(this);
    m_hisItem->setIconPath("dcc_cleaner_history_icon");
    m_hisItem->setTitles(tr("Traces"), tr("Activity traces of the system and applications"));
    m_hisItem->setIndex(HISTORY_CONFIG_INDEX);
    m_hisItem->setAccessibleName("tracesFrame");

    m_scanBtn = new DSuggestButton(this);
    QFont scanBtnFont = m_scanBtn->font();
    m_scanBtn->setFont(scanBtnFont);
    m_scanBtn->setFixedSize(150, 36);
    m_scanBtn->setText(tr("Scan Now", "button"));
    m_scanBtn->setFocusPolicy(Qt::NoFocus);

    QGridLayout *mainLayout = new QGridLayout(m_trashCleanHomePageWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    m_trashCleanHomePageWidget->setLayout(mainLayout);

    mainLayout->setColumnStretch(0, 107);
    mainLayout->setColumnMinimumWidth(0, 107);
    mainLayout->setColumnStretch(1, 516);
    mainLayout->setColumnMinimumWidth(1, 516);
    mainLayout->setColumnStretch(2, 147);
    mainLayout->setColumnMinimumWidth(2, 147);

    mainLayout->setRowMinimumHeight(0, 230);
    mainLayout->addWidget(m_logo, 0, 0, 1, -1, Qt::AlignBottom | Qt::AlignHCenter);
    mainLayout->setRowMinimumHeight(1, 54);
    mainLayout->addWidget(m_title, 1, 0, 1, -1, Qt::AlignCenter);
    mainLayout->setRowMinimumHeight(2, 24);
    mainLayout->addWidget(m_greeting, 2, 0, 1, -1, Qt::AlignCenter);
    mainLayout->setRowMinimumHeight(3, 22);

    mainLayout->setRowMinimumHeight(4, 42);
    mainLayout->setRowMinimumHeight(5, 42);
    mainLayout->setRowMinimumHeight(6, 42);
    mainLayout->addWidget(m_sysItem, 4, 1);
    mainLayout->addWidget(m_hisItem, 5, 1);

    mainLayout->setRowStretch(6, 100);
    mainLayout->setRowMinimumHeight(6, 88);
    mainLayout->addWidget(m_scanBtn, 6, 0, 1, -1, Qt::AlignCenter);

    mainLayout->setRowMinimumHeight(8, 4);

    return m_trashCleanHomePageWidget;
}

QWidget *TrashCleanWidget::getTrashCleanResultPage()
{
    m_trashCleanResultPageWidget = new QWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(m_trashCleanResultPageWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
    m_trashCleanResultPageWidget->setLayout(mainLayout);

    // 上部分标题视图绘制
    m_headerItem = new TrashCleanResultHeaderItem(this);
    mainLayout->addWidget(m_headerItem, 0, Qt::AlignCenter);

    // 表格
    m_treeWidget = new DTreeWidget(this);
    m_treeWidget->setAccessibleName("centerResultFrame_treeWidget");
    m_treeWidget->setFixedSize(750, 368);
    m_treeWidget->setFrameShape(QFrame::NoFrame);
    m_treeWidget->setIndentation(16);
    m_treeWidget->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
    m_treeWidget->setBackgroundRole(QPalette::Window);
    m_treeWidget->setHeaderHidden(true);
    m_treeWidget->setColumnCount(1);
    m_treeWidget->setFocusPolicy(Qt::NoFocus);
    m_delegate = new TCTableDelegateTree(this);
    m_treeWidget->setItemDelegate(m_delegate);
    mainLayout->addWidget(m_treeWidget, 0, Qt::AlignCenter);

    // 系统垃圾
    m_systemItem = new TrashCleanResultItemWidget(this);
    m_systemItem->setAsRoot();
    m_systemItem->setHeadText(tr("System junk"));
    m_treeSystemItem = new QTreeWidgetItem(m_treeWidget);
    m_treeSystemItem->setData(0, Qt::DisplayRole, "");
    m_treeWidget->setItemWidget(m_treeSystemItem, 0, m_systemItem);
    m_treeSystemItem->setHidden(true);

    // 系统垃圾 -回收站
    m_systemTrashItem = new TrashCleanResultItemWidget(this);
    m_systemTrashItem->setAsChild();
    m_systemTrashItem->setHeadText(tr("Trash"));
    m_systemTrashItem->setHeadTipText(tr("Make sure all files in the trash can be deleted"));
    QTreeWidgetItem *itemTrashWidget = new QTreeWidgetItem();
    itemTrashWidget->setData(0, Qt::DisplayRole, "");
    m_treeWidget->topLevelItem(0)->addChild(itemTrashWidget);
    m_treeWidget->setItemWidget(itemTrashWidget, 0, m_systemTrashItem);

    // 系统垃圾 -缓存
    m_systemCacheItem = new TrashCleanResultItemWidget(this);
    m_systemCacheItem->setAsChild();
    m_systemCacheItem->setHeadText(tr("System caches"));
    m_systemCacheItem->setHeadTipText(tr("Caches created by the system"));
    QTreeWidgetItem *itemCacheWidget = new QTreeWidgetItem();
    itemCacheWidget->setData(0, Qt::DisplayRole, "");
    m_treeWidget->topLevelItem(0)->addChild(itemCacheWidget);
    m_treeWidget->setItemWidget(itemCacheWidget, 0, m_systemCacheItem);

    // 系统垃圾 -日志
    m_systemLogItem = new TrashCleanResultItemWidget(this);
    m_systemLogItem->setAsChild();
    m_systemLogItem->setHeadText(tr("System logs"));
    m_systemLogItem->setHeadTipText(tr("Log files created by the system"));
    QTreeWidgetItem *itemLogWidget = new QTreeWidgetItem();
    itemLogWidget->setData(0, Qt::DisplayRole, "");
    m_treeWidget->topLevelItem(0)->addChild(itemLogWidget);
    m_treeWidget->setItemWidget(itemLogWidget, 0, m_systemLogItem);

    // 痕迹信息
    m_hisInfoItem = new TrashCleanResultItemWidget(this);
    m_hisInfoItem->setAsRoot();
    m_hisInfoItem->setHeadText("Traces");
    m_treeHisInfoItem = new QTreeWidgetItem(m_treeWidget);
    m_treeHisInfoItem->setData(0, Qt::DisplayRole, "");
    m_treeWidget->setItemWidget(m_treeHisInfoItem, 0, m_hisInfoItem);
    m_treeHisInfoItem->setHidden(true);

    // 痕迹信息 -系统和应用痕迹
    m_hisInfoAppItem = new TrashCleanResultItemWidget(this);
    m_hisInfoAppItem->setAsChild();
    m_hisInfoAppItem->setHeadText(tr("System and Application traces"));
    QTreeWidgetItem *itemHisInfoAppWidget = new QTreeWidgetItem();
    itemHisInfoAppWidget->setData(0, Qt::DisplayRole, "");
    m_treeWidget->topLevelItem(1)->addChild(itemHisInfoAppWidget);
    m_treeWidget->setItemWidget(itemHisInfoAppWidget, 0, m_hisInfoAppItem);

    mainLayout->addStretch();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);
    layout->addStretch();
    // 完成按钮
    m_scanDoneButton = new DPushButton(this);
    m_scanDoneButton->setFixedSize(QSize(150, 36));
    m_scanDoneButton->setText(tr("Done", "button"));
    layout->addWidget(m_scanDoneButton);

    // 清理按钮
    m_nowCleanButton = new DSuggestButton(this);
    m_nowCleanButton->setFixedSize(QSize(150, 36));
    m_nowCleanButton->setText(tr("Clean Up"));
    layout->addWidget(m_nowCleanButton);

    m_scanAgainButton = new DSuggestButton(this);
    m_scanAgainButton->setFixedSize(QSize(150, 36));
    m_scanAgainButton->setText(tr("Scan Again"));
    layout->addWidget(m_scanAgainButton);
    mainLayout->addLayout(layout);

    return m_trashCleanResultPageWidget;
}

void TrashCleanWidget::onItemStageChanged(int id, int stage)
{
    if (id < 0 || id > m_scanConfigList.length() - 1) {
        return;
    }
    m_scanConfigList[id] = stage;
    m_scanBtn->setDisabled(m_scanConfigList.count(Qt::Unchecked) == m_scanConfigList.length());
}

void TrashCleanWidget::showResultWidget()
{
    // 初始化垃圾清理界面显示
    m_trashCleanHomePageWidget->setVisible(false);
    m_trashCleanResultPageWidget->setVisible(true);
    m_nowCleanButton->setVisible(true);
    m_scanAgainButton->setVisible(false);

    // 初始化垃圾文件大小
    m_trashFileCount = 0.00;
    m_cacheFileCount = 0.00;
    m_logFileCount = 0.00;
    m_historyFileCount = 0.00;

    // 初始化检测结果界面
    if (m_scanConfigList[SYS_CONFIG_INDEX]) {
        m_trashFileCount = m_model->getTrashFileCount();
        m_cacheFileCount = m_model->getCacheFileCount();
        m_logFileCount = m_model->getLogFileCount();
        m_treeSystemItem->setHidden(false);

        m_systemItem->setScanResult(fileSizeToString(quint64(m_trashFileCount + m_cacheFileCount + m_logFileCount)));
        m_systemTrashItem->setScanResult(fileSizeToString(quint64(m_trashFileCount)));
        m_systemCacheItem->setScanResult(fileSizeToString(quint64(m_cacheFileCount)));
        m_systemLogItem->setScanResult(fileSizeToString(quint64(m_logFileCount)));

        double systemTrashCount = m_trashFileCount + m_cacheFileCount + m_logFileCount;
        if (systemTrashCount < 1) {
            m_systemItem->setUncheckable();
            m_systemItem->setCheckBoxStatus(false);
        }
        if (m_trashFileCount < 1) {
            m_systemTrashItem->setUncheckable();
            m_systemTrashItem->setCheckBoxStatus(false);
        }
        if (m_cacheFileCount < 1) {
            m_systemCacheItem->setUncheckable();
            m_systemCacheItem->setCheckBoxStatus(false);
        }
        if (m_logFileCount < 1) {
            m_systemLogItem->setUncheckable();
            m_systemLogItem->setCheckBoxStatus(false);
        }
    }
    if (m_scanConfigList[HISTORY_CONFIG_INDEX]) {
        m_historyFileCount = m_model->getHistoryFileCount();
        m_treeHisInfoItem->setHidden(false);

        m_hisInfoItem->setScanResult(fileSizeToString(quint64(m_historyFileCount)));
        m_hisInfoAppItem->setScanResult(fileSizeToString(quint64(m_historyFileCount)));

        if (m_historyFileCount < 1) {
            m_hisInfoItem->setUncheckable();
            m_hisInfoItem->setCheckBoxStatus(false);
            m_hisInfoAppItem->setUncheckable();
            m_hisInfoAppItem->setCheckBoxStatus(false);
        }
    }

    // 初始化结果页头部信息
    double count = m_trashFileCount + m_cacheFileCount + m_logFileCount + m_historyFileCount;
    m_headerItem->setLogo("dcc_cleaner_scanned");
    QString resultStr = tr("%1 junk files, %2 selected")
                            .arg(fileSizeToString(quint64(count)))
                            .arg(fileSizeToString(quint64(count)));
    m_headerItem->setTitle(resultStr);

    // 检测结果展开
    m_treeWidget->expandAll();
}

void TrashCleanWidget::showHomeWidget()
{
    m_trashCleanHomePageWidget->setVisible(true);
    m_trashCleanResultPageWidget->setVisible(false);
}

void TrashCleanWidget::cleanTrashFiles()
{
    double cleanSize = 0.00;
    QStringList itemList;

    if (m_systemItem->getCheckBoxStatus()) {
        m_systemItem->setUncheckable();
        m_systemItem->setScanResult(fileSizeToString(0));
    }
    if (m_systemTrashItem->getCheckBoxStatus()) {
        itemList.append(TRASH_CLEAN_ITEM_TYPE_TRASH);
        cleanSize += m_trashFileCount;
        m_systemTrashItem->setCleanDone();
    }
    if (m_systemCacheItem->getCheckBoxStatus()) {
        itemList.append(TRASH_CLEAN_ITEM_TYPE_CACHE);
        cleanSize += m_cacheFileCount;
        m_systemCacheItem->setCleanDone();
    }
    if (m_systemLogItem->getCheckBoxStatus()) {
        itemList.append(TRASH_CLEAN_ITEM_TYPE_LOG);
        cleanSize += m_logFileCount;
        m_systemLogItem->setCleanDone();
    }
    if (m_hisInfoItem->getCheckBoxStatus()) {
        m_hisInfoItem->setUncheckable();
        m_hisInfoItem->setScanResult(fileSizeToString(0));
    }
    if (m_hisInfoItem->getCheckBoxStatus()) {
        itemList.append(TRASH_CLEAN_ITEM_TYPE_HISTORY);
        cleanSize += m_historyFileCount;
        m_hisInfoItem->setCleanDone();
    }

    if (itemList.count()) {
        m_model->cleanTrashFile(itemList);
    }

    // 清理完成
    m_headerItem->setLogo("dcc_cleaner_cleaned");
    QString titleInfo = tr("%1 removed").arg(fileSizeToString(quint64(cleanSize)));
    m_headerItem->setCleanFinished(titleInfo, "");

    m_nowCleanButton->setVisible(false);
    m_scanAgainButton->setVisible(true);
}

void TrashCleanWidget::trashCleanAgianScan()
{
    showResultWidget();
}

QString TrashCleanWidget::fileSizeToString(quint64 filesize)
{
    QStringList unit;
    unit << "Byte"
         << "KB"
         << "MB"
         << "GB";

    double quotient = 0;

    // 以1024为单位换算
    int i = 0;
    while (1024 <= filesize && i < unit.size() - 1) {
        quotient = filesize % 1024;
        filesize /= 1024;
        i++;
    }

    double size = quotient / 1024 + filesize;
    // 输出时，如果有小数，保留两位
    return QString("%1 %2").arg(QString::number(size, 'f', 2)).arg(unit[i]);
}

void TrashCleanWidget::trashCleanSystemItemChecked(bool status)
{
    if (m_systemTrashItem->getCheckBoxEnable()) {
        m_systemTrashItem->setCheckBoxStatus(status);
    }
    if (m_systemCacheItem->getCheckBoxEnable()) {
        m_systemCacheItem->setCheckBoxStatus(status);
    }
    if (m_systemLogItem->getCheckBoxEnable()) {
        m_systemLogItem->setCheckBoxStatus(status);
    }
}
void TrashCleanWidget::trashCleanSystemTrashItemChecked(bool status)
{
    Q_UNUSED(status);
    m_systemItem->setCheckBoxStatus(trashCleanItemAllStatus());
}
void TrashCleanWidget::trashCleanSystemCacheItemChecked(bool status)
{
    Q_UNUSED(status);
    m_systemItem->setCheckBoxStatus(trashCleanItemAllStatus());
}
void TrashCleanWidget::trashCleanSystemLogItemChecked(bool status)
{
    Q_UNUSED(status);
    m_systemItem->setCheckBoxStatus(trashCleanItemAllStatus());
}
bool TrashCleanWidget::trashCleanItemAllStatus()
{
    bool status = false;
    if (m_systemTrashItem->getCheckBoxStatus() || m_systemCacheItem->getCheckBoxStatus() || m_systemLogItem->getCheckBoxStatus()) {
        status = true;
    }
    return status;
}

void TrashCleanWidget::trashCleanHistoryItemChecked(bool status)
{
    if (m_hisInfoAppItem->getCheckBoxEnable()) {
        m_hisInfoAppItem->setCheckBoxStatus(status);
    }
}
void TrashCleanWidget::trashCleanHistoryInfoItemChecked(bool status)
{
    m_hisInfoItem->setCheckBoxStatus(status);
}

// 代理函数
TCTableDelegateTree::TCTableDelegateTree(QObject *parent)
    : QItemDelegate(parent)
{
}

// 重绘操作
void TCTableDelegateTree::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(index);
}

void TrashCleanWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}
