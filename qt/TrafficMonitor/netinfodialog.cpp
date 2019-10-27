#include "netinfodialog.h"
#include <QStringList>
#include <QTime>
#include <QDateTime>
#include "formatutils.h"
#include "ui_netinfodialog.h"

NetInfoDialog::NetInfoDialog(MIB_IFROW network_info, int in, int out, QDateTime tm, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetInfoDialog),
    m_network_info(network_info),
    m_in_bytes(in),
    m_out_bytes(out),
    m_start_time(tm)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    QStringList headerTitle;
    headerTitle << "项目" << "值";
    ui->tableWidget->setHorizontalHeaderLabels(headerTitle);

    GetIPAddress();
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
    //ui->tableWidget->setRowCount(10);
    QString temp;

    auto funcAdd = [=](const QString &strKey, const QString &strValue) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(strKey));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(strValue));
    };

    funcAdd(QString(QObject::tr("接口名")), QString::fromStdWString(m_network_info.wszName));
    funcAdd(QString(QObject::tr("接口描述")), QString::fromUtf8((const char*)m_network_info.bDescr));

    switch (m_network_info.dwType)
    {
    case IF_TYPE_OTHER: temp = QString(QObject::tr("其他类型网络")); break;
    case IF_TYPE_ETHERNET_CSMACD: temp = QString(QObject::tr("以太网网络")); break;
    case IF_TYPE_ISO88025_TOKENRING: temp = temp = QString(QObject::tr("令牌环网络")); break;
    case IF_TYPE_FDDI: temp = temp = QString(QObject::tr("光纤分布式数据接口 (FDDI) 网络")); break;
    case IF_TYPE_PPP: temp = temp = QString(QObject::tr("PPP 网络")); break;
    case IF_TYPE_SOFTWARE_LOOPBACK: temp = temp = QString(QObject::tr("软件环回网络")); break;
    case IF_TYPE_ATM: temp = temp = QString(QObject::tr("ATM 网络")); break;
    case IF_TYPE_IEEE80211: temp = temp = QString(QObject::tr("IEEE 802.11 无线网络")); break;
    case IF_TYPE_TUNNEL: temp = temp = QString(QObject::tr("隧道类型封装网络")); break;
    case IF_TYPE_IEEE1394: temp = temp = QString(QObject::tr("IEEE 1394 (Firewire) 高性能串行总线网络")); break;
    case IF_TYPE_IEEE80216_WMAN: temp = temp = QString(QObject::tr("对于 WiMax 设备的移动宽带")); break;
    case IF_TYPE_WWANPP: temp = temp = QString(QObject::tr("基于 GSM 网络设备的移动宽带")); break;
    case IF_TYPE_WWANPP2: temp = temp = QString(QObject::tr("基于 CDMA 的设备移动宽带")); break;
    default: temp = temp = QString(QObject::tr("未知网络")); break;
    }
    funcAdd(QString(QObject::tr("连接类型")), temp);

    //m_info_list.InsertItem(3, _T("最大传输单位大小"));
    //temp.Format(_T("%u"), m_network_info.dwMtu);
    //m_info_list.SetItemText(3, 1, temp);

    temp = QString("%1Mbps").arg(m_network_info.dwSpeed / 1000000.0);
    funcAdd(QString(QObject::tr("速度")), temp);

    temp = QString();
    char buff[3];
    for (int i = 0; i < m_network_info.dwPhysAddrLen; i++)
    {
        //_itoa_s(m_network_info.bPhysAddr[i], buff, 16);
        sprintf_s(buff, "%.2x", m_network_info.bPhysAddr[i]);
        temp.append(buff);
        if (i != m_network_info.dwPhysAddrLen - 1)
            temp.append('-');
    }
    funcAdd(QString(QObject::tr("适配器物理地址")), temp);

    funcAdd(QString(QObject::tr("IP地址")), m_ip_address);

    funcAdd(QString(QObject::tr("子网掩码")), m_subnet_mask);

    funcAdd(QString(QObject::tr("默认网关")), m_default_gateway);

    ////temp.Format(_T("物理地址长度：%d\r\n"), m_network_info.dwPhysAddrLen);
    ////out_info += temp;
    ////temp = _T("适配器物理地址：");
    ////temp += StrToUnicode((const char*)m_network_info.bPhysAddr).c_str();
    ////out_info += temp;
    ////out_info += _T("\r\n");

    //m_info_list.InsertItem(5, _T("管理员状态"));
    //m_info_list.SetItemText(5, 1, m_network_info.dwAdminStatus ? _T("启用") : _T("禁用"));

    switch (m_network_info.dwOperStatus)
    {
    case IF_OPER_STATUS_NON_OPERATIONAL: temp = QString(QObject::tr("LAN 适配器已被禁用")); break;
    case IF_OPER_STATUS_UNREACHABLE: temp = QString(QObject::tr("WAN 适配器未连接")); break;
    case IF_OPER_STATUS_DISCONNECTED: temp = QString(QObject::tr("网络电缆断开连接或无载体")); break;
    case IF_OPER_STATUS_CONNECTING: temp = QString(QObject::tr("WAN 适配器正在连接")); break;
    case IF_OPER_STATUS_CONNECTED: temp = QString(QObject::tr("WAN 适配器连接到远程对等方")); break;
    case IF_OPER_STATUS_OPERATIONAL: temp = QString(QObject::tr("LAN 适配器已连接")); break;
    default: temp = QString(QObject::tr("未知状态")); break;
    }
    funcAdd(QString(QObject::tr("连接状态")), temp);


    temp = QString("%1 (%2)").arg(m_network_info.dwInOctets).arg(FormatUtils::SpeedToString(m_network_info.dwInOctets));
    funcAdd(QString(QObject::tr("已接收字节数")), temp);

    temp = QString("%1 (%2)").arg(m_network_info.dwOutOctets).arg(FormatUtils::SpeedToString(m_network_info.dwOutOctets));
    funcAdd(QString(QObject::tr("已发送字节数")), temp);

    temp = QString("%1 (%2)").arg(m_in_bytes).arg(FormatUtils::SpeedToString(m_in_bytes));
    funcAdd(QString(QObject::tr("自程序启动以来已接收字节数")), temp);

    temp = QString("%1 (%2)").arg(m_out_bytes).arg(FormatUtils::SpeedToString(m_out_bytes));
    funcAdd(QString(QObject::tr("自程序启动以来已发送字节数")), temp);

    QDateTime now = QDateTime::currentDateTime();
    QTime spec(0, 0, 0);
    spec = spec.addSecs(m_start_time.secsTo(now));
    temp = QString("%1小时%2分%3秒").arg(spec.hour()).arg(spec.minute()).arg(spec.second());
    funcAdd(QString(QObject::tr("程序已运行时间")), temp);
}

void NetInfoDialog::on_closeBtn_clicked()
{
    close();
}
