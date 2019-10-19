#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetupTray();
    m_trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetupTray()
{
    m_trayMenu = new QMenu(this);

    m_trayIcon = new QSystemTrayIcon(this);
    QIcon icon(":/image/notifyicon.ico");
    m_trayIcon->setIcon(icon);
    m_trayIcon->setContextMenu(m_trayMenu);
}
