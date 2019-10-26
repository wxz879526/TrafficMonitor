#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <commonheader.h>
#include <QMainWindow>
#include <QSystemTrayIcon>

//嵌套结构体，保存一个网络连接的索引和描述等信息
struct NetWorkConection
{
    int index;				//网络索引
    QString description;		//网络描述
    unsigned int in_bytes;	//初始时已接收字节数
    unsigned int out_bytes;	//初始时已发送字节数
    NetWorkConection(int idx, QString desc, unsigned int in_bytes, unsigned out_bytes)
        : index(idx)
        , description(desc)
        , in_bytes(in_bytes)
        , out_bytes(out_bytes)		//构造函数
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
    ~MainWindow() override;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

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
    bool m_auto_select{ true };	//自动选择连接
    unsigned int m_in_bytes;		//当前已接收的字节数
    unsigned int m_out_bytes;	//当前已发送的字节数
    unsigned int m_last_in_bytes{};	//上次已接收的字节数
    unsigned int m_last_out_bytes{};	//上次已发送的字节数
    QString m_connection_name;	//当前选择网络的名称

    FILETIME m_preidleTime;
    FILETIME m_prekernelTime;
    FILETIME m_preuserTime;

    int m_restart_cnt{ -1 };	//重新初始化次数
    bool m_connection_change_flag{ false };		//如果执行过IniConnection()函数，该flag会置为true

    //以下数据定义为App类中的公共成员，以便于在主对话框和任务栏窗口中都能访问
    unsigned int m_in_speed{};		//下载速度
    unsigned int m_out_speed{};		//上传速度
    int m_cpu_usage{};		//CPU利用率
    int m_memory_usage{};	//内存利用率
    int m_used_memory{};	//可用物理内存（单位为KB）
    int m_total_memory{};	//物理内存总量（单位为KB）
};

#endif // MAINWINDOW_H
