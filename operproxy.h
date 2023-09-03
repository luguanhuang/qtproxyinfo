
#ifndef OPERPROXY_H_H
#define OPERPROXY_H_H
#include <QObject>
#include <QDateTime>
#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include<winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <QThread>

typedef BOOL(__stdcall *InitData)(int ProxyPort, int DrvMode, BOOL Switch);  //初始化
typedef BOOL(__stdcall *heckSocks5)(char* s5ip, int s5port, char *user, char* pass, char * & OutProxyIP);  //检测代理是否可达
typedef BOOL(__stdcall *SetProxyInfo)(int PrxMode, int DnsProxyMode, char * DnsIP, int DnsPort, char * ExeName, DWORD ProcessID, char * ProxyIP, int ProxyPort, const char * UserName, const char * PassWord, const char * Key);  //设置代理
typedef void(__stdcall *PFR0MonitorProcessInfo)(BOOLEAN CreateTag, DWORD CurProcessID, char * CurProcessName, char * CurProcessPath, DWORD ParentPID);  //驱动回调
typedef BOOL(__stdcall *GetR0ProcessInfo)(BOOL Switch, BOOL Mode, PFR0MonitorProcessInfo pfCallback, char * Key);  //驱动监视进程
typedef BOOL(__stdcall *StopProxy)(DWORD PID);
typedef BOOL(__stdcall *RepairNet)();  //修复网络
typedef BOOL(__stdcall *leanData)();  //清理数据,一般程序退出构析函数里使用
typedef BOOL(__stdcall *StartVirtual)(char * Key);
typedef BOOL(__stdcall *StopVirtual)(void);

using namespace std;

#define ZF5_BUFFER_SIZE1 255
#define ZF5_BUFFER_SIZE2 1024
#define ZF5_BUFFER_SIZE3 8192
class zf5_sock5_udp
{
public:

    //构造函数
    zf5_sock5_udp(void);

    //析构函数
    virtual ~zf5_sock5_udp(void)
    {

    }

    //创建sock5的upd代理
    int connect_sock5_udp(char *sz_addr_proxy,int n_port_proxy,struct sockaddr_in *p_addr);

    //通过sock5发送udp数据
//    int send_sock5_udp(int s_udp,char *sz_buf,int n_len,struct sockaddr_in *p_addr);

//    //通过sock5接收udp数据
//    int recv_sock5_udp(int s_udp,char *sz_buf,struct sockaddr_in *p_addr);

//    //关闭sock5的udp代理
//    int close_sock5_udp();

    //代理服务器用户名称
    char m_sz_name[ZF5_BUFFER_SIZE1+2];

    //代理服务器用户密码
    char m_sz_pass[ZF5_BUFFER_SIZE1+2];

    private:
    //远端udp服务器地址
    char m_sz_udp_srv[ZF5_BUFFER_SIZE2+2];

    //远端udp服务器端口
    int m_n_port;

    //地址类型
    int m_n_addrtype;

    //sock5代理的tcp连接
    int m_n_sock5;
};

class COperProxy : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QString myString READ myString WRITE setmyString NOTIFY myStringChanged)
//    Q_PROPERTY(NOTIFY myStringChanged)
    public:
        COperProxy(QObject *parent=nullptr);
        ~COperProxy();
        Q_INVOKABLE int checkSocks5Tcp(QString ip, QString port, QString name, QString passwd);
        Q_INVOKABLE int checkSocks5Udp(QString ip, QString port, QString name, QString passwd);
        Q_INVOKABLE QString getIpInfp(QString ip);
        Q_INVOKABLE QString pingDetect(QString ip);
        Q_INVOKABLE QString batAddProxy(QString proxyInfo, QString assignable, QString comment);
        Q_INVOKABLE QString addProxy(QString proxyInfo);
        Q_INVOKABLE QString readProxyConf();
        Q_INVOKABLE int delProxyConf(int id);
        Q_INVOKABLE QString getOneRowData(QString id);
        Q_INVOKABLE int updateProxyConf(int id, QString data);
        Q_INVOKABLE QString chgComment(int id, QString data);

        Q_INVOKABLE QString chgCanAssigned(int id, QString data);

        Q_INVOKABLE QString chgDeadline(int id, QString data);

        Q_INVOKABLE QString chgAllocProxy(int id, QString data);

        Q_INVOKABLE QString enableproxy(int id);
        Q_INVOKABLE QString disableproxy(int id);
        Q_INVOKABLE int getfilecnt();

        Q_INVOKABLE QString getselcomm(QString strcomm);
        Q_INVOKABLE QString getseldeadline(QString deadline);

        Q_INVOKABLE QString getseltypes(QString name);

        Q_INVOKABLE QString delRepeatRows(QString rows);

        Q_INVOKABLE QString delAll();


        Q_INVOKABLE QString copyproxy(int id);

    signals:
        void myStringChanged(QString strtext);
        void myClassInfoSignal(QString strtext);
};

class CopyFileThread: public QThread
{
    Q_OBJECT
public:
    CopyFileThread(QString strtext, COperProxy *proxy, int idx, QObject * parent = 0);
    QString m_strtext;
    COperProxy *m_proxy;
    int m_idx;
protected:
    void run(); // 新线程入口
//// 省略掉一些内容
};


#endif
