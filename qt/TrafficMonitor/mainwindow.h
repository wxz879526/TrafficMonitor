﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma execution_character_set("UTF-8")

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <windows.h>
#include <iphlpapi.h>

//嵌套结构体，保存一个网络连接的索引和描述等信息
struct NetWorkConection
{
    int index;				//网络索引
    QString description;		//网络描述
    unsigned int in_bytes;	//初始时已接收字节数
    unsigned int out_bytes;	//初始时已发送字节数
    NetWorkConection(int idx, QString desc, unsigned int in_bytes, unsigned out_bytes)
        : index{ idx }, description{ desc }, in_bytes{ in_bytes }, out_bytes{out_bytes}		//构造函数
    {}
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void SetupTray();
    void initConnections();
    void autoSelect();		//自动选择连接
    void showInfo();		//将上传下载速度信息显示到窗口中

private Q_SLOTS:
    void OnTimerForNetSpeed();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayMenu;

    QMenu *m_connectionSubMenu;
    QAction *m_pConnDetailAction;
    QAction *m_pTopMostAction;
    QAction *m_pMouseHackAction;
    QAction *m_pLockWndPosAction;
    QAction *m_pShowTrayNotifyAction;
    QAction *m_pMoreInfoAction;
    QAction *m_pTaskWndAction;
    QAction *m_pHideMainWndAction;
    QAction *m_pSettingsAction;
    QAction *m_pAboutAction;
    QAction *m_pExitAction;

    QPoint m_dragPosition;
    std::vector<NetWorkConection> m_connections;	//保存获取到的要显示到“选择网卡”菜单项中的所有网络连接
    MIB_IFTABLE* m_pIfTable{nullptr};
    DWORD m_dwSize{0};	//m_pIfTable的大小
    int m_connection_selected{ 0 };	//要显示流量的连接的序号
    bool m_auto_select{ false };	//自动选择连接
    unsigned int m_in_bytes;		//当前已接收的字节数
    unsigned int m_out_bytes;	//当前已发送的字节数
    unsigned int m_last_in_bytes{};	//上次已接收的字节数
    unsigned int m_last_out_bytes{};	//上次已发送的字节数
    QString m_connection_name;	//当前选择网络的名称

    int m_restart_cnt{ -1 };	//重新初始化次数
    bool m_connection_change_flag{ false };		//如果执行过IniConnection()函数，该flag会置为true
};

#endif // MAINWINDOW_H
