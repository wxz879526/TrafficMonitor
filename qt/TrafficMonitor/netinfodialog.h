#ifndef NETINFODIALOG_H
#define NETINFODIALOG_H

#include "commonheader.h"
#include <QDateTime>
#include <QDialog>

namespace Ui {
class NetInfoDialog;
}

class NetInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetInfoDialog(MIB_IFROW network_info, int in, int out, QDateTime tm, QWidget *parent = nullptr);
    ~NetInfoDialog();

private:
    void GetIPAddress();	//获取IP地址
    void AddItems();

private slots:
    void on_closeBtn_clicked();

private:
    Ui::NetInfoDialog *ui;
    MIB_IFROW m_network_info;		//当前查看的网络连接信息
    QString m_ip_address;		//IP地址
    QString m_subnet_mask;	//子网掩码
    QString m_default_gateway;	//默认网关

    unsigned int m_in_bytes;		//当前查看的网络自启动以来已接收字节数
    unsigned int m_out_bytes;		//当前查看的网络自启动以来已发送字节数
    QDateTime m_start_time;		//程序启动的时间
};

#endif // NETINFODIALOG_H
