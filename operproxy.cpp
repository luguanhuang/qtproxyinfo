
#include "operproxy.h"
#include <QtWidgets/QTableWidget>
#include <QtGui>
#include <stdio.h>
#include <time.h> // time_t数据类型，日历时间头文件
#include <QDateTime>
#include <stdio.h>
#include <stdlib.h>
#include "xdb_searcher.h"
#include <QClipboard>
#include <QApplication>
#include <QImage>
#include <QMimeData>
#include <QUrl>
#include <QByteArray>

#pragma comment(lib,"ws2_32.lib")

extern char path[MAX_PATH];
extern heckSocks5 CheckSocks5_;

//IP报头
typedef struct
{
    unsigned char hdr_len:4;         //4位头部长度
    unsigned char version:4;         //4位版本号
    unsigned char tos;               //8位服务类型
    unsigned short total_len;        //16位总长度
    unsigned short identifier;       //16位标识符
    unsigned short frag_and_flags;   //3位标志加13位片偏移
    unsigned char ttl;               //8位生存时间
    unsigned char protocol;          //8位上层协议号
    unsigned short checksum;         //16位效验和
    unsigned long sourceIP;          //32位源IP地址
    unsigned long destIP;            //32位目的IP地址
}IP_HEADER;

//ICMP报头
typedef struct
{
    BYTE type;     //8位类型字段
    BYTE code;     //8位代码字段
    USHORT cksum;  //16位效验和
    USHORT id;     //16位标识符
    USHORT seq;    //16位序列号
}ICMP_HEADER;


//报文解码结构
typedef struct
{
    USHORT usSeqNo;          //序列号
    DWORD dwRoundTripTime;   //返回时间
    in_addr dwIPaddr;        //返回报文的IP地址
}DECODE_RESULT;

//计算网际效验和函数
USHORT checksum(USHORT *pBuf,int iSize)
{
    unsigned long cksum=0;
    while(iSize>1)
    {
        cksum+=*pBuf++;
        iSize-=sizeof(USHORT);
    }
    if(iSize)
    {
        cksum+=*(USHORT*)pBuf;
    }
    cksum=(cksum>>16)+(cksum&0xffff);
    cksum+=(cksum>>16);
    return(USHORT)(~cksum);
}

//对数据包进行解码
BOOL DecodeIcmpResponse(char *pBuf,int iPacketSize,DECODE_RESULT &DecodeResult,BYTE ICMP_ECHO_REPLY,BYTE ICMP_TIMEOUT)
{
    //检查数据报大小的合法性
    IP_HEADER *pIpHdr=(IP_HEADER*)pBuf;
    int iIpHdrLen=pIpHdr->hdr_len*4;
    if(iPacketSize<(int)(iIpHdrLen+sizeof(ICMP_HEADER)))
        return FALSE;
    //根据ICMP报文类型提取ID字段和序列号字段
    ICMP_HEADER *pIcmpHdr=(ICMP_HEADER*)(pBuf+iIpHdrLen);
    USHORT usID,usSquNo;
    if(pIcmpHdr->type==ICMP_ECHO_REPLY)    //ICMP回显应答报文
    {
        usID=pIcmpHdr->id;   //报文ID
        usSquNo=pIcmpHdr->seq;  //报文序列号
    }
    else if(pIcmpHdr->type==ICMP_TIMEOUT)   //ICMP超时差错报文
        {
            char *pInnerIpHdr=pBuf+iIpHdrLen+sizeof(ICMP_HEADER);  //载荷中的IP头
            int iInnerIPHdrLen=((IP_HEADER*)pInnerIpHdr)->hdr_len*4; //载荷中的IP头长
            ICMP_HEADER *pInnerIcmpHdr=(ICMP_HEADER*)(pInnerIpHdr+iInnerIPHdrLen);//载荷中的ICMP头
            usID=pInnerIcmpHdr->id;  //报文ID
            usSquNo=pInnerIcmpHdr->seq;  //序列号
        }else{
            return false;
        }
        //检查ID和序列号以确定收到期待数据报
        if(usID!=(USHORT)GetCurrentProcessId()||usSquNo!=DecodeResult.usSeqNo)
        {
            return false;
        }
        //记录IP地址并计算往返时间
        DecodeResult.dwIPaddr.s_addr=pIpHdr->sourceIP;
        DecodeResult.dwRoundTripTime=GetTickCount()-DecodeResult.dwRoundTripTime;

        //处理正确收到的ICMP数据报
        if(pIcmpHdr->type==ICMP_ECHO_REPLY||pIcmpHdr->type==ICMP_TIMEOUT)
        {
            return true;
        }else{
            return false;
        }
        return true;
}

COperProxy::COperProxy(QObject *parent):QObject(parent)
{

}

COperProxy::~COperProxy()
{

}

void fillIcmpData(char *icmpData, int dataSize);

// IP数据包头结构
typedef struct iphdr
{
    unsigned int headLen:4;
    unsigned int version:4;
    unsigned char tos;
    unsigned short totalLen;
    unsigned short ident;
    unsigned short fragAndFlags;
    unsigned char ttl;
    unsigned char proto;
    unsigned short checkSum;
    unsigned int sourceIP;
    unsigned int destIP;
}IpHeader;

// ICMP数据头结构
typedef struct ihdr
{
    unsigned char iType;
    unsigned char iCode;
    unsigned short iCheckSum;
    unsigned short iID;
    unsigned short iSeq;
    unsigned long  timeStamp;
}IcmpHeader;

// 计算ICMP包的校验和(发送前要用)
unsigned short checkSum(unsigned short *buffer, int size)
{
    unsigned long ckSum = 0;

    while(size > 1)
    {
        ckSum += *buffer++;
        size -= sizeof(unsigned short);
    }

    if(size)
    {
        ckSum += *(unsigned char*)buffer;
    }

    ckSum = (ckSum >> 16) + (ckSum & 0xffff);
    ckSum += (ckSum >>16);
    unsigned short result = ~ckSum;
    return result;
//    return unsigned short(~ckSum);

}


// 对返回的IP数据包进行解析，定位到ICMP数据
int decodeResponse(char *buf, int bytes, struct sockaddr_in *from, int tid)
{
    IpHeader *ipHead = (IpHeader *)buf;
    unsigned short ipHeadLen = ipHead->headLen * 4 ;
//    qDebug() << "bytes=" << bytes << " ipHeadLen + 8=" << ipHeadLen + 8;
    if (bytes < ipHeadLen + 8) // ICMP数据不完整, 或者不包含ICMP数据
    {
        return -1;
    }

    IcmpHeader *icmpHead = (IcmpHeader*)(buf + ipHeadLen);  // 定位到ICMP包头的起始位置
//    qDebug() << "iType=" << icmpHead->iType;
    if (icmpHead->iType != 0)   // 0表示回应包
    {
        return -2;
    }

//    qDebug() << "iID=" << icmpHead->iID << " tid=" << tid;
    if (icmpHead->iID != (unsigned short)tid) // 理应相等
    {
        return -3;
    }

    int time = GetTickCount() - (icmpHead->timeStamp); // 返回时间与发送时间的差值
//    qDebug() << "time=" << time;
    if(time >= 0)
    {
        return time;
    }

    return -4; // 时间错误
}

// ping操作
int ping(const char *ip, unsigned int timeout)
{
    // 网络初始化
    unsigned int sockRaw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);  // 注意，第三个参数非常重要，指定了是icmp
    setsockopt(sockRaw, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));  // 设置套接字的接收超时选项
    setsockopt(sockRaw, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));  // 设置套接字的发送超时选项

    // 准备要发送的数据
    int  dataSize = sizeof(IcmpHeader) + 32; // 待会儿会有32个x
    char icmpData[1024] = {0};
//    qDebug() << "ping 11";
    fillIcmpData(icmpData, dataSize);
    unsigned long startTime = ((IcmpHeader *)icmpData)->timeStamp;
//    qDebug() << "ping 12";
    // 远程通信端
    struct sockaddr_in dest;

    memset(&dest, 0, sizeof(dest));
    struct hostent *hp = gethostbyname(ip);
    if (NULL == hp)
    {
//        qDebug() << "ping ip=" << ip;
        dest.sin_family = AF_INET;
//        dest.sin_port = 0;
        dest.sin_addr.s_addr =inet_addr(ip); //for XP
//        if (inet_pton(ip, (struct in_addr*)&dest.sin_addr.s_addr) == 0)
//        {
//            return -1;
//        };

        qDebug() << "ping null ip=" << ip;
//        memcpy(&(dest.sin_addr), ip, strlen(ip));
//         dest.sin_family = hp->h_addrtype;
    }
    else
    {
        memcpy(&(dest.sin_addr), hp->h_addr, hp->h_length);
         dest.sin_family = hp->h_addrtype;
//        qDebug() << "ping not null";
    }

    // 发送数据
    int ret = sendto(sockRaw, icmpData, dataSize, 0, (struct sockaddr*)&dest, sizeof(dest));
    if (ret < 0)
        return ret;

    int iRet = -1;
    struct sockaddr_in from;
    int fromLen = sizeof(from);

    while(1)
    {
        char recvBuf[1024] = {0};
        int iRecv = recvfrom(sockRaw, recvBuf, 1024, 0, (struct sockaddr*)&from, &fromLen);
        int time  = decodeResponse(recvBuf, iRecv, &from, GetCurrentThreadId());
//        qDebug() << "iRecv=" << iRecv;
        if(time >= 0)
        {
            iRet = 0;   // ping ok
            break;
        }
        else if( GetTickCount() - startTime >= timeout || GetTickCount() < startTime)
        {

            iRet = -1;  // ping超时
            break;
        }

        break;
    }


    // 释放
    closesocket(sockRaw);
//    WSACleanup();

    return iRet;
}

// ping操作
int ping1(const char *ip, unsigned int timeout)
{
    u_long ulDestIP=inet_addr(ip);
    //转换不成功时按域名解析
    if(ulDestIP==INADDR_NONE)
    {
        hostent *pHostent=gethostbyname(ip);
        if(pHostent)
        {
            ulDestIP=(*(in_addr*)pHostent->h_addr).s_addr;
        }
        else
        {
            qDebug()<<"valid";
//            WSACleanup();
            return 0;
        }
    }
    else
    {
        qDebug() << "ok";
    }

    qDebug()<<"正在 Ping "<<ip<<"具有 32 字节的数据：";
    //填充目的端socket地址
    sockaddr_in destSockAddr;
    ZeroMemory(&destSockAddr,sizeof(sockaddr_in));
    destSockAddr.sin_family=AF_INET;
    destSockAddr.sin_addr.s_addr=ulDestIP;
    //创建原始套接字
    SOCKET sockRaw=WSASocket(AF_INET,SOCK_RAW,IPPROTO_ICMP,NULL,0,WSA_FLAG_OVERLAPPED);
    //超时时间
    int iTimeout=3000;
    //接收超时
    setsockopt(sockRaw,SOL_SOCKET,SO_RCVTIMEO,(char*)&iTimeout,sizeof(iTimeout));
    //发送超时
    setsockopt(sockRaw,SOL_SOCKET,SO_SNDTIMEO,(char*)&iTimeout,sizeof(iTimeout));


    //构造ICMP回显请求消息，并以TTL递增的顺序发送报文
    //ICMP类型字段
    const BYTE ICMP_ECHO_REQUEST=8;   //请求回显
    const BYTE ICMP_ECHO_REPLY=0;     //回显应答
    const BYTE ICMP_TIMEOUT=11;       //传输超时

    //其他常量定义
    const int DEF_ICMP_DATA_SIZE=32;    //ICMP报文默认数据字段长度
    const int MAX_ICMP_PACKET_SIZE=1024; //ICMP报文最大长度（包括报头）
    const DWORD DEF_ICMP_TIMEOUT=3000;   //回显应答超时时间
    const int DEF_MAX_HOP=4;            //最大跳站数

    //填充ICMP报文中每次发送时不变的字段
    char IcmpSendBuf[sizeof(ICMP_HEADER)+DEF_ICMP_DATA_SIZE]; //发送缓冲区
    memset(IcmpSendBuf,0,sizeof(IcmpSendBuf));     //初始化发送缓冲区
    char IcmpRecvBuf[MAX_ICMP_PACKET_SIZE];        //接收缓冲区
    memset(IcmpRecvBuf,0,sizeof(IcmpRecvBuf));     //初始化接收缓冲区

    ICMP_HEADER *pIcmpHeader=(ICMP_HEADER*)IcmpSendBuf;
    pIcmpHeader->type=ICMP_ECHO_REQUEST;             //类型为请求回显
    pIcmpHeader->code=0;                             //代码字段为0
    pIcmpHeader->id=(USHORT)GetCurrentProcessId();   //ID字段为当前进程号
    memset(IcmpSendBuf+sizeof(ICMP_HEADER),'E',DEF_ICMP_DATA_SIZE);   //数据字段
    USHORT usSeqNo=0;                //ICMP报文序列号
    int iTTL=64;                      //TTL初始值为1
    int r=0,o=0;
    BOOL bReachDestHost=FALSE;       //循环退出标志
    int iMaxHot=DEF_MAX_HOP;         //循环的最大次数
    DECODE_RESULT DecodeResult;      //传递给报文解码函数的结构化参数
    int totalms = 0;
    while(!bReachDestHost&&iMaxHot--)
    {
        //设置IP报头的TTL字段
        setsockopt(sockRaw,IPPROTO_IP,IP_TTL,(char*)&iTTL,sizeof(iTTL));
        //填充ICMP报文中每次发送变化的字段
        ((ICMP_HEADER*)IcmpSendBuf)->cksum=0;                 //效验和先置为0
        ((ICMP_HEADER*)IcmpSendBuf)->seq=htons(usSeqNo++);    //填充序列号
        ((ICMP_HEADER*)IcmpSendBuf)->cksum=checksum((USHORT*)IcmpSendBuf,sizeof(ICMP_HEADER)+DEF_ICMP_DATA_SIZE);  //计算效验和

        //记录序列号和当前时间
        DecodeResult.usSeqNo=((ICMP_HEADER*)IcmpSendBuf)->seq;    //当前序号
        DecodeResult.dwRoundTripTime=GetTickCount();              //当前时间
        //发送TCP回显请求信息
        int ret = sendto(sockRaw,IcmpSendBuf,sizeof(IcmpSendBuf),0,(sockaddr*)&destSockAddr,sizeof(destSockAddr));
//        qDebug() << "ret=" << ret << " err=" << WSAGetLastError();
        //接收ICMP差错报文并进行解析处理
        sockaddr_in from;              //对端socket地址
        int iFromLen=sizeof(from);     //地址结构大小
        int iReadDataLen;              //接收数据长度

        while(1)
        {
            //接收数据
            iReadDataLen=recvfrom(sockRaw,IcmpRecvBuf,MAX_ICMP_PACKET_SIZE,0,(sockaddr*)&from,&iFromLen);
            if(iReadDataLen!=SOCKET_ERROR)//有数据达到
            {
                //对数据包进行解码
                if(DecodeIcmpResponse(IcmpRecvBuf,iReadDataLen,DecodeResult,ICMP_ECHO_REPLY,ICMP_TIMEOUT))
                {
                    //到达目的地，退出循环
                    if(DecodeResult.dwIPaddr.s_addr==destSockAddr.sin_addr.s_addr){
                        //输出IP地址
                        if(DecodeResult.dwRoundTripTime)
                        {
                            totalms += DecodeResult.dwRoundTripTime;
                               qDebug()<<"来自 "<<inet_ntoa(DecodeResult.dwIPaddr)<<" 的回复：字节32 时间="<<DecodeResult.dwRoundTripTime<<"ms TTL:"<<iTTL<<endl;
                         }

                        else
                             qDebug()<<"来自 "<<inet_ntoa(DecodeResult.dwIPaddr)<<" 的回复：字节32 时间< 1ms TTL:"<<iTTL<<endl;
                        r++;
                        break;
                    }

                }
            }
            else if(WSAGetLastError()==WSAETIMEDOUT) //接收超时，输出星号
            {
                qDebug()<<"timeout";
                o++;
                break;
            }
            else{
                break;
            }
        }
    }

        qDebug() << "ping1=" << totalms << "r=" << r << " res=" << totalms / r;
   return totalms / r;

//    qDebug() << ip << " ping stat:";
    qDebug() << "packet：send=4，recv=" << r << "，loss=" << o;

//    qDebug()<<"packet：send=4，recv="<<r<<"，loss="<<o;
    return 0;
}

// 填充ICMP请求包的具体参数
void fillIcmpData(char *icmpData, int dataSize)
{
    IcmpHeader *icmpHead = (IcmpHeader*)icmpData;
    icmpHead->iType = 8;  // 8表示请求包
    icmpHead->iCode = 0;
    icmpHead->iID = (unsigned short)GetCurrentThreadId();
    icmpHead->iSeq = 0;
    icmpHead->timeStamp = GetTickCount();
    char *datapart = icmpData + sizeof(IcmpHeader);
    memset(datapart, 'x', dataSize - sizeof(IcmpHeader)); // 数据部分为xxx..., 实际上有32个x
    icmpHead->iCheckSum = checkSum((unsigned short*)icmpData, dataSize); // 千万要注意，这个一定要放到最后
}

char * ResolverDomainIP(const char * pszName)  //域名解析IP
{
    qDebug() << "ResolverDomainIP=" << pszName;
//    struct hostent *h;
    HOSTENT* h=gethostbyname(pszName);
//    h = gethostbyname(pszName);
      if(h == NULL)
      {
          qDebug()<<"get hostbyname error!";
          exit(1);
      }

      struct in_addr *in=(in_addr *)h->h_addr;
      char *ch = inet_ntoa(*in);
      qDebug() << "ch=[" << ch << "]";
        return ch;
}

QString COperProxy::pingDetect(QString ip)
{
//    qDebug() << "pingDetect func begin";

    int succeedcnt = 0;
    for (int i=0; i<1; i++)
    {

        int iRet = ping1(ip.toStdString().c_str(), 1500);
//        if(0 == iRet)
//         {

////             printf("pingDetect %s ONLINE\n", ip.toStdString().c_str());
//            succeedcnt++;
//         }
//         else if(-1 == iRet && i==0)
//         {

////             printf("pingDetect %s TIMEOUT\n", ip.toStdString().c_str());
//             return "-1";
//         }
//         else
//         {
//             printf("pingDetect Unkonw");
//             return "-2";
//         }

        float rate = (float)(4-succeedcnt) / 4;
//        QString strDouble = QString::number(rate, 'f', 1);
        QString strDouble = QString::number(iRet);
        return strDouble;
    }

//    float rate = (float)(4-succeedcnt) / 4;
//    QString strDouble = QString::number(rate, 'f', 2);

//    QString res = QString::fromLocal8Bit("丢包率: ");
//     QString test = res+strDouble+"%%";


}

QString getYMD(int Mo)
{
    QDate* thisDate = new QDate();
    int year = thisDate->currentDate().year();
    int month = thisDate->currentDate().month();
    int day = thisDate->currentDate().day();

    QDateTime datetime = QDateTime::currentDateTime();

    int returnYear = 0;
    int returnMonth = 0;
    int returnDay = 0;

    //首先得到年
//    returnYear = year+((month+Mo)/12);
    returnYear = year+((month+Mo)/13);
    //然后得到月
//    returnMonth = (month+Mo)/12 == 0 ? month+Mo : (month+Mo)%12;
    returnMonth = (month+Mo)/13 == 0 ? month+Mo : (month+Mo)%13;
    //最后得到日
    int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if((returnYear % 4 == 0 && returnYear % 100 != 0) || (returnYear % 400 == 0) )
    {
        //闰年2月有29
        days[1] = 29;
    }
    else
    {
        //平年2月有28
        days[2] = 28;
    }
    //当前月的天数
    int thisMD = days[month-1];
    if(day==thisMD || day>=days[returnMonth-1]){
        returnDay = days[returnMonth-1];
    }
    else
    {
        returnDay = day;
    }

    char buf[64];

    QString result;
//    result = QString::number(returnYear).append(QString::number(returnMonth)).append(QString::number(returnDay));
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d", returnYear, returnMonth, returnDay,
             datetime.time().hour(), datetime.time().minute(), datetime.time().second());
    result = buf;
    return result;
}

QString COperProxy::getselcomm(QString strcomm)
{
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"File open failed";
    QTextStream in(&fileread);
    QString strSel = "";
    while(!in.atEnd())
    {
        QString lines=in.readLine();

        QStringList oneline=lines.split("|");
//        qDebug()<<"id=" << id << " oneline=" << oneline[0];
        if (strcomm == oneline[14].toLocal8Bit().data())
        {
            strSel += oneline[0].toLocal8Bit().data();
            strSel += ",";
        }
    }

    return strSel;
}

QString COperProxy::delAll()
{
    QString strfillpath = path;
    strfillpath += "\\proxyinfo.txt";
    QFile file(strfillpath);
    file.remove();
    return "删除全部代码完成, 共计5";
}

QString COperProxy::delRepeatRows(QString rows)
{
    QVector <QString> vecstr;
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"File open failed";
    QTextStream in(&fileread);
    QString strSel = "";
    QMap <QString, QString> mapstr;
    while(!in.atEnd())
    {
        QString lines=in.readLine();
        vecstr.push_back(lines);
    }

    QString str = vecstr[0];

    for (int i=0; i<vecstr.size(); i++)
    {
        for (int j=0; j<vecstr.size(); j++)
        {
//            if (str == )
        }
    }

    for (auto data:vecstr)
    {

        qDebug() << "data=" << data;
    }

    fileread.close();
//    return strSel;
//    QStringList oneline=rows.split(",");
//    for (int i=0; i<oneline.count(); i++)
//    {
//        qDebug() << "id=" << oneline[i];
//    }
    return "";
}

QString COperProxy::getseltypes(QString name)
{
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"File open failed";
    QTextStream in(&fileread);
    QString strSel = "";
    while(!in.atEnd())
    {
        QString lines=in.readLine();

        QStringList oneline=lines.split("|");
//        qDebug()<<"id=" << id << " oneline=" << oneline[0];
        if (name == oneline[6].toLocal8Bit().data())
        {
            strSel += oneline[0].toLocal8Bit().data();
            strSel += ",";
        }
    }

    fileread.close();
    return strSel;
}

QString COperProxy::getseldeadline(QString deadline)
{
    QString strfillpath = path;
    strfillpath += "\\proxyinfo.txt";
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"File open failed";
    QTextStream in(&fileread);
    QString strSel = "";
    while(!in.atEnd())
    {
        QString lines=in.readLine();

        QStringList oneline=lines.split("|");
         qDebug()<<"deadline="+deadline;
        if (strstr(deadline.toLatin1().data(), " ") == NULL)
        {
            QString strbegin = deadline + " 00:00:00";
            QString strend = deadline + " 23:59:59";

            qDebug()<<"strbegin="+strbegin+" strend="+strend+" value="+oneline[5];
            if (oneline[5] >= strbegin && oneline[5] <= strend)
            {
                strSel += oneline[0].toLocal8Bit().data();
                strSel += ",";
            }
        }
        else
        {
            if (deadline == oneline[5].toLocal8Bit().data())
            {
                strSel += oneline[0].toLocal8Bit().data();
                strSel += ",";
            }
        }

    }

    return strSel;
}

QString COperProxy::addProxy(QString proxyInfo)
{
    qDebug() << "addProxy="+proxyInfo;
    QString strfillpath = path;
    strfillpath += "\\proxyinfo.txt";
    int idx = 1;
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"File open failed";
    }
    else
    {
        QFile fileread(strfillpath);
        if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
            qDebug()<<"File open failed";
        QTextStream in(&fileread);



        QString dev;
        QString lines = "";
        while(!in.atEnd())
        {
            lines=in.readLine();
        }

        qDebug() << "lines111=" << lines;
        if (lines != "")
        {
            QStringList strlist=lines.split("|");
            if (strlist.size() > 0)
            {
                idx = atoi(strlist[0].toUtf8().data());

                idx++;
                 qDebug() << "idx11221=" << idx;
            }

        }

    }

    fileread.close();

    QFile file(strfillpath);

    bool isok=file.open(QIODevice::ReadWrite | QIODevice::Append);//利用只读模式打开文件
    if(!isok)
    {
        qDebug() << "write error";
    }

    QVector<int> vecint;
    QStringList oneline=proxyInfo.split("|");
    QString strtmp = oneline[0] + "|" + oneline[1] + "|" + oneline[2] + "|" + oneline[3]
            + "|" + oneline[5] + "|" + "SOCKS5" + "|"
            +  "|" +  "|" + "|"
            + oneline[4] + "|" + "1" + "|"
            + oneline[6] + "|" + "1" + "|"+oneline[7]+"\n";
    QString stridx = QString::number(idx);
    QString strwrite = stridx + "|" + strtmp;


    file.write(strwrite.toStdString().data());//c_str
//    for(int i=0; i < oneline.size(); i++)
//    {
//        QStringList resitem=oneline[i].split("|");
//        qDebug() << "resitemsize=" << resitem.size();
//        int ret = resitem.size();
//        if (ret < 5)
//        {
//            QString strmon = getYMD(6);
//            oneline[i] += "|";
//            oneline[i] += strmon;
//            qDebug() << "strmon=" << strmon;


//            qDebug() << "fullstr=" << oneline[i];
//        }
//        else
//        {
//            if (strstr(resitem[4].toLatin1().data(), " ") == NULL)
//            {
//                char buf[64];
//                QDateTime datetime = QDateTime::currentDateTime();
//                snprintf(buf, sizeof(buf), " %02d:%02d:%02d", datetime.time().hour(), datetime.time().minute(), datetime.time().second());
//                oneline[i] += " 00:00:00";
//                qDebug() << "dont have";

//            }
//            else
//            {
//                qDebug() << "have";
//            }
//        }

//        QString stridx = QString::number(idx);
//        qDebug() << "stridx=" << idx;
//        qDebug() << "stridx111=" << stridx;

//        QString strline = stridx +"|"+oneline[i];
//        strline += "|SOCKS5|||-1|";
//        strline = strline + assignable + "|";
//        strline = strline +  "0|ALLNAME;ALLMD5;|1|"+comment+"\n";
//        qDebug() << "strline="+strline;
//        file.write(strline.toStdString().data());//c_str
//        vecint.push_back(idx);
//        idx++;
//    }

    file.close();//关闭文件
    CopyFileThread * m_cpyThread;
    m_cpyThread = new CopyFileThread(strtmp, this, idx);
    m_cpyThread->start();
//    QString strres = "";//"批量添加代理完成,共计"+strcnt+",存储到数据库成功"+strcnt+",添加成功"+strcnt;
//    CopyFileThread * m_cpyThread;
//    // mainwindow.cpp中
//    oneline=proxyInfo.split("\n");
//    for(int i=0; i < oneline.size(); i++)
//    {
//        m_cpyThread = new CopyFileThread(oneline[i], this, vecint[i]);
//        m_cpyThread->start();
////        QStringList resitem=oneline[i].split("|");

////        for(int j=0; j < resitem.size(); j++)
////        {
//////            qDebug() << "data =" << resitem[j];
////            vecdata.push_back(resitem[j]);
////            if (j == 3)
////            {
////                break;
////            }
////        }

////        qDebug() << "resitemsize=" << resitem.size();
////        int ret = resitem.size();
////        if (ret < 5)
////        {

////        }
//    }

////    for (auto data:vecdata)
////    {
//////        qDebug() << "datainfo =" << data;
////    }

    QString strres = "添加代理成功";//"批量添加代理完成,共计"+strcnt+",存储到数据库成功"+strcnt+",添加成功"+strcnt;
    return strres;
}

QString COperProxy::batAddProxy(QString proxyInfo, QString assignable, QString comment)
{
    QString strfillpath = path;
    strfillpath += "\\proxyinfo.txt";
    qDebug() << "proxyInfo="+proxyInfo;
    qDebug() << "assignable="+assignable;
    qDebug() << "comment="+comment;
    QStringList oneline=proxyInfo.split("\n");
    int cnt = oneline.size();
    QString strcnt = QString::number(cnt);
    int idx = 1;
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"File open failed";
    }
    else
    {
        QFile fileread(strfillpath);
        if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
            qDebug()<<"File open failed";
        QTextStream in(&fileread);



        QString dev;
        QString lines = "";
        while(!in.atEnd())
        {
            lines=in.readLine();
        }

        qDebug() << "lines111=" << lines;
        if (lines != "")
        {
            QStringList strlist=lines.split("|");
            if (strlist.size() > 0)
            {
                idx = atoi(strlist[0].toUtf8().data());

                idx++;
                 qDebug() << "idx11221=" << idx;
            }

        }

    }

    fileread.close();

    QFile file(strfillpath);

    bool isok=file.open(QIODevice::ReadWrite | QIODevice::Append);//利用只读模式打开文件
    if(!isok)
    {
        qDebug() << "write error";
    }


//    QStringList oneline=proxyInfo.split("\n");
    QVector<int> vecint;
    for(int i=0; i < oneline.size(); i++)
    {
        QStringList resitem=oneline[i].split("|");
        qDebug() << "resitemsize=" << resitem.size();
        int ret = resitem.size();
        if (ret < 5)
        {
            QString strmon = getYMD(6);
            oneline[i] += "|";
            oneline[i] += strmon;
            qDebug() << "strmon=" << strmon;


            qDebug() << "fullstr=" << oneline[i];
        }
        else
        {
            if (strstr(resitem[4].toLatin1().data(), " ") == NULL)
            {
                char buf[64];
                QDateTime datetime = QDateTime::currentDateTime();
                snprintf(buf, sizeof(buf), " %02d:%02d:%02d", datetime.time().hour(), datetime.time().minute(), datetime.time().second());
                oneline[i] += " 00:00:00";
                qDebug() << "dont have";

            }
            else
            {
                qDebug() << "have";
            }
        }

        QString stridx = QString::number(idx);
        qDebug() << "stridx=" << idx;
        qDebug() << "stridx111=" << stridx;

        QString strline = stridx +"|"+oneline[i];
        strline += "|SOCKS5|||-1|";
        strline = strline + assignable + "|";
        strline = strline +  "0|ALLNAME;ALLMD5;|1|"+comment+"\n";
        qDebug() << "strline="+strline;
        file.write(strline.toStdString().data());//c_str
        vecint.push_back(idx);
        idx++;
    }

    file.close();//关闭文件
    QString strres = "批量添加代理完成,共计"+strcnt+",存储到数据库成功"+strcnt+",添加成功"+strcnt;
    CopyFileThread * m_cpyThread;
    // mainwindow.cpp中
    oneline=proxyInfo.split("\n");
    for(int i=0; i < oneline.size(); i++)
    {
        m_cpyThread = new CopyFileThread(oneline[i], this, vecint[i]);
        m_cpyThread->start();
//        QStringList resitem=oneline[i].split("|");

//        for(int j=0; j < resitem.size(); j++)
//        {
////            qDebug() << "data =" << resitem[j];
//            vecdata.push_back(resitem[j]);
//            if (j == 3)
//            {
//                break;
//            }
//        }

//        qDebug() << "resitemsize=" << resitem.size();
//        int ret = resitem.size();
//        if (ret < 5)
//        {

//        }
    }

//    for (auto data:vecdata)
//    {
////        qDebug() << "datainfo =" << data;
//    }


    return strres;
//    return "";
}

QString COperProxy::readProxyConf()
{
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";
    qDebug() << "strfillpath=" << strfillpath;
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"File open failed";
    QTextStream in(&fileread);

    int idx = 0;
    int id = 0;
    QString dev;

    while(!in.atEnd())
    {
        QJsonObject pageObject;
        QString lines=in.readLine();

        QStringList oneline=lines.split("|");
        pageObject.insert("id", oneline[0].toLocal8Bit().data());
        pageObject.insert("ip", oneline[1].toLocal8Bit().data());
        pageObject.insert("port", oneline[2].toLocal8Bit().data());
        pageObject.insert("account", oneline[3].toLocal8Bit().data());
        pageObject.insert("passwd", oneline[4].toLocal8Bit().data());
        pageObject.insert("deadline", oneline[5].toLocal8Bit().data());
        pageObject.insert("type", oneline[6].toLocal8Bit().data());
        QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
        QString strpos = oneline[7];
        strpos = utf8->toUnicode(strpos.toLocal8Bit().data());
        pageObject.insert("pos", oneline[7].toLocal8Bit().data());
//        pageObject.insert("pos", strpos.toLocal8Bit().data());
        pageObject.insert("line", oneline[8].toLocal8Bit().data());
        pageObject.insert("delay", oneline[9].toLocal8Bit().data());
//        pageObject.insert("comment", oneline[10].toLocal8Bit().data());

        pageObject.insert("canassigned", oneline[10].toLocal8Bit().data());

        pageObject.insert("alreadyassigned", oneline[11].toLocal8Bit().data());
        pageObject.insert("process", oneline[12].toLocal8Bit().data());
        pageObject.insert("status", oneline[13].toLocal8Bit().data());
//        pageObject.insert("canassigned", oneline[14].toLocal8Bit().data());
        pageObject.insert("comment", oneline[14].toLocal8Bit().data());

//        pageObject.insert("size", size);
//        for (int i=0; i<size; i++)
//        {
//            qDebug() << "data=" << oneline[i];
//        }
//        qDebug() << "oneline=" << lines;
        dev = "proxy" + QString::number(idx);

        json.insert(dev, QJsonValue(pageObject));
        idx++;
        //           stu_lines.append(lines);
    }

    fileread.close();
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
//    qDebug() << "data="+strJson;
    return strJson;
}

int GetIpInfo(QString &strip, QString &strdata)
{
    char path[1024] = { 0 };
    GetModuleFileNameA(NULL, path, MAX_PATH);
    *strrchr(path, '\\') = '\0';                    // 截取路径，如：E:\Tools

    char resdata[256];
    snprintf(resdata, sizeof(resdata), "%s\\data\\ip2region.xdb", path);
    qDebug() << "resdata=" << resdata;
    xdb_searcher_t searcher;
    char region_buffer[256];
    //*ip = (char *)"117.80.235.145";
    int res = test();
    qDebug() << "res=" << res;
    // 1、从 db_path 初始化 xdb 查询对象
    int err = xdb_new_with_file_only(&searcher, resdata);
    if (err != 0)
    {
        printf("failed to create xdb searcher from `%s` with errno=%d\n", resdata, err);
        return -1;
    }

    // 2、调用 search API 查询
    // 得到的 region 信息会存储到 region_buffer 里面，如果你自定义了数据，请确保给足 buffer 的空间。
//    s_time = xdb_now();
    err = xdb_search_by_string(&searcher, strip.toStdString().c_str(), region_buffer, sizeof(region_buffer));
    if (err != 0)
    {
        printf("failed search(%s) with errno=%d\n", strip.toStdString().c_str(), err);
        return -1;
    }
    else
    {
    //      printf("{region: %s, took: %d μs}", region_buffer, (int)(xdb_now() - s_time));
//            qDebug() << "region_buffer=" << region_buffer;
            strdata = region_buffer;
    }

    // 备注：并发使用，没一个线程需要单独定义并且初始化一个 searcher 查询对象。

    // 3、关闭 xdb 查询器
    xdb_close(&searcher);
    return 0;
}

int COperProxy::checkSocks5Tcp(QString ip, QString port, QString name, QString passwd)
{

    qDebug() << "ip=[" << ip.toStdString().c_str()
             << "] port=[" << port.toStdString().c_str()
             << "] name=[" << name.toStdString().c_str()
             << "] passwd=[" << passwd.toStdString().c_str() << "]";
    SOCKADDR_IN  sa_1;
    sa_1.sin_family=AF_INET;
    sa_1.sin_addr.s_addr=inet_addr(ip.toStdString().c_str());

    if(sa_1.sin_addr.s_addr==INADDR_NONE)
    {
        qDebug() << "checkSocks5Tcp: aaa";
        struct hostent *p=gethostbyname(ip.toStdString().c_str());
        if(p==NULL)
        {
            qDebug() << "checkSocks5Tcp: aab";
            return -1;
        }

        qDebug() << "checkSocks5Tcp: aac";
        struct in_addr *in=(in_addr *)p->h_addr;
        char *ch = inet_ntoa(*in);
        qDebug() << "checkSocks5Tcp: ch=[" << ch << "]";
        ip = ch;

//        qDebug() << "checkSocks5Tcp: 111";
    }

    char *OutSuccessIP = NULL;
    qDebug() << "checkSocks5Tcp: 112";
    if (CheckSocks5_((char *)ip.toStdString().c_str(), atoi(port.toStdString().c_str()), (char *)name.toStdString().c_str(), (char *)passwd.toStdString().c_str(), OutSuccessIP))
    {
//        qDebug() << "CheckSocks5_ succeed ipinfo=" << ipinfo;
         qDebug() << "checkSocks5Tcp succeed";
         return 0;
    }
    else
    {
//        qDebug() << "ip1111=[" << ip.toStdString().c_str()
//                 << "] port=[" << port.toStdString().c_str()
//                 << "] name=[" << name.toStdString().c_str()
//                 << "] passwd=[" << passwd.toStdString().c_str() << "]";
        qDebug() << "checkSocks5Tcp failure";
         return -1;
    }
}

QString COperProxy::getIpInfp(QString ip)
{
    QString ipinfo;
    if (GetIpInfo(ip, ipinfo))
    {
        return "";
    }

    qDebug() << "getIpInfp ip=" << ip;
    return ipinfo;
}

QString COperProxy::chgCanAssigned(int id, QString data)
{
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";
    //    qDebug() << "strfillpath=" << strfillpath;
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly))
      qDebug()<<"File open failed";
    QTextStream in(&fileread);
    QString dev;
    QString strres = "";

    while(!in.atEnd())
    {
      QJsonObject pageObject;
      QString lines=in.readLine();
      QString strtmp = lines;
      QStringList oneline=lines.split("|");
      int resid = atoi(oneline[0].toLocal8Bit().data());
      if (resid == id)
      {
          qDebug() << "enableproxy i find it lines=" << lines;
          qDebug() << "status=" << oneline[13].toLocal8Bit().data();

         QString strline = oneline[0] +"|"+oneline[1]
                             +"|"+oneline[2] +"|"+oneline[3]
                             +"|"+oneline[4] +"|"+oneline[5]
                             +"|"+oneline[6] +"|"+oneline[7]
                             +"|"+oneline[8] +"|"+oneline[9]
                             +"|"+data +"|"+oneline[11]
                             +"|"+oneline[12] +"|"+oneline[13]+"|"+oneline[14];
            qDebug() << "id=" << resid << " can find";
            strres=strres+strline+"\n";
      }
      else
      {
        strres=strres+strtmp+"\n";
      }
    }

    fileread.close();
    QFile filewrite(strfillpath);
    if(!filewrite.open(QIODevice::WriteOnly))
    {
       qDebug()<<"File open failed";
       return "修改成功0, 失败1";
    }

    QTextStream out(&filewrite);
    filewrite.write(strres.toStdString().data());//c_str
    filewrite.close();
    return "修改成功1";
}

QString COperProxy::chgDeadline(int id, QString data)
{
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";
    //    qDebug() << "strfillpath=" << strfillpath;
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly))
      qDebug()<<"File open failed";
    QTextStream in(&fileread);
    QString dev;
    QString strres = "";

    while(!in.atEnd())
    {
      QJsonObject pageObject;
      QString lines=in.readLine();
      QString strtmp = lines;
      QStringList oneline=lines.split("|");
      int resid = atoi(oneline[0].toLocal8Bit().data());
      if (resid == id)
      {
          qDebug() << "enableproxy i find it lines=" << lines;
          qDebug() << "status=" << oneline[13].toLocal8Bit().data();

         QString strline = oneline[0] +"|"+oneline[1]
                             +"|"+oneline[2] +"|"+oneline[3]
                             +"|"+oneline[4] +"|"+data
                             +"|"+oneline[6] +"|"+oneline[7]
                             +"|"+oneline[8] +"|"+oneline[9]
                             +"|"+oneline[10] +"|"+oneline[11]
                             +"|"+oneline[12] +"|"+oneline[13]+"|"+oneline[14];
            qDebug() << "id=" << resid << " can find";
            strres=strres+strline+"\n";
      }
      else
      {
        strres=strres+strtmp+"\n";
      }
    }

    fileread.close();
    QFile filewrite(strfillpath);
    if(!filewrite.open(QIODevice::WriteOnly))
    {
       qDebug()<<"File open failed";
       return "修改成功0, 失败1";
    }

    QTextStream out(&filewrite);
    filewrite.write(strres.toStdString().data());//c_str
    filewrite.close();
    return "修改成功1";
}

QString COperProxy::chgAllocProxy(int id, QString data)
{
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";
    //    qDebug() << "strfillpath=" << strfillpath;
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly))
      qDebug()<<"File open failed";
    QTextStream in(&fileread);
    QString dev;
    QString strres = "";

    while(!in.atEnd())
    {
      QJsonObject pageObject;
      QString lines=in.readLine();
      QString strtmp = lines;
      QStringList oneline=lines.split("|");
      int resid = atoi(oneline[0].toLocal8Bit().data());
      if (resid == id)
      {
          qDebug() << "enableproxy i find it lines=" << lines;
          qDebug() << "status=" << oneline[13].toLocal8Bit().data();

         QString strline = oneline[0] +"|"+oneline[1]
                             +"|"+oneline[2] +"|"+oneline[3]
                             +"|"+oneline[4] +"|"+oneline[5]
                             +"|"+oneline[6] +"|"+oneline[7]
                             +"|"+oneline[8] +"|"+oneline[9]
                             +"|"+oneline[10] +"|"+oneline[11]
                             +"|"+data +"|"+oneline[13]+"|"+oneline[14];
            qDebug() << "id=" << resid << " can find";
            strres=strres+strline+"\n";
      }
      else
      {
        strres=strres+strtmp+"\n";
      }
    }

    fileread.close();
    QFile filewrite(strfillpath);
    if(!filewrite.open(QIODevice::WriteOnly))
    {
       qDebug()<<"File open failed";
       return "修改成功0, 失败1";
    }

    QTextStream out(&filewrite);
    filewrite.write(strres.toStdString().data());//c_str
    filewrite.close();
    return "修改成功1";
}

QString COperProxy::chgComment(int id, QString data)
{
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";
    //    qDebug() << "strfillpath=" << strfillpath;
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly))
      qDebug()<<"File open failed";
    QTextStream in(&fileread);
    QString dev;
    QString strres = "";

    while(!in.atEnd())
    {
      QJsonObject pageObject;
      QString lines=in.readLine();
      QString strtmp = lines;
      QStringList oneline=lines.split("|");
      int resid = atoi(oneline[0].toLocal8Bit().data());
      if (resid == id)
      {
          qDebug() << "enableproxy i find it lines=" << lines;
          qDebug() << "status=" << oneline[13].toLocal8Bit().data();

         QString strline = oneline[0] +"|"+oneline[1]
                             +"|"+oneline[2] +"|"+oneline[3]
                             +"|"+oneline[4] +"|"+oneline[5]
                             +"|"+oneline[6] +"|"+oneline[7]
                             +"|"+oneline[8] +"|"+oneline[9]
                             +"|"+oneline[10] +"|"+oneline[11]
                             +"|"+oneline[12] +"|"+oneline[13]+"|"+data;
            qDebug() << "id=" << resid << " can find";
            strres=strres+strline+"\n";
      }
      else
      {
        strres=strres+strtmp+"\n";
      }
    }

    fileread.close();
    QFile filewrite(strfillpath);
    if(!filewrite.open(QIODevice::WriteOnly))
    {
       qDebug()<<"File open failed";
       return "修改备注成功0, 失败1";
    }

    QTextStream out(&filewrite);
    filewrite.write(strres.toStdString().data());//c_str
    filewrite.close();
    return "修改备注成功1";
}

 QString COperProxy::enableproxy(int id)
{
     qDebug() << "enableproxy id=" << id;

     QString strfillpath = path;
     QJsonObject json;
     strfillpath += "\\proxyinfo.txt";
     //    qDebug() << "strfillpath=" << strfillpath;
     QFile fileread(strfillpath);
     if(!fileread.open(QIODevice::ReadOnly))
       qDebug()<<"File open failed";
     QTextStream in(&fileread);
     QString dev;
     QString strres = "";

     while(!in.atEnd())
     {
       QJsonObject pageObject;
       QString lines=in.readLine();
       QString strtmp = lines;
       QStringList oneline=lines.split("|");
       int resid = atoi(oneline[0].toLocal8Bit().data());
       if (resid == id)
       {
           qDebug() << "enableproxy i find it lines=" << lines;
           qDebug() << "status=" << oneline[13].toLocal8Bit().data();
            if (oneline[13] == "1")
            {
//                qDebug() << "exist yet";
                return "成功0, 已是启用状态无需修改1, 失败0";
            }

          QString strline = oneline[0] +"|"+oneline[1]
                              +"|"+oneline[2] +"|"+oneline[3]
                              +"|"+oneline[4] +"|"+oneline[5]
                              +"|"+oneline[6] +"|"+oneline[7]
                              +"|"+oneline[8] +"|"+oneline[9]
                              +"|"+oneline[10] +"|"+oneline[11]
                              +"|"+oneline[12] +"|"+"1"+"|"+oneline[14];

             qDebug() << "id=" << resid << " can find";
             strres=strres+strline+"\n";
       }
       else
       {
         strres=strres+strtmp+"\n";
       }
     }

     fileread.close();
     QFile filewrite(strfillpath);
     if(!filewrite.open(QIODevice::WriteOnly))
     {
        qDebug()<<"File open failed";
        return "成功0, 已是启用状态无需修改0, 失败1";
     }

     QTextStream out(&filewrite);
     filewrite.write(strres.toStdString().data());//c_str
     filewrite.close();
//     if (isfind == true)
//         return 0;
//     else
//         return -1;
     return "成功1, 已是启用状态无需修改0, 失败0";
}

 QString COperProxy::copyproxy(int id)
 {
     qDebug() << "copyproxy id=" << id;


     QString strfillpath = path;
     QJsonObject json;
     strfillpath += "\\proxyinfo.txt";
     //    qDebug() << "strfillpath=" << strfillpath;
     QFile fileread(strfillpath);
     if(!fileread.open(QIODevice::ReadOnly))
       qDebug()<<"File open failed";
     QTextStream in(&fileread);



     while(!in.atEnd())
     {
       QJsonObject pageObject;
       QString lines=in.readLine();
       QString strtmp = lines;
       QStringList oneline=lines.split("|");
       int resid = atoi(oneline[0].toLocal8Bit().data());
       if (resid == id)
       {
            QString strline = oneline[1]+"|"+oneline[2] +"|"+oneline[3]
                                +"|"+oneline[4] +"|"+oneline[5];
            QClipboard *board = QApplication::clipboard();
            board->setText(strline);
            break;
       }
     }

     fileread.close();

     return "完成";
 }

 int COperProxy::getfilecnt()
 {
     QString strfillpath = path;
     strfillpath += "\\proxyinfo.txt";

     int cnt = 0;
     QFile fileread(strfillpath);
     if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
     {
         qDebug()<<"File open failed";
     }
     else
     {
         QFile fileread(strfillpath);
         if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
             qDebug()<<"File open failed";
         QTextStream in(&fileread);
         while(!in.atEnd())
         {
             QString lines=in.readLine();
             cnt++;
         }
     }

     fileread.close();
     return cnt;
 }

 QString COperProxy::disableproxy(int id)
{
     qDebug() << "enableproxy id=" << id;

     QString strfillpath = path;
     QJsonObject json;
     strfillpath += "\\proxyinfo.txt";
     //    qDebug() << "strfillpath=" << strfillpath;
     QFile fileread(strfillpath);
     if(!fileread.open(QIODevice::ReadOnly))
       qDebug()<<"File open failed";
     QTextStream in(&fileread);
     QString dev;
     QString strres = "";
     bool isfind = false;
     while(!in.atEnd())
     {
       QJsonObject pageObject;
       QString lines=in.readLine();
       QString strtmp = lines;
       QStringList oneline=lines.split("|");
       int resid = atoi(oneline[0].toLocal8Bit().data());
       if (resid == id)
       {
           qDebug() << "enableproxy i find it lines=" << lines;
           qDebug() << "status=" << oneline[13].toLocal8Bit().data();
            if (oneline[13] == "0")
            {
//                qDebug() << "exist yet22";
                return "成功0, 已是禁用状态无需修改1, 失败0";
            }

            QString strline = oneline[0] +"|"+oneline[1]
                                +"|"+oneline[2] +"|"+oneline[3]
                                +"|"+oneline[4] +"|"+oneline[5]
                                +"|"+oneline[6] +"|"+oneline[7]
                                +"|"+oneline[8] +"|"+oneline[9]
                                +"|"+oneline[10] +"|"+oneline[11]
                                +"|"+oneline[12] +"|"+"0"+"|"+oneline[14];
           isfind = true;
           qDebug() << "id=" << resid << " can find";
           strres=strres+strline+"\n";
       }
       else
       {
         strres=strres+strtmp+"\n";
       }
     }

     fileread.close();
     QFile filewrite(strfillpath);
     if(!filewrite.open(QIODevice::WriteOnly))
     {
        qDebug()<<"File open failed";
        return "成功0, 已是启用状态无需修改0, 失败1";
     }

     QTextStream out(&filewrite);
     filewrite.write(strres.toStdString().data());//c_str

     return "成功1, 已是启用状态无需修改0, 失败0";
}


int COperProxy::updateProxyConf(int id, QString resdata)
{
    qDebug() << "updateProxyConf id=" << id;

    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";
    //    qDebug() << "strfillpath=" << strfillpath;
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly))
      qDebug()<<"File open failed";
    QTextStream in(&fileread);
    QString dev;
    QString strres = "";
    bool isfind = false;
    while(!in.atEnd())
    {
      QJsonObject pageObject;
      QString lines=in.readLine();
      QString strtmp = lines;
      QStringList oneline=lines.split("|");
      int resid = atoi(oneline[0].toLocal8Bit().data());
      if (resid == id)
      {
         QStringList chgoneline=resdata.split("|");
        QString strid = QString::number(id);
         QString strline = strid +"|"+chgoneline[1]
                             +"|"+chgoneline[2] +"|"+chgoneline[3]
                             +"|"+chgoneline[4] +"|"+chgoneline[5]
                             +"|"+oneline[7] +"|"+oneline[8]
                             +"|"+oneline[9] +"|"+chgoneline[6]
                             +"|"+chgoneline[7] +"|"+chgoneline[8]
                             +"|"+chgoneline[9] +"|"+chgoneline[10]+"|"+chgoneline[11];
//            data
            isfind = true;
            qDebug() << "id=" << resid << " can find";
            strres=strres+strline+"\n";
      }
      else
      {
        strres=strres+strtmp+"\n";
      }
    }

    fileread.close();
    QFile filewrite(strfillpath);
    if(!filewrite.open(QIODevice::WriteOnly))
    {
       qDebug()<<"File open failed";
       return -1;
    }

    QTextStream out(&filewrite);
    filewrite.write(strres.toStdString().data());//c_str
    filewrite.close();
    if (isfind == true)
        return 0;
    else
        return -1;
}

int COperProxy::delProxyConf(int id)
{
    qDebug() << "delProxyConf id=" << id;

    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";
    //    qDebug() << "strfillpath=" << strfillpath;
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly))
      qDebug()<<"File open failed";
    QTextStream in(&fileread);
    QString dev;
    QString strres = "";
    bool isfind = false;
    while(!in.atEnd())
    {
      QJsonObject pageObject;
      QString lines=in.readLine();
      QString strtmp = lines;
      QStringList oneline=lines.split("|");
      int resid = atoi(oneline[0].toLocal8Bit().data());
      if (resid == id)
      {
            isfind = true;
            qDebug() << "id=" << resid << " can find";
      }
      else
      {

          QTextCodec *codec = QTextCodec::codecForName("UTF-8");
          QTextCodec *utf8 = QTextCodec::codecForName("gbk");

            QString province1 = codec->toUnicode( oneline[7].toLocal8Bit().data());
            QString line1 = codec->toUnicode( oneline[8].toLocal8Bit().data());

            province1 = utf8->toUnicode(province1.toLocal8Bit().data());
            line1 = utf8->toUnicode(line1.toLocal8Bit().data());

            QString strline = oneline[0] +"|"+oneline[1]
                                +"|"+oneline[2] +"|"+oneline[3]
                                +"|"+oneline[4] +"|"+oneline[5]
                                +"|"+oneline[6] +"|"+province1
                                +"|"+line1 +"|"+oneline[9]
                                +"|"+oneline[10] +"|"+oneline[11]
                                +"|"+oneline[12] +"|"+oneline[13]+"|"+oneline[14];
            strres=strres+strline+"\n";
//        strres=strres+strtmp+"\n";
      }
    }

    fileread.close();
    QFile filewrite(strfillpath);
    if(!filewrite.open(QIODevice::WriteOnly))
    {
       qDebug()<<"File open failed";
       return -1;
    }

    QTextStream out(&filewrite);
    filewrite.write(strres.toStdString().data());//c_str
    filewrite.close();
    if (isfind == true)
        return 0;
    else
        return -1;
}

int COperProxy::checkSocks5Udp(QString ip, QString port, QString name, QString passwd)
{
    zf5_sock5_udp sock5;
    struct sockaddr_in p_addr;
    p_addr.sin_family=AF_INET;
    p_addr.sin_addr.s_addr=INADDR_ANY;
    p_addr.sin_port=htons(4001);
    return sock5.connect_sock5_udp((char *)ip.toStdString().c_str(), atoi(port.toStdString().c_str()), &p_addr);
}

QString COperProxy::getOneRowData(QString id)
{
    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"File open failed";
    QTextStream in(&fileread);

    int idx = 0;
    QString dev;

    while(!in.atEnd())
    {
        QJsonObject pageObject;
        QString lines=in.readLine();

        QStringList oneline=lines.split("|");
//        qDebug()<<"id=" << id << " oneline=" << oneline[0];
        if (id == oneline[0])
        {
            pageObject.insert("id", oneline[0].toLocal8Bit().data());
            pageObject.insert("ip", oneline[1].toLocal8Bit().data());
            pageObject.insert("port", oneline[2].toLocal8Bit().data());
            pageObject.insert("account", oneline[3].toLocal8Bit().data());
            pageObject.insert("passwd", oneline[4].toLocal8Bit().data());
            pageObject.insert("deadline", oneline[5].toLocal8Bit().data());
            pageObject.insert("type", oneline[6].toLocal8Bit().data());
            pageObject.insert("pos", oneline[7].toLocal8Bit().data());
            pageObject.insert("line", oneline[8].toLocal8Bit().data());
            pageObject.insert("delay", oneline[9].toLocal8Bit().data());
    //        pageObject.insert("comment", oneline[10].toLocal8Bit().data());

            pageObject.insert("canassigned", oneline[10].toLocal8Bit().data());

            pageObject.insert("alreadyassigned", oneline[11].toLocal8Bit().data());
            pageObject.insert("process", oneline[12].toLocal8Bit().data());
            pageObject.insert("status", oneline[13].toLocal8Bit().data());
    //        pageObject.insert("canassigned", oneline[14].toLocal8Bit().data());
            pageObject.insert("comment", oneline[14].toLocal8Bit().data());

            dev = "proxy" + QString::number(idx);

            json.insert(dev, QJsonValue(pageObject));
            qDebug()<<"id1111=" << id << " oneline=" << oneline[0];
            idx++;
            break;
        }


        //           stu_lines.append(lines);
    }

    fileread.close();
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    qDebug() << "data="+strJson;
    return strJson;
}

zf5_sock5_udp::zf5_sock5_udp(void)
{
    m_n_addrtype=-1;
    m_n_port=0;
    m_n_sock5=INVALID_SOCKET;
    m_sz_name[0]=0;
    m_sz_pass[0]=0;
    snprintf(m_sz_name, sizeof(m_sz_name), "ks600");
    snprintf(m_sz_pass, sizeof(m_sz_pass), "ks600");
    m_sz_udp_srv[0]=0;

}

//创建sock5的upd代理
int zf5_sock5_udp::connect_sock5_udp(char *sz_addr_proxy,int n_port_proxy,struct sockaddr_in *p_addr)
{
    SOCKET s_sock5_tcp=INVALID_SOCKET;
     s_sock5_tcp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //创建套接字（socket）。
    if(s_sock5_tcp==INVALID_SOCKET)
    {
        return -1;
    }

//    timeval tv;
//    tv.tv_sec=10;
//    tv.tv_usec=0;
//    if(setsockopt(s_sock5_tcp,SOL_SOCKET,SO_RCVTIMEO,(char *)&tv,sizeof(timeval))==SOCKET_ERROR)
//    {
//        closesocket(s_sock5_tcp);
//        return -1;
//    }


    SOCKADDR_IN  sa_1;
    sa_1.sin_family=AF_INET;
    sa_1.sin_addr.s_addr=inet_addr(sz_addr_proxy);
    sa_1.sin_port=htons(n_port_proxy);
    qDebug() << "sz_addr_proxy111=" << sz_addr_proxy;
    if(sa_1.sin_addr.s_addr==INADDR_NONE)
    {
        qDebug() << "sz_addr_proxy222=" << sz_addr_proxy;
        struct hostent *p=gethostbyname(sz_addr_proxy);
        if(p==NULL)
        {
            closesocket(s_sock5_tcp);
            return -1;
        }
        sa_1.sin_addr.s_addr=((LPIN_ADDR)p->h_addr)->s_addr;
    }

    qDebug() << "sz_addr_proxy=" << sz_addr_proxy;
    qDebug() << "n_port_proxy=" << n_port_proxy;
    if(connect(s_sock5_tcp,(struct sockaddr *)&sa_1,sizeof(SOCKADDR_IN))==SOCKET_ERROR)
    {
         qDebug() << "connect error";
        closesocket(s_sock5_tcp);
        return -1;
    }
    else
    {
         qDebug() << "connect succeed";
         return 0;
    }

    char sz_buf[ZF5_BUFFER_SIZE2+2];

    memset(sz_buf,0,ZF5_BUFFER_SIZE2);
    sz_buf[0]=5;
    sz_buf[1]=1;
    sz_buf[2]=2;
//    sz_buf[3]=0x02;
    int ret = send(s_sock5_tcp,sz_buf,3,0);
//    qDebug() << "send succeed sz_buf0=" << (int)sz_buf[0] << ret;
//    qDebug() << "send succeed sz_buf1=" << (int)sz_buf[1] << ret;
//    qDebug() << "send succeed sz_buf2=" << (int)sz_buf[2] << ret;
    qDebug() << "send succeed ret=" << ret << ret;
    if(ret==SOCKET_ERROR)
    {
        qDebug() << "send error 11";
        closesocket(s_sock5_tcp);
        return -1;
    }

    memset(sz_buf,0,ZF5_BUFFER_SIZE2);
    ret = recv(s_sock5_tcp,sz_buf,24,0);
    qDebug() << "recv succeed ret=" << ret;
    if(ret ==SOCKET_ERROR)
    {

        qDebug() << "recv error 11="<< WSAGetLastError();
        closesocket(s_sock5_tcp);
        return -1;
    }

    if(sz_buf[0]==0x05&&sz_buf[1]==0x02)
    {
        qDebug() << "1111";
        int n_temp1=0;
        int n_temp2=0;
        memset(sz_buf,0,ZF5_BUFFER_SIZE2);
        n_temp1=strlen(m_sz_name);
        sz_buf[0]=0x01;
        sz_buf[1]=n_temp1;
        memcpy((void *)&(sz_buf[2]),(void *)&(m_sz_name[0]),n_temp1);
        n_temp2=strlen(m_sz_pass);
        sz_buf[1+n_temp1+1]=n_temp2;
        memcpy((void *)&(sz_buf[1+n_temp1+2]),(void *)&(m_sz_pass[0]),n_temp2);
        if(send(s_sock5_tcp,sz_buf,n_temp1+n_temp2+3,0)==SOCKET_ERROR)
        {
             qDebug() << "send error 22";
            closesocket(s_sock5_tcp);
            return -1;
        }

        memset(sz_buf,0,ZF5_BUFFER_SIZE2);
        if(recv(s_sock5_tcp,sz_buf,ZF5_BUFFER_SIZE2,0)==SOCKET_ERROR)
        {
            qDebug() << "recv error 22";
            closesocket(s_sock5_tcp);
            return -1;
        }

        qDebug() << "0=" << (int)sz_buf[0];
        qDebug() << "2=" << (int)sz_buf[1];

        if(sz_buf[0]!=0x01||sz_buf[1]!=0x00)
        {
            closesocket(s_sock5_tcp);
            qDebug() << "111444";
            return -1;
        }

        qDebug() << "1112";
    }
    else if(sz_buf[0]!=0x05||sz_buf[1]!=0x00)
    {
        closesocket(s_sock5_tcp);
        qDebug() << "1vv1";
        return -1;
    }

    qDebug() << "1113";

    memset(sz_buf,0,ZF5_BUFFER_SIZE2);
    sz_buf[0]=0x05;
    sz_buf[1]=0x03;
    sz_buf[2]=0x00;
    sz_buf[3]=0x01;
    qDebug() << "udp";
    *((int *)(&sz_buf[4]))=p_addr->sin_addr.s_addr;
    *((short *)(&(sz_buf[8])))=(short)p_addr->sin_port;

    if(send(s_sock5_tcp,sz_buf,10,0)==SOCKET_ERROR)
    {
        closesocket(s_sock5_tcp);
        return -1;
    }

    qDebug() << "udp 1";
    memset(sz_buf,0,ZF5_BUFFER_SIZE2);
    if(recv(s_sock5_tcp,sz_buf,ZF5_BUFFER_SIZE2,0)==SOCKET_ERROR)
    {
        qDebug() << "recv error 33";
        closesocket(s_sock5_tcp);
        return -1;
    }

    if(sz_buf[0]!=0x05||sz_buf[1]!=0x00)
    {
        qDebug() << "type error 44";
        closesocket(s_sock5_tcp);
        return -1;
    }

    qDebug() << "udp 2";
    if(sz_buf[3]==0x01)
    {
//        qDebug() << "udp 3";
        int n_ip=*((int *)&sz_buf[4]);
        struct in_addr in;
        in.S_un.S_addr=n_ip;
        memset((void *)m_sz_udp_srv,0,sizeof(m_sz_udp_srv));
        strcpy(m_sz_udp_srv,inet_ntoa(in));
        m_n_port=htons(*((short *)&(sz_buf[8])));
        m_n_addrtype=0x01;
    }
    else if(sz_buf[3]==0x03)
    {
//        qDebug() << "udp 4";
        memset((void *)m_sz_udp_srv,0,sizeof(m_sz_udp_srv));
        strncpy(m_sz_udp_srv,&sz_buf[5],sz_buf[4]);
        m_n_port=htons(*((short *)&(sz_buf[5+sz_buf[4]])));
        m_n_addrtype=0x03;
    }
    else
    {
        closesocket(s_sock5_tcp);
        return -1;
    }


    closesocket(s_sock5_tcp);
//    m_n_sock5=s_sock5_tcp;

    return 0;
}


static std::string _GetServerIP(char *host)
{
    if (host == NULL)
    {
        return std::string();
    }

    struct hostent *pstHostent = NULL;
    if (inet_addr(host) == INADDR_NONE)
    {
        if ((pstHostent = gethostbyname(host)) == NULL)
        {
            return std::string();
        }
        char *ipaddr = inet_ntoa(*(struct in_addr *)pstHostent->h_addr);
        if (ipaddr)
        {
            return std::string(ipaddr);
        }
    }
    else
    {
        return std::string(host);
    }
    return std::string();
}

CopyFileThread::CopyFileThread(QString strtext, COperProxy *proxy, int idx, QObject * parent)
//CopyFileThread::CopyFileThread(QString strtext, QObject * parent)
{
    m_strtext = strtext;
    m_proxy = proxy;
    m_idx = idx;
}

void CopyFileThread::run()
{
    // 新线程入口
    // 初始化和操作放在这里
    string strip;
    QVector<QString> vecdata;
    QStringList resitem=m_strtext.split("|");
    for(int j=0; j < resitem.size(); j++)
    {
        vecdata.push_back(resitem[j]);
        if (j == 3)
        {
            break;
        }
    }

    for (auto data:vecdata)
    {
//        qDebug() << "datainfo =" << data;
    }

   qDebug("ipdata=%s", vecdata[0].toStdString().c_str());

    strip = _GetServerIP((char *)vecdata[0].toStdString().c_str());

//    QString qRes = m_proxy->pingDetect(vecdata[0]);
        QString qRes = m_proxy->pingDetect(strip.c_str());
    qDebug() << "qRes=" << qRes;
    if (qRes != "-1" && qRes != "-2")
    {

    }
#if 1
    int retval = m_proxy->checkSocks5Tcp(strip.c_str(), vecdata[1], vecdata[2], vecdata[3]);

    QString province;
    QString line;
    QTextCodec::ConverterState state;
    QTextCodec::ConverterState state1;
    QTextCodec *gbk = QTextCodec::codecForName("gbk");
    if (0 == retval)
    {

        retval = m_proxy->checkSocks5Udp(strip.c_str(), vecdata[1], vecdata[2], vecdata[3]);
        qDebug() << "call checkSocks5Tcp succeed retval="<<retval<<" 0=" << strip.c_str() <<" 1="<<vecdata[1]<<" 2="<<vecdata[2]<<" 3="+vecdata[3];
        if (0 == retval)
        {
            QString ipinfo = m_proxy->getIpInfp(strip.c_str());
            QStringList oneline=ipinfo.split("|");
            province = oneline[2]+oneline[3];
            province = gbk->toUnicode(province.toLocal8Bit().data());

            line = oneline[4];
            line = gbk->toUnicode(line.toLocal8Bit().data());

            qDebug() << "invalidChars=" << state.invalidChars;
            if (state.invalidChars > 0)
            {

            }
            else
           {

           }
            qDebug() << "call checkSocks5Udp succeed ipinfo="+ipinfo;
        }
        else
        {
            qDebug() << "call checkSocks5Udp failure";
            return;
        }
    }
    else
    {
        qDebug() << "call checkSocks5Tcp failure";
        return;
    }

//    qDebug() << "CopyFileThread::run idx=" << m_idx;

    QString strfillpath = path;
    QJsonObject json;
    strfillpath += "\\proxyinfo.txt";
    //    qDebug() << "strfillpath=" << strfillpath;
    QFile fileread(strfillpath);
    if(!fileread.open(QIODevice::ReadOnly))
      qDebug()<<"File open failed";
    QTextStream in(&fileread);

    QString strres = "";
    bool isfind = false;

    while(!in.atEnd())
    {
      QJsonObject pageObject;
      QString lines=in.readLine();
      QString strtmp = lines;
      QStringList oneline=lines.split("|");
      int resid = atoi(oneline[0].toLocal8Bit().data());
      int id = m_idx;
//      qDebug() << "resid=" << resid << " id=" << id << " province=" << province << " line=" << line;
      if (resid == id)
      {
        QString strid = QString::number(id);
        QString strline = oneline[0] +"|"+oneline[1]
                         +"|"+oneline[2] +"|"+oneline[3]
                         +"|"+oneline[4] +"|"+oneline[5]
                         +"|"+oneline[6] +"|"+province
                         +"|"+line +"|"+qRes
                         +"|"+oneline[10] +"|"+oneline[11]
                         +"|"+oneline[12] +"|"+oneline[13]+"|"+oneline[14];

            isfind = true;
//            qDebug() << "id=" << resid << " can find";
            strres=strres+strline+"\n";
      }
      else
      {
            QTextCodec *codecUTF8 = QTextCodec::codecForName("UTF-8");

            QString province1 = codecUTF8->toUnicode( oneline[7].toLocal8Bit().data(), oneline[7].toLocal8Bit().size(), &state);
            QString line1 = codecUTF8->toUnicode( oneline[8].toLocal8Bit().data(), oneline[8].toLocal8Bit().size(), &state1);


            province1 = gbk->toUnicode(province1.toLocal8Bit().data(), province1.toLocal8Bit().size(), &state);
            line1 = gbk->toUnicode(line1.toLocal8Bit().data(), line1.toLocal8Bit().size(), &state1);

            QString strline = oneline[0] +"|"+oneline[1]
                                +"|"+oneline[2] +"|"+oneline[3]
                                +"|"+oneline[4] +"|"+oneline[5]
                                +"|"+oneline[6] +"|"+province1
                                +"|"+line1 +"|"+oneline[9]
                                +"|"+oneline[10] +"|"+oneline[11]
                                +"|"+oneline[12] +"|"+oneline[13]+"|"+oneline[14];

//            qDebug() << "invalidChars112=" << state.invalidChars;
//            qDebug() << "invalidChars112111=" << state1.invalidChars;
//            return;

//            QString text = codec->toUnicode( oneline[7].toLocal8Bit()., oneline[7].data().size(), &state);
//            if (state.invalidChars > 0)
//            {
////              text = QTextCodec::codecForName( "GBK" )->toUnicode(ba);
//            }
//            else
//           {
////               text = ba;
//           }


//        strres=strres+operProxystrtmp+"\n";
            strres=strres+strline+"\n";
      }
    }

    fileread.close();
    QFile filewrite(strfillpath);
    if(!filewrite.open(QIODevice::WriteOnly))
    {
       qDebug()<<"File open failed";
       return;
    }

    QTextStream out(&filewrite);
    filewrite.write(strres.toStdString().data());//c_str
    filewrite.close();

    emit m_proxy->myClassInfoSignal("testttt");	// 发射信号
#endif
}

