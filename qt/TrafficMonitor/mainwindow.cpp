#include "mainwindow.h"
#include <QMenu>
#include <QPixmap>
#include <QBitmap>
#include <QPalette>
#include <QRegion>
#include <QMouseEvent>
#include <QSettings>
#include <QTimer>
#include "netinfodialog.h"
#include "formatutils.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetupTray();
    initConnections();
    m_trayIcon->show();

    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

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

    /*QSettings configFile(strPath + "\\skins\\0默认皮肤\\skin.ini", QSettings::IniFormat);
    configFile.beginGroup("skin");
    auto color = configFile.value("text_color", 1000).toInt();
    configFile.endGroup();*/

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
    auto pConnDetailAction = new QAction(QObject::tr("连接详情"), this);
    connect(pConnDetailAction, &QAction::triggered, this, [=](){
        NetInfoDialog dlg(m_pIfTable->table[m_connections[m_connection_selected].index], this);
        dlg.exec();
    });
    auto pTopMostAction = new QAction(QObject::tr("总是置顶"), this);

    // 置顶
    pTopMostAction->setCheckable(true);
    connect(pTopMostAction, &QAction::triggered, qApp, [=](){
        auto bChecked = pTopMostAction->isChecked();
        HWND hWnd = reinterpret_cast<HWND>(winId());
        HWND hTargetWnd = nullptr;
        if (bChecked)
        {
            hTargetWnd = reinterpret_cast<HWND>(HWND_TOPMOST);
        }
        else
        {
             hTargetWnd = reinterpret_cast<HWND>(HWND_NOTOPMOST);
        }
        SetWindowPos(hWnd, hTargetWnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    });

    // 鼠标穿透
    m_pMouseHackAction = new QAction(QObject::tr("鼠标穿透"), this);
    m_pMouseHackAction->setCheckable(true);
    connect(m_pMouseHackAction, &QAction::triggered, this, [&](){
        auto bChecked = m_pMouseHackAction->isChecked();
        HWND hWnd = reinterpret_cast<HWND>(winId());
        if (bChecked)
        {
            setWindowOpacity(0.99);
            SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE)|WS_EX_TRANSPARENT);
        }
        else
            SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_TRANSPARENT);
        }
    );
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

    m_trayMenu->addAction(pConnDetailAction);
    m_trayMenu->addSeparator();

    m_trayMenu->addAction(pTopMostAction);
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
    m_trayIcon->setToolTip("流量监控");
}

void MainWindow::initConnections()
{
    //为m_pIfTable开辟所需大小的内存
    free(m_pIfTable);
    m_dwSize = sizeof(MIB_IFTABLE);
    m_pIfTable = static_cast<MIB_IFTABLE *>(malloc(m_dwSize));
    DWORD rtn;
    rtn = GetIfTable(m_pIfTable, &m_dwSize, FALSE);
    if (rtn == ERROR_INSUFFICIENT_BUFFER)	//如果函数返回值为ERROR_INSUFFICIENT_BUFFER，说明m_pIfTable的大小不够
    {
        free(m_pIfTable);
        m_pIfTable = static_cast<MIB_IFTABLE *>(malloc(m_dwSize));	//用新的大小重新开辟一块内存
    }
    //获取当前所有的连接，并保存到m_connections容器中
    m_connections.clear();
    GetIfTable(m_pIfTable, &m_dwSize, FALSE);
    for (unsigned int i{}; i < m_pIfTable->dwNumEntries; i++)
    {
        QString descr = QString(QLatin1String(reinterpret_cast<const char*>(m_pIfTable->table[i].bDescr)));
        if (m_pIfTable->table[i].dwInOctets > 0 || m_pIfTable->table[i].dwOutOctets > 0 || descr == m_connection_name)		//查找接收或发送数据量大于0的连接和上次选择的连接
        {
            m_connections.emplace_back(i, descr, m_pIfTable->table[i].dwInOctets, m_pIfTable->table[i].dwOutOctets);
        }
    }
    if (m_connections.empty())
        m_connections.emplace_back(0, QString(QLatin1String(reinterpret_cast<const char*>(m_pIfTable->table[0].bDescr))), 0, 0);

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
        for (size_t i = 0; i < m_connections.size(); i++)
        {
            if (m_connections[i].description == m_connection_name)
                m_connection_selected = static_cast<int>(i);
        }
    }
    m_connection_name = m_connections[static_cast<size_t>(m_connection_selected)].description;

    //根据已获取到的连接在菜单中添加相应项目
    m_connectionSubMenu->clear();
    QActionGroup *pConnectionsGrp = new QActionGroup(this);
    pConnectionsGrp->setExclusive(true);
    auto autoSelect = new QAction(QObject::tr("自动选择"), pConnectionsGrp);
    autoSelect->setCheckable(true);
    autoSelect->setChecked(true);
    connect(autoSelect, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_connectionSubMenu->addAction(autoSelect);
    //设置“选择网卡”子菜单项
    QString connection_descr;
    for (size_t i = 0; i < m_connections.size(); i++)
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
    for (size_t i = 0; i<m_connections.size(); i++)
    {
        if (m_pIfTable->table[m_connections[i].index].dwOperStatus == IF_OPER_STATUS_OPERATIONAL)		//只选择网络状态为正常的连接
        {
            in_out_bytes = m_pIfTable->table[m_connections[i].index].dwInOctets + m_pIfTable->table[m_connections[i].index].dwOutOctets;
            if (in_out_bytes > max_in_out_bytes)
            {
                max_in_out_bytes = in_out_bytes;
                m_connection_selected = static_cast<int>(i);
            }
        }
    }
}

void MainWindow::showInfo()
{

}

void MainWindow::OnTimerForNetSpeed()
{
    //获取网络连接速度
    GetIfTable(m_pIfTable, &m_dwSize, FALSE);
    size_t index = static_cast<size_t>(m_connection_selected);
    m_in_bytes = m_pIfTable->table[m_connections[index].index].dwInOctets;
    m_out_bytes = m_pIfTable->table[m_connections[index].index].dwOutOctets;

    //如果发送和接收的字节数为0或上次发送和接收的字节数为0或当前连接已改变时，网速无效
    if ((m_in_bytes == 0 && m_out_bytes == 0) || (m_last_in_bytes == 0 && m_last_out_bytes) || m_connection_change_flag)
    {
        m_in_speed = 0;
        m_out_speed = 0;
    }
    else
    {
        m_in_speed = m_in_bytes - m_last_in_bytes;
        m_out_speed = m_out_bytes - m_last_out_bytes;
    }
    //如果大于1GB/s，说明可能产生了异常，网速无效
    if (m_in_speed > 1073741824)
        m_in_speed = 0;
    if (m_out_speed > 1073741824)
        m_out_speed = 0;

    m_connection_change_flag = false;	//清除连接发生变化的标志

    m_last_in_bytes = m_in_bytes;
    m_last_out_bytes = m_out_bytes;

    //获取CPU利用率
    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);

    qint64 idle = FormatUtils::CompareFileTime2(m_preidleTime, idleTime);
    qint64 kernel = FormatUtils::CompareFileTime2(m_prekernelTime, kernelTime);
    qint64 user = FormatUtils::CompareFileTime2(m_preuserTime, userTime);

    if (kernel + user == 0)
    {
        m_cpu_usage = 0;
    }
    else
    {
        //（总的时间-空闲时间）/总的时间=占用cpu的时间就是使用率
        m_cpu_usage = static_cast<int>(abs((kernel + user - idle) * 100 / (kernel + user)));
    }
    m_preidleTime = idleTime;
    m_prekernelTime = kernelTime;
    m_preuserTime = userTime;

    //获取内存利用率
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    m_memory_usage = static_cast<int>(statex.dwMemoryLoad);
    m_used_memory = static_cast<int>((statex.ullTotalPhys - statex.ullAvailPhys) / 1024);
    m_total_memory  = static_cast<int>(statex.ullTotalPhys / 1024);

    QString strSpeedIn = FormatUtils::SpeedToString(m_in_speed);
    ui->m_speedDown->setText(QString(QObject::tr("下载: ")) + strSpeedIn);

    QString strSpeedOut = FormatUtils::SpeedToString(m_out_speed);
    ui->m_speedUp->setText(QString(QObject::tr("上传: ")) + strSpeedOut);

    QString strCPU = QString(" CPU: %1%").arg(m_cpu_usage);
    ui->m_cpuUsage->setText(strCPU);

    QString strMem = QString("内存: %1%").arg(m_memory_usage);
    ui->m_memUsage->setText(strMem);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    m_trayMenu->exec(QCursor::pos());
}
