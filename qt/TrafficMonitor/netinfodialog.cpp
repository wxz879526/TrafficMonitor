﻿#include "netinfodialog.h"
#include <QStringList>
#include "ui_netinfodialog.h"

NetInfoDialog::NetInfoDialog(MIB_IFROW network_info, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetInfoDialog),
    m_network_info(network_info)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    QStringList headerTitle;
    headerTitle << "项目" << "值";
    ui->tableWidget->setHorizontalHeaderLabels(headerTitle);

    AddItems();
}

NetInfoDialog::~NetInfoDialog()
{
    delete ui;
}

void NetInfoDialog::GetIPAddress()
{
    PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();		//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
    unsigned long stSize = sizeof(IP_ADAPTER_INFO);		//得到结构体大小,用于GetAdaptersInfo参数
    int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量

    if (ERROR_BUFFER_OVERFLOW == nRel)
    {
        //如果函数返回的是ERROR_BUFFER_OVERFLOW
        //则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
        //这也是说明为什么stSize既是一个输入量也是一个输出量
        delete pIpAdapterInfo;	//释放原来的内存空间
        pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];	//重新申请内存空间用来存储所有网卡信息
        nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);		//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
    }

    PIP_ADAPTER_INFO pIpAdapterInfoHead = pIpAdapterInfo;	//保存pIpAdapterInfo链表中第一个元素的地址
    if (ERROR_SUCCESS == nRel)
    {
        std::string current_network_descr{ (const char*)m_network_info.bDescr };		//MIB_IFROW结构中的当前选中的网络连接的描述
        //获取网卡信息
        //可能有多网卡,因此通过循环来查找当前要显示的网卡
        while (pIpAdapterInfo)
        {
            if(current_network_descr.find(pIpAdapterInfo->Description) != std::string::npos)
            {
                m_ip_address = QString::fromUtf8(pIpAdapterInfo->IpAddressList.IpAddress.String);
                m_subnet_mask = QString::fromUtf8(pIpAdapterInfo->IpAddressList.IpMask.String);
                m_default_gateway = QString::fromUtf8(pIpAdapterInfo->GatewayList.IpAddress.String);
                break;
            }
            pIpAdapterInfo = pIpAdapterInfo->Next;
        }
    }
    //释放内存空间
    if (pIpAdapterInfoHead)
    {
        delete pIpAdapterInfoHead;
    }
}

void NetInfoDialog::AddItems()
{
    ui->tableWidget->setRowCount(10);
    QString temp;

    auto funcAdd = [=](int row, int col, const QString &strValue) {
        ui->tableWidget->setItem(row, col, new QTableWidgetItem(strValue));
    };

    funcAdd(0, 0, QString(QObject::tr("接口名")));
    funcAdd(0, 1, QString::fromStdWString(m_network_info.wszName));

    funcAdd(1, 0, QString(QObject::tr("接口描述")));
    funcAdd(1, 1, QString::fromUtf8((const char*)m_network_info.bDescr));


    /*m_info_list.InsertItem(2, _T("连接类型"));
    switch (m_network_info.dwType)
    {
    case IF_TYPE_OTHER: temp = _T("其他类型网络"); break;
    case IF_TYPE_ETHERNET_CSMACD: temp = _T("以太网网络"); break;
    case IF_TYPE_ISO88025_TOKENRING: temp = _T("令牌环网络"); break;
    case IF_TYPE_FDDI: temp = _T("光纤分布式数据接口 (FDDI) 网络"); break;
    case IF_TYPE_PPP: temp = _T("PPP 网络"); break;
    case IF_TYPE_SOFTWARE_LOOPBACK: temp = _T("软件环回网络"); break;
    case IF_TYPE_ATM: temp = _T("ATM 网络"); break;
    case IF_TYPE_IEEE80211: temp = _T("IEEE 802.11 无线网络"); break;
    case IF_TYPE_TUNNEL: temp = _T("隧道类型封装网络"); break;
    case IF_TYPE_IEEE1394: temp = _T("IEEE 1394 (Firewire) 高性能串行总线网络"); break;
    case IF_TYPE_IEEE80216_WMAN: temp = _T("对于 WiMax 设备的移动宽带"); break;
    case IF_TYPE_WWANPP: temp = _T("基于 GSM 网络设备的移动宽带"); break;
    case IF_TYPE_WWANPP2: temp = _T("基于 CDMA 的设备移动宽带"); break;
    default: temp = _T("未知网络"); break;
    }
    m_info_list.SetItemText(2, 1, temp);

    //m_info_list.InsertItem(3, _T("最大传输单位大小"));
    //temp.Format(_T("%u"), m_network_info.dwMtu);
    //m_info_list.SetItemText(3, 1, temp);

    m_info_list.InsertItem(3, _T("速度"));
    temp.Format(_T("%dMbps"), m_network_info.dwSpeed / 1000000);
    m_info_list.SetItemText(3, 1, temp);

    m_info_list.InsertItem(4, _T("适配器物理地址"));
    temp = _T("");
    char buff[3];
    for (int i{}; i < m_network_info.dwPhysAddrLen; i++)
    {
        //_itoa_s(m_network_info.bPhysAddr[i], buff, 16);
        sprintf_s(buff, "%.2x", m_network_info.bPhysAddr[i]);
        temp += buff;
        if (i != m_network_info.dwPhysAddrLen - 1)
            temp += _T('-');
    }
    m_info_list.SetItemText(4, 1, temp);

    m_info_list.InsertItem(5, _T("IP地址"));
    m_info_list.SetItemText(5, 1, m_ip_address.c_str());

    m_info_list.InsertItem(6, _T("子网掩码"));
    m_info_list.SetItemText(6, 1, m_subnet_mask.c_str());

    m_info_list.InsertItem(7, _T("默认网关"));
    m_info_list.SetItemText(7, 1, m_default_gateway.c_str());

    ////temp.Format(_T("物理地址长度：%d\r\n"), m_network_info.dwPhysAddrLen);
    ////out_info += temp;
    ////temp = _T("适配器物理地址：");
    ////temp += StrToUnicode((const char*)m_network_info.bPhysAddr).c_str();
    ////out_info += temp;
    ////out_info += _T("\r\n");

    //m_info_list.InsertItem(5, _T("管理员状态"));
    //m_info_list.SetItemText(5, 1, m_network_info.dwAdminStatus ? _T("启用") : _T("禁用"));

    m_info_list.InsertItem(8, _T("连接状态"));
    switch (m_network_info.dwOperStatus)
    {
    case IF_OPER_STATUS_NON_OPERATIONAL: temp = _T("LAN 适配器已被禁用"); break;
    case IF_OPER_STATUS_UNREACHABLE: temp = _T("WAN 适配器未连接"); break;
    case IF_OPER_STATUS_DISCONNECTED: temp = _T("网络电缆断开连接或无载体"); break;
    case IF_OPER_STATUS_CONNECTING: temp = _T("WAN 适配器正在连接"); break;
    case IF_OPER_STATUS_CONNECTED: temp = _T("WAN 适配器连接到远程对等方"); break;
    case IF_OPER_STATUS_OPERATIONAL: temp = _T("LAN 适配器已连接"); break;
    default: temp = _T("未知状态"); break;
    }
    m_info_list.SetItemText(8, 1, temp);

    m_info_list.InsertItem(9, _T("已接收字节数"));
    temp.Format(_T("%u (%s)"), m_network_info.dwInOctets, CCommon::DataSizeToString(m_network_info.dwInOctets));
    m_info_list.SetItemText(9, 1, temp);

    m_info_list.InsertItem(10, _T("已发送字节数"));
    temp.Format(_T("%u (%s)"), m_network_info.dwOutOctets, CCommon::DataSizeToString(m_network_info.dwOutOctets));
    m_info_list.SetItemText(10, 1, temp);

    m_info_list.InsertItem(11, _T("自程序启动以来已接收字节数"));
    temp.Format(_T("%u (%s)"), m_in_bytes, CCommon::DataSizeToString(m_in_bytes));
    m_info_list.SetItemText(11, 1, temp);

    m_info_list.InsertItem(12, _T("自程序启动以来已发送字节数"));
    temp.Format(_T("%u (%s)"), m_out_bytes, CCommon::DataSizeToString(m_out_bytes));
    m_info_list.SetItemText(12, 1, temp);

    m_info_list.InsertItem(13, _T("程序已运行时间"));
    SYSTEMTIME current_time, time;
    GetLocalTime(&current_time);
    time = CCommon::CompareSystemTime(current_time, m_start_time);
    temp.Format(_T("%d小时%d分%d秒"), time.wHour, time.wMinute, time.wSecond);
    m_info_list.SetItemText(13, 1, temp);*/
}

void NetInfoDialog::on_closeBtn_clicked()
{
    close();
}
