﻿#include "mainwindow.h"
#include <QMenu>
#include <QPixmap>
#include <QBitmap>
#include <QPalette>
#include <QRegion>
#include <QMouseEvent>
#include <QSettings>
#include <QTimer>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetupTray();
    initConnections();
    m_trayIcon->show();

    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    resize(280, 60);

    QString strPath = QCoreApplication::applicationDirPath();
    QPixmap backImg(strPath + "\\skins\\0默认皮肤\\background_l.bmp");
    backImg = backImg.scaled(this->size());
    QPalette palette;
    palette.setBrush(QPalette::Window, backImg);
    setPalette(palette);

    // QBitmap mask(strPath + "\\skins\\0默认皮肤\\background_mask_l.bmp");
    /* QRegion region(0,0, 214, 42);
    region = region.subtracted(QRegion(0,0,1,1));
    region = region.subtracted(QRegion(1,0,1,1));
    region = region.subtracted(QRegion(1,1,1,1));
    region = region.subtracted(QRegion(0,1,1,1));
    region = region.subtracted(QRegion(0,2,1,1));
    region = region.subtracted(QRegion(2,0,1,1));
    setMask(region);*/

    QSettings configFile(strPath + "\\skins\\0默认皮肤\\skin.ini", QSettings::IniFormat);
    configFile.beginGroup("skin");
    auto color = configFile.value("text_color", 1000).toInt();
    configFile.endGroup();

    QTimer *pTimer = new QTimer(this);
    connect(pTimer, &QTimer::timeout, this, &MainWindow::OnTimerForNetSpeed);
    pTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::SetupTray()
{
    m_trayMenu = new QMenu(this);
    m_pConnDetailAction = new QAction(QObject::tr("连接详情"), this);
    connect(m_pConnDetailAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_pTopMostAction = new QAction(QObject::tr("总是置顶"), this);
    connect(m_pTopMostAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_pMouseHackAction = new QAction(QObject::tr("鼠标穿透"), this);
    m_pMouseHackAction->setCheckable(true);
    connect(m_pMouseHackAction, &QAction::triggered, this, [&](){
        auto bChecked = m_pMouseHackAction->isChecked();
        if (bChecked)
            SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE)|WS_EX_TRANSPARENT);
        else
            SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) & ~WS_EX_TRANSPARENT);
    });
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
    connect(m_pExitAction, &QAction::triggered, qApp, &QCoreApplication::quit);


    m_connectionSubMenu = m_trayMenu->addMenu(QObject::tr("选择网络连接"));

    m_trayMenu->addAction(m_pConnDetailAction);
    m_trayMenu->addSeparator();

    m_trayMenu->addAction(m_pTopMostAction);
    m_trayMenu->addAction(m_pMouseHackAction);
    m_trayMenu->addAction(m_pLockWndPosAction);
    m_trayMenu->addAction(m_pShowTrayNotifyAction);
    m_trayMenu->addAction(m_pMoreInfoAction);
    m_trayMenu->addAction(m_pTaskWndAction);
    m_trayMenu->addAction(m_pHideMainWndAction);

    // 窗口透明度设置
    auto wndTransparent = m_trayMenu->addMenu(QObject::tr("窗口不透明度"));
    auto pTransparentSetGrp = new QActionGroup(this);
    pTransparentSetGrp->setExclusive(true);
    auto addTransparentAction = [=](const QString &strTitle, qreal opacity){
        auto percentAction = new QAction(strTitle, pTransparentSetGrp);
        connect(percentAction, &QAction::triggered, this, [=](){
            setWindowOpacity(opacity);
            percentAction->setCheckable(true);
            percentAction->setChecked(true);
        });
        wndTransparent->addAction(percentAction);
        return percentAction;
    };

    addTransparentAction(QObject::tr("100%"), 1);
    addTransparentAction(QObject::tr("80%"), 0.8);
    addTransparentAction(QObject::tr("60%"), 0.6);
    addTransparentAction(QObject::tr("40%"), 0.4);

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

void MainWindow::initConnections()
{
    //为m_pIfTable开辟所需大小的内存
    free(m_pIfTable);
    m_dwSize = sizeof(MIB_IFTABLE);
    m_pIfTable = (MIB_IFTABLE *)malloc(m_dwSize);
    int rtn;
    rtn = GetIfTable(m_pIfTable, &m_dwSize, FALSE);
    if (rtn == ERROR_INSUFFICIENT_BUFFER)	//如果函数返回值为ERROR_INSUFFICIENT_BUFFER，说明m_pIfTable的大小不够
    {
        free(m_pIfTable);
        m_pIfTable = (MIB_IFTABLE *)malloc(m_dwSize);	//用新的大小重新开辟一块内存
    }
    //获取当前所有的连接，并保存到m_connections容器中
    m_connections.clear();
    GetIfTable(m_pIfTable, &m_dwSize, FALSE);
    for (unsigned int i{}; i < m_pIfTable->dwNumEntries; i++)
    {
        QString descr = QString(QLatin1String((const char*)m_pIfTable->table[i].bDescr));
        if (m_pIfTable->table[i].dwInOctets > 0 || m_pIfTable->table[i].dwOutOctets > 0 || descr == m_connection_name)		//查找接收或发送数据量大于0的连接和上次选择的连接
        {
            m_connections.emplace_back(i, descr, m_pIfTable->table[i].dwInOctets, m_pIfTable->table[i].dwOutOctets);
        }
    }
    if (m_connections.empty())
        m_connections.emplace_back(0, QString(QLatin1String((const char*)m_pIfTable->table[0].bDescr)), 0, 0);

    //if (m_connection_selected < 0 || m_connection_selected >= m_connections.size() || m_auto_select)
    //	AutoSelect();
    //选择网络连接
    if (m_auto_select)	//自动选择
    {
        if (m_restart_cnt != -1)	//当m_restart_cnt不等于-1时，即不是第一次初始化时，需要延时5秒再重新初始化连接
        {
            // KillTimer(DELAY_TIMER);
            //SetTimer(DELAY_TIMER, 5000, NULL);
        }
        else
        {
            autoSelect();
        }
    }
    else		//查找网络名为上次选择的连接
    {
        m_connection_selected = 0;
        for (size_t i{}; i < m_connections.size(); i++)
        {
            if (m_connections[i].description == m_connection_name)
                m_connection_selected = i;
        }
    }
    m_connection_name = m_connections[m_connection_selected].description;

    //根据已获取到的连接在菜单中添加相应项目
    m_connectionSubMenu->clear();
    QActionGroup *pConnectionsGrp = new QActionGroup(this);
    pConnectionsGrp->setExclusive(true);
    auto autoSelect = new QAction(QObject::tr("自动选择"), pConnectionsGrp);
    connect(autoSelect, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_connectionSubMenu->addAction(autoSelect);
    //设置“选择网卡”子菜单项
    QString connection_descr;
    for (int i{}; i < m_connections.size(); i++)
    {
        auto pAction = new QAction(m_connections[i].description, pConnectionsGrp);
        m_connectionSubMenu->addAction(pAction);
    }

    m_restart_cnt++;	//记录初始化次数
    m_connection_change_flag = true;
}

void MainWindow::autoSelect()
{
    unsigned int max_in_out_bytes{};
    unsigned int in_out_bytes;
    //m_connection_selected = m_connections[0].index;
    m_connection_selected = 0;
    //自动选择连接时，查找已发送和已接收字节数之和最多的那个连接，并将其设置为当前查看的连接
    for (int i{}; i<m_connections.size(); i++)
    {
        if (m_pIfTable->table[m_connections[i].index].dwOperStatus == IF_OPER_STATUS_OPERATIONAL)		//只选择网络状态为正常的连接
        {
            in_out_bytes = m_pIfTable->table[m_connections[i].index].dwInOctets + m_pIfTable->table[m_connections[i].index].dwOutOctets;
            if (in_out_bytes > max_in_out_bytes)
            {
                max_in_out_bytes = in_out_bytes;
                m_connection_selected = i;
            }
        }
    }
}

void MainWindow::showInfo()
{

}

void MainWindow::OnTimerForNetSpeed()
{
    showInfo();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    ui->m_speedUp->setText("sosishshsihs");
}
