// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainwindow.h"
#include "window/widgets/multiselectlistview.h"
#include "window/common/gsettingkey.h"
#include "window/common/invokers/invokerfactory.h"
#include <qsettingbackend.h>

#include "modules/systemcheck/systemcheckmodule.h"
#include "modules/trashclean/trashcleanmodule.h"
#include "modules/toolbox/toolboxmodule.h"

#include <DListView>
#include <DBackgroundGroup>
#include <DTitlebar>
#include <DSettingsDialog>
#include <DSettingsOption>
#include <DSettingsWidgetFactory>
#include <DSettingsDialog>
#include <DRadioButton>
#include <DCheckBox>

#include <QHBoxLayout>
#include <QPushButton>
#include <QDBusInterface>

const int WidgetMinimumWidget = 950;
const int WidgetMinimumHeight = 640;
const int first_widget_min_width = 179;

#define SETTING_JSON ":/dt-settings.json" // 获取文件配置

const QMargins navItemMargin(5, 3, 5, 3);
const QVariant NavItemMargin = QVariant::fromValue(navItemMargin);
MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
    , m_dconfig(nullptr)
{
    m_dconfig = DConfig::create("org.deepin.dde.deepin-pc-manager", "org.deepin.dde.deepin-pc-manager");
    m_serSupportInvokerInter = InvokerFactory::GetInstance().CreateInvoker("com.deepin.dde.ServiceAndSupport",
                                                                           "/com/deepin/dde/ServiceAndSupport",
                                                                           "com.deepin.dde.ServiceAndSupport",
                                                                           ConnectType::SESSION,
                                                                           this);

    QTranslator *translator = new QTranslator(this);
    translator->load(QString("/usr/share/deepin-pc-manager/translations/deepin-pc-manager_%1.qm").arg(QLocale::system().name()));
    QCoreApplication::installTranslator(translator);
    // 设置主窗口属性
    setFixedSize(QSize(WidgetMinimumWidget, WidgetMinimumHeight));
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    // 初始化
    initUI();
    initData();
    initTray();

    QTimer::singleShot(0, this, &MainWindow::initAllModule); // 初始化所有模块
    QTimer::singleShot(0, this, &MainWindow::modulePreInitialize); // 模块初始化前准备
}

MainWindow::~MainWindow()
{
}

// 初始化UI
void MainWindow::initUI()
{
    // Initialize view and layout structure
    QWidget *content = new QWidget(this);
    setCentralWidget(content);

    m_contentLayout = new QHBoxLayout(content);
    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setMargin(0);
    m_contentLayout->setSpacing(0);

    // 左边list
    m_navView = new MultiSelectListView(this);
    m_navView->setLineWidth(0);
    m_navView->setUniformItemSizes(true);
    m_navView->setItemSize(QSize(159, 48));
    m_navView->setIconSize(QSize(48, 48));
    m_navView->setFrameShape(QFrame::Shape::NoFrame);
    m_navView->setEditTriggers(QListView::NoEditTriggers);
    m_navView->setAutoScroll(true); // 滚动条
    m_navView->setTabKeyNavigation(false); // tab按键是否有效
    m_navView->setViewportMargins(QMargins(10, 0, 10, 0)); // 滚动区域边框大小
    m_navView->setFixedWidth(first_widget_min_width); // 固定窗口值
    m_navView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    m_navView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_navView->setAutoFillBackground(true);
    m_navView->setBackgroundType(
        DStyledItemDelegate::BackgroundType(DStyledItemDelegate::BackgroundType::RoundedBackground
                                            | DStyledItemDelegate::BackgroundType::NoNormalState));
    m_navView->setItemSpacing(0);

    // 服务支持 -挂到导航栏
    DFrame *supportFrame = new DFrame(this);
    supportFrame->setLineWidth(0);
    QHBoxLayout *supportLayout = new QHBoxLayout(supportFrame);
    supportLayout->setContentsMargins(26, 0, 0, 20);
    supportLayout->setSpacing(10);
    supportFrame->setLayout(supportLayout);
    QLabel *imageLabel = new QLabel;
    imageLabel->setFixedSize(QSize(26, 26));
    imageLabel->setPixmap(QIcon::fromTheme("dcc_service_support").pixmap(26, 26));
    supportLayout->addWidget(imageLabel);
    m_supportLabel = new DLabel;
    m_supportLabel->setText(tr("Service Support"));
    m_supportLabel->setAlignment(Qt::AlignLeft);
    m_supportLabel->setCursor(QCursor(Qt::PointingHandCursor));
    m_supportLabel->installEventFilter(this);
    supportLayout->addWidget(m_supportLabel);
    m_navView->addFooterWidget(supportFrame);

    // 加载导航栏
    m_contentLayout->addWidget(m_navView);

    // 分割线
    m_splitLine = new DFrame(this);
    m_splitLine->setFrameShape(QFrame::VLine);
    m_splitLine->setFrameShadow(QFrame::Plain);
    m_splitLine->setFixedHeight(600);
    m_splitLine->setFixedWidth(1);
    m_contentLayout->addWidget(m_splitLine);

    // 右侧布局
    m_rightContentLayout = new QHBoxLayout;
    m_rightContentLayout->setContentsMargins(0, 0, 0, 0);

    m_rightView = new DBackgroundGroup(m_rightContentLayout);
    m_rightView->setItemSpacing(2);
    m_rightView->setItemMargins(QMargins(0, 0, 0, 0));
    m_rightView->setContentsMargins(0, 0, 0, 0);
    m_rightView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_contentLayout->addWidget(m_rightView);
}

void MainWindow::initData()
{
    // 标题栏
    DTitlebar *titlebar = this->titlebar();
    titlebar->setIcon(QIcon::fromTheme("deepin-pc-manager"));
    auto menu = titlebar->menu();
    if (!menu) {
        menu = new QMenu(this);
    }
    titlebar->setMenu(menu);

    // 设置弹框
    auto action = new QAction(tr("Settings"), this);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::showDefaultSettingDialog);

    // 主题变换
    m_guiHelper = DGuiApplicationHelper::instance();
    connect(m_guiHelper, SIGNAL(themeTypeChanged(ColorType)), this, SLOT(themeTypeChange(ColorType)));

    m_navModel = new QStandardItemModel(m_navView);
    m_navView->setModel(m_navModel);
    // 连接按下信号与窗口切换槽函数
    connect(m_navView, &DListView::pressed, this, &MainWindow::onFirstItemClick);
}

void MainWindow::initTray()
{
    m_pSystemTray = new QSystemTrayIcon(this);
    m_trayMenu = new QMenu(this);

    // 电脑管家
    QAction *show = new QAction(this);
    show->setText(tr("Deepin PC Manager"));
    show->setToolTip(tr("Deepin PC Manager"));
    m_trayMenu->addAction(show);
    // 点击托盘中的安全中心  激活该窗口并且展现出来
    connect(show, &QAction::triggered, this, [this] {
        this->show();
        this->setWindowState(Qt::WindowState::WindowActive);
    });

    // 托盘退出
    QAction *exit = new QAction(this);
    exit->setText(tr("Exit"));
    m_trayMenu->addAction(exit);
    connect(exit, &QAction::triggered, this, [=] {
        closeWindow();
    });

    // 设置系统托盘的上下文菜单
    m_pSystemTray->setContextMenu(m_trayMenu);
    // 设置系统托盘提示信息、托盘图标
    m_pSystemTray->setToolTip(tr("Deepin PC Manager"));
    m_pSystemTray->setIcon(QIcon::fromTheme("deepin-pc-manager"));
    connect(m_pSystemTray, &QSystemTrayIcon::activated, this, &MainWindow::onTrayActivated);

    m_pSystemTray->show();
}

// 寻找子页面索引
int MainWindow::getPageIndex(QString pageName)
{
    Q_UNUSED(pageName);
    int retIndex = 0;
    return retIndex;
}

void MainWindow::showModulePage(const QString &module, const QString &page)
{
    int moduleIndex = 0;
    int pageIndex = 0;
    ModuleInterface *inter = nullptr;
    for (int i = 0; i < m_modules.count(); i++) {
        inter = m_modules[i].first;
        if (module == inter->name()) {
            moduleIndex = i;
            break;
        }
    }

    // 判断子页面索引
    pageIndex = getPageIndex(page);

    setCurrentModule(moduleIndex, pageIndex);
    // 显示并恢复窗口为正常状态
    showNormal();
    activateWindow();
}

int MainWindow::getModuleCount()
{
    return m_modules.count();
}

// 初始化模块
void MainWindow::initAllModule()
{
    m_modules = {
        {new SystemCheckModule(this, this), tr("Full Check")},
        {new TrashCleanModule(this, this), tr("Cleanup")},
        {new ToolBoxModule(this, this), tr("Toolbox")},
    };

    // 浅色主题图标
    m_modulesLightIconList.clear();
    m_modulesLightIconList << SYSTEM_CHECK_MODEL_LIGHT << TRASH_CLEAN_MODEL_LIGHT << TOOL_BOX_LIGHT;

    // 深色主题图标
    m_modulesDarakIconList.clear();
    m_modulesDarakIconList << SYSTEM_CHECK_MODEL_DARK << TRASH_CLEAN_MODEL_DARK << TOOL_BOX_DARK;

    // 获得主题类型
    int type = m_guiHelper->themeType();
    for (auto it = m_modules.cbegin(); it != m_modules.cend(); ++it) {
        DStandardItem *item = new DStandardItem;
        item->setAccessibleText(it->first->name());
        item->setText(it->second);
        QString iconPath;
        if (type == 1) {
            iconPath = m_modulesLightIconList.at(it - m_modules.cbegin());
        } else {
            iconPath = m_modulesDarakIconList.at(it - m_modules.cbegin());
        }
        QIcon icon = QIcon::fromTheme(iconPath);
        item->setIcon(icon);

        item->setData(NavItemMargin, Dtk::MarginsRole);
        item->setData(QVariant(it->first->name()), Qt::AccessibleTextRole);
        m_navModel->appendRow(item);
    }
}

// 模块初始化前准备
void MainWindow::modulePreInitialize()
{
    for (auto it = m_modules.cbegin(); it != m_modules.cend(); ++it) {
        QElapsedTimer et;
        et.start();
        it->first->preInitialize();
    }

    QModelIndex index0 = m_navModel->index(0, 0);
    if (index0.isValid()) {
        onFirstItemClick(index0); // 默认点击首页
    }
}

// 左侧列表的点击
void MainWindow::onFirstItemClick(const QModelIndex &index)
{
    m_navView->setCurrentIndex(index);
    ModuleInterface *inter = m_modules[index.row()].first;

    // 有数据  并且最后一个和现在点击的一致
    if (!m_contentStack.isEmpty() && m_contentStack.last().first == inter) {
        return;
    }
    popAllWidgets(0);

    inter->active(0);
    m_navView->resetStatus(index);
    m_navView->setFocus();
}

// 模块出栈
void MainWindow::popAndDelWidget()
{
    if (!m_contentStack.size())
        return;

    QWidget *w = m_contentStack.pop().second; // 栈的顶栈的widget
    if (w) {
        m_rightContentLayout->removeWidget(w);
        w->hide();
        w->deleteLater();
    }
}

// 功能模块切换出栈
void MainWindow::popAllWidgets(int place)
{
    Q_UNUSED(place);
    if (!m_contentStack.count())
        return;

    // 系统体检、病毒扫描、垃圾清理widget无需delete
    // 防火墙、安全防护、安全工具widget需要delete
    // popWidget();
    for (int pageCount = m_contentStack.count(); pageCount > place; pageCount--) {
        popWidget(); // 是否删除界面，由模块在deactive时决定
    }

    // pop delete all, means we need to deactive all and active someone later
    for (auto module : m_modules) {
        module.first->deactive();
    }

    // 清除栈容器内所有对象 --确保外部接口调用菜单显示之前栈容器为空
    m_contentStack.clear();
}

// 模块出栈
void MainWindow::popWidget()
{
    if (!m_contentStack.size())
        return;

    // 反激活切换前的模块 - widget是否需要delete，由各个模块的module自己决定

    // 掩藏/移除切换前的模块
    QWidget *w = m_contentStack.pop().second; // 栈的顶栈的widget
    if (w) {
        w->hide();
        m_rightContentLayout->removeWidget(w);

        qInfo() << "[MainWindow] [popWidget] widget name " << w;
    }
}

// 模块入栈
void MainWindow::pushNormalWidget(ModuleInterface *const inter, QWidget *const w)
{
    // Set the newly added page to fill the blank area
    w->setParent(this);
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    w->show();

    m_contentStack.push({inter, w});
    m_rightContentLayout->addWidget(w, m_contentStack.size() == 1 ? 3 : 7);
    qInfo() << "[MainWindow] [pushNormalWidget] widget name " << w;
}

/*********模块功能操作（纯虚函数实现）*********/
// 弹栈
void MainWindow::popWidget(ModuleInterface *const inter)
{
    // 外部模块调用时，判断是否当前模块界面处于首页
    if (m_contentStack.size()) {
        if (inter == m_contentStack.top().first) {
            // 将最后一层窗口出栈
            popWidget();
        }
    }
}
void MainWindow::popAndDelWidget(ModuleInterface *const inter)
{
    Q_UNUSED(inter)
    // 由外部模块发起的pop需要过滤
    if (m_contentStack.top().first != inter) {
        return;
    }

    // 将最后一层窗口出栈并删除
    popAndDelWidget();
}
// 压栈
void MainWindow::pushWidget(ModuleInterface *const inter, QWidget *const w)
{
    if (!inter) {
        qWarning() << Q_FUNC_INFO << " inter is nullptr";
        return;
    }

    if (!w) {
        qWarning() << Q_FUNC_INFO << " widget is nullptr";
        return;
    }

    pushNormalWidget(inter, w);
}
// 模块可视化
void MainWindow::setModuleVisible(ModuleInterface *const inter, const bool visible)
{
    auto find_it = std::find_if(m_modules.cbegin(),
                                m_modules.cend(),
                                [=](const QPair<ModuleInterface *, QString> &pair) {
                                    return pair.first == inter;
                                });

    if (find_it != m_modules.cend()) {
        m_navView->setRowHidden(find_it - m_modules.cbegin(), !visible);
        Q_EMIT moduleVisibleChanged(find_it->first->name(), visible);
    }
}
// 设置当前模块
void MainWindow::setCurrentModule(int iModuleIdx, int iPageIdx)
{
    // 获取QModelIndex
    QModelIndex modelIndex = m_navModel->index(iModuleIdx, 0);
    m_navView->setCurrentIndex(m_navModel->index(iModuleIdx, 0));
    // 将显示栈清空
    popAllWidgets(0);
    this->setFocus();

    // 激活模块
    ModuleInterface *inter = m_modules[modelIndex.row()].first;
    inter->active(iPageIdx);
}
ModuleInterface *MainWindow::getCurrentModule()
{
    if (m_contentStack.isEmpty()) {
        return nullptr;
    }

    return m_contentStack.top().first;
}
// 获得模块下标
int MainWindow::getModuleIndex(const QString &name)
{
    int index = 0;
    for (QPair<ModuleInterface *, QString> p : m_modules) {
        if (p.first->name() == name)
            return index;
        index++;
    }
    return -1;
}
// 设置后退按钮可用状态
void MainWindow::setBackForwardButtonStatus(bool status)
{
    Q_UNUSED(status);
}

/*********应用基础操作*********/
// 显示默认设置弹框
void MainWindow::showDefaultSettingDialog()
{
    QTemporaryFile *tmpFile = new QTemporaryFile;
    tmpFile->open();
    auto backend = new Dtk::Core::QSettingBackend(tmpFile->fileName(), this);

    // 创建设置数据
    Dtk::Core::DSettings *settings;

    settings = Dtk::Core::DSettings::fromJsonFile(SETTING_JSON);
    settings->setParent(this);
    settings->setBackend(backend);

    // 脚本直接设置的部分
    /* 基础设置 */
    // 基础设置-根据关闭窗口属性gsetting配置，更新设置界面选项
    // 优先显示每次询问按钮是否选中
    if (0 == this->getAskType()) {
        settings->setOption(SETTING_BASE_CLOSE_WINDOW, getCloseType());
    } else if (1 == this->getAskType()) {
        settings->setOption(SETTING_BASE_CLOSE_WINDOW, 2);
    }

    // 连接设置信号
    // 脚本直接设置的部分
    connect(settings, &Dtk::Core::DSettings::valueChanged, this, [=](const QString &key, const QVariant &value) {
        if (key.contains(SETTING_CLOSE_WINDOW_TYPE)) {
            int type = value.toInt();
            if (type >= Tray && type <= Exit) {
                this->setCloseType(type); // 设置关闭类型 托盘还是推出
                this->setAskType(0); // 设置访问类型
            } else if (type == Ask) {
                this->setAskType(1); // 设置访问类型是每次访问
            }
        }
    });

    // 设置弹窗
    m_dsd = new DSettingsDialog(this);
    m_dsd->updateSettings(settings);
    m_dsd->setModal(true);
    this->setWindowState(Qt::WindowState::WindowActive);
    this->show();
    m_dsd->show();
    m_dsd->scrollToGroup("");
}

// 获得关闭类型 0托盘最小化 1推出
int MainWindow::getCloseType()
{
    int type = 0;
    if (m_dconfig->isValid()) {
        type = m_dconfig->value(CLOSE_TYPE).toInt();
    }
    return type;
}
// 0 : Not Ask  1 : Ask      0 托盘最小化  1 退出
void MainWindow::setCloseType(int value)
{
    if (m_dconfig->isValid()) {
        m_dconfig->setValue(CLOSE_TYPE, value);
    }
}
// 获得访问类型 0不访问 1访问
int MainWindow::getAskType()
{
    int type = 0;
    if (m_dconfig->isValid()) {
        type = m_dconfig->value(ASK_TYPE).toInt();
    }
    return type;
}
// 设置访问  0 不访问 1访问
void MainWindow::setAskType(int value)
{
    if (m_dconfig->isValid()) {
        m_dconfig->setValue(ASK_TYPE, value);
    }
}

// 托盘处理
void MainWindow::onTrayActivated(QSystemTrayIcon::ActivationReason state)
{
    if (state == QSystemTrayIcon::ActivationReason::Trigger) {
        if (!this->isVisible()) {
            // 页面隐藏，点击托盘重新show
            this->show();
        } else {
            // 页面能显示的时候，点击托盘会＂正常/最小化＂切换窗口
            if (this->isMinimized()) {
                this->setWindowState(Qt::WindowState::WindowActive);
            } else {
                this->setWindowState(Qt::WindowState::WindowMinimized);
            }
        }
    }
}

// 应用退出
void MainWindow::closeWindow()
{
    qApp->quit();
}

// 关闭弹窗  只有在设置是访问才有这个
void MainWindow::initCloseDialog(QCloseEvent *event)
{
    m_trayDialag = new DDialog(this);
    m_trayDialag->setModal(true);
    m_trayDialag->setIcon(QIcon::fromTheme("deepin-pc-manager"));
    DRadioButton *radioExit = new DRadioButton(tr("Exit"), this);
    DRadioButton *radioMin = new DRadioButton(tr("Minimize to system tray"), this);
    DCheckBox *radioAsk = new DCheckBox(tr("Do not ask again"), this);
    closeType nChoose = static_cast<closeType>(getCloseType());

    // 根据GSettings设置settings页面的默认值
    switch (nChoose) {
    case Exit:
        radioExit->setChecked(true);
        break;
    case Tray:
    default:
        radioMin->setChecked(true);
        break;
    }
    radioAsk->setCheckState(Qt::CheckState::Unchecked);

    // 选择你的操作
    m_trayDialag->setMessage(tr("Please choose your action"));
    m_trayDialag->addContent(radioMin);
    m_trayDialag->addContent(radioExit);
    m_trayDialag->addContent(radioAsk);

    // 针对DTK弹窗目前accessibleName会随系统语言变化的情况
    // 通过insertButton的形式定制按钮,保持accessiableName的一致性
    QPushButton *cancelBtn = new QPushButton(m_trayDialag);
    cancelBtn->setText(tr("Cancel", "button"));
    cancelBtn->setObjectName("cancelBtn");
    cancelBtn->setAttribute(Qt::WA_NoMousePropagation);
    m_trayDialag->insertButton(0, cancelBtn, false);

    QPushButton *confirmBtn = new QPushButton(m_trayDialag);

    confirmBtn->setText(tr("Confirm", "button"));
    confirmBtn->setObjectName("ConfirmBtn");
    confirmBtn->setAttribute(Qt::WA_NoMousePropagation);
    m_trayDialag->insertButton(1, confirmBtn, false);

    int a = m_trayDialag->exec();
    if (1 != a) {
        event->ignore();
        return;
    } else {
        // 确认按钮点击
        if (radioMin->isChecked()) { // 最小花点击
            this->hide();
            event->ignore();
        } else if (radioExit->isChecked()) { // 退出点击
            closeWindow();
            event->ignore();
        }

        // 根据选择的方式  设置关闭方式
        if (radioMin->isChecked()) {
            setCloseType(Tray);
        } else if (radioExit->isChecked()) {
            setCloseType(Exit);
        }

        // 只有当选择radioAsk后才会设置不再询问
        if (radioAsk->isChecked()) {
            setAskType(0);
        }

        m_trayDialag->deleteLater();
        m_trayDialag = nullptr;
    }
}

// 显示用户反馈弹框
void MainWindow::showUserReplyDialog()
{
    DBUS_NOBLOCK_INVOKE(m_serSupportInvokerInter, "ServiceSession", 1);
}

// 背景主题变化
void MainWindow::themeTypeChange(ColorType themeType)
{
    Q_UNUSED(themeType);
    // 获得所有行
    int count = m_navModel->rowCount();
    for (int i = 0; i < count; i++) {
        // 每行的item
        QStandardItem *item = m_navModel->item(i, 0);
        QString iconPath;
        if (m_guiHelper->themeType() == DGuiApplicationHelper::ColorType::LightType) {
            iconPath = m_modulesLightIconList.at(i);
        } else {
            iconPath = m_modulesDarakIconList.at(i);
        }
        // 每行item的图标设置
        QIcon icon = QIcon::fromTheme(iconPath);
        item->setIcon(icon);
        item->setData(NavItemMargin, Dtk::MarginsRole);
        m_navModel->setItem(i, 0, item);
    }
}

/*********界面事件*********/
// 关闭窗口事件
void MainWindow::closeEvent(QCloseEvent *event)
{
    // 获得访问类型  0 不访问  1是展现访问关闭窗口
    if (!getAskType()) {
        // 不访问 获得关闭类型 0最小化 1退出
        if (getCloseType()) {
            closeWindow();
            event->ignore();
        } else {
            hide();
            event->ignore();
        }
    } else {
        initCloseDialog(event);
    }
}
// 事件过滤
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // 判断是否鼠标点击
    if (event->type() == QEvent::MouseButtonRelease) {
        // 判断是否在那个对象上面
        if (m_supportLabel == obj) {
            // 跳转到服务与支持
            showUserReplyDialog();
        }
    }
    return QWidget::eventFilter(obj, event);
}
