
#ifndef FUNCTION_H_H
#define FUNCTION_H_H
#include <QObject>
#include <QDateTime>
#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include<winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <QThread>
#include <string>
#include <QSettings>
#include <QVector>
#include <stdint.h>
#include <QFileDialog>  //头文件
#include <QFileDialog>
#include <QApplication>
#include <QWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <QStringList>
#include <QModelIndex>
#include <QDir>
#include <QDebug>
#include <QObject>
#include <QFileDialog>
#include <vector>
using namespace std;

class CFunction;

class captureinfo
{
public:
    int id;
    int port;
    unsigned long pid;
};

class NetworkPerformanceItem
{
public:
    NetworkPerformanceItem()
    {

    }
    ~NetworkPerformanceItem()
    {

    }

    INT ProcessId;
    INT State;
    std::string LocalAddress;
    std::string RemoteAddress;
    int LocalPort;
    int RemotePort;
    LONG BytesOut;
    LONG BytesIn;
    LONG OutboundBandwidth;
    LONG InboundBandwidth;
    int Pass = 0;
    std::string CollectionTime;

};

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

class NetworkPerformanceScanner
{
public:
    NetworkPerformanceScanner();
    ~NetworkPerformanceScanner();
    std::vector<NetworkPerformanceItem> ScanNetworkPerformance(int pass, int port);
//    std::vector<NetworkPerformanceItem> ScanNetworkPerformance(int sampleId);
};

class CProcInfo
{
public:
    unsigned long pid;
    QString strname;
};

class CFunction;

class ProxyFuncThd: public QThread
{
    Q_OBJECT
public:
    ProxyFuncThd(captureinfo &veccap, int type, CFunction *func, QObject * parent = 0);
    static void packet_handler(u_char *argument, const struct pcap_pkthdr *packet_header, const u_char *packet_content);
    static void ip_protool_packet_callback(u_char *argument, const struct pcap_pkthdr* packet_header, const u_char* packet_content);
    static void tcp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr *packet_header,const u_char* packet_content);
    QString m_strtext;
    static CFunction *m_func;
    int m_idx;
protected:
    void run(); // 新线程入口
    int m_type;
    static unsigned long totalsnddata;
    static unsigned long totalrecvdata;
    static time_t lasttime;
    static time_t begintime;
    static QVector <captureinfo> m_veccap;
//// 省略掉一些内容
};

class ProcData
{
public:
    QString strprocname;
};

class ProcFuncThd: public QThread
{
    Q_OBJECT
public:
    ProcFuncThd(int type, CFunction *func, QObject * parent = 0);
    QVector <ProcData> readProcConf();
    static CFunction *m_func;
protected:
    void run(); // 新线程入口

//    void __stdcall R0MonitorProcessCallback(BOOLEAN CreateTag, DWORD CurProcessID, char * CurProcessName, char * CurProcessPath, DWORD ParentPID);  //监视进程信息
//     QString readProcConf();
    int m_type;
//// 省略掉一些内容
};

//定义ARP包数据
  class arppktinfo
  {
  public:
      unsigned short hdtyp;//硬件类型
      unsigned short protyp;//协议类型
      unsigned char hdsize;//硬件地址长度
      unsigned char prosize;//协议地址长度
      unsigned short op;//（操作类型）操作值:ARP/RARP
      u_char smac[6];//源MAC地址
      u_char sip[4];//源IP地址
      u_char dmac[6];//目的MAC地址
      u_char dip[4];//目的IP地址
  };

  //      struct tcp_header *tcp_protocol ;     /*tcp协议变量*/
  //      u_char flags;                          /*标记*/
  //      int header_length;                  /*头长度*/


  //      u_short windows;                /*窗口大小*/
  //      u_short urgent_pointer;     /*紧急指针*/
  //      u_int sequence;                 /*序列号*/
  //      u_int acknowledgement;   /*确认号*/
  //      u_int16_t   checksum;       /*检验和*/

class tcp_headerinfo
{
public:
unsigned short tcp_source_port;
unsigned short tcp_destination_port;
unsigned int tcp_acknowledgement;

  unsigned int tcp_ack;

#ifdef WORDS_BIGENDIAN
 unsigned char tcp_offset:4 ,
    tcp_reserved:4;
#else
 unsigned char tcp_reserved:4,
    tcp_offset:4;
 unsigned char tcp_flags;
   unsigned short tcp_windows;
   unsigned short tcp_checksum;
   unsigned short tcp_urgent_pointer;
#endif
};

class CFileDialog : public QFileDialog
{
    Q_OBJECT
public:
    CFileDialog(QWidget *parent = 0);
    ~CFileDialog();

public slots:
    void onChiose();
};



class MultiFileDirDialog : public QFileDialog
{
    Q_OBJECT

public:
    MultiFileDirDialog(QWidget *parent = NULL);
    ~MultiFileDirDialog();
    QStringList m_listSelect;
signals:

    public slots :
        void go();

private:

};

class CFunction : public QObject
{
    Q_OBJECT
    public:
        CFunction(QObject *parent=nullptr);
        Q_INVOKABLE int addProcInfo(int type);
        Q_INVOKABLE QString addMultiProcInfo(QString strname);
//        int procleishenmsg(QString strName);
        int procleishenmsg(vector <string> &vecstr);
        DWORD GetProcessPID(QString pProcName);
        QString addProc(QString procname, QString procpath, QString imagepath, int needmd5);
        BOOL IsExistProcess(vector <string> &vecstr, string &strres, string &strname, string &imagepath);

        Q_INVOKABLE QString readProcConf();
        Q_INVOKABLE QString getOneRowData(QString id);
        Q_INVOKABLE int opendir(QString id);
        void tcp_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr *packet_header,const u_char* packet_content);
        static int add();

        Q_INVOKABLE int resetlsp();


         int writeProxyConf(QVector <QString> &vecdata);

        Q_INVOKABLE QString getConfInfo();
        Q_INVOKABLE int isbannetwork();
        Q_INVOKABLE int getdrivertype();
        Q_INVOKABLE QString setConfInfo(QString conf);

        Q_INVOKABLE QString isopenproxy();

        Q_INVOKABLE QString editproc(QString procinfo);

        Q_INVOKABLE QString sethighlevelsetting(QString conf);

        Q_INVOKABLE QString reCapture(int isautodetect, QString strprocname = "");

        std::string getFileMD5(const std::string& filename);
        Q_INVOKABLE int delProcConf(int id);
        QVector<CProcInfo> getpidinfo(const char*  szProcessName);
        QVector <QString> readProxyConf();
        QVector <CProcInfo> GetProceeIDfromParentID(DWORD dwParentProcessId);
        char *MD5Data(char * StrSrc);
        Q_INVOKABLE int addDragInfo(QString strName);
        Q_INVOKABLE int addPath();
         Q_INVOKABLE QString getHighLevelConfInfo();
        Q_INVOKABLE int captureProc(int type);
        Q_INVOKABLE int resethost();
        Q_INVOKABLE int delfile();

        Q_INVOKABLE int repairnet();

        Q_INVOKABLE HRESULT SetFileRight(char* pFileObject);

        Q_INVOKABLE int addProccess(QString strname);
        ~CFunction()
        {

        }

        // 配置文件对象
        QSettings *settings;
        QSettings *highlevelsettings;
        QFileDialog file_dialog;
        int issubsameaddr;
        int iscapture;
        int packetvitual;
signals:

    void updateproxyinfo(QString strid, QString timeinfo, QString snddata, QString recvdata, QString strconn);

    void addproxyinfo(QString strdata);
};

class FileDialog : public QFileDialog
{
    Q_OBJECT
    public:
        CFunction *m_obj;
        explicit FileDialog(CFunction *obj, QWidget *parent = Q_NULLPTR)
            : QFileDialog(parent)
        {
            m_obj = obj;
//            setOption(QFileDialog::DontUseNativeDialog);
//            setFileMode(QFileDialog::Directory);
//            // setFileMode(QFileDialog::ExistingFiles);
//            for (auto *pushButton : findChildren<QPushButton*>()) {
//                qDebug() << pushButton->text();
//                if (pushButton->text() == "&Open" || pushButton->text() == "&Choose") {
//                    openButton = pushButton;
//                    break;
//                }
//            }
//            disconnect(openButton, SIGNAL(clicked(bool)));
//            connect(openButton, &QPushButton::clicked, this, &FileDialog::openClicked);
//            treeView = findChild<QTreeView*>();
//        setFileMode(QFileDialog::DirectoryOnly);
//                    setFileMode(FileMode(QFileDialog::Directory || QFileDialog::ExistingFile));
//                            setFileMode(FileMode(QFileDialog::ExistingFile));
//                     setFileMode(QFileDialog::ExistingFiles　　);
        setOption(QFileDialog::DontUseNativeDialog, true);
//        setOption(QFileDialog::ShowDirsOnly, false);
       BOOL isfind = false;
        for (auto *pushButton : findChildren<QPushButton*>()) {
                       qDebug() << pushButton->text();
                       if (pushButton->text() == "打开(&O)" || pushButton->text() == "&Choose") {
                           openButton = pushButton;
                           isfind = true;
                           break;
                       }
                   }

        if (isfind == true)
        {
            qDebug("find");
        }
        else
        {
            qDebug("cant find");
        }
        connect(openButton, &QPushButton::clicked, this, &FileDialog::openClicked);
        if (exec() == QDialog::Accepted)
          {
            QStringList files = selectedFiles();
            if (files.isEmpty())
                return;
              QStringList m_listSelect = selectedFiles();
//              QStringList list = m_listSelect[0].split("/");//QString字符串分割函数
//              int cnt111 = list.count();
//              qDebug() <<"select=" << m_listSelect[0];
//              qDebug() <<"list[cnt111-1]=" <<list[cnt111-1] << " path=" << m_listSelect[0];
              m_obj->addProccess(m_listSelect[0]);
//                 qDebug() <<" path111=" << m_listSelect[0];
          }

        }

        QStringList selected() const
        {
            return selectedFilePaths;
        }

    public slots:
        void openClicked()
        {
            QStringList files = selectedFiles();
            if (files.isEmpty())
            return;
             qDebug("openClicked");
             m_listSelect = selectedFiles();
             int cnt = m_listSelect.count();


             QStringList list = m_listSelect[0].split("/");//QString字符串分割函数
             int cnt111 = list.count();
             qDebug() <<"cnt111=" <<cnt111;
             qDebug() <<"list[cnt111-1]=" <<list[cnt111-1] << " path=" << m_listSelect[0];

             emit filesSelected(files);
             QDialog::accept();
//            QDialog::accept();
//            selectedFilePaths.clear();
//            qDebug() << treeView->selectionModel()->selection();
//            for (const auto& modelIndex : treeView->selectionModel()->selectedIndexes()) {
//                qDebug() << modelIndex.column();
//                if (modelIndex.column() == 0)
//                    selectedFilePaths.append(directory().absolutePath() + modelIndex.data().toString());
//            }
//            emit filesSelected(selectedFilePaths);
//            hide();
//            qDebug() << selectedFilePaths;
       }

    private:
        QTreeView *treeView;
        QPushButton *openButton;
        QStringList selectedFilePaths;
        QStringList m_listSelect;
};

class CaptureData
{
public:
    int id;
    int pid;
    QString procname;
    int connection;
    int sendnum;
    int recvnum;
    QString proxyip;
    QString strmac;
    QString strpos;
};





#endif
