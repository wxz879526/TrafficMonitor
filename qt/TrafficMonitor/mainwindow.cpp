#include "mainwindow.h"
#include <QMenu>
#include <QPixmap>
#include <QBitmap>
#include <QPalette>
#include <QRegion>
#include <QMouseEvent>
#include <QSettings>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetupTray();
    m_trayIcon->show();

    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    resize(220, 48);

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
    connect(m_pExitAction, &QAction::triggered, qApp, &QCoreApplication::quit);


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

void MainWindow::paintEvent(QPaintEvent *event)
{

}
