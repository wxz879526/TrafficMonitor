#include "mainwindow.h"
#include <QMenu>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetupTray();
    m_trayIcon->show();

    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetupTray()
{
    m_trayMenu = new QMenu(this);
    m_pConnDetailAction = new QAction(QObject::tr("连接详情"), this);
    connect(m_pConnDetailAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_pTopMostAction = new QAction(QObject::tr("总是置顶"), this);
    connect(m_pTopMostAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_pMouseHackAction = new QAction(QObject::tr("鼠标穿透"), this);
    connect(m_pMouseHackAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_pLockWndPosAction = new QAction(QObject::tr("锁定窗口位置"), this);
    connect(m_pLockWndPosAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_pShowTrayNotifyAction = new QAction(QObject::tr("显示通知区域图标"), this);
    connect(m_pShowTrayNotifyAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_pMoreInfoAction = new QAction(QObject::tr("显示更多消息"), this);
    connect(m_pMoreInfoAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_pTaskWndAction = new QAction(QObject::tr("显示任务栏窗口"), this);
    connect(m_pTaskWndAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_pHideMainWndAction = new QAction(QObject::tr("隐藏主窗口"), this);
    connect(m_pHideMainWndAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_pSettingsAction = new QAction(QObject::tr("选项"), this);
    connect(m_pSettingsAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_pAboutAction = new QAction(QObject::tr("关于"), this);
    connect(m_pAboutAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_pExitAction = new QAction(QObject::tr("退出程序"), this);
    connect(m_pExitAction, SIGNAL(triggered()), qApp, SLOT(quit()));


    auto netConnMenu = m_trayMenu->addMenu(QObject::tr("选择网络连接"));
    auto autoSelect = new QAction(QObject::tr("自动选择"), this);
    connect(autoSelect, SIGNAL(triggered()), qApp, SLOT(quit()));
    netConnMenu->addAction(autoSelect);

    m_trayMenu->addAction(m_pConnDetailAction);
    m_trayMenu->addSeparator();

    m_trayMenu->addAction(m_pTopMostAction);
    m_trayMenu->addAction(m_pMouseHackAction);
    m_trayMenu->addAction(m_pLockWndPosAction);
    m_trayMenu->addAction(m_pShowTrayNotifyAction);
    m_trayMenu->addAction(m_pMoreInfoAction);
    m_trayMenu->addAction(m_pTaskWndAction);
    m_trayMenu->addAction(m_pHideMainWndAction);
    auto wndTransparent = m_trayMenu->addMenu(QObject::tr("窗口不透明度"));
    auto percent_100 = new QAction(QObject::tr("100%"), this);
    connect(percent_100, SIGNAL(triggered()), qApp, SLOT(quit()));
    wndTransparent->addAction(percent_100);
    auto percent_80 = new QAction(QObject::tr("80%"), this);
    connect(percent_80, SIGNAL(triggered()), qApp, SLOT(quit()));
    wndTransparent->addAction(percent_80);
    auto percent_60 = new QAction(QObject::tr("60%"), this);
    connect(percent_60, SIGNAL(triggered()), qApp, SLOT(quit()));
    wndTransparent->addAction(percent_60);
    auto percent_40 = new QAction(QObject::tr("40%"), this);
    connect(percent_40, SIGNAL(triggered()), qApp, SLOT(quit()));
    wndTransparent->addAction(percent_40);
    m_trayMenu->addSeparator();
    auto otherFuncMenu = m_trayMenu->addMenu(QObject::tr("其他功能"));
    auto changeSkin = new QAction(QObject::tr("更换皮肤"), this);
    connect(changeSkin, SIGNAL(triggered()), qApp, SLOT(quit()));
    otherFuncMenu->addAction(changeSkin);
    auto changeNotifyIcon = new QAction(QObject::tr("更换通知区图标"), this);
    connect(changeNotifyIcon, SIGNAL(triggered()), qApp, SLOT(quit()));
    otherFuncMenu->addAction(changeNotifyIcon);
    otherFuncMenu->addSeparator();
    auto allowEage = new QAction(QObject::tr("允许超出屏幕边界"), this);
    connect(allowEage, SIGNAL(triggered()), qApp, SLOT(quit()));
    otherFuncMenu->addAction(allowEage);
    otherFuncMenu->addSeparator();
    auto historyStat = new QAction(QObject::tr("历史流量统计"), this);
    connect(historyStat, SIGNAL(triggered()), qApp, SLOT(quit()));
    otherFuncMenu->addAction(historyStat);

    m_trayMenu->addAction(m_pSettingsAction);
    m_trayMenu->addSeparator();

    m_trayMenu->addAction(m_pAboutAction);
    m_trayMenu->addAction(m_pExitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    QIcon icon(":/image/notifyicon.ico");
    m_trayIcon->setIcon(icon);
    m_trayIcon->setContextMenu(m_trayMenu);
}
