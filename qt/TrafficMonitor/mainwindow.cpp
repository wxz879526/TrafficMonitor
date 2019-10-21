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
    m_pCloseAction = new QAction(QObject::tr("退出程序"), this);
    connect(m_pCloseAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_trayMenu->addAction(m_pCloseAction);
    m_trayMenu->addSeparator();

    m_trayIcon = new QSystemTrayIcon(this);
    QIcon icon(":/image/notifyicon.ico");
    m_trayIcon->setIcon(icon);
    m_trayIcon->setContextMenu(m_trayMenu);
}
